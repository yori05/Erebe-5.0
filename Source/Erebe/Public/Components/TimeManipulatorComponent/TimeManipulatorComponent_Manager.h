// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Common/TimeStates.h"

#include "TimeManipulatorComponent_Manager.generated.h"

class UTimeManipulatorComponent_Basic;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FChangeTimeStateEventSignatureT, TEnumAsByte<ETimeStates>, NewTimeState, TEnumAsByte<ETimeStates>, PreviousTimeState);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAnnonceTimeStateEventSignatureT);

/**
* A TimeManipulatorComponent_Manager
* Part of the Time Manipulator system
* This component should be add on a game mode cause the components whome will search for him will try to access it from a game mode
* A component used to manage other Time manipulator components, these component are add on every component that should use Time manipulator system
* See @UTimeManipulatorComponent_Basic or child of this class for component using it.
* He save and direct Time manipulator component on other actor, tell them when come back in time, hold or go forward in time
* 
* ToDo : Function to synchronise component, send them time saved and launch update on it if needed.
* -> synchronise time step, allow a common time step
* -> curve to smothe the slow of movement / acceleration
* -> Stop it when TimeSaved go / try to go under 0
*/
UCLASS( ClassGroup=(TimeManipulator), meta=(BlueprintSpawnableComponent) )
class EREBE_API UTimeManipulatorComponent_Manager : public UActorComponent
{
	GENERATED_BODY()

	/**-----------------	Variable Part		-----------------*/
protected:

	/** Check if the character is in first person or third */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Management", meta = (AllowPrivateAccess = "true"))
		TArray<TObjectPtr<UTimeManipulatorComponent_Basic>> TimeManipulatorComponents;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time", meta = (AllowPrivateAccess = "true"))
		float TimeSaved;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time", meta = (AllowPrivateAccess = "true"))
		float TimeElapsed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time", meta = (AllowPrivateAccess = "true"))
		float TimeStep = 0.02f;

	/** Used to define the state of Time  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TimeManipulator", meta = (AllowPrivateAccess = "true"))
		ETimeStates ETimeState;

	/** Block backward time on time try to / go to under zero */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TimeManipulator", meta = (AllowPrivateAccess = "true"))
		bool bBlockOnTimeZero;


	/** Block backward time on time try to / go to under zero */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Management", meta = (AllowPrivateAccess = "true"))
		bool bCallUpdateComponents;


	/**-----------------	Callback Part		-----------------*/
public:


	//Called when the time state backward begin
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
		FAnnonceTimeStateEventSignatureT OnBackwardTimeState;

	//Called when the time state stop begin
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
		FAnnonceTimeStateEventSignatureT OnStopTimeState;

	//Called when the time state forward begin
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
		FAnnonceTimeStateEventSignatureT OnForwardTimeState;

	//Called when the time state change
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
		FChangeTimeStateEventSignatureT OnChangeTimeState;

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
	* Launch the replay in case the Time State is not set as TIMESTATES_Backward
	* Call the function ChangeTimeState to do so
	*/
	UFUNCTION(BlueprintCallable)
		virtual void LaunchBackwardTime();

	/**
	* Check if can launch backward time
	*/
	UFUNCTION(BlueprintCallable)
		virtual bool CanLaunchBackwardTime();

	/**
	* Launch the hold in case the Time State is not set as TIMESTATES_Stop
	* Call the function ChangeTimeState to do so
	*/
	UFUNCTION(BlueprintCallable)
		virtual void LaunchStopTime();

	/**
	* Check if can launch stop time
	*/
	UFUNCTION(BlueprintCallable)
	virtual bool CanLaunchStopTime();

	/**
	* Launch the replay in case the Time State is not set as TIMESTATES_Forward
	* Call the function ChangeTimeState to do so
	*/
	UFUNCTION(BlueprintCallable)
		virtual void LaunchForwardTime();

	/**
	* Check if can launch forward time
	*/
	UFUNCTION(BlueprintCallable)
		virtual bool CanLaunchForwardTime();

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

	/**-----------------	Accessor Part		-----------------*/
public:

	/** Returns Time manipulator components saved **/
	FORCEINLINE TArray<TObjectPtr<UTimeManipulatorComponent_Basic>> GetTimeManipulatorComponents() const { return TimeManipulatorComponents; }

	/** Returns Time State **/
	FORCEINLINE ETimeStates GetTimeState() const { return ETimeState; }

	/** Returns Time Saved **/
	FORCEINLINE float GetTimeSaved() const { return TimeSaved; }

	/** Returns if is in Backward time state **/
	FORCEINLINE bool IsInBackwardTimeState() const { return ETimeState == ETimeStates::TIMESTATES_Backward; }

	/** Returns if is in Stop time state **/
	FORCEINLINE bool IsInStopTimeState() const { return ETimeState == ETimeStates::TIMESTATES_Stop; }

	/** Returns if is in Forward time state **/
	FORCEINLINE bool IsInForwardTimeState() const { return ETimeState == ETimeStates::TIMESTATES_Forward; }
};

