// Fill out your copyright notice in the Description page of Project Settings.


#include "Ammo/Ammo_Base.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Weapon/Weapon_Shooting_Base.h"

// Sets default values
AAmmo_Base::AAmmo_Base(const FObjectInitializer& ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SceneRoot->SetupAttachment(GetRootComponent());

	SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
	if (SphereCollider != nullptr)
	{
		//SetRootComponent(HookSphereCollider);
		SphereCollider->SetupAttachment(GetRootComponent());
		SphereCollider->SetCollisionProfileName(TEXT("OverlapAll"));
	}

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	if (StaticMesh != nullptr)
	{
		StaticMesh->SetupAttachment(SphereCollider);
		StaticMesh->SetWorldScale3D(FVector(0.2f));

	}

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
	if (MovementComponent != nullptr)
	{
		MovementComponent->InitialSpeed = 2500.f;
		MovementComponent->MaxSpeed = 3000.f;
		MovementComponent->bRotationFollowsVelocity = true;
	}
}

// Called when the game starts or when spawned
void AAmmo_Base::BeginPlay()
{
	Super::BeginPlay();
	
	if (SphereCollider != nullptr)
	{
		SphereCollider->OnComponentBeginOverlap.AddUniqueDynamic(this, &AAmmo_Base::OnSphereColliderBeginOverlap_Implementation);
	}

}

// Called every frame
void AAmmo_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAmmo_Base::Inactive(bool bHide)
{
	SetActorTickEnabled(false);
	SetActorEnableCollision(false);
	if (MovementComponent != nullptr)
	{
		MovementComponent->Deactivate();
	}

	if (bHide)
	{
		Hide();
	}
}

void AAmmo_Base::Active(bool bShow)
{
	SetActorTickEnabled(true);
	SetActorEnableCollision(true);
	if (MovementComponent != nullptr)
	{
		MovementComponent->Activate(true);
		MovementComponent->SetVelocityInLocalSpace(FVector(MovementComponent->InitialSpeed, 0.f, 0.f));
	}

	if (bShow)
	{
		Show();
	}
}

void AAmmo_Base::Hide()
{
	StaticMesh->SetVisibility(false);
}

void AAmmo_Base::Show()
{
	StaticMesh->SetVisibility(true);
}

void AAmmo_Base::OnSphereColliderBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}
