// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CharacterMovementComponent/ErebeCharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Math/Vector.h"

const float VERTICAL_SLOPE_NORMAL_Z = 0.001f; // Slope is vertical if Abs(Normal.Z) <= this threshold. Accounts for precision problems that sometimes angle normals slightly off horizontal for vertical surface.

DEFINE_LOG_CATEGORY_STATIC(LogErebeMovementState, Log, All);

uint8 UErebeCharacterMovementComponent::GetCustomMovementMode() const
{
	return CustomMovementMode;
}

EErebeMovementMode UErebeCharacterMovementComponent::GetAdvancedMovementMode() const
{
	return (EErebeMovementMode)CustomMovementMode;
}


void UErebeCharacterMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
	Super::PhysCustom(deltaTime, Iterations);

	switch (GetAdvancedMovementMode())
	{
	case EErebeMovementMode::EREBE_MOVE_None :
		break;
	case EErebeMovementMode::EREBE_MOVE_Rush :
		PhysRush(deltaTime, Iterations);
		break;
	case EErebeMovementMode::EREBE_MOVE_Dodge :
		PhysDodge(deltaTime, Iterations);
		break;
	default:
		UE_LOG(LogErebeMovementState, Warning, TEXT("%s has unsupported custom movement mode %d"), *CharacterOwner->GetName(), int32(CustomMovementMode));
		SetMovementMode(MOVE_None);
		break;
	}
}

/** ------------------- Rush Part Begin ---------------------------------- */
bool UErebeCharacterMovementComponent::CanRush() const
{
	return MovementMode == EMovementMode::MOVE_Walking;
}

void UErebeCharacterMovementComponent::Rush(const FVector& NewRushDirection)
{
	if (!CanRush())
		return;

	SetMovementMode(EMovementMode::MOVE_Custom, (uint8)EErebeMovementMode::EREBE_MOVE_Rush);
	bOldOrientRotationToMovement = bOrientRotationToMovement;
	bOrientRotationToMovement = false;

	if (NewRushDirection.IsNormalized())
	{
		RushDirection = NewRushDirection;
	}
	else if (NewRushDirection.IsNearlyZero())
	{
		RushDirection = CharacterOwner->GetActorForwardVector();
	}
	else
	{
		auto NormalizedDirection = NewRushDirection;
		RushDirection = (NormalizedDirection.Normalize() ? NormalizedDirection : CharacterOwner->GetActorForwardVector());
	}
	RushDistanceTravel = 0.f;

}

void UErebeCharacterMovementComponent::StopRush(bool ResetMovementMode)
{
	if (!IsRushing())
		return;

	if (ResetMovementMode)
	{
		SetMovementMode(EMovementMode::MOVE_Walking, 0);
	}

	bOrientRotationToMovement = bOldOrientRotationToMovement;
}

void UErebeCharacterMovementComponent::PhysRush(float deltaTime, int32 Iterations)
{
	if (deltaTime < MIN_TICK_TIME)
	{
		return;
	}

	if (RushDistanceTravel >= RushDistance)
		StopRush();

	RushDistanceTravel += RushSpeed * deltaTime;
	FHitResult hit;
	const auto Adjusted = RushDirection * deltaTime * RushSpeed;
	SafeMoveUpdatedComponent(Adjusted, UpdatedComponent->GetComponentRotation(), true, hit);
}

bool UErebeCharacterMovementComponent::IsRushing() const
{
	return (MovementMode == EMovementMode::MOVE_Custom && GetAdvancedMovementMode() == EErebeMovementMode::EREBE_MOVE_Rush && UpdatedComponent);
}

/** ------------------- Rush Part End ---------------------------------- */

/** ------------------- Dodge Part Begin ---------------------------------- */
bool UErebeCharacterMovementComponent::CanDodge() const
{
	return MovementMode == EMovementMode::MOVE_Walking ||
		(MovementMode == EMovementMode::MOVE_Custom && CustomMovementMode == EErebeMovementMode::EREBE_MOVE_Rush);
}

