// Fill out your copyright notice in the Description page of Project Settings.


#include "Ammo/Ammo_Base.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Weapon/Weapon_Shooting_Base.h"
#include "Engine/World.h"

// Sets default values
AAmmo_Base::AAmmo_Base(const FObjectInitializer& ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
	if (SphereCollider != nullptr)
	{
		SphereCollider->SetupAttachment(GetRootComponent());
		SphereCollider->SetCollisionProfileName(TEXT("OverlapAll"));
	}
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	if (StaticMesh != nullptr)
	{
		StaticMesh->SetupAttachment(SphereCollider);
	}
	
	//MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
	if (MovementComponent != nullptr)
	{
		MovementComponent->InitialSpeed = 1500.f;
		MovementComponent->MaxSpeed = 1500.f;
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

	if (bCanMove)
	{
		FVector DistanceTravel = Velocity * DeltaTime;
		Velocity = (bUseGravity) ? Velocity + (GravityScale * 9.81f * DeltaTime) : Velocity;
		
		if (GetWorld() != nullptr)
		{
			TArray<FHitResult> LineTraceHits;
			GetWorld()->LineTraceMultiByProfile(LineTraceHits, GetActorLocation(), GetActorLocation() + DistanceTravel, TEXT("OverlapAll"));
		}

		AddActorLocalOffset(DistanceTravel);
	}

}

void AAmmo_Base::OnSphereColliderBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->ComponentHasTag(TEXT("NotHitable")))
	{
		return;
	}

	if (WeaponOwner == nullptr || Cast<AActor>(WeaponOwner) == OtherActor)
	{
		return;
	}

	if (WeaponOwner->GetActorOwner() == nullptr || Cast<AActor>(WeaponOwner->GetActorOwner()) == OtherActor)
	{
		return;
	}

	HitValideActor(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AAmmo_Base::Inactive(bool bHide)
{
	SetActorTickEnabled(false);
	SetActorEnableCollision(false);
	if (MovementComponent != nullptr)
	{
		MovementComponent->Deactivate();
	}
	

	bCanMove = false;
	Velocity = FVector::ZeroVector;

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

	bCanMove = true;
	Velocity = InitialVelocity;
	
	if (bShow)
	{
		Show();
	}
}

void AAmmo_Base::Hide()
{
	if (StaticMesh != nullptr)
	{
		StaticMesh->SetVisibility(false);
	}
}

void AAmmo_Base::Show()
{
	if (StaticMesh != nullptr)
	{
		StaticMesh->SetVisibility(true);
	}
}

void AAmmo_Base::HitValideActor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Inactive(false);
}
