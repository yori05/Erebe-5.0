// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/GameMode_TimeManipulator.h"
#include "Components/TimeManipulatorComponent/TimeManipulatorComponent_Manager.h"
#include "Character/Character_TimeManipulator.h"
#include "PlayerController/PlayerController_TimeManipulator.h"
#include "Actors/TimeManipulatorManager.h"
#include "Engine/World.h"


AGameMode_TimeManipulator::AGameMode_TimeManipulator()
{
	DefaultPawnClass = ACharacter_TimeManipulator::StaticClass();
	PlayerControllerClass = APlayerController_TimeManipulator::StaticClass();

	// Create Time Manipulator Manager Component 
	TimeManagerComponent = CreateDefaultSubobject<UTimeManipulatorComponent_Manager>(TEXT("TimeManipulatorManager"));
}

void AGameMode_TimeManipulator::StartPlay()
{
	//if (TimeManagerClass == nullptr)
	//{
	//	TimeManagerClass = ATimeManipulatorManager::StaticClass();
	//}
	//
	//if (GetWorld() != nullptr)
	//{
	//	FTransform SpawnLocation;
	//	TimeManager = (ATimeManipulatorManager*) GetWorld()->SpawnActor(TimeManagerClass, &SpawnLocation);
	//}
	//
	Super::StartPlay();

	
}


//void AGameMode_TimeManipulator::CallBackwardTime()
//{
//	if (ETimeState != ETimeStates::TIMESTATES_Backward && ETimeState < ETimeStates::TIMESTATES_Max)
//	{
//		ChangeTimeState((ETimeStates)((uint8)ETimeState + 1));
//	}
//	//if (ETimeState == ETimeStates::TIMESTATES_Forward)
//	//{
//	//	ChangeTimeState(ETimeStates::TIMESTATES_Stop);
//	//}
//	//else if (ETimeState == ETimeStates::TIMESTATES_Stop)
//	//{
//	//	ChangeTimeState(ETimeStates::TIMESTATES_Backward);
//	//}
//}
//
//void AGameMode_TimeManipulator::CallForwardTime()
//{
//	if (ETimeState != ETimeStates::TIMESTATES_Forward && (uint8)ETimeState > (uint8)0)
//	{
//		ChangeTimeState((ETimeStates)((uint8)ETimeState - 1));
//	}
//	//if (ETimeState == ETimeStates::TIMESTATES_Backward)
//	//{
//	//	ChangeTimeState(ETimeStates::TIMESTATES_Stop);
//	//}
//	//else if (ETimeState == ETimeStates::TIMESTATES_Stop)
//	//{
//	//	ChangeTimeState(ETimeStates::TIMESTATES_Forward);
//	//}
//}
//
//void AGameMode_TimeManipulator::ChangeTimeState(ETimeStates _NewTimeState)
//{
//	ETimeStates PrevState = ETimeState;
//	ETimeState = _NewTimeState;
//
//	OnChangeTimeState.Broadcast(ETimeState, PrevState);
//	ReceiveChangeTimeState(ETimeState);
//
//	switch (ETimeState)
//	{
//	case ETimeStates::TIMESTATES_Forward:
//		ForwardTime();
//		break;
//	case ETimeStates::TIMESTATES_Stop:
//		StopTime();
//		break;
//	case ETimeStates::TIMESTATES_Backward:
//		BackwardTime();
//		break;
//	case ETimeStates::TIMESTATES_Max:
//		break;
//	default:
//		break;
//	}
//}
//
//void AGameMode_TimeManipulator::BackwardTime()
//{
//	OnBackwardTimeState.Broadcast();
//	ReceiveBackwardTime();
//}
//
//void AGameMode_TimeManipulator::StopTime()
//{
//	OnStopTimeState.Broadcast();
//	ReceiveStopTime();
//}
//
//void AGameMode_TimeManipulator::ForwardTime()
//{
//	OnForwardTimeState.Broadcast();
//	ReceiveForwardTime();
//}
//