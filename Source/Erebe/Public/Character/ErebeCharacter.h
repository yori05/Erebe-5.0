// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/InteractiveCharacter.h"
#include "ErebeCharacter.generated.h"

class AWeapon_Base;

/**	
 *	ErebeCharacterState
 *	Define the state of the character
 *	Used by a ErebePlayerController to give order depend of the state and the input to a ErebeCharacter 
 */
UENUM(BlueprintType)
enum EErebeCharacterState
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
 *  ErebeCharacter
 *	Character used by the player with a ErebePlayerController
 *	Can move, jump,
 */
UCLASS()
class EREBE_API AErebeCharacter : public AInteractiveCharacter
{
	GENERATED_BODY()

		/**-----------------	Component Part		-----------------*/
private :
	/** First person mesh  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* FPMesh;

	/**	First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FPCamera;

	/**	Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/**	Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** Erebe Movement component used the same way as the CharacterMovement of the Character, with some custom movement added. */
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UErebeCharacterMovementComponent* ErebeCharacterMovement;

	/** Basic Health component used to handle health for the character */
	UPROPERTY(Category = Health, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UBasicHealthComponent* HealthComponent;

	/** Damage Hit box, a box component detect other actor whom we will inflict damage  */
	UPROPERTY(Category = Damage, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* DamageHitbox;

	/** Component used to handle Dialogue */
	UPROPERTY(Category = "Dialogue", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UDialogueComponent* DialogueComponent;

	/**-----------------	Variable Part		-----------------*/
protected :
	/** CharacterState used to define the state of this character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<enum EErebeCharacterState> CharacterState = EErebeCharacterState::CHARSTATE_Free;

	/**
	* Saved class for the weapon equipped automatically spawn in the begin play of this actor.
	* if set as null won't spawn a weapon
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<AWeapon_Base> WeaponClass;

	/** The weapon currently used by this character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	AWeapon_Base* EquipedWeapon;

	/** The direction of the movement for this character used in case we use the root motion animation */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Input")
	FVector MotionInput;

	/** Check if can pass in fly mode */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	bool bCanFly;

	/** Check if the character is in first person or third */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FirstP/ThirdP")
	bool bIsInFP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	bool bUseRootMotionAnimation;

public:
	/** Name of the HealthComponent. Use this name if you want to use a different class (with ObjectInitializer.SetDefaultSubobjectClass). */
	static FName HealthComponentName;

	/**-----------------	Inherit Function Part		-----------------*/
public :
	// Sets default values for this character's properties (can use the ObjectInitializer to change the character movement component class
	AErebeCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


protected :
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/**-----------------	Erebe Character Function Part		-----------------*/

	/**-----------------	Action Part		-----------------*/
public :
	// Called to make the character move in the forward direction
	UFUNCTION(BlueprintCallable)
	void MoveForward(float Value);

	// Called to make the character move in the right direction
	UFUNCTION(BlueprintCallable)
	void MoveRight(float Value);

	// Called to make the character move in the up direction
	UFUNCTION(BlueprintCallable)
	void MoveUp(float Value); //Need to be check before being used

	// Called to know if the character can go in fly state
	UFUNCTION(BlueprintCallable)
	virtual bool CanFly() { return bCanFly; }

	// Called to make the character jump
	virtual void Jump() override;

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

	/**
	 *  Called to launch a damage on actors in the DamageHitbox, Actors need a HealthComponent
	 */
	UFUNCTION(BlueprintCallable)
	virtual void Punch();

	/**
	 *  Called to launch a interaction using the damageHitbox, Actors need to implement the InteractiveInterface
	 *  Stop on the first actor of whom we can interact 
	 */
	UFUNCTION(BlueprintCallable)
	virtual void Interact();

	UFUNCTION(BlueprintCallable)
	virtual void Run();

	UFUNCTION(BlueprintCallable)
	virtual void StopRunning();

	/**
	*  Called to fire with the weapon equiped if she can fire
	*/
	UFUNCTION(BlueprintCallable)
		virtual void FirePress();

	/**
	*  Called to fire with the weapon equiped if she can fire
	*/
	UFUNCTION(BlueprintCallable)
		virtual void FireRelease();

	// Called to use the follow camera and the third person mesh
	UFUNCTION(BlueprintCallable)
		virtual void PassInThirdPerson();

	// Called to use the first person mesh and camera
	UFUNCTION(BlueprintCallable)
		virtual void PassInFirstPerson();

	/**-----------------	State Part		-----------------*/
protected :
	/** Perform State on the character will be called by the Tick function every frame */
	UFUNCTION(BlueprintCallable)
	void PerformState(float DeltaTime);

protected :
	/** Start a new free state  */
	UFUNCTION(BlueprintCallable)
	void StartFreeState(EErebeCharacterState PreviousCharacterState);

	/** Start a new free state  */
	UFUNCTION(BlueprintCallable)
	void StartFlyingState(EErebeCharacterState PreviousCharacterState);

	/** Start a new focus state  */
	UFUNCTION(BlueprintCallable)
	void StartFocusState(EErebeCharacterState PreviousCharacterState);

	/** Start a new dead state */
	UFUNCTION(BlueprintCallable)
	void StartDeadState(EErebeCharacterState PreviousCharacterState);

public :
	/**
	 * Set a new state for the character state 
	 * @param NewCharacterState This is the new value of the CharacterState if the value is the same of the previous one the function won't do anything
	 */
	UFUNCTION(BlueprintCallable)
	void SetCharacterState(EErebeCharacterState NewCharacterState =  EErebeCharacterState::CHARSTATE_Free);

	/**
	 * Get the current state for the CharacterState return a EErebeCharacterStateValue
	 */
	UFUNCTION(BlueprintCallable)
	EErebeCharacterState GetCharacterState();

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

	/**-----------------	Variable Accessor Part		-----------------*/
public :
	/** Returns the input motion vector **/
	FORCEINLINE FVector GetInputMotionVector() const { return MotionInput; }
	/** Returns the boolean to know if the character can fly **/
	FORCEINLINE bool GetCanFly() const { return bCanFly; }
	/** Returns if the character is currently using fps or tps **/
	FORCEINLINE bool GetIsInFirstPerson() const { return bIsInFP; }
	/** Returns the boolean to know if the character root motion based animation **/
	FORCEINLINE bool GetUseRootMotionAnimation() const { return bUseRootMotionAnimation; }


	/**-----------------	Component Accessor Part		-----------------*/
public :
	/** Returns ErebeCharacterMovement subobject **/
	FORCEINLINE class UErebeCharacterMovementComponent* GetErebeCharacterMovement() const { return ErebeCharacterMovement; }
	/** Returns First person mesh subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetFPMesh() const { return FPMesh; }
	/** Returns First person Camera subobject **/
	FORCEINLINE class UCameraComponent* GetFPCamera() const { return FPCamera; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	/** Returns HealthComponent subobject */
	FORCEINLINE class UBasicHealthComponent* GetHealthComponent() const { return HealthComponent; }
	/** Returns DamageHitbox subobject */
	FORCEINLINE class UBoxComponent* GetDamageHitbox() const { return DamageHitbox; }
	/** Returns DialogueComponent subobject */
	FORCEINLINE class UDialogueComponent* GetDialogueComponent() const { return DialogueComponent; }

};
