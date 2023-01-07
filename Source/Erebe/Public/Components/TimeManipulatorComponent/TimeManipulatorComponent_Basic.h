// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "TimeManipulatorComponent_Basic.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FActivateModeEventSignature);

/**
*  TimeManipulatorComponent_Basic
* Part of the Time Manipulator system
* 
* A default time manipulator component will be linked to the TM component manager,
* Used to call event and function on change in the time state
* Set the action for the time and when and how should we call the curve add on child class of this component
* In case you want to add curve on this component you should use a child of this class
* 
* This component handle :
* -> Time saved by it
* -> Time step when a new tick for record of the curve is launch
* -> Time elapsed between two steps
* 
* Sup feature / Bug fix :
* 
* [Done] : 
* 
* [ToDo] : 
* -> Add a function to jump in case we want this component to go to a specified time, when replaying recorded curve of this moment 
*
*/
UCLASS( ClassGroup=(TimeManipulator), meta=(BlueprintSpawnableComponent) )
class EREBE_API UTimeManipulatorComponent_Basic : public UActorComponent
{
	GENERATED_BODY()

/**-----------------	Variable Part		-----------------*/
protected:


	/**
	* Time saved by this component, used to replay / record curve of this component
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time", meta = (AllowPrivateAccess = "true"))
		float TimeSaved = 0.f;

	/**
	* Time elapsed between two step of the time
	* When this time go over the time step or under 0 time saved is updated
	* And a call is launch to record on every time manipulator component
	* Can be used to update or no @TimeSaved as well
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time", meta = (AllowPrivateAccess = "true"))
		float TimeElapsed = 0.f;

	/**
	* Time step : time that should elaps between two record on time manipulator component
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time", meta = (AllowPrivateAccess = "true"))
		float TimeStep = 0.02f;

	/** 
	* Can this component record used when time state is set as forward
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Activity", meta = (AllowPrivateAccess = "true"))
		bool bCanRecord = true;

	/**
	* Is this component is set as reverse set when the time state is set as backward
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Activity", meta = (AllowPrivateAccess = "true"))
		bool bReverse = false;

	/**
	* Can this component update the time, used when the time state is set as stop
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Activity", meta = (AllowPrivateAccess = "true"))
		bool bCanUpdate = true;

	/**
	* Has time elapsed go to / over time step, in case yes this will be set as true
	* Used to know if a step is made
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Activity", meta = (AllowPrivateAccess = "true"))
		bool bHasAStep = true;

	/**
	* Has time elapsed go to / under 0, in case yes this will be set as true
	* Used to know if a step is made when in reverse mode
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Activity", meta = (AllowPrivateAccess = "true"))
		bool bHasAReverseStep = true;

/**-----------------	Callback Part		-----------------*/
public:


	//Called when activate record is called
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
		FActivateModeEventSignature OnActivateFastRecord;

	//Called when activate record is called
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
		FActivateModeEventSignature OnActivateRecord;

	//Called when activate stop is called
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
		FActivateModeEventSignature OnActivateStop;

	//Called when activate replay is called
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
		FActivateModeEventSignature OnActivateReplay;

	//Called when activate record is called
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
		FActivateModeEventSignature OnActivateFastReplay;

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
/**-----------------	Management Part		-----------------*/
public: 

	/**
	* Request link to the manager :
	* -> Add it in a array to be able to interact with it
	* -> Add function to callback
	*/
	UFUNCTION(BlueprintCallable)
	virtual void RequestLinkToManager();

	/**
	* Remove link to the manager :
	* -> remove it in from the array
	* -> remove function to callback
	*/
	UFUNCTION(BlueprintCallable)
	virtual void RemoveLinkToManager();

/**-----------------	Time State Part		-----------------*/
public:

	/**
	* Action on the activation fast record on this component
	* Currently used when the time is set as Fast Forward
	*/
	UFUNCTION()
		virtual void ActivateFastRecord(float TimeDilatation);


