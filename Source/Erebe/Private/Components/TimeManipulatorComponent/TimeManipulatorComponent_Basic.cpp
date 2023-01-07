// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/TimeManipulatorComponent/TimeManipulatorComponent_Basic.h"
#include "Engine/World.h"
#include "GameMode/GameMode_TimeManipulator.h"
#include "Components/TimeManipulatorComponent/TimeManipulatorComponent_Manager.h"


/**-----------------	Constructor Part		-----------------*/

// Sets default values for this component's properties
UTimeManipulatorComponent_Basic::UTimeManipulatorComponent_Basic()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ComponentTags.Add(TEXT("TimeManipulator"));

	TimeSaved = 0.f;
	// ...
}

/**-----------------	Inherit Function Part		-----------------*/

// Called when the game starts
void UTimeManipulatorComponent_Basic::BeginPlay()
{
	Super::BeginPlay();

	CreateCurves();

	if (GetOwner() != nullptr)
	{
		if (bCanRecord)
		{
			SetCurves(TimeSaved, true);
		}
	}
	
	RequestLinkToManager();
}


// Called every frame
void UTimeManipulatorComponent_Basic::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bCanUpdate)
	{
		if (bCanRecord)
		{
			UpdateTimeByStep(DeltaTime);

			if (bHasAStep)
			{
				RecordCurves(TimeSaved);
				bHasAStep = false;
			}
		}
		if (bReverse)
		{
			UpdateTime(DeltaTime);
			ReplayCurves(TimeSaved);
		}
	}
}

// Called when the game end for this component
void UTimeManipulatorComponent_Basic::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	RemoveLinkToManager();
}

/**-----------------	TimeManipulatorPart Function Part		-----------------*/
/**-----------------	Management Part		-----------------*/

void UTimeManipulatorComponent_Basic::RequestLinkToManager()
{
	if (GetWorld() != nullptr)
	{
		auto GM = GetWorld()->GetAuthGameMode();

		if (IsValid(GM))
		{
			UTimeManipulatorComponent_Manager* TimeManager = Cast<UTimeManipulatorComponent_Manager>(GM->GetComponentByClass(UTimeManipulatorComponent_Manager::StaticClass()));

			if (IsValid(TimeManager))
			{
				bool bRequest = TimeManager->RequestAdd(this);

				if (bRequest)
				{
					TimeManager->OnFastBackwardTimeState.AddUniqueDynamic(this, &UTimeManipulatorComponent_Basic::ActivateFastReplay);
					TimeManager->OnBackwardTimeState.AddUniqueDynamic(this, &UTimeManipulatorComponent_Basic::ActivateReplay);
					TimeManager->OnStopTimeState.AddUniqueDynamic(this, &UTimeManipulatorComponent_Basic::ActivateStop);
					TimeManager->OnForwardTimeState.AddUniqueDynamic(this, &UTimeManipulatorComponent_Basic::ActivateRecord);
					TimeManager->OnFastForwardTimeState.AddUniqueDynamic(this, &UTimeManipulatorComponent_Basic::ActivateFastRecord);
				}
			}
		}
	}
}

void UTimeManipulatorComponent_Basic::RemoveLinkToManager()
{
	if (GetWorld() != nullptr)
	{
		auto GM = GetWorld()->GetAuthGameMode();

		if (IsValid(GM))
		{
			UTimeManipulatorComponent_Manager* TimeManager = Cast<UTimeManipulatorComponent_Manager>(GM->GetComponentByClass(UTimeManipulatorComponent_Manager::StaticClass()));

			if (IsValid(TimeManager))
			{
				if (TimeManager->OnFastBackwardTimeState.IsAlreadyBound(this, &UTimeManipulatorComponent_Basic::ActivateFastReplay))
				{
					TimeManager->OnBackwardTimeState.RemoveDynamic(this, &UTimeManipulatorComponent_Basic::ActivateReplay);
				}
				if (TimeManager->OnBackwardTimeState.IsAlreadyBound(this, &UTimeManipulatorComponent_Basic::ActivateReplay))
				{
					TimeManager->OnBackwardTimeState.RemoveDynamic(this, &UTimeManipulatorComponent_Basic::ActivateReplay);
				}
				if (TimeManager->OnStopTimeState.IsAlreadyBound(this, &UTimeManipulatorComponent_Basic::ActivateStop))
				{
					TimeManager->OnStopTimeState.RemoveDynamic(this, &UTimeManipulatorComponent_Basic::ActivateStop);
				}
				if (TimeManager->OnForwardTimeState.IsAlreadyBound(this, &UTimeManipulatorComponent_Basic::ActivateRecord))
				{
					TimeManager->OnForwardTimeState.RemoveDynamic(this, &UTimeManipulatorComponent_Basic::ActivateRecord);
				}
				if (TimeManager->OnFastForwardTimeState.IsAlreadyBound(this, &UTimeManipulatorComponent_Basic::ActivateFastRecord))
				{
					TimeManager->OnFastForwardTimeState.RemoveDynamic(this, &UTimeManipulatorComponent_Basic::ActivateFastRecord);
				}
			}
		}
	}
}

