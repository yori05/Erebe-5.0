// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/PlayerController_Basic.h"
#include "Character/Character_Basic.h"
#include "GameFramework/CharacterMovementComponent.h"

/**-----------------	Constructor Part		-----------------*/
APlayerController_Basic::APlayerController_Basic(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
}

/**-----------------	Inherit Function Part		-----------------*/
void APlayerController_Basic::SetupInputComponent()
{
	Super::SetupInputComponent();

	check(InputComponent);

	/** Locomotion Input */
	InputComponent->BindAxis("MoveForward", this, &APlayerController_Basic::MoveForwardInput);
	InputComponent->BindAxis("MoveRight", this, &APlayerController_Basic::MoveRightInput);

	/**
	  * We have 2 versions of the rotation bindings to handle different kinds of devices differently
	  * "Turn" handles devices that provide an absolute delta, such as a mouse.
	  * "TurnRate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	*/
	InputComponent->BindAxis("Turn", this, &APlayerController_Basic::AddYawInput);
	InputComponent->BindAxis("TurnRate", this, &APlayerController_Basic::TurnAtRateInput);
	InputComponent->BindAxis("LookUp", this, &APlayerController_Basic::AddPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &APlayerController_Basic::LookUpAtRateInput);

	/** Jump Input */
	InputComponent->BindAction("Jump", IE_Pressed, this, &APlayerController_Basic::JumpInput);
	InputComponent->BindAction("Jump", IE_Released, this, &APlayerController_Basic::StopJumpingInput);

	/** Viewpoint Input */
	InputComponent->BindAction("SwapViewMode", IE_Pressed, this, &APlayerController_Basic::SwapViewPoint);
	InputComponent->BindAction("FirstPersonViewMode", IE_Pressed, this, &APlayerController_Basic::UseFirstPersonViewPoint);
	InputComponent->BindAction("ThirdPersonViewMode", IE_Pressed, this, &APlayerController_Basic::UseThirdPersonViewPoint);

	/** Combat Input */
	InputComponent->BindAction("Fire", IE_Pressed, this, &APlayerController_Basic::LaunchAttack);
}

/**-----------------	Input Function Part		-----------------*/

/**-----------------	Movement Function Part		-----------------*/

void APlayerController_Basic::MoveForwardInput(float Value)
{
	auto MyPawn = GetPawn<ACharacter_Basic>();

	if (IsValid(MyPawn))
	{
		auto MyMovementComp = MyPawn->GetCharacterMovement();

		if (IsValid(MyMovementComp))
		{
			if (MyMovementComp->IsSwimming())
			{
				MyPawn->MoveForwardCamera(Value);
			}
		}
		MyPawn->MoveForward(Value);
	}
}

void APlayerController_Basic::MoveRightInput(float Value)
{
	auto MyPawn = GetPawn<ACharacter_Basic>();

	if (IsValid(MyPawn))
	{
		auto MyMovementComp = MyPawn->GetCharacterMovement();

		if (IsValid(MyMovementComp))
		{
			if (MyMovementComp->IsSwimming())
			{
				MyPawn->MoveRightCamera(Value);
			}
		}
		MyPawn->MoveRight(Value);
	}
}

/**-----------------	Orientation Function Part		-----------------*/

void APlayerController_Basic::LookUpAtRateInput(float Rate)
{
	if (GetWorld() != nullptr)
	{
		AddPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	}
}
void APlayerController_Basic::TurnAtRateInput(float Rate)
{
	if (GetWorld() != nullptr)
	{
		AddYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	}
}

/**-----------------	Jump Function Part		-----------------*/

void APlayerController_Basic::JumpInput()
{
	auto MyPawn = GetPawn<ACharacter_Basic>();

	if (IsValid(MyPawn))
	{
		if (MyPawn->CanJump())
		{
			MyPawn->Jump();
		}
	}
}

void APlayerController_Basic::StopJumpingInput()
{
	auto MyPawn = GetPawn<ACharacter_Basic>();

	if (IsValid(MyPawn))
	{
		MyPawn->StopJumping();
	}
}

/**-----------------	Viewpoint Function Part		-----------------*/

void APlayerController_Basic::SwapViewPoint()
{
	auto MyPawn = GetPawn<ACharacter_Basic>();

	if (IsValid(MyPawn))
	{
		if (MyPawn->GetIsInFirstPerson())
		{
			UseThirdPersonViewPoint();
		}
		else
		{
			UseFirstPersonViewPoint();
		}
	}
}

void APlayerController_Basic::UseFirstPersonViewPoint()
{
	auto MyPawn = GetPawn<ACharacter_Basic>();

	if (IsValid(MyPawn))
	{
		MyPawn->UseFirstPerson();
	}
}

void APlayerController_Basic::UseThirdPersonViewPoint()
{
	auto MyPawn = GetPawn<ACharacter_Basic>();

	if (IsValid(MyPawn))
	{
		MyPawn->UseThirdPerson();
	}
}

/**-----------------	Combat Function Part		-----------------*/

void APlayerController_Basic::LaunchAttack()
{
	auto MyPawn = GetPawn<ACharacter_Basic>();

	if (IsValid(MyPawn))
	{
		MyPawn->Attack();
	}
}