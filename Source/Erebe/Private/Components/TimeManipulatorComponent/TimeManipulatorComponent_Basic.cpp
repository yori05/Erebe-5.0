// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/TimeManipulatorComponent/TimeManipulatorComponent_Basic.h"
#include "Engine/CurveTable.h"
#include "Curves/CurveVector.h"
#include "Curves/CurveFloat.h"
#include "Engine/World.h"
#include "GameMode/GameMode_TimeManipulator.h"
#include "Actors/TimeManipulatorManager.h"
#include "Components/TimeManipulatorComponent/TimeManipulatorComponent_Manager.h"
#include "Components/PrimitiveComponent.h"

/**-----------------	Constructor Part		-----------------*/

// Sets default values for this component's properties
UTimeManipulatorComponent_Basic::UTimeManipulatorComponent_Basic()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	TimeSaved = 0.f;
	// ...
}

/**-----------------	Inherit Function Part		-----------------*/

// Called when the game starts
void UTimeManipulatorComponent_Basic::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(RotationAdvanced))
	{
		RotationAdvanced = NewObject<UCurveVector>();
	}
	if (!IsValid(PositionRecorded))
	{
		PositionRecorded = NewObject<UCurveVector>();
	}

	if (!IsValid(RotationRecorded))
	{
		RotationRecorded = NewObject<UCurveVector>();
	}

	if (!IsValid(RotationWRecorded))
	{
		RotationWRecorded = NewObject<UCurveFloat>();
	}

	if (GetOwner() != nullptr)
	{
		if (bCanRecord)
		{
			SetCurves(TimeSaved, true);
		}
	}
	// ...
	
	if (GetWorld() != nullptr)
	{
		//AGameMode_TimeManipulator* GMTM = Cast<AGameMode_TimeManipulator>(GetWorld()->GetAuthGameMode());
		//
		//if (GMTM != nullptr && GMTM->GetTimeManipulatorManager() != nullptr)
		//{
		//	if (GMTM->GetTimeManipulatorManager()->RequestAdd(this))
		//	{
		//		GMTM->GetTimeManipulatorManager()->OnBackwardTimeState.AddUniqueDynamic(this, &UTimeManipulatorComponent_Basic::ActivateReplay);
		//		GMTM->GetTimeManipulatorManager()->OnStopTimeState.AddUniqueDynamic(this, &UTimeManipulatorComponent_Basic::ActivateStop);
		//		GMTM->GetTimeManipulatorManager()->OnForwardTimeState.AddUniqueDynamic(this, &UTimeManipulatorComponent_Basic::ActivateRecord);
		//	}
		//}

		auto GM = GetWorld()->GetAuthGameMode();

		if (IsValid(GM))
		{
			UTimeManipulatorComponent_Manager* TimeManager = Cast<UTimeManipulatorComponent_Manager>(GM->GetComponentByClass(UTimeManipulatorComponent_Manager::StaticClass()));
			
			if (IsValid(TimeManager))
			{
				bool bRequest = TimeManager->RequestAdd(this);

				if (bRequest)
				{
					TimeManager->OnBackwardTimeState.AddUniqueDynamic(this, &UTimeManipulatorComponent_Basic::ActivateReplay);
					TimeManager->OnStopTimeState.AddUniqueDynamic(this, &UTimeManipulatorComponent_Basic::ActivateStop);
					TimeManager->OnForwardTimeState.AddUniqueDynamic(this, &UTimeManipulatorComponent_Basic::ActivateRecord);
				}
			}
		}
	}

	if (SavedPhysicComponent == nullptr)
	{
		if (GetOwner() != nullptr)
		{
			UActorComponent* AcTemp = GetOwner()->GetComponentByClass(UPrimitiveComponent::StaticClass());
			SavedPhysicComponent = Cast<UPrimitiveComponent>(AcTemp);
		}
	}
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

void UTimeManipulatorComponent_Basic::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (GetWorld() != nullptr)
	{
		auto GM = GetWorld()->GetAuthGameMode();

		if (IsValid(GM))
		{
			UTimeManipulatorComponent_Manager* TimeManager = Cast<UTimeManipulatorComponent_Manager>(GM->GetComponentByClass(UTimeManipulatorComponent_Manager::StaticClass()));

			if (IsValid(TimeManager))
			{
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
			}
		}
	}
}

