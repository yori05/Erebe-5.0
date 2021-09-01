// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Weapon_Base.h"
#include "Weapon_Shooting_Base.generated.h"

class AAmmo_Base;

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

public :
	AWeapon_Shooting_Base(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called to spawn every projectile used for this weapon
	UFUNCTION(BlueprintCallable)
	virtual void IntializeProjectilePool();

public:
	UFUNCTION(BlueprintCallable)
	TSubclassOf<AAmmo_Base> GetProjectileClass();

	UFUNCTION(BlueprintCallable)
	void SetProjectileClass(TSubclassOf<AAmmo_Base> NewProjectileClass);

	UFUNCTION(BlueprintCallable)
	TArray<AAmmo_Base*> GetProjectilePool();

	UFUNCTION(BlueprintCallable)
	int32 GetPoolIndex();

	UFUNCTION(BlueprintCallable)
	int32 GetPoolSize();

	UFUNCTION(BlueprintCallable)
	virtual void Shoot();
};
