// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Common/TimeStates.h"

#include "TimeManipulatorComponent_Manager.generated.h"

class UTimeManipulatorComponent_Basic;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FChangeTimeStateEventSignature, ETimeStates, NewTimeState, ETimeStates, PreviousTimeState);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFastTimeEventSignature, float, SpeedRatio);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTimeStateEventSignature);

/**
* A TimeManipulatorComponent_Manager
* Part of the Time Manipulator system
* 
* This component should be add on a game mode because the components whome will search for him will try to access it from a game mode
* A component used to manage other Time manipulator components, these component are add on every component that should use Time manipulator system
* See @UTimeManipulatorComponent_Basic or child of this class for component using it.
* He save and direct Time manipulator component on other actor, tell them when come back in time, hold or go forward in time
* 
* Sup feature / Bug fix :
* 
* [Done] : 
* -> Stop it when TimeSaved go / try to go under 0
* 
* [ToDo] : 
* ->Function to synchronise component, send them time saved and launch update on it if needed.
* -> synchronise time step, allow a common time step
* -> curve to smothe the slow of movement / acceleration
* -> add a second TimeSaved for the player and other desynchronise actor
* 
*/
UCLASS( ClassGroup=(TimeManipulator), meta=(BlueprintSpawnableComponent) )
class EREBE_API UTimeManipulatorComponent_Manager : public UActorComponent
{
	GENERATED_BODY()

	/**-----------------	Variable Part		-----------------*/
protected:

	/** 
	* Array of time manipulator component registered
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Management", meta = (AllowPrivateAccess = "true"))
		TArray<TObjectPtr<UTimeManipulatorComponent_Basic>> TimeManipulatorComponents;

	/**
	* Time saved by this component, should be the time used by all the component registered
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time", meta = (AllowPrivateAccess = "true"))
		float TimeSaved;

	/**
	* Time elapsed between two step of the time
	* When this time go over the time step or under 0 time saved is updated
	* And a call is launch to record on every time manipulator component
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time", meta = (AllowPrivateAccess = "true"))
		float TimeElapsed = 0.f;

	/**
	* Time step : time that should elaps between two record on time manipulator component
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time", meta = (AllowPrivateAccess = "true"))
		float TimeStep = 0.02f;

	/**
	* Fast Time Ratio : Ratio send and used by TimeManipulator Component when in fast backward / forward time state
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time", meta = (AllowPrivateAccess = "true"))
		float FastTimeRatio = 5.f;

	/** Used to define the state of Time  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Management", meta = (AllowPrivateAccess = "true"))
		ETimeStates ETimeState;

	/** Block backward time on time try to / go to under zero */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Management", meta = (AllowPrivateAccess = "true"))
		bool bBlockOnTimeZero;

	/** Should Call update on Time Manipulator Component in the array */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Management", meta = (AllowPrivateAccess = "true"))
		bool bCallUpdateComponents;


	/** Can the time state go to accelerate time state */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Management", meta = (AllowPrivateAccess = "true"))
		bool bCanFastTimeState = false;

	/**-----------------	Callback Part		-----------------*/
public:

	//Called when the time state backward begin
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
		FFastTimeEventSignature OnFastBackwardTimeState;

	
	//Called when the time state backward begin
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
		FTimeStateEventSignature OnBackwardTimeState;

	//Called when the time state stop begin
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
		FTimeStateEventSignature OnStopTimeState;

	//Called when the time state forward begin
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
		FTimeStateEventSignature OnForwardTimeState;

	//Called when the time state backward begin
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
		FFastTimeEventSignature OnFastForwardTimeState;

	//Called when the time state change
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
		FChangeTimeStateEventSignature OnChangeTimeState;

	/**-----------------	Constructor Part		-----------------*/
public:

	// Sets default values for this component's properties
	UTimeManipulatorComponent_Manager();

