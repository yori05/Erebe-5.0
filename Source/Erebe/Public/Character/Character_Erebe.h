// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Character_Basic.h"

#include "Character_Erebe.generated.h"

class UErebeCharacterMovementComponent;
class UBasicHealthComponent;
class UBoxComponent;
class AWeapon_Base;

/**
 *	CharacterState
 *	Define the state of the character
 *	Used by a PlayerController_Erebe to give order depend of the state and the input to a Character_Erebe
 */
UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	/** The default state for the character allow him to use all the default movement */
	CHARSTATE_Free UMETA(DisplayName = "Free"),

	/**
	 * The state that let the character fly
	 * Allow to move in every direction
	 */
	 CHARSTATE_Flying UMETA(DisplayName = "Flying"),

	 /**
	  * The state that let the character focus on a target or a direction will make him move when keeping the target in front of him
	  * Allow focus movement like dodge or directional jump
	  */
	  CHARSTATE_Focus UMETA(DisplayName = "Focus"),

	  /**
	   * The state that block the character from making any movement
	   */
	   CHARSTATE_Dead UMETA(DisplayName = "Dead"),

	   /** Size of this enum can be used to know the number of state this enum give access to. In case we want make a enum encapsulate this one could be used on the first one of the new enum */
	   CHARSTATE_Max UMETA(Hidden),
};

/**
 *  Character_Erebe
 * 
 *  Child of Character_Basic
 *	Character used by the player with a PlayerController_Erebe
 *	Can move, jump,
 * 
 *  For now used to remove ErebeCharacter and link previous code with new one making it more accessible
 */
UCLASS()
class EREBE_API ACharacter_Erebe : public ACharacter_Basic
{
	GENERATED_BODY()

	/**-----------------	Component Part		-----------------*/
private:

	/** Erebe Movement component used the same way as the CharacterMovement of the Character, with some custom movement added. */
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UErebeCharacterMovementComponent> ErebeCharacterMovement;

	/** Basic Health component used to handle health for the character */
	UPROPERTY(Category = Health, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UBasicHealthComponent> HealthComponent;

	/** Damage Hit box, a box component detect other actor whom we will inflict damage  */
	UPROPERTY(Category = Damage, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UBoxComponent> DamageHitbox;

	/**-----------------	Variable Part		-----------------*/
public:

	/** Name of the HealthComponent. Use this name if you want to use a different class (with ObjectInitializer.SetDefaultSubobjectClass). */
	static FName HealthComponentName;

protected:

	/** CharacterState used to define the state of this character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
		ECharacterState CharacterState = ECharacterState::CHARSTATE_Free;

	/**
	* Saved class for the weapon equipped automatically spawn in the begin play of this actor.
	* if set as null won't spawn a weapon
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSubclassOf<AWeapon_Base> WeaponClass;

	/** The weapon currently used by this character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
		TObjectPtr<AWeapon_Base> EquipedWeapon;

	/** Check if can pass in fly mode */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
		bool bCanFly;

	/**-----------------	Constructor Part		-----------------*/
public:

	// Sets default values for this character's properties
	ACharacter_Erebe(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


	/**-----------------	Actor Function Part		-----------------*/
protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/**-----------------	Action Part		-----------------*/
	/**-----------------	Jump Part		-----------------*/
protected:

	/**
	* It's better to call "CallJump" rather than "Jump" and let call jump called it
	* Will call the character jump
	*/
	virtual void Jump() override;

public:

	// Called to make the character fly
	UFUNCTION(BlueprintCallable)
		virtual void DebugFly();

	// Called to know if the character can go in fly state
	UFUNCTION(BlueprintCallable)
		virtual bool CanFly() { return bCanFly; }

	// Called to make the character rush in a direction
	UFUNCTION(BlueprintCallable)
		virtual void Rush();

	/** Event fired when the character has just started jumping */
	/*
	UFUNCTION(BlueprintNativeEvent, Category = Character)
		void OnJumped();
	virtual void OnJumped_Implementation();
	*/

	// Called to make the character dodge in a direction
	UFUNCTION(BlueprintCallable)
		virtual void Dodge();

	/**-----------------	Combat Part		-----------------*/

protected:

	// Called to Attack with the Combat Component
	virtual void Attack() override;

public:

	UFUNCTION(BlueprintCallable)
		virtual void Run();

	UFUNCTION(BlueprintCallable)
		virtual void StopRunning();

	/**-----------------	State Part		-----------------*/
protected:
	/** Perform State on the character will be called by the Tick function every frame */
	UFUNCTION(BlueprintCallable)
		void PerformState(float DeltaTime);

protected:
	/** Start a new free state  */
	UFUNCTION(BlueprintCallable)
		void StartFreeState(ECharacterState PreviousCharacterState);

	/** Start a new free state  */
	UFUNCTION(BlueprintCallable)
		void StartFlyingState(ECharacterState PreviousCharacterState);

	/** Start a new focus state  */
	UFUNCTION(BlueprintCallable)
		void StartFocusState(ECharacterState PreviousCharacterState);

	/** Start a new dead state */
	UFUNCTION(BlueprintCallable)
		void StartDeadState(ECharacterState PreviousCharacterState);

public:
	/**
	 * Set a new state for the character state
	 * @param NewCharacterState This is the new value of the CharacterState if the value is the same of the previous one the function won't do anything
	 */
	UFUNCTION(BlueprintCallable)
		void SetCharacterState(ECharacterState NewCharacterState);

	/**
	 * Get the current state for the CharacterState return a EErebeCharacterStateValue
	 */
	UFUNCTION(BlueprintCallable)
		ECharacterState GetCharacterState();

	/** Set the character state has free */
	UFUNCTION(BlueprintCallable)
		void SetFreeState();

	/** Set the character state has flying */
	UFUNCTION(BlueprintCallable)
		void SetFlyingState();

	/** Set the character state has focus */
	UFUNCTION(BlueprintCallable)
		void SetFocusState();

	/** Set the character state has dead */
	UFUNCTION(BlueprintCallable)
		void SetDeadState();

};
