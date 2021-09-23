// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/GrappleHook_Gun.h"
#include "CableComponent.h"
#include "Ammo/GrappleHook_Ammo.h"

AGrappleHook_Gun::AGrappleHook_Gun()
{
	// Try to set the GrappleHook ammo blueprint as the projectile class, if want to use a other class need to be set in the blueprint of this class
	
	if (ProjectileClass == nullptr)
	{
		static ConstructorHelpers::FClassFinder<AAmmo_Base> ProjectileDefaultClass(TEXT("/Game/BlueprintClasses/Ammo/Ammo_GrappleHook_BP"));
		
		if (ProjectileDefaultClass.Class != nullptr)
		{
			ProjectileClass = ProjectileDefaultClass.Class;
		}
		else
		{
			// In case we can't find the blueprint the c++ class will be set
			ProjectileClass = AGrappleHook_Ammo::StaticClass();
		}
	}

	//Spawn and set the cable component
	Cable = CreateDefaultSubobject<UCableComponent>("Cable");
	if (Cable != nullptr)
	{
		Cable->SetupAttachment(GetMesh(),TEXT("Muzzle"));
		Cable->CableLength = 0;
		Cable->CableWidth = 2.0f;
		Cable->SetCollisionProfileName(TEXT("GrappleHook_Cable"));
		Cable->EndLocation = FVector::ZeroVector;
		Cable->SetVisibility(false);
	}

	bUseOwnerViewpoint = true;
}

void AGrappleHook_Gun::BeginPlay()
{
	Super::BeginPlay();

	if (PoolSize > 0 && ProjectilePool[0] != nullptr)
	{
		//Cable->SetAttachEndToComponent(ProjectilePool[0]->GetStaticMeshComponent());
	}
}

void AGrappleHook_Gun::Shoot()
{
	Super::Shoot();

	//Cable->SetVisibility(true);

}