	/**-----------------	Inherit Function Part		-----------------*/
protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**-----------------	Component Management Function Part		-----------------*/
public:

	/** Request for a component to be added on the array with every component */
	UFUNCTION(BlueprintCallable)
		virtual bool RequestAdd(UTimeManipulatorComponent_Basic* _NewComponent);

	/**-----------------	Time Manipulation Function Part		-----------------*/
public:

	/** Set a new time saved for the component */
	UFUNCTION(BlueprintCallable)
		virtual void SetTimeComponent(float _NewTimeComponent);

	/** 
	* Update the time on this component 
	* @TimeSaved
	* 
	*/
	UFUNCTION(BlueprintCallable)
		virtual void UpdateTime(float _DeltaTime);

	/**
	* Launch the fast replay
	* Call the function ChangeTimeState to do so
	*/
	UFUNCTION(BlueprintCallable)
		virtual void LaunchFastBackwardTime();

	/**
	* Check if can launch fast backward time
	* In case the Time State is not set as TIMESTATES_FastBackward and @bCanFastTimeState is true
	*/
	UFUNCTION(BlueprintCallable)
		virtual bool CanLaunchFastBackwardTime();

	/**
	* Launch the replay
	* Call the function ChangeTimeState to do so
	*/
	UFUNCTION(BlueprintCallable)
		virtual void LaunchBackwardTime();

	/**
	* Check if can launch backward time
	* In case the Time State is not set as TIMESTATES_Backward
	*/
	UFUNCTION(BlueprintCallable)
		virtual bool CanLaunchBackwardTime();

	/**
	* Launch the hold
	* Call the function ChangeTimeState to do so
	*/
	UFUNCTION(BlueprintCallable)
		virtual void LaunchStopTime();

	/**
	* Check if can launch stop time
	*Iin case the Time State is not set as TIMESTATES_Stop
	*/
	UFUNCTION(BlueprintCallable)
	virtual bool CanLaunchStopTime();

	/**
	* Launch the record
	* Call the function ChangeTimeState to do so
	*/
	UFUNCTION(BlueprintCallable)
		virtual void LaunchForwardTime();

	/**
	* Check if can launch forward time
	* In case the Time State is not set as TIMESTATES_Forward
	*/
	UFUNCTION(BlueprintCallable)
		virtual bool CanLaunchForwardTime();

	/**
	* Launch the fast record
	* Call the function ChangeTimeState to do so
	*/
	UFUNCTION(BlueprintCallable)
		virtual void LaunchFastForwardTime();

	/**
	* Check if can launch fast forward time
	* In case the Time State is not set as TIMESTATES_Forward and @bCanFastTimeState is true
	*/
	UFUNCTION(BlueprintCallable)
		virtual bool CanLaunchFastForwardTime();

	/**
	* Move the time state a state before
	* Can't be used if the time is set as backward
	* Will go from Forward state to Stop
	* And from Stop state to Backward
	*/
	UFUNCTION(BlueprintCallable)
		virtual void MoveBackTime();

	/**
	* Check if can launch move back
	*/
	UFUNCTION(BlueprintCallable)
		virtual bool CanMoveBackTime();

	/**
	* Move the time state a state after
	* Can't be used if the time is set as forward
	* Will go from Backward state to Stop
	* And from Stop state to Forward
	*/
	UFUNCTION(BlueprintCallable)
		virtual void MoveFrontTime();

	/**
	* Check if can launch move front
	*/
	UFUNCTION(BlueprintCallable)
		virtual bool CanMoveFrontTime();



protected:

	/**
	* Change the state of the time function called by : 
	* -> LaunchReplay
	* -> LaunchStop
	* -> LaunchRecord
	* Call the Delegate @OnChangeTimeState
	* Call the Blueprint function ReceiveChangeTimeState
	*/
	virtual void ChangeTimeState(ETimeStates _NewTimeState);

