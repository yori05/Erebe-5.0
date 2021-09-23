// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Weapon_Base.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AWeapon_Base::AWeapon_Base(const FObjectInitializer& ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	CapsuleComponent->SetupAttachment(GetRootComponent());

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(CapsuleComponent);

	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh1P"));
	Mesh1P->SetupAttachment(CapsuleComponent);

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

void AWeapon_Base::SetOwnerViewpoint(USceneComponent* NewOwnerViewpoint, bool bShouldUseIt)
{
	OwnerViewpont = NewOwnerViewpoint;	
	bUseOwnerViewpoint = OwnerViewpont != nullptr && bShouldUseIt;
}

USceneComponent* AWeapon_Base::GetOwnerViewpoint()
{
	return OwnerViewpont;
}

void AWeapon_Base::SetOwnerImpactpoint(USceneComponent* NewOwnerImpactpoint, bool bShouldUseIt)
{
	OwnerImpactPoint = NewOwnerImpactpoint;
	bUseOwnerImpactpoint = OwnerImpactPoint != nullptr && bShouldUseIt;
}

USceneComponent* AWeapon_Base::GetOwnerImpactpoint()
{
	return OwnerImpactPoint;
}

void AWeapon_Base::AssociateToActorOwner(AActor* NewActorOwner, USceneComponent* NewOwnerViewpoint, USceneComponent* NewOwnerImpactpoint)
{
	if (NewActorOwner != nullptr)
	{
		if (ActorOwner != nullptr)
		{
			DissociatesToActorOwner();
		}

		ActorOwner = NewActorOwner;
		SetOwnerImpactpoint(NewOwnerImpactpoint, true);
		SetOwnerViewpoint(NewOwnerViewpoint, true);
		AttachToActor(ActorOwner, FAttachmentTransformRules::SnapToTargetIncludingScale);
	}
}

void AWeapon_Base::DissociatesToActorOwner() 
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	Mesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	Mesh1P->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
}

void AWeapon_Base::AttachMeshToComponent(USceneComponent* NewParentComponent, FName SocketName)
{
	Mesh->AttachToComponent(NewParentComponent, FAttachmentTransformRules::SnapToTargetIncludingScale,SocketName);
}

void AWeapon_Base::AttachMesh1PToComponent(USceneComponent* NewParentComponent, FName SocketName)
{
	Mesh1P->AttachToComponent(NewParentComponent, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
}

void AWeapon_Base::UseMesh1P()
{
	bUse1P = true;
	Mesh->SetVisibility(false, true);
	Mesh1P->SetVisibility(true, true);
}

void AWeapon_Base::UseMesh3P()
{
	bUse1P = false;
	Mesh->SetVisibility(true, true);
	Mesh1P->SetVisibility(false, true);
}

bool AWeapon_Base::IsUsing1P()
{
	return bUse1P;
}
