// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/MovementComponent.h"
#include "MovementComponent_Ammo.generated.h"

/**
 *	MovementComponentAmmo 
 *  A movement component used principally by ammo to move.
 *  Make a simple jump to a point on a other.
 *	If collision is activated will use a raycast to see if the object collide with something
 */
UCLASS()
class EREBE_API UMovementComponent_Ammo : public UActorComponent
{
	GENERATED_BODY()
	
};