void UErebeCharacterMovementComponent::Dodge(const FVector& NewDodgeDirection)
{
	if (CharacterOwner && CharacterOwner->CanJump())
	{
		// Don't jump if we can't move up/down.
		if (!bConstrainToPlane || FMath::Abs(PlaneConstraintNormal.Z) != 1.f)
		{
			if (NewDodgeDirection.IsNormalized())
			{
				Velocity = NewDodgeDirection;
			}
			else if (NewDodgeDirection.IsNearlyZero())
			{
				Velocity = CharacterOwner->GetActorForwardVector();
			}
			else
			{
				auto NormalizedDirection = NewDodgeDirection;
				Velocity = (NormalizedDirection.Normalize() ? NormalizedDirection : CharacterOwner->GetActorForwardVector());
			}

			Velocity.Z = FMath::Max(Velocity.Z, JumpZVelocity * 0.75f);
			Velocity.X = Velocity.X * JumpZVelocity * 0.75f;
			Velocity.Y = Velocity.Y * JumpZVelocity * 0.75f;


			SetMovementMode(MOVE_Custom, EErebeMovementMode::EREBE_MOVE_Dodge);
		}
	}
}

void UErebeCharacterMovementComponent::StopDodge(bool ResetMovementMode)
{
	if (!IsDodging())
		return;

	if (ResetMovementMode)
	{
		SetMovementMode(EMovementMode::MOVE_Walking, 0);
	}

	bOrientRotationToMovement = bOldOrientRotationToMovement;
}

