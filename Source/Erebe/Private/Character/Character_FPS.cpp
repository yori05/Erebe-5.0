// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Character_FPS.h"
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
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Create First person Camera 
	Camera1P = CreateDefaultSubobject<UCameraComponent>(TEXT("FPCamera"));
	if (Camera1P != nullptr)
	{
		Camera1P->SetupAttachment(GetCapsuleComponent());
		Camera1P->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
		Camera1P->bUsePawnControlRotation = true;
	}

	// Create First person Skeletal Mesh 
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FPMesh"));
	if (Mesh1P != nullptr)
	{
		Mesh1P->SetupAttachment(Camera1P);
		Mesh1P->SetOnlyOwnerSee(true);
		Mesh1P->bCastDynamicShadow = false;
		Mesh1P->CastShadow = false;
		Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
		Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));
	}

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
// Called to make the character move in the forward direction
void ACharacter_FPS::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.f))
	{
		// Use the controller rotation ...
		const FRotator Rotation = Controller->GetControlRotation();
		// ... To catch the yaw direction using the controller rotation
		const FRotator YawDirection(0, Rotation.Yaw, 0);

		// Get forward direction vector
		const FVector Direction = FRotationMatrix(YawDirection).GetUnitAxis(EAxis::X);
		// Add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

// Called to make the character move in the right direction
void ACharacter_FPS::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.f))
	{
		// Use the controller rotation ...
		const FRotator Rotation = Controller->GetControlRotation();
		// ... To catch the yaw direction using the controller rotation
		const FRotator YawDirection(0, Rotation.Yaw, 0);

		// Get right direction vector
		const FVector Direction = FRotationMatrix(YawDirection).GetUnitAxis(EAxis::Y);
		// Add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

// Called to make the character jump
void ACharacter_FPS::Jump()
{
	Super::Jump();
}


// Called to make the character stop jump
void ACharacter_FPS::StopJumping()
{
	Super::StopJumping();
}

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
		EquippedWeapon->SetActorOwner(this);
		EquippedWeapon->AttachToComponent(GetMesh1P(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("grip_r"));
		EquippedWeapon->SetOwnerViewpoint(GetCamera1P());
	}
}

/**
 * Release the equipped weapon
 */
void ACharacter_FPS::ReleaseWeapon()
{
	EquippedWeapon->SetActorOwner(nullptr);
	EquippedWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	EquippedWeapon->SetOwnerViewpoint(nullptr);
}

/**
 * Destroy the equipped weapon
 */
void ACharacter_FPS::DestroyWeapon()
{
	ReleaseWeapon();

	EquippedWeapon->Destroy();
}
