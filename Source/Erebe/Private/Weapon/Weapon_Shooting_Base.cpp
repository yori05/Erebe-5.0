// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Weapon_Shooting_Base.h"
#include "Ammo/Ammo_Base.h"
#include "Math/UnrealMathUtility.h"
#include "Engine/World.h"

AWeapon_Shooting_Base::AWeapon_Shooting_Base(const FObjectInitializer& ObjectInitializer)
{
	bUseOwnerViewpoint = true;

	/** Set Ammo blueprint as default class for the ProjectileClass */
	static ConstructorHelpers::FClassFinder<AAmmo_Base> ProjectileDefaultClass(TEXT("/Game/BlueprintClasses/Ammo/Ammo_Base_BP"));

	if (ProjectileDefaultClass.Class != nullptr)
	{
		ProjectileClass = ProjectileDefaultClass.Class;
	}
	else
	{
		ProjectileClass = AAmmo_Base::StaticClass();
	}

	ProjectilePool.Reserve(PoolSize);
	AvaibleProjectilePool.Reserve(PoolSize);
	SpawnSocketName = TEXT("Muzzle");
}

void AWeapon_Shooting_Base::BeginPlay()
{
	Super::BeginPlay();

	if (bInitializePoolOnBegin)
	{
		IntializeProjectilePool();
	}
}

void AWeapon_Shooting_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bFire)
	{
		if (!bHasFirstFire)
		{
			if (!FMath::IsNearlyZero(FirstHold_Duration) && FirstHold_Timer < FirstHold_DurationMax)
			{
				FirstHold_Timer += DeltaTime;
			}

			if (FireTimeState == EFireTimeState::SHOOTTIME_Press || FireTimeState == EFireTimeState::SHOOTTIME_Continue)
			{
				if (FirstHold_Timer >= FirstHold_Duration)
				{
					Shoot();
					bHasFirstFire = true;
				}
			}
		}
		else if (FireTimeState == EFireTimeState::SHOOTTIME_Continue)
		{
			if (!FMath::IsNearlyZero(ColdDown_Duration))
			{
				ColdDown_Timer += DeltaTime;

				if (ColdDown_Timer >= ColdDown_Duration)
				{
					Shoot();
					ColdDown_Timer = 0.f;
				}
			}
		}
	}

}

void AWeapon_Shooting_Base::IntializeProjectilePool()
{
	auto World = GetWorld();
	AAmmo_Base* Temp = nullptr;

	if (ProjectilePool.Num() != PoolSize)
	{
		ProjectilePool.Reserve(PoolSize);
	}

	if (PoolSize > 0 && World != nullptr)
	{
		for (int i = 0; i < PoolSize; i++)
		{
			Temp = World->SpawnActor<AAmmo_Base>(ProjectileClass, GetActorTransform());
			Temp->AssociateToWeapon(this, i);
			Temp->Inactive();
			ProjectilePool.Add(Temp);
			AvaibleProjectilePool.Add(Temp);
		}
	}	
}

void AWeapon_Shooting_Base::FirePress()
{
	bFire = true;
	bHasFirstFire = false;

	if (FireTimeState == EFireTimeState::SHOOTTIME_Press || FireTimeState == EFireTimeState::SHOOTTIME_Continue)
	{
		if (FMath::IsNearlyZero(FirstHold_Duration))
		{
			Shoot();
			bHasFirstFire = true;
		}
	}
}

void AWeapon_Shooting_Base::FireRelease()
{
	bFire = false;
	bHasFirstFire = false;

	if (FireTimeState == EFireTimeState::SHOOTTIME_Release)
	{
		if (FMath::IsNearlyZero(FirstHold_Duration) || FMath::IsNearlyEqual(FirstHold_Duration, FirstHold_Timer))
		{
			Shoot();
		}
	}
}

void AWeapon_Shooting_Base::Shoot()
{
	auto World = GetWorld();
	FVector ProjectileLocation;
	FRotator ProjectileRotation;

	for (int32 i = 0; i < NumberOfProjectileFire; i++)
	{

		if (bUseOwnerImpactpoint && OwnerImpactPoint != nullptr)
		{
			ProjectileLocation = OwnerImpactPoint->GetComponentLocation();
		}
		else if (GetMesh() != nullptr)
		{
			ProjectileLocation = GetMesh()->GetSocketLocation(SpawnSocketName);
		}
		else
		{
			ProjectileLocation = GetActorLocation();
		}

		if (bUseOwnerViewpoint && OwnerViewpont != nullptr)
		{
			ProjectileRotation = OwnerViewpont->GetComponentRotation();
		}
		else if (GetMesh() != nullptr)
		{
			ProjectileRotation = GetMesh()->GetSocketRotation(SpawnSocketName);
		}
		else
		{
			ProjectileRotation = GetActorRotation();
		}

		if (ProjectilePool[PoolIndex] == nullptr)
		{
			ProjectilePool[PoolIndex] = World->SpawnActor<AAmmo_Base>(ProjectileClass, GetActorTransform());
			ProjectilePool[PoolIndex]->AssociateToWeapon(this, i);
		}

		ProjectilePool[PoolIndex]->Reset();
		ProjectilePool[PoolIndex]->SetActorLocation(ProjectileLocation);
		ProjectilePool[PoolIndex]->SetActorRotation(ProjectileRotation);
		ProjectilePool[PoolIndex]->Show();
		ProjectilePool[PoolIndex]->Active();
		PoolIndex = (PoolIndex + 1 >= ProjectilePool.Num()) ? 0 : PoolIndex + 1;
	}
}
