// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Character_FPS.h"
#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Weapon/Weapon_Base.h"
#include "Weapon/Weapon_Shooting_Base.h"
#include "Engine/World.h"

DEFINE_LOG_CATEGORY_STATIC(LogChararacterFPS, Warning, All);

/**-----------------	Constructor Part		-----------------*/
// Sets default values
ACharacter_FPS::ACharacter_FPS(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{

	// Param Camera Component
	USceneComponent* FPVPCamera = GetFPViewPointCamera();
	if (FPVPCamera != nullptr)
	{
		FPVPCamera->SetupAttachment(GetCapsuleComponent());
		FPVPCamera->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	}

	// Param SkeletalMesh Component
	USkeletalMeshComponent* SkeletalMesh = GetMesh();
	if (SkeletalMesh != nullptr)
	{
		SkeletalMesh->SetupAttachment(FPVPCamera);
		SkeletalMesh->SetOnlyOwnerSee(true);
		SkeletalMesh->bCastDynamicShadow = false;
		SkeletalMesh->CastShadow = false;
		SkeletalMesh->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
		SkeletalMesh->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));
	}

	//We make sure that our viewpoint is the fps one
	UseFirstPerson();

}

/**-----------------	Inherit Function Part		-----------------*/
// Called when the game starts or when spawned
void ACharacter_FPS::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnAndEquipDefaultWeapon();
}

// Called every frame
void ACharacter_FPS::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/**-----------------	Action Part		-----------------*/
/**
*  Called to fire with the weapon equipped if she can fire
*/
UFUNCTION(BlueprintCallable)
void ACharacter_FPS::FirePress()
{
	if (EquippedWeapon != nullptr)
	{
		auto ShootingWeapon = Cast<AWeapon_Shooting_Base>(EquippedWeapon);

		if (ShootingWeapon != nullptr)
		{
			ShootingWeapon->FirePress();
		}
	}
}

/**
*  Called to fire with the weapon equipped if she can fire
*/
UFUNCTION(BlueprintCallable)
void ACharacter_FPS::FireRelease()
{
	if (EquippedWeapon != nullptr)
	{
		auto ShootingWeapon = Cast<AWeapon_Shooting_Base>(EquippedWeapon);

		if (ShootingWeapon != nullptr)
		{
			ShootingWeapon->FireRelease();
		}
	}
}

/**-----------------	Weapon Function Part		-----------------*/
/**
 * Spawn and equip a new weapon on this character and release or destroy the previous one
 * if @NewWeaponClass null don't spawn any weapon
 * if @bRealeasePreviousWeapon is set as false destroy the previous weapon
 */
void ACharacter_FPS::SpawnAndEquipWeapon(TSubclassOf<AWeapon_Base> NewWeaponClass, bool bReleasePreviousWeapon)
{
	auto World = GetWorld();

	AWeapon_Base* NewWeapon = nullptr;

	if (World && NewWeaponClass != nullptr)
	{
		NewWeapon = World->SpawnActor<AWeapon_Base>(NewWeaponClass, GetActorTransform());
		//EquippedWeapon = World->SpawnActor<AWeapon_Base>(WeaponClass, GetActorTransform());

	}

	EquipWeapon(NewWeapon, bReleasePreviousWeapon);
}

/**
 * Spawn and equip a new weapon on this character and release or destroy the previous one
 * if @bRealeasePreviousWeapon is set as false destroy the previous weapon
 */
void ACharacter_FPS::SpawnAndEquipDefaultWeapon(bool bReleasePreviousWeapon)
{
	SpawnAndEquipWeapon(DefaultWeaponClass, bReleasePreviousWeapon);
}

/**
 * Equip a weapon already spawn and release or destroy the equipped weapon
 * if @bRealeasePreviousWeapon is set as false destroy the previous weapon
 */
void ACharacter_FPS::EquipWeapon(AWeapon_Base* NewWeapon, bool bReleasePreviousWeapon)
{
	if (EquippedWeapon != nullptr)
	{
		if (bReleasePreviousWeapon)
		{
			ReleaseWeapon();
		}
		else
		{
			DestroyWeapon();
		}
	}
	if (NewWeapon != nullptr)
	{
		EquippedWeapon = NewWeapon;
		EquippedWeapon->AssociateToActorOwner(this, GetFPViewPointCamera());
		EquippedWeapon->AttachMesh1PToComponent(GetMesh(), TEXT("grip_r"));
		EquippedWeapon->UseMesh1P();
	}
}

/**
 * Release the equipped weapon
 */
void ACharacter_FPS::ReleaseWeapon()
{
	EquippedWeapon->DissociatesToActorOwner();
	EquippedWeapon = nullptr;
}

/**
 * Destroy the equipped weapon
 */
void ACharacter_FPS::DestroyWeapon()
{
	AWeapon_Base* Weapon = EquippedWeapon;

	ReleaseWeapon();

	Weapon->Destroy();
}
