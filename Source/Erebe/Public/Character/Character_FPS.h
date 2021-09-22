// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Character_FPS.generated.h"

/**
 *  CharacterFPS
 *	Character used by the player with a PlayerController_FPS
 *	Can move, jump, Fire
 */
UCLASS(config = Game)
class EREBE_API ACharacter_FPS : public ACharacter
{
	GENERATED_BODY()

	/**-----------------	Variable Part		-----------------*/
protected:
	/**
	* Saved class for the weapon equipped automatically spawn in the begin play of this actor.
	* if set as null won't spawn a weapon
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<class AWeapon_Base> DefaultWeaponClass;

	/** The weapon currently equipped by this character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	class AWeapon_Base* EquippedWeapon;

	/**-----------------	Component Part		-----------------*/
private:
	/** First person mesh  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* Mesh1P;

	/**	First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* Camera1P;

	/**-----------------	Constructor Part		-----------------*/
public:
	// Sets default values for this character's properties
	ACharacter_FPS(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**-----------------	Inherit Function Part		-----------------*/
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/**-----------------	Character_FPS Part		-----------------*/
	/**-----------------	Action Part		-----------------*/
public:
	// Called to make the character move in the forward direction
	UFUNCTION(BlueprintCallable)
		void MoveForward(float Value);

	// Called to make the character move in the right direction
	UFUNCTION(BlueprintCallable)
		void MoveRight(float Value);

	// Called to make the character jump
	virtual void Jump() override;

	// Called to make the character stop jump
	virtual void StopJumping() override;

//	/**
//	*  Called to fire with the weapon equipped if she can fire
//	*/
//	UFUNCTION(BlueprintCallable)
//		virtual void FirePress();
//
//	/**
//	*  Called to fire with the weapon equipped if she can fire
//	*/
//	UFUNCTION(BlueprintCallable)
//		virtual void FireRelease();

	/**-----------------	Weapon Function Part		-----------------*/
public:
	/**
	 * Spawn and equip a new weapon on this character and release or destroy the previous one  
	 * if @NewWeaponClass null don't spawn any weapon
	 * if @bRealeasePreviousWeapon is set as false destroy the previous weapon
	 */ 
	UFUNCTION(BlueprintCallable)
	virtual void SpawnAndEquipWeapon(TSubclassOf<class AWeapon_Base> NewWeaponClass = nullptr, bool bReleasePreviousWeapon = false);

	/**
	 * Spawn and equip a new weapon on this character and release or destroy the previous one
	 * if @bRealeasePreviousWeapon is set as false destroy the previous weapon
	 */
	UFUNCTION(BlueprintCallable)
	virtual void SpawnAndEquipDefaultWeapon(bool bReleasePreviousWeapon = false);

	/** 
	 * Equip a weapon already spawn
	 * if @bRealeasePreviousWeapon is set as false destroy the previous weapon
	 */
	UFUNCTION(BlueprintCallable)
	virtual void EquipWeapon(class AWeapon_Base* NewWeapon = nullptr, bool bReleasePreviousWeapon = false);

	/**
	 * Release the equipped weapon
	 */
	UFUNCTION(BlueprintCallable)
	virtual void ReleaseWeapon();

	/**
	 * Destroy the equipped weapon
	 */
	UFUNCTION(BlueprintCallable)
	virtual void DestroyWeapon();

	/**-----------------	Variable Accessor Part		-----------------*/
public:
	/** Returns default weapon class **/
	FORCEINLINE TSubclassOf<class AWeapon_Base> GetDefaultWeaponClass() const { return DefaultWeaponClass; }
	/** Returns equipped weapon object **/
	FORCEINLINE class AWeapon_Base* GetEquippedWeapon() const { return EquippedWeapon; }

	/**-----------------	Component Accessor Part		-----------------*/
public:
	/** Returns First person mesh subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns First person Camera subobject **/
	FORCEINLINE class UCameraComponent* GetCamera1P() const { return Camera1P; }

};
