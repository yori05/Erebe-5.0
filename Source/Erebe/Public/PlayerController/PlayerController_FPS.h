// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerController/PlayerController_Basic.h"
#include "PlayerController_FPS.generated.h"

/**
 *	PlayerController_FPS
 *  Child of PlayerController_Basic
 *	A player controller specialized to interact with a Character_FPS
 */
UCLASS()
class EREBE_API APlayerController_FPS : public APlayerController_Basic
{
	GENERATED_BODY()

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
	 *  Called via input fire1 pressed, will call the character fire press action
	 */
	UFUNCTION(BlueprintCallable)
		void FirePressInput();
	/**
	 *  Called via input fire1 released, will call the character fire release action
	 */
	UFUNCTION(BlueprintCallable)
		void FireReleaseInput();

	/**
	 *  Called via input Drop, will call the character drop weapon action
	 */
	UFUNCTION(BlueprintCallable)
	void ReleaseWeaponInput();


};
