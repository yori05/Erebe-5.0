// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerController_FPS.generated.h"

/**
 *	PlayerController_FPS
 *	A player controller specialized to interact with a Character_FPS
 */
UCLASS()
class EREBE_API APlayerController_FPS : public APlayerController
{
	GENERATED_BODY()

		/**-----------------	Variable Part		-----------------*/
public:

	/** Base turn rate, in deg/sec. Other scaling may affect the final turn rate */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up / down rate, in deg/sec. Other scaling may affect the final look up rate */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	/**-----------------	Constructor Part		-----------------*/
public:
	/** Set the PlayerController's default value */
	APlayerController_FPS(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**-----------------	Inherit Function Part		-----------------*/
protected:
	/** Used to bind the input feedback on the controller action */
	virtual void SetupInputComponent() override;

	/**-----------------	Input Function Part		-----------------*/
protected: 

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
	* Called via input to jump with the character.
	*/
	UFUNCTION(BlueprintCallable)
		void JumpInput();

	/**
	* Called via input to stop jumping with the character.
	*/
	UFUNCTION(BlueprintCallable)
		void StopJumpingInput();

	/**
	 *  Called via input to fire1 pressed, will call the character fire press action
	 */
	UFUNCTION(BlueprintCallable)
		void FirePressInput();
	/**
	 *  Called via input to fire1 released, will call the character fire release action
	 */
	UFUNCTION(BlueprintCallable)
		void FireReleaseInput();

};
