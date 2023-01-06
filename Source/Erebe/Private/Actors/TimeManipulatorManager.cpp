// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/TimeManipulatorManager.h"
#include "Components/TimeManipulatorComponent/TimeManipulatorComponent_Basic.h"

// Sets default values
ATimeManipulatorManager::ATimeManipulatorManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATimeManipulatorManager::BeginPlay()
{
	Super::BeginPlay();
	

}

// Called every frame
void ATimeManipulatorManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateTime(DeltaTime);
}

bool ATimeManipulatorManager::RequestAdd(UTimeManipulatorComponent_Basic* _NewComponent)
{
	if (_NewComponent != nullptr)
	{
		TimeManipulatorComponents.Add(_NewComponent);
	}

	return false;
}


void ATimeManipulatorManager::SetTimeComponent(float _NewTimeComponent)
{
	TimeSaved = _NewTimeComponent;
}

void ATimeManipulatorManager::UpdateTime(float _DeltaTime)
{
	TimeSaved += _DeltaTime;
}

void ATimeManipulatorManager::LaunchReplay()
{
	for (auto& i : TimeManipulatorComponents)
	{
		i->ActivateReplay();
	}
}

void ATimeManipulatorManager::LaunchStop()
{
	for (auto& i : TimeManipulatorComponents)
	{
		i->ActivateStop();
	}
}

void ATimeManipulatorManager::LaunchRecord()
{
	for (auto& i : TimeManipulatorComponents)
	{
		i->ActivateRecord();
	}
}

//void ATimeManipulatorManager::CallBackwardTime()
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
//void ATimeManipulatorManager::CallForwardTime()
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

void ATimeManipulatorManager::ChangeTimeState(ETimeStates _NewTimeState)
{
	ETimeStates PrevState = ETimeState;
	ETimeState = _NewTimeState;

	OnChangeTimeState.Broadcast(ETimeState, PrevState);
	ReceiveChangeTimeState(ETimeState);

	switch (ETimeState)
	{
	case ETimeStates::TIMESTATES_Forward:
		ForwardTime();
		break;
	case ETimeStates::TIMESTATES_Stop:
		StopTime();
		break;
	case ETimeStates::TIMESTATES_Backward:
		BackwardTime();
		break;
	case ETimeStates::TIMESTATES_Max:
		break;
	default:
		break;
	}
}

void ATimeManipulatorManager::BackwardTime()
{
	//LaunchReplay();
	OnBackwardTimeState.Broadcast();
	ReceiveBackwardTime();
}

void ATimeManipulatorManager::StopTime()
{
	//LaunchStop();
	OnStopTimeState.Broadcast();
	ReceiveStopTime();
}

void ATimeManipulatorManager::ForwardTime()
{
	//LaunchRecord();
	OnForwardTimeState.Broadcast();
	ReceiveForwardTime();
}
