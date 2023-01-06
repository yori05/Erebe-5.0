// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Character_RPG.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/World.h"

/**-----------------	Constructor Part		-----------------*/
// Sets default values
ACharacter_RPG::ACharacter_RPG(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

	//We make sure that our viewpoint is the fps one
	UseThirdPerson();

}

/**-----------------	Inherit Function Part		-----------------*/
// Called when the game starts or when spawned
void ACharacter_RPG::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ACharacter_RPG::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

