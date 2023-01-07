// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Character_Basic.h"
#include "Character_TimeManipulator.generated.h"

/**
 * 
 */
UCLASS()
class EREBE_API ACharacter_TimeManipulator : public ACharacter_Basic
{
	GENERATED_BODY()
	
	/**-----------------	Constructor Part		-----------------*/
public:

	// Sets default values for this character's properties
	ACharacter_TimeManipulator(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

};
