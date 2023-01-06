// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TimeManipulatorComponent_Basic.generated.h"

class UCurveTable;
class UCurveVector;
class UCurveFloat;
class UPrimitiveComponent;

UCLASS( ClassGroup=(TimeManipulator), meta=(BlueprintSpawnableComponent) )
class EREBE_API UTimeManipulatorComponent_Basic : public UActorComponent
{
	GENERATED_BODY()

/**-----------------	Variable Part		-----------------*/
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation", meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UCurveVector> RotationAdvanced;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Position", meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UCurveVector> PositionRecorded;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation", meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UCurveVector> RotationRecorded;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation", meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UCurveFloat> RotationWRecorded;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics", meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UPrimitiveComponent> SavedPhysicComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Location", meta = (AllowPrivateAccess = "true"))
		FVector LocationVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation", meta = (AllowPrivateAccess = "true"))
		FRotator RotationRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation", meta = (AllowPrivateAccess = "true"))
		FQuat RotationQuat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time", meta = (AllowPrivateAccess = "true"))
		float TimeSaved = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time", meta = (AllowPrivateAccess = "true"))
		float TimeElapsed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time", meta = (AllowPrivateAccess = "true"))
		float TimeStep = 0.02f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Activity", meta = (AllowPrivateAccess = "true"))
		bool bCanRecord = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Activity", meta = (AllowPrivateAccess = "true"))
		bool bReverse = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Activity", meta = (AllowPrivateAccess = "true"))
		bool bCanUpdate = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Activity", meta = (AllowPrivateAccess = "true"))
		bool bHasAStep = true;


/**-----------------	Constructor Part		-----------------*/
public:	

	// Sets default values for this component's properties
	UTimeManipulatorComponent_Basic();

/**-----------------	Inherit Function Part		-----------------*/
protected:

	// Called when the game starts
	virtual void BeginPlay() override;


public:	

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Called when the game end
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	/**-----------------	TimeManipulatorPart Function Part		-----------------*/
/**-----------------	Record / Replay Part		-----------------*/
public:

	UFUNCTION(BlueprintCallable)
	virtual void ActivateRecord();

	UFUNCTION(BlueprintCallable)
	virtual void ActivateStop();

	UFUNCTION(BlueprintCallable)
	virtual void ActivateReplay();

/**-----------------	Time Parameter Part		-----------------*/
public:

	UFUNCTION(BlueprintCallable)
	virtual void UpdateTime(float _DeltaTime);

	UFUNCTION(BlueprintCallable)
		virtual void UpdateTimeByStep(float _DeltaTime);

/**-----------------	Set Curve Part		-----------------*/
public:

	UFUNCTION(BlueprintCallable)
		virtual void SetCurves(float _TimeScopted, bool _bDoItTimeBothZeroAndScopted = false);

protected:

	UFUNCTION(BlueprintCallable)
		virtual void ResetCurves();

	UFUNCTION(BlueprintCallable)
		virtual void ResetLocationCurve();

	UFUNCTION(BlueprintCallable)
		virtual void ResetRotationCurve();

/**-----------------	Record Curve Part		-----------------*/
public:

	UFUNCTION(BlueprintCallable)
	virtual void RecordCurves(float _TimeScopted);

protected:

	UFUNCTION(BlueprintCallable)
	virtual void RecordLocationCurve(float _TimeScopted);

	UFUNCTION(BlueprintCallable)
		virtual void RecordRotationCurve(float _TimeScopted);

/**-----------------	Replay Curve Part		-----------------*/
public:

	UFUNCTION(BlueprintCallable)
	virtual void ReplayCurves(float _TimeScopted);

protected:

	UFUNCTION(BlueprintCallable)
	virtual void ReplayLocationCurve(float _TimeScopted);

	UFUNCTION(BlueprintCallable)
		virtual void ReplayRotationCurve(float _TimeScopted);


/**-----------------	TimeManipulatorPart Accessor Part		-----------------*/
public:

	FORCEINLINE bool GetCanUpdate() { return bCanUpdate; }

	UFUNCTION(BlueprintCallable)
	virtual void SetCanUpdate(bool _bNewCanUpdate);

	FORCEINLINE float GetTimeSaved() { return TimeSaved; }

	FORCEINLINE float GetTimeElapsed() { return TimeElapsed; }

	FORCEINLINE float GetTimeStep() { return TimeStep; }

	UFUNCTION(BlueprintCallable)
	virtual void SetTimeComponent(float _NewTimeSave, float _NewTimeElapsed = 0.f);

};
