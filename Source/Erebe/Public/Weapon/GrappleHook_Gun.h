// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Weapon_Shooting_Base.h"
#include "GrappleHook_Gun.generated.h"

class UCableComponent;

/**
 * 
 */
UCLASS()
class EREBE_API AGrappleHook_Gun : public AWeapon_Shooting_Base
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCableComponent* Cable;

public:
	/** Used to set the default value of this actor */
	AGrappleHook_Gun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called to shot a projectille
	virtual void Shoot() override;

public:
	/** Return the cable subobject */
	FORCEINLINE UCableComponent* GetCableComponent() { return Cable; }

};
