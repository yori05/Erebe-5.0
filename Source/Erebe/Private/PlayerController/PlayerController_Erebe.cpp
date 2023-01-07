// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/PlayerController_Erebe.h"
#include "Character/Character_Erebe.h"

APlayerController_Erebe::APlayerController_Erebe(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{

}

void APlayerController_Erebe::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (DoubleJumpClock < DoubleJumpTimingMax * DoubleJumpRateMax)
	{
		DoubleJumpClock += DeltaTime;
	}
}

void APlayerController_Erebe::SetupInputComponent()
{
	Super::SetupInputComponent();

	check(InputComponent);

	/** Focus Input */
	InputComponent->BindAction("Focus", IE_Pressed, this, &APlayerController_Erebe::FocusInput);
	InputComponent->BindAction("Focus", IE_Released, this, &APlayerController_Erebe::StopFocusInput);
}

void APlayerController_Erebe::RushInput()
{
	auto MyPawn = GetPawn<ACharacter_Erebe>();

	if (IsValid(MyPawn))
	{
		MyPawn->Rush();
	}
}

void APlayerController_Erebe::JumpInput()
{
	auto MyPawn = GetPawn<ACharacter_Erebe>();

	if (IsValid(MyPawn))
	{
		if (MyPawn->GetCharacterState() == ECharacterState::CHARSTATE_Focus)
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
		else if (MyPawn->GetCharacterState() != ECharacterState::CHARSTATE_Flying)
		{
			if (MyPawn->CanJump())
			{
				MyPawn->CallJump();
			}
			else if (MyPawn->CanFly())
			{
				MyPawn->SetCharacterState(ECharacterState::CHARSTATE_Flying);
			}
		}
	}
	else
	{
		Super::JumpInput();
	}
}

void APlayerController_Erebe::StopJumpingInput()
{
	auto MyPawn = GetPawn<ACharacter_Erebe>();

	if (IsValid(MyPawn))
	{
		MyPawn->CallStopJumping();
	}
	else
	{
		Super::StopJumpingInput();
	}

}

void APlayerController_Erebe::FocusInput()
{
	auto MyPawn = GetPawn<ACharacter_Erebe>();

	if (IsValid(MyPawn))
	{
		MyPawn->SetFocusState();
		DoubleJumpClock = DoubleJumpTimingMax * DoubleJumpRateMax;
	}
}

void APlayerController_Erebe::StopFocusInput()
{
	auto MyPawn = GetPawn<ACharacter_Erebe>();

	if (IsValid(MyPawn))
	{
		MyPawn->SetFreeState();
	}
}
