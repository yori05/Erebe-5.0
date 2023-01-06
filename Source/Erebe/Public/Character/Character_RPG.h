// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Character_Basic.h"
#include "Character_RPG.generated.h"

/**
 *  CharacterRPG
 *  Child of Character_Basic
 *	Character used by the player with a PlayerController_RPG
 *	Can move, jump, Fire
 *  Has for default viewpoint Third Person
 */
UCLASS(config = Game)
class EREBE_API ACharacter_RPG : public ACharacter_Basic
{
	GENERATED_BODY()

		/**-----------------	Variable Part		-----------------*/
protected:

	/**-----------------	Constructor Part		-----------------*/
public:
	// Sets default values for this character's properties
	ACharacter_RPG(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**-----------------	Inherit Function Part		-----------------*/
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
