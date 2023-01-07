// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/TimeManipulatorComponent/TimeManipulatorComponent_Manager.h"
#include "Components/TimeManipulatorComponent/TimeManipulatorComponent_Basic.h"


// Sets default values for this component's properties
UTimeManipulatorComponent_Manager::UTimeManipulatorComponent_Manager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ComponentTags.Add(TEXT("TimeManipulator"));
	LaunchForwardTime();

	// ...
}

// Called when the game starts
void UTimeManipulatorComponent_Manager::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void UTimeManipulatorComponent_Manager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	UpdateTime(DeltaTime);
}

bool UTimeManipulatorComponent_Manager::RequestAdd(UTimeManipulatorComponent_Basic* _NewComponent)
{
	if (_NewComponent != nullptr)
	{
		TimeManipulatorComponents.Add(_NewComponent);

		return true;
	}

	return false;
}


void UTimeManipulatorComponent_Manager::SetTimeComponent(float _NewTimeComponent)
{
	TimeSaved = _NewTimeComponent;
}

void UTimeManipulatorComponent_Manager::UpdateTime(float _DeltaTime)
{
	if (IsInBackwardTimeState())
	{
		TimeSaved -= _DeltaTime;
		TimeElapsed -= _DeltaTime;

		if (TimeElapsed <= 0.f)
		{
			TimeElapsed += TimeStep;
		}

		if (TimeSaved <= 0.f)
		{
			TimeSaved = 0.f;

			LaunchStopTime();
		}
	}
	else if (IsInForwardTimeState())
	{
		TimeSaved += _DeltaTime;
		TimeElapsed += _DeltaTime;

		if (TimeElapsed >= TimeStep)
		{
			TimeElapsed -= TimeStep;
		}
	}
}

void UTimeManipulatorComponent_Manager::LaunchFastBackwardTime()
{
	if (CanLaunchFastBackwardTime())
	{
		ChangeTimeState(ETimeStates::TIMESTATES_FastBackward);
	}
}

bool UTimeManipulatorComponent_Manager::CanLaunchFastBackwardTime()
{
	if (!IsInFastBackwardTimeState())
	{
		return bCanFastTimeState;
	}

	return false;
}

void UTimeManipulatorComponent_Manager::LaunchBackwardTime()
{
	if (CanLaunchBackwardTime())
	{
		ChangeTimeState(ETimeStates::TIMESTATES_Backward);
	}
}

bool UTimeManipulatorComponent_Manager::CanLaunchBackwardTime()
{
	if (!IsInBackwardTimeState())
	{
		return true;
	}

	return false;
}

void UTimeManipulatorComponent_Manager::LaunchStopTime()
{
	if (CanLaunchStopTime())
	{
		ChangeTimeState(ETimeStates::TIMESTATES_Stop);
	}
}

bool UTimeManipulatorComponent_Manager::CanLaunchStopTime()
{
	if (!IsInStopTimeState())
	{
		return true;
	}

	return false;
}

void UTimeManipulatorComponent_Manager::LaunchForwardTime()
{
	if (CanLaunchForwardTime())
	{
		ChangeTimeState(ETimeStates::TIMESTATES_Forward);
	}
}

bool UTimeManipulatorComponent_Manager::CanLaunchForwardTime()
{
	if (!IsInForwardTimeState())
	{
		return true;
	}

	return false;
}

void UTimeManipulatorComponent_Manager::LaunchFastForwardTime()
{
	if (CanLaunchFastForwardTime())
	{
		ChangeTimeState(ETimeStates::TIMESTATES_FastForward);
	}
}

bool UTimeManipulatorComponent_Manager::CanLaunchFastForwardTime()
{
	if (!IsInFastForwardTimeState())
	{
		return bCanFastTimeState;
	}

	return false;
}

void UTimeManipulatorComponent_Manager::MoveBackTime()
{
	if (CanMoveBackTime())
	{
		if (IsInForwardTimeState())
		{
			LaunchStopTime();
		}
		else if (IsInStopTimeState())
		{
			LaunchBackwardTime();
		}
		else if (IsInBackwardTimeState())
		{
			LaunchFastBackwardTime();
		}
	}
}

bool UTimeManipulatorComponent_Manager::CanMoveBackTime()
{
	if (IsInBackwardTimeState())
	{
		return bCanFastTimeState;
	}

	return !IsInFastBackwardTimeState();
}

void UTimeManipulatorComponent_Manager::MoveFrontTime()
{
	if (CanMoveFrontTime())
	{
		if (IsInBackwardTimeState())
		{
			LaunchStopTime();
		}
		else if (IsInStopTimeState())
		{
			LaunchForwardTime();
		}
		else if (IsInForwardTimeState())
		{
			LaunchFastForwardTime();
		}
	}
}

bool UTimeManipulatorComponent_Manager::CanMoveFrontTime()
{
	if (IsInForwardTimeState())
	{
		return bCanFastTimeState;
	}

	return !IsInFastForwardTimeState();
}

void UTimeManipulatorComponent_Manager::ChangeTimeState(ETimeStates _NewTimeState)
{
	ETimeStates PrevState = ETimeState;
	ETimeState = _NewTimeState;

	OnChangeTimeState.Broadcast(ETimeState, PrevState);
	ReceiveChangeTimeState(ETimeState);

	switch (ETimeState)
	{
	case ETimeStates::TIMESTATES_FastForward:
		FastForwardTime();
		break;
	case ETimeStates::TIMESTATES_Forward:
		ForwardTime();
		break;
	case ETimeStates::TIMESTATES_Stop:
		StopTime();
		break;
	case ETimeStates::TIMESTATES_Backward:
		BackwardTime();
		break;
	case ETimeStates::TIMESTATES_FastBackward:
		FastBackwardTime();
		break;
	default:
		break;
	}
}


void UTimeManipulatorComponent_Manager::FastBackwardTime()
{
	GetOwner()->CustomTimeDilation = FastTimeRatio;
	ReceiveFastBackwardTime();
	OnFastBackwardTimeState.Broadcast(FastTimeRatio);
}

void UTimeManipulatorComponent_Manager::BackwardTime()
{
	GetOwner()->CustomTimeDilation = 1.f;
	ReceiveBackwardTime();
	OnBackwardTimeState.Broadcast();
}

void UTimeManipulatorComponent_Manager::StopTime()
{
	GetOwner()->CustomTimeDilation = 1.f;
	ReceiveStopTime();
	OnStopTimeState.Broadcast();
}

void UTimeManipulatorComponent_Manager::ForwardTime()
{
	GetOwner()->CustomTimeDilation = 1.f;
	ReceiveForwardTime();
	OnForwardTimeState.Broadcast();
}

void UTimeManipulatorComponent_Manager::FastForwardTime()
{
	GetOwner()->CustomTimeDilation = FastTimeRatio;
	ReceiveFastBackwardTime();
	OnFastBackwardTimeState.Broadcast(FastTimeRatio);
}