/**-----------------	TimeManipulatorPart Function Part		-----------------*/
/**-----------------	Record / Replay Part		-----------------*/

void UTimeManipulatorComponent_Basic::ActivateRecord()
{
	if (SavedPhysicComponent != nullptr)
	{
		//SavedPhysicComponent->SetEnableGravity(true);
		SavedPhysicComponent->SetSimulatePhysics(true);
	}

	bCanRecord = true;
	bReverse = false;
	bCanUpdate = true;
}

void UTimeManipulatorComponent_Basic::ActivateStop()
{
	if (SavedPhysicComponent != nullptr)
	{
		//SavedPhysicComponent->SetEnableGravity(false);
		SavedPhysicComponent->SetSimulatePhysics(false);
	}

	bCanRecord = false;
	bReverse = false;
	bCanUpdate = false;
}

void UTimeManipulatorComponent_Basic::ActivateReplay()
{
	if (SavedPhysicComponent != nullptr)
	{
		//SavedPhysicComponent->SetEnableGravity(false);
		SavedPhysicComponent->SetSimulatePhysics(false);
	}

	bCanRecord = false;
	bReverse = true;
	bCanUpdate = true;
}

/**-----------------	Time Parameter Part		-----------------*/

void UTimeManipulatorComponent_Basic::UpdateTime(float _DeltaTime)
{
	if (bReverse)
	{
		TimeSaved -= _DeltaTime;
		TimeElapsed -= _DeltaTime;

		if (TimeElapsed <= 0.f)
		{
			TimeElapsed += TimeStep;
		}
	}
	else
	{
		TimeSaved += _DeltaTime;
		TimeElapsed += _DeltaTime;

		if (TimeElapsed >= TimeStep)
		{
			TimeElapsed -= TimeStep;
		}
	}
}

void UTimeManipulatorComponent_Basic::UpdateTimeByStep(float _DeltaTime)
{
	TimeElapsed += _DeltaTime;

	if (TimeElapsed >= TimeStep)
	{
		TimeSaved += (bReverse ? -TimeStep : TimeStep);
		TimeElapsed -= TimeStep;
		bHasAStep = true;
	}
}

/**-----------------	Set Curve Part		-----------------*/

void UTimeManipulatorComponent_Basic::SetCurves(float _TimeScopted, bool _bDoItTimeBothZeroAndScopted)
{
	ResetCurves();

	if (_bDoItTimeBothZeroAndScopted)
	{
		RecordCurves(0.f);
	}

	RecordCurves(_TimeScopted);
}

void UTimeManipulatorComponent_Basic::ResetCurves() 
{
	ResetLocationCurve();
	ResetRotationCurve();
}

void UTimeManipulatorComponent_Basic::ResetLocationCurve()
{
	if (IsValid(PositionRecorded))
	{
		PositionRecorded->ResetCurve();
	}
}

void UTimeManipulatorComponent_Basic::ResetRotationCurve()
{
	if (IsValid(RotationRecorded))
	{
		RotationRecorded->ResetCurve();
	}
	if (IsValid(RotationWRecorded))
	{
		RotationWRecorded->ResetCurve();
	}
	if (IsValid(RotationAdvanced))
	{
		RotationAdvanced->ResetCurve();
	}
}

/**-----------------	Record Curve Part		-----------------*/

void UTimeManipulatorComponent_Basic::RecordCurves(float _TimeScopted)
{
	RecordLocationCurve(_TimeScopted);
	RecordRotationCurve(_TimeScopted);
}

void UTimeManipulatorComponent_Basic::RecordLocationCurve(float _TimeScopted)
{
	if (GetOwner() != nullptr)
	{
		LocationVector = GetOwner()->GetActorLocation();

		if (IsValid(PositionRecorded))
		{
			PositionRecorded->FloatCurves[0].UpdateOrAddKey(_TimeScopted, LocationVector.X, false, TimeStep / 2.f);
			PositionRecorded->FloatCurves[1].UpdateOrAddKey(_TimeScopted, LocationVector.Y, false, TimeStep / 2.f);
			PositionRecorded->FloatCurves[2].UpdateOrAddKey(_TimeScopted, LocationVector.Z, false, TimeStep / 2.f);
		}
	}
}