	/**
	* Called by ActivateFastRecord
	* Can be override by a blueprint in case you want to change his action
	*/
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "ActivateFastRecord"))
		void ReceiveActivateFastRecord(float TimeDilatation);

	/** 
	* Action on the activation record on this component
	* Currently used when the time is set as Forward
	*/
	UFUNCTION()
	virtual void ActivateRecord();


	/**
	* Called by ActivateRecord
	* Can be override by a blueprint in case you want to change his action
	*/
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "ActivateRecord"))
		void ReceiveActivateRecord();

	/**
	* Action on the activation stop on this component
	* Currently used when the time is set as Stop
	*/
	UFUNCTION()
	virtual void ActivateStop();

	/**
	* Called by ActivateStop
	* Can be override by a blueprint in case you want to change his action
	*/
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "ActivateStop"))
		void ReceiveActivateStop();

	/**
	* Action on the activation replay on this component
	* Currently used when the time is set as Backward
	*/
	UFUNCTION()
	virtual void ActivateReplay();

	/**
	* Called by ActivateReplay
	* Can be override by a blueprint in case you want to change his action
	*/
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "ActivateReplay"))
		void ReceiveActivateReplay();

	/**
	* Action on the activation fast replay on this component
	* Currently used when the time is set as FastBackward
	*/
	UFUNCTION()
		virtual void ActivateFastReplay(float TimeDilatation);

	/**
	* Called by ActivateFastReplay
	* Can be override by a blueprint in case you want to change his action
	*/
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "ActivateReplay"))
		void ReceiveActivateFastReplay(float TimeDilatation);

/**-----------------	Time Parameter Part		-----------------*/
public:

	/** 
	* Update time for this component
	* Add / Out @_DeltaTime to @TimeElapsed until he is egal to @TimeStep or zero in case time is reversed (@bReverse)
	* In case @TimeElasped satisfy on this condition @HasAStep or @HasAReverseStep is set as true
	* Update @TimeSaved at the same time as @TimeElapsed
	* 
	*/
	UFUNCTION(BlueprintCallable)
	virtual void UpdateTime(float _DeltaTime);

	/**
	* Update time for this component
	* Add / Out @_DeltaTime to @TimeElapsed until he is egal to @TimeStep or zero in case time is reversed (@bReverse)
	* In case @TimeElapsed satisfy one of this condition @HasAStep or @HasAReverseStep is set as true
	* Update @TimeSaved in case @TimeElapsed one of this condition by @TimeStep
	*
	*/
	UFUNCTION(BlueprintCallable)
		virtual void UpdateTimeByStep(float _DeltaTime);

/**-----------------	Curve Creation / Destroy Part		-----------------*/
protected:

	/**
	* Create curves on this component
	* Should use boolean as handle for the creation of curve
	*/
	UFUNCTION(BlueprintCallable)
		virtual void CreateCurves();

/**-----------------	Set Curve Part		-----------------*/
public:

	/** 
	* Set curves will reset curves than record them at a specified time, can set them at 0 at the same time if needed
	* @_TimeScopted : Time to set the curve
	* @_bZeroAndScopted : record them at _TimeScopted = 0 as well
	*/
	UFUNCTION(BlueprintCallable)
		virtual void SetCurves(float _TimeScopted, bool _bZeroAndScoped = false);

protected:

	/** Reset all curves of this components  */
	UFUNCTION(BlueprintCallable)
		virtual void ResetCurves();

/**-----------------	Record Curve Part		-----------------*/
public:

	/** 
	* Launch a call to record for all curves of this component  
	* Use @_TimeScopted
	*/
	UFUNCTION(BlueprintCallable)
		virtual void RecordCurves(float _TimeScopted);

/**-----------------	Replay Curve Part		-----------------*/
public:

	/**
	* Launch a call to replay for all curves of this component
	* Use @_TimeScopted
	*/
	UFUNCTION(BlueprintCallable)
		virtual void ReplayCurves(float _TimeScopted);

/**-----------------	TimeManipulatorPart Accessor Part		-----------------*/
public:

	/** Return if this component can update time / curves */
	FORCEINLINE bool GetCanUpdate() { return bCanUpdate; }

	/** Set if this component can update time / curves */
	UFUNCTION(BlueprintCallable)
	virtual void SetCanUpdate(bool _bNewCanUpdate);

	/** Return Time saved by this  component */
	FORCEINLINE float GetTimeSaved() { return TimeSaved; }

	/** Return Time elapsed of this component */
	FORCEINLINE float GetTimeElapsed() { return TimeElapsed; }

	/** Return Time step of this component */
	FORCEINLINE float GetTimeStep() { return TimeStep; }

	/** Set Time saved and elapsed of this component */
	UFUNCTION(BlueprintCallable)
	virtual void SetTimeComponent(float _NewTimeSave, float _NewTimeElapsed = 0.f);

};