/**-----------------	Time State Part		-----------------*/

void UTimeManipulatorComponent_Basic::ActivateFastRecord(float TimeDilatation)
{
	bCanRecord = true;
	bReverse = false;
	bCanUpdate = true;

	if (IsValid(GetOwner()))
	{
		GetOwner()->CustomTimeDilation = TimeDilatation;
	}

	ReceiveActivateFastRecord(TimeDilatation);
	OnActivateFastRecord.Broadcast();
}

void UTimeManipulatorComponent_Basic::ActivateRecord()
{
	bCanRecord = true;
	bReverse = false;
	bCanUpdate = true;

	if (IsValid(GetOwner()))
	{
		GetOwner()->CustomTimeDilation = 1.f;
	}

	ReceiveActivateRecord();
	OnActivateRecord.Broadcast();
}

void UTimeManipulatorComponent_Basic::ActivateStop()
{
	bCanRecord = false;
	bReverse = false;
	bCanUpdate = false;

	if (IsValid(GetOwner()))
	{
		GetOwner()->CustomTimeDilation = 1.f;
	}

	ReceiveActivateStop();
	OnActivateStop.Broadcast();
}


void UTimeManipulatorComponent_Basic::ActivateReplay()
{
	bCanRecord = false;
	bReverse = true;
	bCanUpdate = true;

	if (IsValid(GetOwner()))
	{
		GetOwner()->CustomTimeDilation = 1.f;
	}

	ReceiveActivateReplay();
	OnActivateReplay.Broadcast();
}

void UTimeManipulatorComponent_Basic::ActivateFastReplay(float TimeDilatation)
{
	bCanRecord = false;
	bReverse = true;
	bCanUpdate = true;

	if (IsValid(GetOwner()))
	{
		GetOwner()->CustomTimeDilation = TimeDilatation;
	}

	ReceiveActivateFastReplay(TimeDilatation);
	OnActivateFastReplay.Broadcast();
}

/**-----------------	Time Parameter Part		-----------------*/

void UTimeManipulatorComponent_Basic::UpdateTime(float _DeltaTime)
{
	TimeSaved += (bReverse ? -_DeltaTime : _DeltaTime);
	TimeElapsed += (bReverse ? -_DeltaTime : _DeltaTime);

	if (TimeElapsed <= 0.f)
	{
		TimeElapsed += TimeStep;
		bHasAReverseStep = true;
	}
	else if (TimeElapsed >= TimeStep)
	{
		TimeElapsed -= TimeStep;
		bHasAStep = true;
	}
}

void UTimeManipulatorComponent_Basic::UpdateTimeByStep(float _DeltaTime)
{
	TimeElapsed += (bReverse ? -_DeltaTime : _DeltaTime);

	if (TimeElapsed >= TimeStep)
	{
		TimeSaved += TimeStep;
		TimeElapsed -= TimeStep;
		bHasAStep = true;
	}
	else if (TimeElapsed <= 0.f)
	{
		TimeSaved -= TimeStep;
		TimeElapsed += TimeStep;
		bHasAReverseStep = true;
	}
}

/**-----------------	Curve Creation / Destroy Part		-----------------*/

void UTimeManipulatorComponent_Basic::CreateCurves()
{
	//Create curves for this component
}

/**-----------------	Set Curve Part		-----------------*/

void UTimeManipulatorComponent_Basic::SetCurves(float _TimeScopted, bool _bZeroAndScoped)
{
	ResetCurves();

	if (_bZeroAndScoped)
	{
		RecordCurves(0.f);
	}

	RecordCurves(_TimeScopted);
}

void UTimeManipulatorComponent_Basic::ResetCurves() 
{
	//Reset call for all curves of this components
}


/**-----------------	Record Curve Part		-----------------*/

void UTimeManipulatorComponent_Basic::RecordCurves(float _TimeScopted)
{
	//Record call for all curves of this components
}


/**-----------------	Replay Curve Part		-----------------*/

void UTimeManipulatorComponent_Basic::ReplayCurves(float _TimeScopted)
{
	//Replay call for all curves of this components
}

/**-----------------	TimeManipulatorPart Accessor Part		-----------------*/

void UTimeManipulatorComponent_Basic::SetTimeComponent(float _NewTimeSaved, float _NewTimeElapsed)
{
	TimeSaved = _NewTimeSaved;
	TimeElapsed = _NewTimeElapsed;
}

void UTimeManipulatorComponent_Basic::SetCanUpdate(bool _bNewCanUpdate)
{
	bCanUpdate = _bNewCanUpdate;
}
