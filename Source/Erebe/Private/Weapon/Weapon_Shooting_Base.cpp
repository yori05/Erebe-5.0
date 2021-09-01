// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Weapon_Shooting_Base.h"
#include "Ammo/Ammo_Base.h"
#include "Math/UnrealMathUtility.h"
#include "Engine/World.h"

AWeapon_Shooting_Base::AWeapon_Shooting_Base(const FObjectInitializer& ObjectInitializer)
{
	bUseOwnerViewpoint = true;

	/** Set Ammo blueprint as default class for the ProjectileClass */
	static ConstructorHelpers::FClassFinder<AAmmo_Base> ProjectileDefaultClass(TEXT("/Game/BlueprintClasses/Ammo/Ammo_Base_BP"));

	if (ProjectileDefaultClass.Class != nullptr)
	{
		ProjectileClass = ProjectileDefaultClass.Class;
	}
	else
	{
		ProjectileClass = AAmmo_Base::StaticClass();
	}

	ProjectilePool.SetNum(PoolSize);
	AvaibleProjectilePool.SetNum(PoolSize);
	SpawnSocketName = TEXT("Muzzle");
}

void AWeapon_Shooting_Base::BeginPlay()
{
	Super::BeginPlay();

	if (bInitializePoolOnBegin)
	{
		IntializeProjectilePool();
	}
}

void AWeapon_Shooting_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeapon_Shooting_Base::IntializeProjectilePool()
{
	auto World = GetWorld();
	AAmmo_Base* Temp = nullptr;

	if (PoolSize > 0 && World != nullptr)
	{
		for (int i = 0; i < PoolSize; i++)
		{
			Temp = World->SpawnActor<AAmmo_Base>(ProjectileClass, GetActorTransform());
			Temp->SetWeaponOwner(this);
			Temp->SetProjectileIndex(i);
			Temp->Inactive();
			ProjectilePool[i] = Temp;
			AvaibleProjectilePool[i] = Temp;
		}
	}	
}

void AWeapon_Shooting_Base::FirePress()
{
	bFire = true;

	if (FireTimeState == EFireTimetate::SHOOTTIME_Press)
	{
		if (FMath::IsNearlyZero(FirstHold_Duration))
		{
			Shoot();
		}
	}
}

void AWeapon_Shooting_Base::FireRelease()
{
	bFire = false;

	if (FireTimeState == EFireTimetate::SHOOTTIME_Release)
	{
		if (FMath::IsNearlyZero(FirstHold_Duration) || FMath::IsNearlyEqual(FirstHold_Duration, FirstHold_Timer))
		{
			Shoot();
		}
	}
}

void AWeapon_Shooting_Base::Shoot()
{
	auto World = GetWorld();
	FVector ProjectileLocation;
	FRotator ProjectileRotation;

	for (int32 i = 0; i < NumberOfProjectileFire; i++)
	{

		if (bUseOwnerImpactpoint && OwnerImpactPoint != nullptr)
		{
			ProjectileLocation = OwnerImpactPoint->GetComponentLocation();
		}
		else if (GetSkeletalMeshComponent() != nullptr)
		{
			ProjectileLocation = GetSkeletalMeshComponent()->GetSocketLocation(SpawnSocketName);
		}
		else
		{
			ProjectileLocation = GetActorLocation();
		}

		if (bUseOwnerViewpoint && OwnerViewpont != nullptr)
		{
			ProjectileRotation = OwnerViewpont->GetComponentRotation();
		}
		else if (GetSkeletalMeshComponent() != nullptr)
		{
			ProjectileRotation = GetSkeletalMeshComponent()->GetSocketRotation(SpawnSocketName);
		}
		else
		{
			ProjectileRotation = GetActorRotation();
		}

		if (ProjectilePool[PoolIndex] == nullptr)
		{
			ProjectilePool[PoolIndex] = World->SpawnActor<AAmmo_Base>(ProjectileClass, GetActorTransform());
		}

		ProjectilePool[PoolIndex]->SetActorLocation(ProjectileLocation);
		ProjectilePool[PoolIndex]->SetActorRotation(ProjectileRotation);
		ProjectilePool[PoolIndex]->Show();
		ProjectilePool[PoolIndex]->Active();
		PoolIndex = (PoolIndex + 1 >= ProjectilePool.Num()) ? 0 : PoolIndex + 1;
	}
}
