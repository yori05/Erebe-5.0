// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimeManipulatorComponent/TimeManipulatorComponent_Basic.h"
#include "TMComponent_PhysicBasic.generated.h"

class UCurveVector;
class UCurveFloat;
class UPrimitiveComponent;

/**
 *  TimeManipulatorComponent_PhysicBasic
 * Part of the Time Manipulator system
 * 
 * Child of TimeManipulatorComponent_Basic
 * Used to handle basic physic for actor of this component
 * You can handle or unhandle curve in case you don't use it
 * If you don't handle a part of it Curve won't be create, 
 * you won't be able to handle a part of it in case you change it on runtime if you don't spawn the curve.
 * 
 * This component handle :
 * -> Location
 * -> Rotation
 * -> Scale
 * 
 */
UCLASS(ClassGroup = (TimeManipulator), meta = (BlueprintSpawnableComponent))
class EREBE_API UTMComponent_PhysicBasic : public UTimeManipulatorComponent_Basic
{
	GENERATED_BODY()

/**-----------------	Variable Part		-----------------*/
protected:

	/**-----------------	Component Dependent		-----------------*/

	/**
	* Used to stop replay the phycis simulation on the actor owner when stop / backward time state
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics", meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UPrimitiveComponent> SavedPhysicComponent;

	/**-----------------	Curves		-----------------*/

	/** Curve use to record the location of the actor owner, use Time Step / Time Saved to set key */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Position", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCurveVector> PositionRecorded;

	/** Curve use to record the rotation of the actor owner, use Time Step / Time Saved to set key */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCurveVector> RotationRecorded;

	/** Curve use to record the scale of the actor owner, use Time Step / Time Saved to set key */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scale", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCurveVector> ScaleRecorded;

	/**-----------------	Data update		-----------------*/

	/** Used to save the location of the acter owner before record / replay it can be used on debug */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Location", meta = (AllowPrivateAccess = "true"))
		FVector LocationVector;

	/** Used to save the rotation of the acter owner before record / replay it can be used on debug */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation", meta = (AllowPrivateAccess = "true"))
		FRotator RotationRotator;

	/** 
	* Used to save the rotation of the acter owner before record / replay it can be used on debug
	* X = Pitch / Y = Yaw / Z = Roll
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation", meta = (AllowPrivateAccess = "true"))
		FVector RotationVector;

	/** Used to save the scale of the acter owner before record / replay it can be used on debug */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scale", meta = (AllowPrivateAccess = "true"))
		FVector ScaleVector;

	/**-----------------	Functionnality handle		-----------------*/

	/** Can this component handle the location, will record it on Forwardand replay on Backward Time State */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Location", meta = (AllowPrivateAccess = "true"))
		bool bHandleLocation;

	/** Can this component handle the rotation, will record it on Forwardand replay on Backward Time State */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation", meta = (AllowPrivateAccess = "true"))
		bool bHandleRotation;

	/** 
	* Can this component handle the scale, will record it on Forwardand replay on Backward Time State
	* Set as false by default
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scale", meta = (AllowPrivateAccess = "true"))
		bool bHandleScale;

	/**
	* Can this component stop physic simulation on Stop and Backward Time state
	* Set as true by default
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics", meta = (AllowPrivateAccess = "true"))
		bool bStopPhysicOnState;

/**-----------------	Constructor Part		-----------------*/
public:

	// Sets default values for this component's properties
	UTMComponent_PhysicBasic();
	
/**-----------------	Inherit Function Part		-----------------*/
protected:

	// Called when the game starts
	virtual void BeginPlay() override;

/**-----------------	TimeManipulatorPart Function Part		-----------------*/
/**-----------------	Time State Part		-----------------*/
public:

	/**
	* Action on the activation record on this component
	* Currently used when the time is set as Forward
	*/
	virtual void ActivateRecord() override;

	/**
	* Action on the activation stop on this component
	* Currently used when the time is set as Stop
	*/
	virtual void ActivateStop() override;

	/**
	* Action on the activation replay on this component
	* Currently used when the time is set as Backward
	*/
	virtual void ActivateReplay() override;

/**-----------------	Curve Creation / Destroy Part		-----------------*/
protected:

	/**
	* Create curves on this component
	* Won't Create a curve in case the handle of the curve is set as false
	* (Check @bHandleLocation, ...)
	*/
	virtual void CreateCurves() override;

/**-----------------	Set Curve Part		-----------------*/
protected:

	/** Reset all curves of this components  */
	virtual void ResetCurves() override;

	/** Reset the location curve of this actor  */
	UFUNCTION(BlueprintCallable)
		virtual void ResetLocationCurve();

	/** Reset the rotation curve of this actor  */
	UFUNCTION(BlueprintCallable)
		virtual void ResetRotationCurve();

	/** Reset the scale curve of this actor  */
	UFUNCTION(BlueprintCallable)
		virtual void ResetScaleCurve();

/**-----------------	Record Curve Part		-----------------*/
public:

	/** Launch a call to record for all curves of this component  */
	virtual void RecordCurves(float _TimeScopted) override;

protected:

	/** Launch a call to record the location curve  */
	UFUNCTION(BlueprintCallable)
		virtual void RecordLocationCurve(float _TimeScopted);

	/** Launch a call to record the rotation curve  */
	UFUNCTION(BlueprintCallable)
		virtual void RecordRotationCurve(float _TimeScopted);

	/** Launch a call to record the scale curve  */
	UFUNCTION(BlueprintCallable)
		virtual void RecordScaleCurve(float _TimeScopted);

/**-----------------	Replay Curve Part		-----------------*/
public:

	/** Launch a call to replay for curves of this component  */
	virtual void ReplayCurves(float _TimeScopted) override;

protected:

	/** Launch a call to replay the location curve  */
	UFUNCTION(BlueprintCallable)
		virtual void ReplayLocationCurve(float _TimeScopted);

	/** Launch a call to replay the rotation curve  */
	UFUNCTION(BlueprintCallable)
		virtual void ReplayRotationCurve(float _TimeScopted);

	/** Launch a call to record the scale curve  */
	UFUNCTION(BlueprintCallable)
		virtual void ReplayScaleCurve(float _TimeScopted);

};