void UTimeManipulatorComponent_Basic::RecordRotationCurve(float _TimeScopted)
{
	if (GetOwner() != nullptr)
	{
		//RotationRecorded->FloatCurves[0].UpdateOrAddKey(_TimeScopted, RotationRotator.Euler().X, false, TimeStep / 2.f);
		//RotationRecorded->FloatCurves[1].UpdateOrAddKey(_TimeScopted, RotationRotator.Euler().Y, false, TimeStep / 2.f);
		//RotationRecorded->FloatCurves[2].UpdateOrAddKey(_TimeScopted, RotationRotator.Euler().Z, false, TimeStep / 2.f);

		RotationRotator = GetOwner()->GetActorRotation();
		RotationQuat = RotationRotator.Quaternion();

		if (IsValid(RotationRecorded))
		{
			RotationRecorded->FloatCurves[0].UpdateOrAddKey(_TimeScopted, RotationQuat.X, false, TimeStep / 2.f);
			RotationRecorded->FloatCurves[1].UpdateOrAddKey(_TimeScopted, RotationQuat.Y, false, TimeStep / 2.f);
			RotationRecorded->FloatCurves[2].UpdateOrAddKey(_TimeScopted, RotationQuat.Z, false, TimeStep / 2.f);
		}
		if (IsValid(RotationWRecorded))
		{
			RotationWRecorded->FloatCurve.UpdateOrAddKey(_TimeScopted, RotationQuat.W, false, TimeStep / 2.f);
		}
		if (IsValid(RotationAdvanced))
		{
			RotationAdvanced->FloatCurves[0].UpdateOrAddKey(_TimeScopted, RotationRotator.Pitch, false, TimeStep / 2.f);
			RotationAdvanced->FloatCurves[1].UpdateOrAddKey(_TimeScopted, RotationRotator.Yaw, false, TimeStep / 2.f);
			RotationAdvanced->FloatCurves[2].UpdateOrAddKey(_TimeScopted, RotationRotator.Roll, false, TimeStep / 2.f);
		}
	}
}

/**-----------------	Replay Curve Part		-----------------*/

void UTimeManipulatorComponent_Basic::ReplayCurves(float _TimeScopted)
{
	ReplayLocationCurve(_TimeScopted);
	ReplayRotationCurve(_TimeScopted);
}

void UTimeManipulatorComponent_Basic::ReplayLocationCurve(float _TimeScopted)
{
	if (GetOwner() != nullptr)
	{
		if (IsValid(PositionRecorded))
		{
			LocationVector = PositionRecorded->GetVectorValue(_TimeScopted);
		}

		GetOwner()->SetActorLocation(LocationVector);
	}
}

void UTimeManipulatorComponent_Basic::ReplayRotationCurve(float _TimeScopted)
{
	if (GetOwner() != nullptr)
	{
		FVector RotTemp;

		if (IsValid(RotationRecorded))
		{
			RotationQuat.X = RotationRecorded->GetVectorValue(_TimeScopted).X;
			RotationQuat.Y = RotationRecorded->GetVectorValue(_TimeScopted).Y;
			RotationQuat.Z = RotationRecorded->GetVectorValue(_TimeScopted).Z;
		}
		if (IsValid(RotationWRecorded))
		{
			RotationQuat.W = RotationWRecorded->GetFloatValue(_TimeScopted);
		}
		if (IsValid(RotationAdvanced))
		{
			RotTemp = RotationAdvanced->GetVectorValue(_TimeScopted);
			//RotationAdvanced->
		}
		RotationRotator = RotationQuat.Rotator();
		RotationRotator = FRotator(RotTemp.X, RotTemp.Y, RotTemp.Z);
		//RotationRotator.MakeFromEuler(RotationRecorded->GetVectorValue(_TimeScopted));
		GetOwner()->SetActorRotation(RotationRotator);
	}
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
