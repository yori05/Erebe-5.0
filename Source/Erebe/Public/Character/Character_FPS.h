// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Character_Basic.h"
#include "Character_FPS.generated.h"

/**
 *  CharacterFPS
 *  Child of Character_Basic
 *	Character used by the player with a PlayerController_FPS
 *	Can move, jump, Fire
 *  Has for default viewpoint First Person
 */
UCLASS(config = Game)
class EREBE_API ACharacter_FPS : public ACharacter_Basic
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

	/**
	*  Called to fire with the weapon equipped if she can fire
	*/
	UFUNCTION(BlueprintCallable)
	virtual void FirePress();

	/**
	*  Called to fire with the weapon equipped if she can fire
	*/
	UFUNCTION(BlueprintCallable)
	virtual void FireRelease();

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

};
