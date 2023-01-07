// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerController_Basic.generated.h"

/**
 *	BasicPlayerController
 *	A player controller designed to interact with a BasicCharacter
 *  Should be a base for every PlayerController specialized to interact with BasicCharacter's child
 */
UCLASS()
class EREBE_API APlayerController_Basic : public APlayerController
{
	GENERATED_BODY()
	
/**-----------------	Variable Part		-----------------*/
protected:

	/** Direction recorded by the input on this frame */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input)
		FVector DirectionInput;

public:

	/** Base turn rate, in deg/sec. Other scaling may affect the final turn rate */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up / down rate, in deg/sec. Other scaling may affect the final look up rate */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	/** Base Zoom  rate, in ratio (0 to 1). Other scaling may affect the final zoom rate */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseZoomRate;

	/** Base Zoom speed, in cm/sec. Other scaling may affect the final zoom speed */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseZoomSpeed;

/**-----------------	Constructor Part		-----------------*/
public:

	/** Set the PlayerController's default value */
	APlayerController_Basic(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


/**-----------------	Inherit Function Part		-----------------*/
protected:

	/** Used to bind the input feedback on the controller action */
	virtual void SetupInputComponent() override;


/**-----------------	Input Function Part		-----------------*/
protected:

	/**-----------------	Movement Function Part		-----------------*/

	/**
	* Called for forwards/backward input
	* @param Value  This is the value of the desired movement (if the value is less than 0 the pawn will go backward)
	*/
	UFUNCTION(BlueprintCallable)
		virtual void MoveForwardInput(float Value);

	/**
	* Called for right/left input
	* @param Value  This is the value of the desired movement (if the value is less than 0 the pawn will go left)
	*/
	UFUNCTION(BlueprintCallable)
		virtual void MoveRightInput(float Value);

	/**
	* Called for up/down input
	* @param Value  This is the value of the desired movement (if the value is less than 0 the pawn will go left)
	*/
	UFUNCTION(BlueprintCallable)
		virtual void MoveUpInput(float Value);

	/**-----------------	Orientation Function Part		-----------------*/

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate  This is a normalized rate, i.e 1.0 means 100% of desired turn rate
	 */
	UFUNCTION(BlueprintCallable)
		virtual void LookUpAtRateInput(float Rate);

	/**
	 * Called via input to turn at a given rate.
	* @param Rate  This is a normalized rate, i.e 1.0 means 100% of desired turn rate
	 */
	UFUNCTION(BlueprintCallable)
		virtual void TurnAtRateInput(float Rate);

	/**-----------------	Jump Function Part		-----------------*/

	/**
	* Called via input to jump with the character.
	*/
	UFUNCTION(BlueprintCallable)
		virtual void JumpInput();

	/**
	* Called via input to stop jumping with the character.
	*/
	UFUNCTION(BlueprintCallable)
		virtual void StopJumpingInput();

	/**-----------------	Interaction Function Part		-----------------*/

	/**
	* Called via input to interact with the character.
	*/
	UFUNCTION(BlueprintCallable)
		virtual void InteractInput();


	/**-----------------	Viewpoint Function Part		-----------------*/

	/**
	* Called via input to swap the viewpoint of the character
	*/
	UFUNCTION(BlueprintCallable)
		virtual void SwapViewPoint();

	/**
	* Called via input to use the first person viewpoint for the character
	*/
	UFUNCTION(BlueprintCallable)
		virtual void UseFirstPersonViewPoint();
	/**
	* Called via input to use the third person viewpoint for the character
	*/
	UFUNCTION(BlueprintCallable)
		virtual void UseThirdPersonViewPoint();

	/**-----------------	Zoom Function Part		-----------------*/

	/**
	* Called via input to zoom at a given rate.
	* @param Rate  This is a normalized rate, i.e 1.0 means 100% of desired turn rate
	*/
	UFUNCTION(BlueprintCallable)
		virtual void ZoomAtRateInput(float Rate);

	/**-----------------	Combat Function Part		-----------------*/
	/**
	* Called via input to launch a attack by the character
	*/
	UFUNCTION(BlueprintCallable)
		virtual void LaunchAttack();


};
