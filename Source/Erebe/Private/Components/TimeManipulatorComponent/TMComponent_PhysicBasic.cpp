// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/TimeManipulatorComponent/TMComponent_PhysicBasic.h"
#include "Curves/CurveVector.h"
#include "Curves/CurveFloat.h"
#include "Components/PrimitiveComponent.h"


// Sets default values for this component's properties
UTMComponent_PhysicBasic::UTMComponent_PhysicBasic()
{
	bHandleLocation = true;

	bHandleRotation = true;

	//Set as false by default cause not change frequently
	bHandleScale = false;

	bStopPhysicOnState = true;

}

/**-----------------	Inherit Function Part		-----------------*/

// Called when the game starts
void UTMComponent_PhysicBasic::BeginPlay()
{
	Super::BeginPlay();

	if (SavedPhysicComponent == nullptr)
	{
		if (GetOwner() != nullptr)
		{
			UActorComponent* AcTemp = GetOwner()->GetComponentByClass(UPrimitiveComponent::StaticClass());
			SavedPhysicComponent = Cast<UPrimitiveComponent>(AcTemp);
		}
	}
}

/**-----------------	TimeManipulatorPart Function Part		-----------------*/
/**-----------------	Time State Part		-----------------*/

void UTMComponent_PhysicBasic::ActivateRecord()
{
	Super::ActivateRecord();

	if (SavedPhysicComponent != nullptr && bStopPhysicOnState)
	{
		SavedPhysicComponent->SetSimulatePhysics(true);
	}
}

void UTMComponent_PhysicBasic::ActivateStop()
{
	Super::ActivateStop();

	if (SavedPhysicComponent != nullptr && bStopPhysicOnState)
	{
		SavedPhysicComponent->SetSimulatePhysics(false);
	}
}

void UTMComponent_PhysicBasic::ActivateReplay()
{
	Super::ActivateReplay();

	if (SavedPhysicComponent != nullptr && bStopPhysicOnState)
	{
		SavedPhysicComponent->SetSimulatePhysics(false);
	}
}


/**-----------------	Curve Creation / Destroy Part		-----------------*/

void UTMComponent_PhysicBasic::CreateCurves()
{
	Super::CreateCurves();

	if (!IsValid(PositionRecorded) && bHandleLocation)
	{
		PositionRecorded = NewObject<UCurveVector>();
	}

	if (!IsValid(RotationRecorded) && bHandleRotation)
	{
		RotationRecorded = NewObject<UCurveVector>();
	}

	if (!IsValid(ScaleRecorded) && bHandleScale)
	{
		ScaleRecorded = NewObject<UCurveVector>();
	}
}

/**-----------------	Set Curve Part		-----------------*/

void UTMComponent_PhysicBasic::ResetCurves()
{
	Super::ResetCurves();

	ResetLocationCurve();
	ResetRotationCurve();
	ResetScaleCurve();
}

void UTMComponent_PhysicBasic::ResetLocationCurve()
{
	if (IsValid(PositionRecorded) && bHandleLocation)
	{
		PositionRecorded->ResetCurve();
	}
}

void UTMComponent_PhysicBasic::ResetRotationCurve()
{
	if (IsValid(RotationRecorded) && bHandleRotation)
	{
		RotationRecorded->ResetCurve();
	}
}

void UTMComponent_PhysicBasic::ResetScaleCurve()
{
	if (IsValid(ScaleRecorded) && bHandleScale)
	{
		ScaleRecorded->ResetCurve();
	}
}

/**-----------------	Record Curve Part		-----------------*/

void UTMComponent_PhysicBasic::RecordCurves(float _TimeScopted)
{
	Super::RecordCurves(_TimeScopted);

	RecordLocationCurve(_TimeScopted);
	RecordRotationCurve(_TimeScopted);
	RecordScaleCurve(_TimeScopted);
}

void UTMComponent_PhysicBasic::RecordLocationCurve(float _TimeScopted)
{
	if (GetOwner() != nullptr && bHandleLocation)
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


void UTMComponent_PhysicBasic::RecordRotationCurve(float _TimeScopted)
{
	if (GetOwner() != nullptr && bHandleRotation)
	{
		RotationRotator = GetOwner()->GetActorRotation();
		RotationVector = FVector(RotationRotator.Pitch, RotationRotator.Yaw, RotationRotator.Roll);

		if (IsValid(RotationRecorded))
		{
			RotationRecorded->FloatCurves[0].UpdateOrAddKey(_TimeScopted, RotationRotator.Pitch, false, TimeStep / 2.f);
			RotationRecorded->FloatCurves[1].UpdateOrAddKey(_TimeScopted, RotationRotator.Yaw, false, TimeStep / 2.f);
			RotationRecorded->FloatCurves[2].UpdateOrAddKey(_TimeScopted, RotationRotator.Roll, false, TimeStep / 2.f);
		}
	}
}

void UTMComponent_PhysicBasic::RecordScaleCurve(float _TimeScopted)
{
	if (GetOwner() != nullptr && bHandleScale)
	{
		ScaleVector = GetOwner()->GetActorScale3D();

		if (IsValid(ScaleRecorded))
		{
			ScaleRecorded->FloatCurves[0].UpdateOrAddKey(_TimeScopted, ScaleVector.X, false, TimeStep / 2.f);
			ScaleRecorded->FloatCurves[1].UpdateOrAddKey(_TimeScopted, ScaleVector.Y, false, TimeStep / 2.f);
			ScaleRecorded->FloatCurves[2].UpdateOrAddKey(_TimeScopted, ScaleVector.Z, false, TimeStep / 2.f);
		}
	}
}

/**-----------------	Replay Curve Part		-----------------*/

void UTMComponent_PhysicBasic::ReplayCurves(float _TimeScopted)
{
	Super::ReplayCurves(_TimeScopted);

	ReplayLocationCurve(_TimeScopted);
	ReplayRotationCurve(_TimeScopted);
	ReplayScaleCurve(_TimeScopted);
}

void UTMComponent_PhysicBasic::ReplayLocationCurve(float _TimeScopted)
{
	if (GetOwner() != nullptr && bHandleLocation)
	{
		if (IsValid(PositionRecorded))
		{
			LocationVector = PositionRecorded->GetVectorValue(_TimeScopted);
		}

		GetOwner()->SetActorLocation(LocationVector);
	}
}

void UTMComponent_PhysicBasic::ReplayRotationCurve(float _TimeScopted)
{
	if (GetOwner() != nullptr && bHandleRotation)
	{
		if (IsValid(RotationRecorded))
		{
			RotationVector = RotationRecorded->GetVectorValue(_TimeScopted);
		}

		RotationRotator = FRotator(RotationVector.X, RotationVector.Y, RotationVector.Z);
		GetOwner()->SetActorRotation(RotationRotator);
	}
}

void UTMComponent_PhysicBasic::ReplayScaleCurve(float _TimeScopted)
{
	if (GetOwner() != nullptr && bHandleScale)
	{
		if (IsValid(ScaleRecorded))
		{
			ScaleVector = ScaleRecorded->GetVectorValue(_TimeScopted);
		}

		GetOwner()->SetActorScale3D(ScaleVector);
	}
}
