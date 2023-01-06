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

	/** Fight Input */
	InputComponent->BindAction("Fire", IE_Pressed, this, &APlayerController_FPS::FirePressInput);
	InputComponent->BindAction("Fire", IE_Released, this, &APlayerController_FPS::FireReleaseInput);

	/** Interaction Input */
	InputComponent->BindAction("Drop", IE_Pressed, this, &APlayerController_FPS::ReleaseWeaponInput);
}

/**-----------------	Input Function Part		-----------------*/

void APlayerController_FPS::FirePressInput()
{
	auto MyPawn = GetPawn<ACharacter_FPS>();

	if (IsValid(MyPawn))
	{
		MyPawn->FirePress();
	}
}

void APlayerController_FPS::FireReleaseInput()
{
	auto MyPawn = GetPawn<ACharacter_FPS>();

	if (IsValid(MyPawn))
	{
		MyPawn->FireRelease();
	}
}

void APlayerController_FPS::ReleaseWeaponInput()
{
	auto MyPawn = GetPawn<ACharacter_FPS>();

	if (IsValid(MyPawn))
	{
		MyPawn->ReleaseWeapon();
	}
}
