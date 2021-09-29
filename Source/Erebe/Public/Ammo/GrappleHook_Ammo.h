// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ammo/Ammo_Base.h"
#include "GrappleHook_Ammo.generated.h"

class UCapsuleComponent;

/**
 * 
 */
UCLASS()
class EREBE_API AGrappleHook_Ammo : public AAmmo_Base
{
	GENERATED_BODY()
	
	/**-----------------	Component Part					-----------------*/
private:
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	//UCapsuleComponent* HookCollider;


	/**-----------------	Variable Part					-----------------*/
public:
	AGrappleHook_Ammo(const FObjectInitializer& ObjectInitializer);


	/**-----------------	Inherit Function Part			-----------------*/
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Define the behavior of the projectile when he hit a valid actor
	virtual void HitValideActor(const FHitResult &HitResult) override;

	/**-----------------	Callback Implementation Part	-----------------*/
protected:
//		virtual void OnSphereColliderBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;


public :
	/** Return hook collider subobject */
	//FORCEINLINE UCapsuleComponent* GetHookColliderComponent() { return HookCollider; }
};
