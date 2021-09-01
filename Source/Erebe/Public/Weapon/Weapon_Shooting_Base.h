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
	* Can use @FirstHold_Duration : to define the time to hold after which the projectile is fire
	*/
	SHOOTTIME_Press UMETA(DisplayName = "Press"),

	/**
	* The state that fire the projectile when fire is release
	* Can use @FirstHold_Duration : to define the time to hold after which the projectile can be fire
	*/
	SHOOTTIME_Release UMETA(DisplayName = "Release"),

	/**
	* The state that fire the projectile in continue when fire is hold
	* Can use @FirstHold_Duration : to define the time after which the first projectile is fired
	* Use @ColdDown_Duration : to define the time between two shoot if fire is hold
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

	/**-----------------	Variable Part		-----------------*/
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

	/** Use to save which fire state the weapon use */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TEnumAsByte<enum EFireTimetate> FireTimeState = EFireTimetate::SHOOTTIME_Press;

	/** Use to save which fire state the weapon use */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 NumberOfProjectileFire = 1;

	/** Use to set the time to hold fire for the first projectile to go */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float FirstHold_Duration = 0.f;

	/** Use to time @FirstHold_Duration */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float FirstHold_Timer = 0.f;

	/** Use to set the time between two projectile to go */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float ColdDown_Duration = 0.f;

	/** Use to time @ColdDown_Duration */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float ColdDown_Timer = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bInitializePoolOnBegin = true;

	/** Use to save if fire is press or release */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool bFire = false;

	/**-----------------	Inherit Function Part		-----------------*/
public :
	AWeapon_Shooting_Base(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/**-----------------	Weapon_Shooting Function Part		-----------------*/
protected:
	// Called to spawn every projectile used for this weapon
	UFUNCTION(BlueprintCallable)
	virtual void IntializeProjectilePool();

public :
	/**
	* Press fire for this weapon
	* In case the weapon use the FireTimeState Press and @FirstShoot_Duration is 0, this function will call shoot
	*/
	UFUNCTION(BlueprintCallable)
	virtual void FirePress();

	/**
	* Press fire for this weapon
	* In case the weapon use the FireTimeState Release and @FirstShoot_Duration is 0 or equal to @FirstShoot_Timer , this function will call shoot
	*/
	UFUNCTION(BlueprintCallable)
		virtual void FireRelease();

protected:
	// Shoot the projectile
	UFUNCTION(BlueprintCallable)
		virtual void Shoot();

	/**-----------------	Variable Accessor Part			-----------------*/
public:

	FORCEINLINE TSubclassOf<AAmmo_Base> GetProjectileClass() { return ProjectileClass; };

	FORCEINLINE void SetProjectileClass(TSubclassOf<AAmmo_Base> NewProjectileClass) {ProjectileClass = NewProjectileClass;};

	FORCEINLINE TArray<AAmmo_Base*> GetProjectilePool() {return ProjectilePool;};

	FORCEINLINE int32 GetPoolIndex() { return PoolIndex; };

	FORCEINLINE int32 GetPoolSize() { return PoolSize; };

	FORCEINLINE EFireTimetate GetFireTimeState() { return FireTimeState; };

	FORCEINLINE void SetFireTimeState(EFireTimetate NewFireTimeState) { FireTimeState = NewFireTimeState; };

	FORCEINLINE int32 GetNumberOfProjectileFire() { return NumberOfProjectileFire; };

	FORCEINLINE float GetFirstHold_Duration() { return FirstHold_Duration; };

	FORCEINLINE void SetFirstHold_Duration(float NewFirstHold_Duration) { FirstHold_Duration = NewFirstHold_Duration; };

	FORCEINLINE float GetFirstHold_Timer() { return FirstHold_Timer; };

	FORCEINLINE void SetFirstHold_Timer(float NewFirstHold_Timer) { FirstHold_Timer = NewFirstHold_Timer; };

	FORCEINLINE float GetColdDown_Duration() { return ColdDown_Duration; };

	FORCEINLINE void SetColdDown_Duration(float NewColdDown_Duration) { ColdDown_Duration = NewColdDown_Duration; };

	FORCEINLINE float GetColdDown_Timer() { return ColdDown_Timer; };

	FORCEINLINE void SetColdDown_Timer(float NewColdDown_Timer) { ColdDown_Timer = NewColdDown_Timer; };

	FORCEINLINE bool GetShouldInitializePoolOnBegin() { return bInitializePoolOnBegin; };

	FORCEINLINE bool GetFire() { return bFire; };

};