	/**
	* Called by ChangeTimeState 
	* Can be override by a blueprint in case you want to change his action
	*/
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "ChangeTimeState"))
		void ReceiveChangeTimeState(ETimeStates _NewTimeState);

	/**
	* Launch action for the fast backward time state
	* Call the Delegate @OnFastBackwardTime
	* Call the Blueprint function ReceiveFastBackwardTime
	* Called by the ChangeTimeState
	*/
	virtual void FastBackwardTime();

	/**
	* Action for the BlueprintChild of this class
	* Called by the function FastBackwardTime after ChangeTimeState have been succesfull
	*/
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "FastBackwardTime"))
		void ReceiveFastBackwardTime();

	/**
	* Launch action for the backward time state
	* Call the Delegate @OnBackwardTime
	* Call the Blueprint function ReceiveBackwardTime
	* Called by the ChangeTimeState
	*/
	virtual void BackwardTime();

	/**
	* Action for the BlueprintChild of this class
	* Called by the function BackwardTime after ChangeTimeState have been succesfull
	*/
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "BackwardTime"))
		void ReceiveBackwardTime();

	/**
	* Launch action for the stop time state
	* Call the Delegate @OnStopTime
	* Call the Blueprint function ReceiveStopTime
	* Called by the ChangeTimeState
	*/
	virtual void StopTime();

	/**
	* Action for the BlueprintChild of this class
	* Called by the function StopTime after ChangeTimeState have been succesfull
	*/
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "StopTime"))
		void ReceiveStopTime();

	/**
	* Launch action for the Forward time state
	* Call the Delegate @OnForwardTime
	* Call the Blueprint function ReceiveForwardTime
	* Called by the ChangeTimeState
	*/
	virtual void ForwardTime();

	/**
	* Action for the BlueprintChild of this class
	* Called by the function ForwardTime after ChangeTimeState have been succesfull
	*/
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "ForwardTime"))
		void ReceiveForwardTime();

	/**
	* Launch action for the Fast Forward time state
	* Call the Delegate @OnFastForwardTime
	* Call the Blueprint function ReceiveFastForwardTime
	* Called by the ChangeTimeState
	*/
	virtual void FastForwardTime();

	/**
	* Action for the BlueprintChild of this class
	* Called by the function ForwardTime after ChangeTimeState have been succesfull
	*/
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "FastForwardTime"))
		void ReceiveFastForwardTime();

	/**-----------------	Accessor Part		-----------------*/
public:

	/** Returns Time manipulator components saved **/
	FORCEINLINE TArray<TObjectPtr<UTimeManipulatorComponent_Basic>> GetTimeManipulatorComponents() const { return TimeManipulatorComponents; }

	/** Returns Time State **/
	FORCEINLINE ETimeStates GetTimeState() const { return ETimeState; }

	/** Returns Time Saved **/
	FORCEINLINE float GetTimeSaved() const { return TimeSaved; }

	/** Returns Time Saved **/
	FORCEINLINE float GetTimeRatio() const { return FastTimeRatio; }

	/** Returns if is in Backward time state **/
	FORCEINLINE bool IsInFastBackwardTimeState() const { return ETimeState == ETimeStates::TIMESTATES_FastBackward; }

	/** Returns if is in Backward time state **/
	FORCEINLINE bool IsInBackwardTimeState() const { return ETimeState == ETimeStates::TIMESTATES_Backward; }

	/** Returns if is in Stop time state **/
	FORCEINLINE bool IsInStopTimeState() const { return ETimeState == ETimeStates::TIMESTATES_Stop; }

	/** Returns if is in Forward time state **/
	FORCEINLINE bool IsInForwardTimeState() const { return ETimeState == ETimeStates::TIMESTATES_Forward; }

	/** Returns if is in Forward time state **/
	FORCEINLINE bool IsInFastForwardTimeState() const { return ETimeState == ETimeStates::TIMESTATES_FastForward; }

};

