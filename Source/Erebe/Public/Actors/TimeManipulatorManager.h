// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Common/TimeStates.h"

#include "TimeManipulatorManager.generated.h"

class UTimeManipulatorComponent_Basic;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FChangeTimeStateEventSignature, TEnumAsByte<ETimeStates>, NewTimeState, TEnumAsByte<ETimeStates>, PreviousTimeState);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAnnonceTimeStateEventSignature);

UCLASS(ClassGroup=(TimeManipulator))
class EREBE_API ATimeManipulatorManager : public AActor
{
	GENERATED_BODY()

/**-----------------	Variable Part		-----------------*/
protected:

	/** Check if the character is in first person or third */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FirstP/ThirdP", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UTimeManipulatorComponent_Basic>> TimeManipulatorComponents;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time", meta = (AllowPrivateAccess = "true"))
	float TimeSaved;

	/** Used to define the state of Time  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TimeManipulator", meta = (AllowPrivateAccess = "true"))
		ETimeStates ETimeState;

	/**-----------------	Callback Part		-----------------*/
public:


	//Called when the time state backward begin
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
		FAnnonceTimeStateEventSignature OnBackwardTimeState;

	//Called when the time state stop begin
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
		FAnnonceTimeStateEventSignature OnStopTimeState;

	//Called when the time state forward begin
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
		FAnnonceTimeStateEventSignature OnForwardTimeState;

	//Called when the time state change
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
		FChangeTimeStateEventSignature OnChangeTimeState;

/**-----------------	Constructor Part		-----------------*/
public:	

	// Sets default values for this actor's properties
	ATimeManipulatorManager();

/**-----------------	Inherit Function Part		-----------------*/
protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/**-----------------	Component Management Function Part		-----------------*/
public:

	UFUNCTION(BlueprintCallable)
		virtual bool RequestAdd(UTimeManipulatorComponent_Basic* _NewComponent);

/**-----------------	Time Manipulation Function Part		-----------------*/
public:

	UFUNCTION(BlueprintCallable)
		virtual void SetTimeComponent(float _NewTimeComponent);

	UFUNCTION(BlueprintCallable)
		virtual void UpdateTime(float _DeltaTime);
	
	UFUNCTION(BlueprintCallable)
		virtual void LaunchReplay();

	UFUNCTION(BlueprintCallable)
		virtual void LaunchStop();

	UFUNCTION(BlueprintCallable)
		virtual void LaunchRecord();

	virtual void ChangeTimeState(ETimeStates _NewTimeState);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "ChangeTimeState"))
		void ReceiveChangeTimeState(ETimeStates _NewTimeState);

protected:

	virtual void BackwardTime();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "BackwardTime"))
		void ReceiveBackwardTime();

	virtual void StopTime();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "StopTime"))
		void ReceiveStopTime();

	virtual void ForwardTime();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "ForwardTime"))
		void ReceiveForwardTime();

/**-----------------	Accessor Part		-----------------*/
public:

	/** Returns Time State **/
	FORCEINLINE ETimeStates GetTimeState() const { return ETimeState; }

};
