// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BasicHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHealthChangeDelegate, int32, Health, int32, HealthMax);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReceiveHealDelegate, int32, HealQuantity);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReceiveDamageDelegate, int32, DamageQuantity);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBasicDelegate);

/**
 *  BasicHealthComponent
 * 
 *	A component used to handle health for a actor
 *	The basic one contain function to increment, decrement, lose all health or restore fully
 *  Get callback for all these event.
 * 
 *  *Health and HealthMax are protected parameter, to easier the use with child of this class
 *  For now health is stored as a int to simplify the use, will pass as a float if it's seem useful later.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class EREBE_API UBasicHealthComponent : public UActorComponent
{
	GENERATED_BODY()

protected :
	/** Health value, will contain the current health can't excess HealthMax */
	UPROPERTY(Category = "Health Setting", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
	int32 Health = 10;

	/** HealthMax value, will contain the max health the actor can have */
	UPROPERTY(Category = "Health Setting", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
	int32 HealthMax = 10;

public :
	/** Callback part Begin */
	
	/** Called in the ChangeHealth function */
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FHealthChangeDelegate OnHealthChange;

	/** Called in the function ChangeHealth when the Health go to the max value */
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBasicDelegate OnHealthToTheMax;

	/** Called in the function ChangeHealth when the Health go to the zero value */
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBasicDelegate OnHealthToZero;

	/** Called in the function ReceiveDamage */
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FReceiveDamageDelegate OnReceiveDamge;

	/** Called in the function ReceiveHeal */
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FReceiveHealDelegate OnReciveHeal;

	/** Called in the function RestorFullHealth */
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBasicDelegate OnRestoreFullHealth;

	/** Called in the function LoseAllHealth */
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBasicDelegate OnLoseAllHealth;

	/** Callback part End */

public:	
	// Sets default values for this component's properties
	UBasicHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


	/**
	 *	Called to change the component's health.
	 *  Will clamp to 0 or HealthMax
	 *  Won't do anything if the @NewValue is the same as @Health current value
	 *  Will call the callback OnHealthChange then :
	 *		- OnGetHealthToTheMax if the health change to the max
	 *		- OnHealthToZero if the health change to zero
	 */
	UFUNCTION(BlueprintCallable)
	virtual void ChangeHealth(int32 NewValue);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**
	 *	Called to damage the component.
	 *  Won't do anything if @DamageValue is zero || if @Health is zero
	 *  Will call the callback OnReceiveDamage after have called the function ChangeHealth
	 */
	UFUNCTION(BlueprintCallable)
	virtual void ReceiveDamage(int32 DamageValue);

	/**
	 *	Called to heal the component.
	 *  Won't do anything if @HealValue is zero || if @Health == @HealthMax
	 *  Will call the callback OnReceiveHeal after have called the function ChangeHealth
	 */
	UFUNCTION(BlueprintCallable)
	virtual void ReceiveHeal(int32 HealValue);

	/**
	 *	Called to restore full health to the component.
	 *  Won't do anything if @Health is already equal to @HealthMax
	 *  Will call the callback OnRestoreFullHealth after have called the function ChangeHealth
	 *  Won't call the function ReceiveHeal
	 */
	UFUNCTION(BlueprintCallable)
	virtual void RestorFullHealth();


	/**
	 *	Called to lose all health to the component.
	 *  Won't do anything if @Health is already equal to zero
	 *  Will call the callback OnLoseAllHealth after have called the function ChangeHealth
	 *  Won't call the function ReceiveDamage
	 */
	UFUNCTION(BlueprintCallable)
	virtual void LoseAllHealth();

public :

	/** Give the Health quantity */
	UFUNCTION(BlueprintCallable)
	int32 GetHealth() { return Health; }

	/** Give the HealthMax quantity */
	UFUNCTION(BlueprintCallable)
	int32 GetHealthMax() { return HealthMax; }

	/** Give the Health/HealthMax ratio */
	UFUNCTION(BlueprintCallable)
	float GetHealthRatio() { return (float)Health / (float)HealthMax; }

	/** Return true if Health == HealthMax */
	UFUNCTION(BlueprintCallable)
	bool IsFullHealth() { return Health == HealthMax; }

	/** Return true if Health > 0 */
	UFUNCTION(BlueprintCallable)
	bool HasHealth() { return Health > 0; }
};
