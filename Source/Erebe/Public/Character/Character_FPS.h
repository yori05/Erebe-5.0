// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Character_FPS.generated.h"

/**
 *  CharacterFPS
 *	Character used by the player with a PlayerController_FPS
 *	Can move, jump, Fire
 */
UCLASS(config = Game)
class EREBE_API ACharacter_FPS : public ACharacter
{
	GENERATED_BODY()

	/**-----------------	Component Part		-----------------*/
private:
	/** First person mesh  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* Mesh1P;

	/**	First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* Camera1P;

	/**-----------------	Constructor Part		-----------------*/
public:
	// Sets default values for this character's properties
	ACharacter_FPS(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**-----------------	Inherit Function Part		-----------------*/
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/**-----------------	Action Part		-----------------*/
public:
	// Called to make the character move in the forward direction
	UFUNCTION(BlueprintCallable)
		void MoveForward(float Value);

	// Called to make the character move in the right direction
	UFUNCTION(BlueprintCallable)
		void MoveRight(float Value);

	// Called to make the character jump
	virtual void Jump() override;

	// Called to make the character stop jump
	virtual void StopJumping() override;

	/**-----------------	Component Accessor Part		-----------------*/
public:
	/** Returns First person mesh subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns First person Camera subobject **/
	FORCEINLINE class UCameraComponent* GetCamera1P() const { return Camera1P; }

};
