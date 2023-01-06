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
		SphereCollider->SetCollisionProfileName(TEXT("NoCollision"));
	}
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	if (StaticMesh != nullptr)
	{
		StaticMesh->SetupAttachment(SphereCollider);
		StaticMesh->SetCollisionProfileName(TEXT("NoCollision"));
	}
	
	//MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
	if (MovementComponent != nullptr)
	{
		MovementComponent->InitialSpeed = 1500.f;
		MovementComponent->MaxSpeed = 1500.f;
		MovementComponent->bRotationFollowsVelocity = true;
	}

	CollisionQueryParams.AddIgnoredActor(this);
	CollisionQueryParams.bTraceComplex = true;
	CollisionQueryParams.bIgnoreTouches = false;
	CollisionQueryParams.bIgnoreBlocks = false;

}

// Called when the game starts or when spawned
void AAmmo_Base::BeginPlay()
{
	Super::BeginPlay();
	
	if (SphereCollider != nullptr)
	{
	//	SphereCollider->OnComponentBeginOverlap.AddUniqueDynamic(this, &AAmmo_Base::OnSphereColliderBeginOverlap_Implementation);
	}

}

// Called every frame
void AAmmo_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bCanMove)
	{
		if (PhysicsInteration == 0)
			PhysicsInteration = 1;

		float PhysicsInterationFloat = PhysicsInteration;
		float RatioIIteraction = 1.f / PhysicsInterationFloat;

		
		for (int i = 0; i < PhysicsInteration; i++)
		{
			FVector DistanceTravel = Velocity * DeltaTime * RatioIIteraction;
			Velocity = (bUseGravity) ? Velocity + (GravityScale * 9.81f * DeltaTime * RatioIIteraction) : Velocity;
			AddActorLocalOffset(DistanceTravel);

			if (GetWorld() != nullptr)
			{
				if (GetWorld()->SweepSingleByProfile(HitMovingCollision, GetActorLocation(), GetActorLocation() + DistanceTravel, GetActorQuat(), TEXT("BlockAll"), FCollisionShape::MakeSphere(SweepTestRadius), CollisionQueryParams))
				{
					HandleCollision();
				}
			}

			if (!bCanMove)
				break;
		}
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

	HitValideActor(SweepResult);
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

void AAmmo_Base::Reset()
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	Velocity = FVector::ZeroVector;

	if (WeaponOwner != nullptr)
	{
		SetActorLocation(WeaponOwner->GetActorLocation());
		SetActorRotation(WeaponOwner->GetActorRotation());
	}
	else
	{
		//SetActorLocation(FVector::ZeroVector);
		//SetActorRotation(FQuat::FQuat());
	}
}

void AAmmo_Base::AssociateToWeapon(AWeapon_Shooting_Base* NewWeaponOwner, int32 NewProjectileIndex)
{
	WeaponOwner = NewWeaponOwner;
	ProjectileIndex = NewProjectileIndex;

	if (WeaponOwner != nullptr)
	{
		CollisionQueryParams.AddIgnoredActor(WeaponOwner);

		if (WeaponOwner->GetActorOwner() != nullptr)
		{
			CollisionQueryParams.AddIgnoredActor(WeaponOwner->GetActorOwner());
		}
	}
}

void AAmmo_Base::HitValideActor(const FHitResult& HitResult)
{
	if (bInactiveOnHit)
	Inactive(false);

	if (bStickToActorOnHit)
		AttachToActor(HitResult.GetActor(), FAttachmentTransformRules::KeepWorldTransform);

	OnAmmoHitValidActor.Broadcast(this, HitResult);
}

void AAmmo_Base::HandleCollision()
{
	if (HitMovingCollision.GetActor() == this)
	{
		return;
	}

	if (HitMovingCollision.GetComponent()->ComponentHasTag(TEXT("NotHitable")))
	{
		return;
	}

	if (WeaponOwner == nullptr || Cast<AActor>(WeaponOwner) == HitMovingCollision.GetActor())
	{
		return;
	}

	if (WeaponOwner->GetActorOwner() == nullptr || Cast<AActor>(WeaponOwner->GetActorOwner()) == HitMovingCollision.GetActor())
	{
		return;
	}

	HitValideActor(HitMovingCollision);
}
