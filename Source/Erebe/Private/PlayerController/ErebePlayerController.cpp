// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/ErebePlayerController.h"
#include "Character/ErebeCharacter.h"
#include "Engine/Engine.h"

AErebePlayerController::AErebePlayerController(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
}

void AErebePlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (DoubleJumpClock < DoubleJumpTimingMax * DoubleJumpRateMax)
	{
		DoubleJumpClock += DeltaTime;
	}
}

void AErebePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	check(InputComponent);

	///** Jump Input */
	//InputComponent->BindAction("Jump", IE_Pressed, this, &AErebePlayerController::JumpInput);
	//InputComponent->BindAction("Jump", IE_Released, this, &AErebePlayerController::StopJumpInput);
	//
	///** Locomotion Input */
	//InputComponent->BindAxis("MoveForward", this, &AErebePlayerController::MoveForwardInput);
	//InputComponent->BindAxis("MoveRight", this, &AErebePlayerController::MoveRightInput);
	//
	///** 
	//  * We have 2 versions of the rotation bindings to handle different kinds of devices differently
	//  * "turn" handles devices that provide an absolute delta, such as a mouse.
	//  * "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	//*/
	//InputComponent->BindAxis("Turn", this, &AErebePlayerController::AddYawInput);
	//InputComponent->BindAxis("TurnRate", this, &AErebePlayerController::TurnAtRateInput);
	//InputComponent->BindAxis("LookUp", this, &AErebePlayerController::AddPitchInput);
	//InputComponent->BindAxis("LookUpRate", this, &AErebePlayerController::LookUpAtRateInput);

	/** Focus Input */
	InputComponent->BindAction("Focus", IE_Pressed, this, &AErebePlayerController::FocusInput);
	InputComponent->BindAction("Focus", IE_Released, this, &AErebePlayerController::StopFocusInput);

	/** Fight Input */
	InputComponent->BindAction("Fire", IE_Pressed, this, &AErebePlayerController::FirePressInput);
	InputComponent->BindAction("Fire", IE_Released, this, &AErebePlayerController::FireReleaseInput);

	/** Interaction Input */
	InputComponent->BindAction("Interact", IE_Pressed, this, &AErebePlayerController::InteractInput);
}

void AErebePlayerController::MoveForwardInput(float Value)
{
	auto MyPawn = GetPawn<AErebeCharacter>();

	if (IsValid(MyPawn))
	{
		MyPawn->MoveForward(Value);
	}
}

void AErebePlayerController::MoveRightInput(float Value)
{
	auto MyPawn = GetPawn<AErebeCharacter>();

	if (IsValid(MyPawn))
	{
		MyPawn->MoveRight(Value);
	}
}

void AErebePlayerController::MoveUpInput(float Value)
{
	auto MyPawn = GetPawn<AErebeCharacter>();

	if (IsValid(MyPawn) && MyPawn->GetCharacterState() == CHARSTATE_Flying)
	{
		MyPawn->MoveUp(Value);
	}
}

void AErebePlayerController::LookUpAtRateInput(float Rate)
{
	if (GetWorld() != nullptr)
	{
		AddPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	}
}
void AErebePlayerController::TurnAtRateInput(float Rate)
{
	if (GetWorld() != nullptr)
	{
		AddYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	}
}
void AErebePlayerController::RushInput()
{
	auto MyPawn = GetPawn<AErebeCharacter>();

	if (IsValid(MyPawn))
	{
		MyPawn->Rush();
	}
}
void AErebePlayerController::JumpInput()
{
	auto MyPawn = GetPawn<AErebeCharacter>();

	if (IsValid(MyPawn))
	{
		if (MyPawn->GetCharacterState() == EErebeCharacterState::CHARSTATE_Focus)
		{
			if (DoubleJumpClock <= DoubleJumpTimingMax)
			{
				MyPawn->Dodge();
				DoubleJumpClock = DoubleJumpTimingMax * DoubleJumpRateMax;
			}
			else
			{
				MyPawn->Rush();
				DoubleJumpClock = 0.f;
			}
		}
		else if (MyPawn->GetCharacterState() != EErebeCharacterState::CHARSTATE_Flying)
		{
			if (MyPawn->CanJump())
			{
				MyPawn->Jump();
			}
			else if (MyPawn->CanFly())
			{
				MyPawn->SetCharacterState(CHARSTATE_Flying);
			}
		}
	}
}
void AErebePlayerController::StopJumpInput()
{
	auto MyPawn = GetPawn<AErebeCharacter>();

	if (IsValid(MyPawn))
	{
		MyPawn->StopJumping();
	}
}

void AErebePlayerController::FocusInput()
{
	auto MyPawn = GetPawn<AErebeCharacter>();

	if (IsValid(MyPawn))
	{
		MyPawn->SetFocusState();
		DoubleJumpClock = DoubleJumpTimingMax * DoubleJumpRateMax;
	}
}

void AErebePlayerController::StopFocusInput()
{
	auto MyPawn = GetPawn<AErebeCharacter>();

	if (IsValid(MyPawn))
	{
		MyPawn->SetFreeState();
	}
}

void AErebePlayerController::PunchInput()
{
	auto MyPawn = GetPawn<AErebeCharacter>();

	if (IsValid(MyPawn))
	{
		MyPawn->Punch();
	}
}

void AErebePlayerController::FirePressInput()
{
	auto MyPawn = GetPawn<AErebeCharacter>();

	if (IsValid(MyPawn))
	{
		MyPawn->FirePress();
	}
}

void AErebePlayerController::FireReleaseInput()
{
	auto MyPawn = GetPawn<AErebeCharacter>();

	if (IsValid(MyPawn))
	{
		MyPawn->FireRelease();
	}
}


void AErebePlayerController::InteractInput()
{
	auto MyPawn = GetPawn<AErebeCharacter>();

	if (IsValid(MyPawn))
	{
		MyPawn->Interact();
	}
}
