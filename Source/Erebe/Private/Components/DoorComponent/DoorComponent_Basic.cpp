// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DoorComponent/DoorComponent_Basic.h"
#include "Curves/CurveFloat.h"

// Sets default values for this component's properties
UDoorComponent_Basic::UDoorComponent_Basic()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	bCanGetOppositeDirection = true;

	TimeMin = -1.f;
	TimeMax = 5.f;

}


// Called when the game starts
void UDoorComponent_Basic::BeginPlay()
{
	Super::BeginPlay();

	// ...
	

	if (IsValid(UsedCurve))
	{
		UsedCurve->GetTimeRange(TimeMin, TimeMax);
	}
}


// Called every frame
void UDoorComponent_Basic::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	if (bUpdateWithTick)
	{
		UpdateDoor(DeltaTime);
	}
}

void UDoorComponent_Basic::CallUpdateDoor(float DeltaTime)
{
	if (!bUpdateWithTick)
	{
		UpdateDoor(DeltaTime);
	}
}

void UDoorComponent_Basic::ChangeState()
{
	if (!GetIsBlocked())
	{
		switch (DoorState)
		{
		case EDoorStates::DOORSTATES_Closed:
			DoorState = EDoorStates::DOORSTATES_Openning;
			break;
		case EDoorStates::DOORSTATES_Closing:
			DoorState = EDoorStates::DOORSTATES_Openning;
			break;
		case EDoorStates::DOORSTATES_Openning:
			DoorState = EDoorStates::DOORSTATES_Closing;
			break;
		case EDoorStates::DOORSTATES_Openned:
			DoorState = EDoorStates::DOORSTATES_Closing;
			break;
		case EDoorStates::DOORSTATES_StuckOpening:
			DoorState = (bOppositeAfterStuck) ? EDoorStates::DOORSTATES_Closing
				: EDoorStates::DOORSTATES_Openning;
			break;
		case EDoorStates::DOORSTATES_StuckClosing:
			DoorState = (bOppositeAfterStuck) ? EDoorStates::DOORSTATES_Openning
				: EDoorStates::DOORSTATES_Closing;
			break;
		case EDoorStates::DOORSTATES_Max:
			StuckDoor();
			return;
			break;
		default:
			break;
		}
	}
}

void UDoorComponent_Basic::UpdateDoor(float DeltaTime)
{
	if (GetIsOpenning() || GetIsClosing())
	{
		if (GetIsOpenning())
		{
			TimeValue += DeltaTime;

			if (TimeValue >= TimeMax)
			{
				TimeValue = TimeMax;
				DoorState = EDoorStates::DOORSTATES_Openned;
				OnDoorOpen.Broadcast();
				OnDoorFinish.Broadcast();
			}
			else
			{
				OnDoorMoving.Broadcast();
			}
		}
		else
		{
			TimeValue -= DeltaTime;

			if (TimeValue <= TimeMin)
			{
				TimeValue = TimeMin;
				DoorState = EDoorStates::DOORSTATES_Closed;
				OnDoorClose.Broadcast();
				OnDoorFinish.Broadcast();
			}
			else
			{
				OnDoorMoving.Broadcast();
			}
		}

		UpdateRatioValue(TimeValue);
	}
}

void UDoorComponent_Basic::UpdateRatioValue(float _TimeValue)
{
	if (IsValid(UsedCurve))
	{
		RatioValue = UsedCurve->GetFloatValue(_TimeValue) * ((bIsOpposedDirection) ? -1.f : 1.f);
	}
	else
	{
		if ((TimeMax - TimeMin) * (TimeMax - TimeMin) > 0.f)
		{
			//Use it to have a ratio of TimeValue between TimeMin and TimeMax 
			RatioValue = (_TimeValue - TimeMin) / (TimeMax - TimeMin) * ((bIsOpposedDirection) ? -1.f : 1.f);
			RatioValue = FMath::Clamp(RatioValue, 0.f, 1.f);
		}
		else
		{
			RatioValue = _TimeValue * ((bIsOpposedDirection) ? -1.f : 1.f);
		}
	}
}

void UDoorComponent_Basic::OpenCloseDoor()
{
	ChangeState();
}

void UDoorComponent_Basic::OpenDoor()
{
	if (GetIsClosing() || GetIsClose())
	{
		ChangeState();
	}
}

void UDoorComponent_Basic::OpenDoorDefault()
{
	if (GetIsClosing() || GetIsClose())
	{
		bIsOpposedDirection = false;
		ChangeState();
	}
}

void UDoorComponent_Basic::OpenDoorReverse()
{
	if (GetIsClosing() || GetIsClose())
	{
		bIsOpposedDirection = true;
		ChangeState();
	}
}

void UDoorComponent_Basic::CloseDoor()
{
	if (GetIsOpenning() || GetIsOpen())
	{
		ChangeState();
	}
}

void UDoorComponent_Basic::StuckDoor()
{
	if (GetIsOpenning() || GetIsClosing())
	{
		DoorState = EDoorStates::DOORSTATES_StuckOpening;
		OnDoorStuck.Broadcast();
	}
	if (GetIsOpenning() || GetIsClosing())
	{
		DoorState = EDoorStates::DOORSTATES_StuckClosing;
		OnDoorStuck.Broadcast();
	}
}

void UDoorComponent_Basic::BlockDoor()
{
	if (!bIsBlocked)
	{
		StuckDoor();

		bIsBlocked = true;
		OnDoorBlock.Broadcast();
	}
}

void UDoorComponent_Basic::UnblockDoor()
{
	if (bIsBlocked)
	{
		bIsBlocked = false;
		OnDoorUnblock.Broadcast();

		if (bMoveOnUnblockStuck)
		{
			ChangeState();
		}
	}
}
