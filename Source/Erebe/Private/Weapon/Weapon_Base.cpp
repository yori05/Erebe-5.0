// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Weapon_Base.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
AWeapon_Base::AWeapon_Base(const FObjectInitializer& ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SceneRoot->SetupAttachment(GetRootComponent());

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(SceneRoot);
}

// Called when the game starts or when spawned
void AWeapon_Base::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon_Base::SetActorOwner(AActor* NewActorOwner)
{
	ActorOwner = NewActorOwner;
}

AActor* AWeapon_Base::GetActorOwner()
{
	return ActorOwner;
}

void AWeapon_Base::SetOwnerViewpoint(USceneComponent* NewOwnerViewpoint)
{
	OwnerViewpont = NewOwnerViewpoint;
}

USceneComponent* AWeapon_Base::GetOwnerViewpoint()
{
	return OwnerViewpont;
}

void AWeapon_Base::SetOwnerImpactpoint(USceneComponent* NewOwnerImpactpoint)
{
	OwnerImpactPoint = NewOwnerImpactpoint;
}

USceneComponent* AWeapon_Base::GetOwnerImpactpoint()
{
	return OwnerImpactPoint;
}
