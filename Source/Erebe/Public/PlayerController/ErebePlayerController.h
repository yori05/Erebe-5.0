// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ErebePlayerController.generated.h"

/**
 *	ErebePlayerController 
 *	A player controller specialized to interact with a ErebeCharacter 
 */
UCLASS()
class EREBE_API AErebePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public :

	/** Base turn rate, in deg/sec. Other scaling may affect the final turn rate */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	/** Base look up / down rate, in deg/sec. Other scaling may affect the final look up rate */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

protected :

	/** Direction recorded by the input on this frame */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input)
	FVector DirectionInput;

	/**
	 *  If the Character is in the focus mode
	 *  The Input "Rush / Jump" will launch a jump if two input are launch with less than this timing
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float DoubleJumpTimingMax = 0.5f;

	/**
	 *  Max rate apply to double jump timing max to update the double jump block
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float DoubleJumpRateMax = 1.1f;

	/**
	 *  Recorded time after the last "Rush / Jump" input won't be updated more than 1.1*DoubleJumpTimingMax
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input)
	float DoubleJumpClock = DoubleJumpTimingMax * DoubleJumpRateMax;

public :
	/** Set the PlayerController's default value */
	AErebePlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected :
	/** 
	  * Tick for the controller 
	  * @param DeltaTime  This is the time elapsed since the last call of this function
	  */
	virtual void PlayerTick(float DeltaTime) override;

	/** Used to bind the input feedback on the controller action */
	virtual void SetupInputComponent() override;
	
protected : /** Input Binding */

	/** 
	  * Called for forwards/backward input 
	  * @param Value  This is the value of the desired movement (if the value is less than 0 the pawn will go backward)
	  */
	UFUNCTION(BlueprintCallable)
	void MoveForwardInput(float Value);

	/**
	  * Called for right/left input
	  * @param Value  This is the value of the desired movement (if the value is less than 0 the pawn will go left)
	  */
	UFUNCTION(BlueprintCallable)
	void MoveRightInput(float Value);

	/**
	* Called for up/down input
	* @param Value  This is the value of the desired movement (if the value is less than 0 the pawn will go down)
	 */
	UFUNCTION(BlueprintCallable)
		void MoveUpInput(float Value);


	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate  This is a normalized rate, i.e 1.0 means 100% of desired turn rate
	 */
	UFUNCTION(BlueprintCallable)
	void LookUpAtRateInput(float Rate);

	/**
	 * Called via input to turn at a given rate.
	* @param Rate  This is a normalized rate, i.e 1.0 means 100% of desired turn rate
	 */
	UFUNCTION(BlueprintCallable)
	void TurnAtRateInput(float Rate);

	/**
	 *  Called via input to jump pressed, will call the character rush action
	 */
	UFUNCTION(BlueprintCallable)
	void RushInput();

	/**
	  * Called via input to jump double-clicked, will call the character jump action 
	  */
	UFUNCTION(BlueprintCallable)
	void JumpInput();

	/** 
	  * Called via input to jump released, will call the character stop jump action 
	  */
	UFUNCTION(BlueprintCallable)
	void StopJumpInput();

	/**
	 * Called via input to focus pressed, will call the character focus action
	 */
	UFUNCTION(BlueprintCallable)
	void FocusInput();

	/**
	 * Called via input to focus release, will call the character stop focus release
	 */
	UFUNCTION(BlueprintCallable)
	void StopFocusInput();
	
	/**
	 *  Called via input to fire1 pressed, will call the character punch action
	 */
	UFUNCTION(BlueprintCallable)
	void PunchInput();

	/**
	 *  Called via input to interact pressed, will call the character Interact action
	 */
	UFUNCTION(BlueprintCallable)
	void InteractInput();

};
