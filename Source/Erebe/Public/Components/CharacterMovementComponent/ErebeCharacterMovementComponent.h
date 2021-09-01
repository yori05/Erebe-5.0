// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ErebeCharacterMovementComponent.generated.h"

/**
 *  ErebeMovementMode
 *  Define custom movement mode for a ErebeCharacterMovementComponent
 *  Will interact with a ErebeCharacter to use a custom movement mode
 */
UENUM(BlueprintType, Category = "MovementMode")
enum EErebeMovementMode
{

	/**
	 *  Custom movement None
	 *  The default movement mode the custom movement mode will take this value when we don't use any custom movement mode
	 */
	EREBE_MOVE_None	UMETA(DisplayName = "None"),

	/**
	 *  Custom movement Rush
	 *  Will make the character rush in a direction
	 */
	EREBE_MOVE_Rush UMETA(DisplayName = "Rush"),

	/**
	 *  Custom movement Rush
	 *  Will make the character Dodge in a direction
	 *  The same movement as a jump, exept the character will make the movement while going in a direction
	 */
	EREBE_MOVE_Dodge UMETA(DisplayName = "Dodge"),

	/**
	 *  Size of the enum, If a Character movement mode is a child of this one, it's recommended to set the default value of the new Custom Movement Mode enum to this element
	 *  This will disallow conflict on the CustomMovementMode value for these two Enum
	 */
	EREBE_MOVE_Max UMETA(Hidden),
};

/**
 *  ErebeCharacterMovementComponent
 *	This is a Character movement component with custom movement mode
 */
UCLASS(BlueprintType)
class EREBE_API UErebeCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
private :

	/** ----- Rush Movement Private Parameter ----------------------*/

	// Used to save the Rush movement direction, used by the rush movement mode
	FVector RushDirection;

	// Used to keep the distance traveled
	float RushDistanceTravel;

	/** ----- Rush Movement Private Parameter End ----------------------*/

	// Used to save the previous orient to movement in the custom modes that can change it
	bool bOldOrientRotationToMovement;

public :

	/** ----- Rush Movement Public Parameter ----------------------*/

	/** The Rush's speed  */
	UPROPERTY(Category = "Advanced Movement (Rush Settings)", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
	float RushSpeed = 1200.0f;

	/** The Rush's Distance before it stopped  */
	UPROPERTY(Category = "Advanced Movement (Rush Settings)", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
	float RushDistance = 300.f;

	/** ----- Rush Movement Public Parameter End ----------------------*/

public :

	/** Return value of the CustomMovement in a uint8 value */
	UFUNCTION(BlueprintCallable, Category = "Pawn|Components|CharacterMovement|AdvancedMovement")
		virtual uint8 GetCustomMovementMode() const;

	/** Return value of the CustomMovement in a EAdvancedMovementMode value */
	UFUNCTION(BlueprintCallable, Category = "Pawn|Components|CharacterMovement|AdvancedMovement")
		virtual EErebeMovementMode GetAdvancedMovementMode() const;

	/** Call the Custom Physics for the Custom Movement Mode Called every frame */
	virtual void PhysCustom(float deltaTime, int32 Iterations) override;

	//---------------- Rush Part Begin --------------------- //

	/**
	 *  Use to know if the movement mode can be set to Rush
	 *  Return false if :
	 *		The character is not in the movement mode walking
	 *
	 *  You can override this function to launch it on other event
	 */
	UFUNCTION(BlueprintCallable, Category = "Pawn|Components|CharacterMovement|AdvancedMovement")
		virtual bool CanRush() const;

	/**
	 * Will launch the movement mode to Rush
	 *@param NewDirectionRush is the direction in which the movement will be apply.
	 *	note that if the vector is set to null it will used on the forward to the character
	*/
	UFUNCTION(BlueprintCallable, Category = "Pawn|Components|CharacterMovement|AdvancedMovement")
		virtual void Rush(const FVector& NewRushDirection);

	/**
	 * Use to stop the Rush movement mode
	 *@param ResetMovementMode if set as true will set the movement mode to Walking
	**/
	UFUNCTION(BlueprintCallable, Category = "Pawn|Components|CharacterMovement|AdvancedMovement")
		virtual void StopRush(bool ResetMovementMode = true);

	//The physic of the Rush movement mode
	UFUNCTION(BlueprintCallable, Category = "Pawn|Components|CharacterMovement|AdvancedMovement")
		virtual void PhysRush(float deltaTime, int32 Iterations);

	//Will return true if the MovementMode is in custom + CustomMovementMode is set as the Rush value and UpdatedMovement is set as true
	UFUNCTION(BlueprintCallable, Category = "Pawn|Components|CharacterMovement|AdvancedMovement")
		virtual bool IsRushing() const;

	//---------------- Rush Part End --------------------- //


	//---------------- Dodge Part Begin --------------------- //

	/**
	 *  Use to know if the movement mode can be set to Dodge
	 *  Return false if :
	 *		The character is not in the movement mode walking
	 *
	 *  You can override this function to launch it on other event
	 */
	UFUNCTION(BlueprintCallable, Category = "Pawn|Components|CharacterMovement|AdvancedMovement")
		virtual bool CanDodge() const;

	/**
	 * Will launch the movement mode to Dodge
	 *@param NewDirectionRush is the direction in which the movement will be apply.
	 *	note that if the vector is set to null it will used on the forward to the character
	*/
	UFUNCTION(BlueprintCallable, Category = "Pawn|Components|CharacterMovement|AdvancedMovement")
		virtual void Dodge(const FVector& NewRushDirection);

	/**
	 * Use to stop the Rush movement mode
	 *@param ResetMovementMode if set as true will set the movement mode to Walking
	**/
	UFUNCTION(BlueprintCallable, Category = "Pawn|Components|CharacterMovement|AdvancedMovement")
		virtual void StopDodge(bool ResetMovementMode = true);

	//The physic of the Dodge movement mode
	UFUNCTION(BlueprintCallable, Category = "Pawn|Components|CharacterMovement|AdvancedMovement")
		virtual void PhysDodge(float deltaTime, int32 Iterations);

	//Will return true if the MovementMode is in custom + CustomMovementMode is set as the Dodge value and UpdatedMovement is set as true
	UFUNCTION(BlueprintCallable, Category = "Pawn|Components|CharacterMovement|AdvancedMovement")
		virtual bool IsDodging() const;

	//---------------- Rush Part End --------------------- //

	// TO DO DODGE
};
