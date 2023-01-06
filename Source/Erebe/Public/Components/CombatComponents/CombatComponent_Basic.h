// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent_Basic.generated.h"

class ACharacter_Basic;

/**
* CombatComponent_Basic
* Used by a Character_Basic 
* This component is used to manage attack deal by the attached character
*/
UCLASS( ClassGroup=(Combat), meta=(BlueprintSpawnableComponent), Blueprintable, BlueprintType )
class EREBE_API UCombatComponent_Basic : public UActorComponent
{
	GENERATED_BODY()

/**-----------------	Constructor Part		-----------------*/
public:	
	// Sets default values for this component's properties
	UCombatComponent_Basic(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
/**-----------------	Variable Part		-----------------*/
protected:

	/** Combat component belongs to */
	UPROPERTY(Transient, DuplicateTransient)
		TObjectPtr<ACharacter_Basic> CharacterOwner;


/**-----------------	Inherit Function Part		-----------------*/
public:
	// Called when the game starts
	virtual void BeginPlay() override;
	// Called On post Load
	virtual void PostLoad() override;
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

/**-----------------	Action Function Part		-----------------*/
public:
	//Called to Attack
	UFUNCTION(BlueprintCallable)
	virtual void Attack();

public:
	/** Get the Character that owns UpdatedComponent. */
	UFUNCTION(BlueprintCallable, Category = "Pawn|Components|CombatComponent")
	ACharacter_Basic* GetCharacterOwner() const;
};

FORCEINLINE ACharacter_Basic* UCombatComponent_Basic::GetCharacterOwner() const
{
	return CharacterOwner;
}