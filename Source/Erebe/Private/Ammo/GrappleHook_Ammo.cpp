// Fill out your copyright notice in the Description page of Project Settings.


#include "Ammo/GrappleHook_Ammo.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Weapon/GrappleHook_Gun.h"

AGrappleHook_Ammo::AGrappleHook_Ammo() 
{
}

void AGrappleHook_Ammo::BeginPlay()
{
	Super::BeginPlay();

}

void AGrappleHook_Ammo::OnSphereColliderBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereColliderBeginOverlap_Implementation(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	//if (OtherComp->ComponentHasTag(TEXT("NotHitable")))
	//{
	//	return;
	//}
	//
	//if (WeaponOwner == nullptr || Cast<AActor>(WeaponOwner) == OtherActor)
	//{
	//	return;
	//}
	//
	//if (WeaponOwner->GetActorOwner() == nullptr || Cast<AActor>(WeaponOwner->GetActorOwner()) == OtherActor)
	//{
	//	return;
	//}
	//
	//Inactive(false);
	//
	//if (GetSphereColliderComponent() == OverlappedComponent)
	//{
	//	//SetActorLocation(SweepResult.ImpactPoint);
	//	//auto TempRota = (-SweepResult.Normal).Rotation();
	//	//SetActorRotation(HookSphereCollider->GetComponentRotation().RotateVector(-SweepResult.Normal).Rotation());
	//	//AddActorLocalOffset(GetActorForwardVector() * (0 - (HookSphereCollider->GetScaledSphereRadius() - SweepResult.PenetrationDepth)));
	//}

	
	//
}

