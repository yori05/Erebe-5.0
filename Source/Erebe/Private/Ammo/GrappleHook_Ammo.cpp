// Fill out your copyright notice in the Description page of Project Settings.


#include "Ammo/GrappleHook_Ammo.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Weapon/GrappleHook_Gun.h"

AGrappleHook_Ammo::AGrappleHook_Ammo(const FObjectInitializer& ObjectInitializer)
{
}

void AGrappleHook_Ammo::BeginPlay()
{
	Super::BeginPlay();

}


void AGrappleHook_Ammo::HitValideActor(const FHitResult &HitResult)
{
	Super::HitValideActor(HitResult);

}