void UErebeCharacterMovementComponent::PhysDodge(float deltaTime, int32 Iterations)
{
	//PhysFalling(deltaTime, Iterations);

	if (deltaTime < MIN_TICK_TIME)
	{
		return;
	}

	FVector FallAcceleration = GetFallingLateralAcceleration(deltaTime);
	FallAcceleration.Z = 0.f;
	const bool bHasLimitedAirControl = ShouldLimitAirControl(deltaTime, FallAcceleration);

	float remainingTime = deltaTime;
	while ((remainingTime >= MIN_TICK_TIME) && (Iterations < MaxSimulationIterations))
	{
		Iterations++;
		float timeTick = GetSimulationTimeStep(remainingTime, Iterations);
		remainingTime -= timeTick;

		const FVector OldLocation = UpdatedComponent->GetComponentLocation();
		const FQuat PawnRotation = UpdatedComponent->GetComponentQuat();
		bJustTeleported = false;

		RestorePreAdditiveRootMotionVelocity();

		const FVector OldVelocity = Velocity;

		// Apply input
		const float MaxDecel = GetMaxBrakingDeceleration();
		if (!HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
		{
			// Compute Velocity
			{
				// Acceleration = FallAcceleration for CalcVelocity(), but we restore it after using it.
				TGuardValue<FVector> RestoreAcceleration(Acceleration, FallAcceleration);
				Velocity.Z = 0.f;
				CalcVelocity(timeTick, FallingLateralFriction, false, MaxDecel);
				Velocity.Z = OldVelocity.Z;
			}
		}

		// Compute current gravity
		const FVector Gravity(0.f, 0.f, GetGravityZ());
		float GravityTime = timeTick;

		// If jump is providing force, gravity may be affected.
		bool bEndingJumpForce = false;
		if (CharacterOwner->JumpForceTimeRemaining > 0.0f)
		{
			// Consume some of the force time. Only the remaining time (if any) is affected by gravity when bApplyGravityWhileJumping=false.
			const float JumpForceTime = FMath::Min(CharacterOwner->JumpForceTimeRemaining, timeTick);
			GravityTime = bApplyGravityWhileJumping ? timeTick : FMath::Max(0.0f, timeTick - JumpForceTime);

			// Update Character state
			CharacterOwner->JumpForceTimeRemaining -= JumpForceTime;
			if (CharacterOwner->JumpForceTimeRemaining <= 0.0f)
			{
				CharacterOwner->ResetJumpState();
				bEndingJumpForce = true;
			}
		}

		// Apply gravity
		Velocity = NewFallVelocity(Velocity, Gravity, GravityTime);

		// See if we need to sub-step to exactly reach the apex. This is important for avoiding "cutting off the top" of the trajectory as framerate varies.
		if (OldVelocity.Z > 0.f && Velocity.Z <= 0.f && NumJumpApexAttempts < MaxJumpApexAttemptsPerSimulation)
		{
			const FVector DerivedAccel = (Velocity - OldVelocity) / timeTick;
			if (!FMath::IsNearlyZero(DerivedAccel.Z))
			{
				const float TimeToApex = -OldVelocity.Z / DerivedAccel.Z;

				// The time-to-apex calculation should be precise, and we want to avoid adding a substep when we are basically already at the apex from the previous iteration's work.
				const float ApexTimeMinimum = 0.0001f;
				if (TimeToApex >= ApexTimeMinimum && TimeToApex < timeTick)
				{
					const FVector ApexVelocity = OldVelocity + DerivedAccel * TimeToApex;
					Velocity = ApexVelocity;
					Velocity.Z = 0.f; // Should be nearly zero anyway, but this makes apex notifications consistent.

					// We only want to move the amount of time it takes to reach the apex, and refund the unused time for next iteration.
					remainingTime += (timeTick - TimeToApex);
					timeTick = TimeToApex;
					Iterations--;
					NumJumpApexAttempts++;
				}
			}
		}

		//UE_LOG(LogCharacterMovement, Log, TEXT("dt=(%.6f) OldLocation=(%s) OldVelocity=(%s) NewVelocity=(%s)"), timeTick, *(UpdatedComponent->GetComponentLocation()).ToString(), *OldVelocity.ToString(), *Velocity.ToString());
		ApplyRootMotionToVelocity(timeTick);

		if (bNotifyApex && (Velocity.Z < 0.f))
		{
			// Just passed jump apex since now going down
			bNotifyApex = false;
			NotifyJumpApex();
		}

		// Compute change in position (using midpoint integration method).
		FVector Adjusted = 0.5f * (OldVelocity + Velocity) * timeTick;

		// Special handling if ending the jump force where we didn't apply gravity during the jump.
		if (bEndingJumpForce && !bApplyGravityWhileJumping)
		{
			// We had a portion of the time at constant speed then a portion with acceleration due to gravity.
			// Account for that here with a more correct change in position.
			const float NonGravityTime = FMath::Max(0.f, timeTick - GravityTime);
			Adjusted = (OldVelocity * NonGravityTime) + (0.5f * (OldVelocity + Velocity) * GravityTime);
		}

		// Move
		FHitResult Hit(1.f);
		SafeMoveUpdatedComponent(Adjusted, PawnRotation, true, Hit);

		if (!HasValidData())
		{
			return;
		}

		float LastMoveTimeSlice = timeTick;
		float subTimeTickRemaining = timeTick * (1.f - Hit.Time);

		if (IsSwimming()) //just entered water
		{
			StopDodge();
			remainingTime += subTimeTickRemaining;
			StartSwimming(OldLocation, OldVelocity, timeTick, remainingTime, Iterations);
			return;
		}
		else if (Hit.bBlockingHit)
		{
			if (IsValidLandingSpot(UpdatedComponent->GetComponentLocation(), Hit))
			{
				StopDodge();
				remainingTime += subTimeTickRemaining;
				ProcessLanded(Hit, remainingTime, Iterations);
				return;
			}
			else
			{
				// Compute impact deflection based on final velocity, not integration step.
				// This allows us to compute a new velocity from the deflected vector, and ensures the full gravity effect is included in the slide result.
				Adjusted = Velocity * timeTick;

				// See if we can convert a normally invalid landing spot (based on the hit result) to a usable one.
				if (!Hit.bStartPenetrating && ShouldCheckForValidLandingSpot(timeTick, Adjusted, Hit))
				{
					const FVector PawnLocation = UpdatedComponent->GetComponentLocation();
					FFindFloorResult FloorResult;
					FindFloor(PawnLocation, FloorResult, false);
					if (FloorResult.IsWalkableFloor() && IsValidLandingSpot(PawnLocation, FloorResult.HitResult))
					{
						StopDodge();
						remainingTime += subTimeTickRemaining;
						ProcessLanded(FloorResult.HitResult, remainingTime, Iterations);
						return;
					}
				}

				HandleImpact(Hit, LastMoveTimeSlice, Adjusted);

				// If we've changed physics mode, abort.
				if (!HasValidData() || !IsDodging())
				{
					StopDodge(false);
					return;
				}

				// Limit air control based on what we hit.
				// We moved to the impact point using air control, but may want to deflect from there based on a limited air control acceleration.
				FVector VelocityNoAirControl = OldVelocity;
				FVector AirControlAccel = Acceleration;
				if (bHasLimitedAirControl)
				{
					// Compute VelocityNoAirControl
					{
						// Find velocity *without* acceleration.
						TGuardValue<FVector> RestoreAcceleration(Acceleration, FVector::ZeroVector);
						TGuardValue<FVector> RestoreVelocity(Velocity, OldVelocity);
						Velocity.Z = 0.f;
						CalcVelocity(timeTick, FallingLateralFriction, false, MaxDecel);
						VelocityNoAirControl = FVector(Velocity.X, Velocity.Y, OldVelocity.Z);
						VelocityNoAirControl = NewFallVelocity(VelocityNoAirControl, Gravity, GravityTime);
					}

					const bool bCheckLandingSpot = false; // we already checked above.
					AirControlAccel = (Velocity - VelocityNoAirControl) / timeTick;
					const FVector AirControlDeltaV = LimitAirControl(LastMoveTimeSlice, AirControlAccel, Hit, bCheckLandingSpot) * LastMoveTimeSlice;
					Adjusted = (VelocityNoAirControl + AirControlDeltaV) * LastMoveTimeSlice;
				}

				const FVector OldHitNormal = Hit.Normal;
				const FVector OldHitImpactNormal = Hit.ImpactNormal;
				FVector Delta = ComputeSlideVector(Adjusted, 1.f - Hit.Time, OldHitNormal, Hit);

				// Compute velocity after deflection (only gravity component for RootMotion)
				if (subTimeTickRemaining > KINDA_SMALL_NUMBER && !bJustTeleported)
				{
					const FVector NewVelocity = (Delta / subTimeTickRemaining);
					Velocity = HasAnimRootMotion() || CurrentRootMotion.HasOverrideVelocityWithIgnoreZAccumulate() ? FVector(Velocity.X, Velocity.Y, NewVelocity.Z) : NewVelocity;
				}

				if (subTimeTickRemaining > KINDA_SMALL_NUMBER && (Delta | Adjusted) > 0.f)
				{
					// Move in deflected direction.
					SafeMoveUpdatedComponent(Delta, PawnRotation, true, Hit);

					if (Hit.bBlockingHit)
					{
						// hit second wall
						LastMoveTimeSlice = subTimeTickRemaining;
						subTimeTickRemaining = subTimeTickRemaining * (1.f - Hit.Time);

						if (IsValidLandingSpot(UpdatedComponent->GetComponentLocation(), Hit))
						{
							StopDodge();
							remainingTime += subTimeTickRemaining;
							ProcessLanded(Hit, remainingTime, Iterations);
							return;
						}

						HandleImpact(Hit, LastMoveTimeSlice, Delta);

						// If we've changed physics mode, abort.
						if (!HasValidData() || !IsDodging())
						{
							StopDodge(false);
							return;
						}

						// Act as if there was no air control on the last move when computing new deflection.
						if (bHasLimitedAirControl && Hit.Normal.Z > VERTICAL_SLOPE_NORMAL_Z)
						{
							const FVector LastMoveNoAirControl = VelocityNoAirControl * LastMoveTimeSlice;
							Delta = ComputeSlideVector(LastMoveNoAirControl, 1.f, OldHitNormal, Hit);
						}

						FVector PreTwoWallDelta = Delta;
						TwoWallAdjust(Delta, Hit, OldHitNormal);

						// Limit air control, but allow a slide along the second wall.
						if (bHasLimitedAirControl)
						{
							const bool bCheckLandingSpot = false; // we already checked above.
							const FVector AirControlDeltaV = LimitAirControl(subTimeTickRemaining, AirControlAccel, Hit, bCheckLandingSpot) * subTimeTickRemaining;

							// Only allow if not back in to first wall
							if (FVector::DotProduct(AirControlDeltaV, OldHitNormal) > 0.f)
							{
								Delta += (AirControlDeltaV * subTimeTickRemaining);
							}
						}

						// Compute velocity after deflection (only gravity component for RootMotion)
						if (subTimeTickRemaining > KINDA_SMALL_NUMBER && !bJustTeleported)
						{
							const FVector NewVelocity = (Delta / subTimeTickRemaining);
							Velocity = HasAnimRootMotion() || CurrentRootMotion.HasOverrideVelocityWithIgnoreZAccumulate() ? FVector(Velocity.X, Velocity.Y, NewVelocity.Z) : NewVelocity;
						}

						// bDitch=true means that pawn is straddling two slopes, neither of which he can stand on
						bool bDitch = ((OldHitImpactNormal.Z > 0.f) && (Hit.ImpactNormal.Z > 0.f) && (FMath::Abs(Delta.Z) <= KINDA_SMALL_NUMBER) && ((Hit.ImpactNormal | OldHitImpactNormal) < 0.f));
						SafeMoveUpdatedComponent(Delta, PawnRotation, true, Hit);
						if (Hit.Time == 0.f)
						{
							// if we are stuck then try to side step
							FVector SideDelta = (OldHitNormal + Hit.ImpactNormal).GetSafeNormal2D();
							if (SideDelta.IsNearlyZero())
							{
								SideDelta = FVector(OldHitNormal.Y, -OldHitNormal.X, 0).GetSafeNormal();
							}
							SafeMoveUpdatedComponent(SideDelta, PawnRotation, true, Hit);
						}

						if (bDitch || IsValidLandingSpot(UpdatedComponent->GetComponentLocation(), Hit) || Hit.Time == 0.f)
						{
							StopDodge();
							remainingTime = 0.f;
							ProcessLanded(Hit, remainingTime, Iterations);
							return;
						}
						else if (GetPerchRadiusThreshold() > 0.f && Hit.Time == 1.f && OldHitImpactNormal.Z >= GetWalkableFloorZ())
						{
							// We might be in a virtual 'ditch' within our perch radius. This is rare.
							const FVector PawnLocation = UpdatedComponent->GetComponentLocation();
							const float ZMovedDist = FMath::Abs(PawnLocation.Z - OldLocation.Z);
							const float MovedDist2DSq = (PawnLocation - OldLocation).SizeSquared2D();
							if (ZMovedDist <= 0.2f * timeTick && MovedDist2DSq <= 4.f * timeTick)
							{
								Velocity.X += 0.25f * GetMaxSpeed() * (RandomStream.FRand() - 0.5f);
								Velocity.Y += 0.25f * GetMaxSpeed() * (RandomStream.FRand() - 0.5f);
								Velocity.Z = FMath::Max<float>(JumpZVelocity * 0.25f, 1.f);
								Delta = Velocity * timeTick;
								SafeMoveUpdatedComponent(Delta, PawnRotation, true, Hit);
							}
						}
					}
				}
			}
		}

		if (Velocity.SizeSquared2D() <= KINDA_SMALL_NUMBER * 10.f)
		{
			Velocity.X = 0.f;
			Velocity.Y = 0.f;
		}
	}
}

bool UErebeCharacterMovementComponent::IsDodging() const
{
	return (MovementMode == EMovementMode::MOVE_Custom && GetAdvancedMovementMode() == EErebeMovementMode::EREBE_MOVE_Dodge && UpdatedComponent);
}

/** ------------------- Dodge Part End ---------------------------------- */
