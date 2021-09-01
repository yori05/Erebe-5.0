// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Weapon_Base.h"
#include "Weapon_Shooting_Base.generated.h"

class AAmmo_Base;

/**
 *	EFireTimetate
 *	Define the time where the weapon should fire
 *	Used by a Weapon_Shooting_Base to save the fire time state of the weapon.
 */
UENUM(BlueprintType)
enum EFireTimetate
{
	/**
	* Default mode
	* The state that fire the projectile when fire is press 
	* Can use @FirstHoldTime : to define the time to hold after which the projectile is fire
	*/
	SHOOTTIME_Press UMETA(DisplayName = "Press"),

	/**
	* The state that fire the projectile when fire is release
	* Can use @FirstHoldTime : to define the time to hold after which the projectile can be fire
	*/
	SHOOTTIME_Release UMETA(DisplayName = "Release"),

	/**
	* The state that fire the projectile in continue when fire is hold
	* Can use @FirstHoldTime : to define the time after which the first projectile is fired
	* Use @ColdDownTime : to define the time between two shoot if fire is hold
	*/
	SHOOTTIME_Continue UMETA(DisplayName = "Continue"),

	/** Size of this enum can be used to know the number of state this enum give access to. In case we want make a enum encapsulate this one could be used on the first one of the new enum */
	SHOOTTIME_Max UMETA(Hidden),
};


/**
 * Base for shooting weapon
 */
UCLASS()
class EREBE_API AWeapon_Shooting_Base : public AWeapon_Base
{
	GENERATED_BODY()

protected:
	/**
	 * Saved class for the projectile.
	 * When using the shoot function will use one of this class to spawn a projectile
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<AAmmo_Base> ProjectileClass;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TArray<AAmmo_Base*> ProjectilePool;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TArray<AAmmo_Base*> AvaibleProjectilePool;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName SpawnSocketName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 PoolIndex = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 PoolSize = 1;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bInitializePoolOnBegin = true;

	/** Use to set the time to hold fire for the first projectile to go */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float FirstHoldTime = 0.f;

	/** Use to set the time between two projectile to go */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float ColdDownTime = 0.f;

	/** Use to save which fire state the weapon use */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EFireTimetate FireTimeState = EFireTimetate::SHOOTTIME_Press;

public :
	AWeapon_Shooting_Base(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called to spawn every projectile used for this weapon
	UFUNCTION(BlueprintCallable)
	virtual void IntializeProjectilePool();

public:
	// Return the projectile class used by the weapon
	UFUNCTION(BlueprintCallable)
	TSubclassOf<AAmmo_Base> GetProjectileClass();

	// Set a new projectile class
	UFUNCTION(BlueprintCallable)
	void SetProjectileClass(TSubclassOf<AAmmo_Base> NewProjectileClass);

	// Return the projectile pool
	UFUNCTION(BlueprintCallable)
	TArray<AAmmo_Base*> GetProjectilePool();

	// Get the current index used in the pool
	UFUNCTION(BlueprintCallable)
	int32 GetPoolIndex();

	// Get the pool projectile size
	UFUNCTION(BlueprintCallable)
	int32 GetPoolSize();

	// Press fire if 
	UFUNCTION(BlueprintCallable)
	virtual void FirePress();

	UFUNCTION(BlueprintCallable)
	virtual void FireRelease();

	// Shoot the projectile
	UFUNCTION(BlueprintCallable)
	virtual void Shoot();
};
