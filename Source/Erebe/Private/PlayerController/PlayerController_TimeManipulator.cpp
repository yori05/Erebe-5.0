// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/PlayerController_TimeManipulator.h"
#include "Engine/World.h"
#include "GameMode/GameMode_TimeManipulator.h"
#include "Actors/TimeManipulatorManager.h"
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

	if (GetWorld() != nullptr && GetWorld()->GetAuthGameMode() != nullptr)
	{
		GMTimeManip = Cast<AGameMode_TimeManipulator>(GetWorld()->GetAuthGameMode());
		
		if (GMTimeManip != nullptr)
		{
			TimeManipManager = GMTimeManip->GetTimeManipulatorManager();
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
	if (TimeManipManager != nullptr)
	{
		//TimeManipManager->UnholdReplay();
	}
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
	if (TimeManipManager != nullptr)
	{
		//TimeManipManager->UnholdReplay();
	}
}

