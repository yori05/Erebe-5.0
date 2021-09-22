// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/PlayerController_FPS.h"
#include "Character/Character_FPS.h"

APlayerController_FPS::APlayerController_FPS(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
}

/**-----------------	Inherit Function Part		-----------------*/
void APlayerController_FPS::SetupInputComponent()
{
	Super::SetupInputComponent();

	check(InputComponent);

	/** Locomotion Input */
	InputComponent->BindAxis("MoveForward", this, &APlayerController_FPS::MoveForwardInput);
	InputComponent->BindAxis("MoveRight", this, &APlayerController_FPS::MoveRightInput);

	/**
	  * We have 2 versions of the rotation bindings to handle different kinds of devices differently
	  * "turn" handles devices that provide an absolute delta, such as a mouse.
	  * "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	*/
	InputComponent->BindAxis("Turn", this, &APlayerController_FPS::AddYawInput);
	InputComponent->BindAxis("TurnRate", this, &APlayerController_FPS::TurnAtRateInput);
	InputComponent->BindAxis("LookUp", this, &APlayerController_FPS::AddPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &APlayerController_FPS::LookUpAtRateInput);

	/** Jump Input */
	InputComponent->BindAction("Jump", IE_Pressed, this, &APlayerController_FPS::JumpInput);
	InputComponent->BindAction("Jump", IE_Released, this, &APlayerController_FPS::StopJumpingInput);

}

/**-----------------	Input Function Part		-----------------*/

void APlayerController_FPS::MoveForwardInput(float Value)
{
	auto MyPawn = GetPawn<ACharacter_FPS>();

	if (IsValid(MyPawn))
	{
		MyPawn->MoveForward(Value);
	}

}

void APlayerController_FPS::MoveRightInput(float Value)
{
	auto MyPawn = GetPawn<ACharacter_FPS>();

	if (IsValid(MyPawn))
	{
		MyPawn->MoveRight(Value);
	}
}

void APlayerController_FPS::LookUpAtRateInput(float Rate)
{
	if (GetWorld() != nullptr)
	{
		AddPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	}
}
void APlayerController_FPS::TurnAtRateInput(float Rate)
{
	if (GetWorld() != nullptr)
	{
		AddYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	}
}

void APlayerController_FPS::JumpInput()
{
	auto MyPawn = GetPawn<ACharacter_FPS>();

	if (IsValid(MyPawn))
	{
		if (MyPawn->CanJump())
		{
			MyPawn->Jump();
		}
	}
}

void APlayerController_FPS::StopJumpingInput()
{
	auto MyPawn = GetPawn<ACharacter_FPS>();

	if (IsValid(MyPawn))
	{
		MyPawn->StopJumping();
	}
}
