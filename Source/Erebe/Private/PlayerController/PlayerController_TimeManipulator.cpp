// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/PlayerController_TimeManipulator.h"
#include "Engine/World.h"
#include "GameFramework/GameModeBase.h"
#include "Components/TimeManipulatorComponent/TimeManipulatorComponent_Manager.h"

APlayerController_TimeManipulator::APlayerController_TimeManipulator(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
}

void APlayerController_TimeManipulator::BeginPlay()
{
	if (IsValid(GetWorld()))
	{
		auto GM = GetWorld()->GetAuthGameMode();

		if (IsValid(GM))
		{
			TimeManipCManager = Cast<UTimeManipulatorComponent_Manager>(GM->GetComponentByClass(UTimeManipulatorComponent_Manager::StaticClass()));
		}
	}
}

/**-----------------	Inherit Function Part		-----------------*/
void APlayerController_TimeManipulator::SetupInputComponent()
{
	Super::SetupInputComponent();

	check(InputComponent);


	/** TimeManipulation Input */
	InputComponent->BindAction("BackwardTime", IE_Pressed, this, &APlayerController_TimeManipulator::HoldBackwardTimeInput);
	InputComponent->BindAction("BackwardTime", IE_Released, this, &APlayerController_TimeManipulator::UnholdBackwardTimeInput);

	/** TimeManipulation Input */
	InputComponent->BindAction("ForwardTime", IE_Pressed, this, &APlayerController_TimeManipulator::HoldForwardTimeInput);
	InputComponent->BindAction("ForwardTime", IE_Released, this, &APlayerController_TimeManipulator::UnholdForwardTimeInput);

}

void APlayerController_TimeManipulator::HoldBackwardTimeInput()
{
	if (IsValid(TimeManipCManager) && TimeManipCManager->CanMoveBackTime())
	{
		TimeManipCManager->MoveBackTime();
	}
}

void APlayerController_TimeManipulator::UnholdBackwardTimeInput()
{

}

void APlayerController_TimeManipulator::HoldForwardTimeInput()
{
	if (IsValid(TimeManipCManager) && TimeManipCManager->CanMoveFrontTime())
	{
		TimeManipCManager->MoveFrontTime();
	}
}

void APlayerController_TimeManipulator::UnholdForwardTimeInput()
{

}

