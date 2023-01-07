// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerController/PlayerController_Basic.h"
#include "PlayerController_TimeManipulator.generated.h"

class UTimeManipulatorComponent_Manager;

/**
 * 
 */
UCLASS()
class EREBE_API APlayerController_TimeManipulator : public APlayerController_Basic
{
	GENERATED_BODY()

/**-----------------	Variable Part		-----------------*/
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TimeManipulator", meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UTimeManipulatorComponent_Manager> TimeManipCManager;

/**-----------------	Constructor Part		-----------------*/
public:

	/** Set the PlayerController's default value */
	APlayerController_TimeManipulator(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

/**-----------------	Inherit Function Part		-----------------*/
protected:

	virtual void BeginPlay() override;

	/** Used to bind the input feedback on the controller action */
	virtual void SetupInputComponent() override;

/**-----------------	Input Function Part		-----------------*/
protected:

	/**-----------------	Time Manipulation Part		-----------------*/

	UFUNCTION(BlueprintCallable)
		virtual void HoldBackwardTimeInput();

	UFUNCTION(BlueprintCallable)
		virtual void UnholdBackwardTimeInput();

	UFUNCTION(BlueprintCallable)
		virtual void HoldForwardTimeInput();

	UFUNCTION(BlueprintCallable)
		virtual void UnholdForwardTimeInput();

};
