// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerController/PlayerController_Basic.h"
#include "PlayerController_Erebe.generated.h"

/**
 *	PlayerController_Erebe
 *  Child of PlayerController_Basic
 * 
 *	A player controller specialized to interact with a Character_Erebe
 *  Can use double jump
 */
UCLASS()
class EREBE_API APlayerController_Erebe : public APlayerController_Basic
{
	GENERATED_BODY()

	/**-----------------	Variable Part		-----------------*/
protected:

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

public:
	/** Set the PlayerController's default value */
	APlayerController_Erebe(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


protected:
	/**
	  * Tick for the controller
	  * @param DeltaTime  This is the time elapsed since the last call of this function
	  */
	virtual void PlayerTick(float DeltaTime) override;

	/** Used to bind the input feedback on the controller action */
	virtual void SetupInputComponent() override;

protected: /** Input Binding */

	/**
	 *  Called via input to jump pressed, will call the character rush action
	 */
	UFUNCTION(BlueprintCallable)
		void RushInput();

	/**
	  * Called via input to jump double-clicked, will call the character jump action
	  */
		virtual void JumpInput() override;

	/**
	  * Called via input to jump released, will call the character stop jump action
	  */
		virtual void StopJumpingInput() override;

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

};
