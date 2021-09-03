// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ErebeCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CharacterMovementComponent/ErebeCharacterMovementComponent.h"
#include "Components/HealthComponent/BasicHealthComponent.h"
#include "Components/DialogueComponent/DialogueComponent.h"
#include "Interfaces/InteractiveInterface.h"
#include "Math/Vector.h"
#include "Logging/LogMacros.h"
#include "Weapon/Weapon_Base.h"
#include "Weapon/Weapon_Shooting_Base.h"
#include "Engine/World.h"

DEFINE_LOG_CATEGORY_STATIC(LogCharacterState, Log, All);

FName AErebeCharacter::HealthComponentName(TEXT("HealthComponent"));

// Sets default values
AErebeCharacter::AErebeCharacter(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UErebeCharacterMovementComponent>(AErebeCharacter::CharacterMovementComponentName)) // Used to edit the CharactermovementComponent
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	bCanFly = false;

	// Set the size for the collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	// Create First person Skeletal Mesh 
	FPMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FPMesh"));
	if (FPMesh != nullptr)
	{
		FPMesh->SetupAttachment(RootComponent);
		FPMesh->SetOnlyOwnerSee(true);
	}

	// Create First person Camera 
	FPCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPCamera"));
	if (FPCamera != nullptr)
	{
		FPCamera->SetupAttachment(FPMesh);
		FPCamera->bUsePawnControlRotation = true;
	}


	// Configure Character Movement Component
	ErebeCharacterMovement = Cast<UErebeCharacterMovementComponent>(GetCharacterMovement());
	if (ErebeCharacterMovement)
	{
		GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of the input ...
		GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f); // ... at this rotation rate
		GetCharacterMovement()->JumpZVelocity = 600.f;
		GetCharacterMovement()->AirControl = 0.2f;
	}

	// Create a camera boom (pulls in towards the player if there is collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.f; // The camera follow at this distance behind the player
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); //Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create a Health component
	HealthComponent = CreateDefaultSubobject<UBasicHealthComponent>(HealthComponentName);
	
	//Create a Damage Hitbox component
	DamageHitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("DamageHitbox"));
	if (DamageHitbox != nullptr)
	{
		DamageHitbox->SetupAttachment(RootComponent);					// We attach it to the root component so he follow the character
		DamageHitbox->SetCollisionProfileName(TEXT("OverlapAllDynamic")); //We make it available to target all dynamic
	}

	//Create a Dialogue component
	DialogueComponent = CreateDefaultSubobject<UDialogueComponent>(TEXT("DialogueComp"));

	if (InteractiveComponent != nullptr)
	{
		InteractiveComponent->TryToAddAInteraction(DialogueComponent, INTERACTIVE_Dialogue, true);
	}

	
	// Note : The Skeletal Mesh and the AnimBblueprint references on the Mesh Component (inherited from Character)
	// will be set on the blueprint asset (to avoid direct content reference in c++)
}

// Called when the game starts or when spawned
void AErebeCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	auto World = GetWorld();

	if (World && WeaponClass != nullptr)
	{
		EquipedWeapon = World->SpawnActor<AWeapon_Base>(WeaponClass, GetActorTransform());

		if (EquipedWeapon != nullptr)
		{
			EquipedWeapon->SetActorOwner(this);
			EquipedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("grip_r"));
			EquipedWeapon->SetOwnerViewpoint(GetFollowCamera());
		}
	}

	if (bIsInFP)
	{
		PassInFirstPerson();
	}
	else
	{
		PassInThirdPerson();
	}
}

// Called every frame
void AErebeCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PerformState(DeltaTime);
}

void AErebeCharacter::MoveForward(float Value)
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
		AddMovementInput(Direction,Value);
	}
}

void AErebeCharacter::MoveRight(float Value)
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

void AErebeCharacter::MoveUp(float Value)
{
	// Need to check this function (maybe she isn't functional with the yaw direction and need to use a other one like the pitch)
	if ((Controller != nullptr) && (Value != 0.f))
	{
		// Use the controller rotation ...
		const FRotator Rotation = Controller->GetControlRotation();
		// ... To catch the yaw direction using the controller rotation
		const FRotator YawDirection(0, Rotation.Yaw, 0);

		// Get up direction vector
		const FVector Direction = FRotationMatrix(YawDirection).GetUnitAxis(EAxis::Z);
		// Add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AErebeCharacter::Jump()
{
	if (Controller != nullptr && ErebeCharacterMovement != nullptr)
	{
		if (ErebeCharacterMovement->IsRushing())
		{
			ErebeCharacterMovement->StopRush(true);
		}
	}

	Super::Jump();

}

void AErebeCharacter::Rush()
{
	if (Controller != nullptr && ErebeCharacterMovement != nullptr)
	{
		if (ErebeCharacterMovement->CanRush())
		{
			//Catch the last movement and normalize it to keep it as a direction. In this state it don't matter if the vector is null that's why we don't watch if it's a success of no
			auto LastMovementInputNormalized = GetLastMovementInputVector();
			LastMovementInputNormalized.Normalize();
			ErebeCharacterMovement->Rush(LastMovementInputNormalized);
		}
	}
}

void AErebeCharacter::Dodge()
{
	if (Controller != nullptr && ErebeCharacterMovement != nullptr)
	{
		if (ErebeCharacterMovement->CanDodge())
		{
			if (ErebeCharacterMovement->IsRushing())
			{
				ErebeCharacterMovement->StopRush(true);
			}

			//Catch the last movement and normalize it to keep it as a direction. In this state it don't matter if the vector is null that's why we don't watch if it's a success of no
			auto LastMovementInputNormalized = GetLastMovementInputVector();
			LastMovementInputNormalized.Normalize();
			ErebeCharacterMovement->Dodge(LastMovementInputNormalized);
		}
	}
}

void AErebeCharacter::Punch()
{
	if (Controller != nullptr && DamageHitbox != nullptr)
	{
		TSet<UPrimitiveComponent*> OverlappingComponents;

		DamageHitbox->GetOverlappingComponents(OverlappingComponents);

		if (OverlappingComponents.Num() > 0)
		{
			UActorComponent* TargetComponent = nullptr;
			UBasicHealthComponent* TargetHealthComponent = nullptr;
			AActor* TargetActor = nullptr;

			for (auto& Element : OverlappingComponents)
			{
				TargetActor = Element->GetOwner();

				if (Element->ComponentHasTag(TEXT("Damageable")) && TargetActor != this && TargetActor != nullptr)
				{
					TargetComponent = TargetActor->GetComponentByClass(UBasicHealthComponent::StaticClass());
					TargetHealthComponent = Cast<UBasicHealthComponent>(TargetComponent);

					if (IsValid(TargetHealthComponent))
					{
						TargetHealthComponent->ReceiveDamage(1);
					}
				}
			}
		}

		/*
		* Actor method depreciate to use the component with tag method
		* 
		TSet<AActor*> OverlappingActors;
		DamageHitbox->GetOverlappingActors(OverlappingActors);
		
		if (OverlappingActors.Num() > 0)
		{
			UActorComponent* TargetComponent = nullptr;
			UBasicHealthComponent* TargetHealthComponent = nullptr;

			for (auto& Element : OverlappingActors)
			{
				if (Element != this)
				{
					TargetComponent = Element->GetComponentByClass(UBasicHealthComponent::StaticClass());
					TargetHealthComponent = Cast<UBasicHealthComponent>(TargetComponent);

					if (IsValid(TargetHealthComponent))
					{
						TargetHealthComponent->ReceiveDamage(1);
					}
				}
			}
		}
		*/
	}
}

void AErebeCharacter::Interact()
{
	if (Controller != nullptr && DamageHitbox != nullptr)
	{
		TSet<UPrimitiveComponent*> OverlappingComponents;

		DamageHitbox->GetOverlappingComponents(OverlappingComponents);

		if (OverlappingComponents.Num() > 0)
		{
			IInteractiveInterface* InteractiveTarget = nullptr;
			AActor* ActorTarget = nullptr;
			UInteractiveComponent* InteractiveComp = nullptr;
			UActorComponent* ActorComp = nullptr;

			for (auto& Element : OverlappingComponents)
			{
				ActorTarget = Element->GetOwner();

				if (Element->ComponentHasTag(TEXT("Interactive")) && ActorTarget != this && ActorTarget != nullptr)
				{
					
					ActorComp = ActorTarget->GetComponentByClass(UInteractiveComponent::StaticClass());
					InteractiveComp = Cast<UInteractiveComponent>(ActorComp);

					if (InteractiveComp != nullptr)
					{
						InteractiveComp->MainInteract(InteractiveComponent);
						return;
					}

					/*
					InteractiveTarget = Cast<IInteractiveInterface>(ActorTarget);

					if (ActorTarget->GetClass()->ImplementsInterface(UInteractiveInterface::StaticClass()) && IInteractiveInterface::Execute_CanMainInteract(ActorTarget, this))
					{
						IInteractiveInterface::Execute_MainInteract(ActorTarget, this);
						return;
					}
					*/
				}
			}
		}
		/*
		* 		Actor method for now we use the component method to filter component without the 'Interactive' Tag
		TSet<AActor*> OverlappingActors;
		DamageHitbox->GetOverlappingActors(OverlappingActors);

		if (OverlappingActors.Num() > 0)
		{
			IInteractiveInterface* TargetInterface = nullptr;

			for (auto& Element : OverlappingActors)
			{
				if (Element != this)
				{
					TargetInterface = Cast<IInteractiveInterface>(Element);

					if (Element->GetClass()->ImplementsInterface(UInteractiveInterface::StaticClass()) && IInteractiveInterface::Execute_CanMainInteract(Element, this))
					{
						IInteractiveInterface::Execute_MainInteract(Element, this);
						return;
					}
				}
			}
		}
		*/
	}
}

void AErebeCharacter::Run()
{
	
}


void AErebeCharacter::StopRunning()
{

}

void AErebeCharacter::FirePress()
{
	AWeapon_Shooting_Base* Weapon = Cast<AWeapon_Shooting_Base>(EquipedWeapon);

	if (EquipedWeapon != nullptr && Weapon != nullptr)
	{
		Weapon->FirePress();
	}
}


void AErebeCharacter::FireRelease()
{
	AWeapon_Shooting_Base* Weapon = Cast<AWeapon_Shooting_Base>(EquipedWeapon);

	if (EquipedWeapon != nullptr && Weapon != nullptr)
	{
		Weapon->FireRelease();
	}
}

void AErebeCharacter::PassInThirdPerson()
{
	if (FPMesh != nullptr)
	{
		FPMesh->SetOwnerNoSee(true);
	}
	if (GetMesh() != nullptr)
	{
		GetMesh()->SetOwnerNoSee(false);
	}
	if (FPCamera != nullptr)
	{
		FPCamera->Deactivate();
	}
	if (FollowCamera != nullptr)
	{
		FollowCamera->Activate();
	}

	bIsInFP = false;
}

void AErebeCharacter::PassInFirstPerson()
{
	if (FPMesh != nullptr)
	{
		FPMesh->SetOwnerNoSee(false);
	}
	if (GetMesh() != nullptr)
	{
		GetMesh()->SetOwnerNoSee(true);
	}
	if (FPCamera != nullptr)
	{
		FPCamera->Activate();
	}
	if (FollowCamera != nullptr)
	{
		FollowCamera->Deactivate();
	}

	bIsInFP = true;
}


void AErebeCharacter::PerformState(float DeltaTime)
{
	switch (CharacterState)
	{
	case EErebeCharacterState::CHARSTATE_Free :

		break;
	case EErebeCharacterState::CHARSTATE_Flying :
		
		break;
	case EErebeCharacterState::CHARSTATE_Focus :
		
		break;
	case EErebeCharacterState::CHARSTATE_Dead :

		break;
	default:
		UE_LOG(LogCharacterState, Warning, TEXT("%s has unsupported state mode %d"), *GetName(), int32(CharacterState));
		SetCharacterState(EErebeCharacterState::CHARSTATE_Free);
		break;
	}
}

void AErebeCharacter::StartFreeState(EErebeCharacterState PreviousCharacterState)
{
	if (PreviousCharacterState >= CHARSTATE_Max && IsValid(ErebeCharacterMovement))
	{
		ErebeCharacterMovement->bOrientRotationToMovement = true;	// Character moves in the direction of the input ...
		bUseControllerRotationYaw = false;							// The controller don't change the character orientation
	}
	if ((PreviousCharacterState == CHARSTATE_Focus || PreviousCharacterState == CHARSTATE_Dead) && IsValid(ErebeCharacterMovement))
	{
		ErebeCharacterMovement->bOrientRotationToMovement = true;	// Character moves in the direction of the input ...
		bUseControllerRotationYaw = false;							// The controller don't change the character orientation
	}

	ErebeCharacterMovement->SetMovementMode(MOVE_Walking);

}

void AErebeCharacter::StartFlyingState(EErebeCharacterState PreviousCharacterState)
{
	if ((PreviousCharacterState == CHARSTATE_Focus || PreviousCharacterState == CHARSTATE_Dead) && IsValid(ErebeCharacterMovement))
	{
		ErebeCharacterMovement->bOrientRotationToMovement = true;	// Character moves in the direction of the input ...
		bUseControllerRotationYaw = false;							// The controller don't change the character orientation
	}

		ErebeCharacterMovement->SetMovementMode(MOVE_Flying);
}

void AErebeCharacter::StartFocusState(EErebeCharacterState PreviousCharacterState)
{
	if ((PreviousCharacterState == CHARSTATE_Free || PreviousCharacterState == CHARSTATE_Flying) && IsValid(ErebeCharacterMovement))
	{
		ErebeCharacterMovement->bOrientRotationToMovement = false;	// Character don't moves in the direction of the input ...
		bUseControllerRotationYaw = true;							// The controller change the character orientation
		
	}
}

void AErebeCharacter::StartDeadState(EErebeCharacterState PreviousCharacterState)
{
	if ((PreviousCharacterState == CHARSTATE_Free || PreviousCharacterState == CHARSTATE_Flying) && IsValid(ErebeCharacterMovement))
	{
		ErebeCharacterMovement->bOrientRotationToMovement = false; // Character moves in the direction of the input ...
	}
	else if (PreviousCharacterState == CHARSTATE_Focus && IsValid(ErebeCharacterMovement))
	{
		ErebeCharacterMovement->bOrientRotationToMovement = true; // Character moves in the direction of the input ...
		bUseControllerRotationYaw = false;
	}

}

void AErebeCharacter::SetCharacterState(EErebeCharacterState NewCharacterState)
{
	if (NewCharacterState == CharacterState)
		return;

	EErebeCharacterState PreviousState = CharacterState;
	CharacterState = NewCharacterState;

	switch (CharacterState)
	{
	case EErebeCharacterState::CHARSTATE_Free :
		StartFreeState(PreviousState);
		break;
	case EErebeCharacterState::CHARSTATE_Flying :
		StartFlyingState(PreviousState);
		break;
	case EErebeCharacterState::CHARSTATE_Focus :
		StartFocusState(PreviousState);
		break;
	case EErebeCharacterState ::CHARSTATE_Dead :
		StartDeadState(PreviousState);
	default:
		//In case we are not in logic state, Call again this function to set it to free
		SetCharacterState(EErebeCharacterState::CHARSTATE_Free);
		break;
	}

}

EErebeCharacterState AErebeCharacter::GetCharacterState()
{
	return CharacterState;
}


void AErebeCharacter::SetFreeState()
{
	if (CharacterState != EErebeCharacterState::CHARSTATE_Free)
		SetCharacterState(EErebeCharacterState::CHARSTATE_Free);
}

void AErebeCharacter::SetFlyingState()
{
	if (CharacterState != EErebeCharacterState::CHARSTATE_Flying)
		SetCharacterState(EErebeCharacterState::CHARSTATE_Flying);
}

void AErebeCharacter::SetFocusState()
{
	if (CharacterState != EErebeCharacterState::CHARSTATE_Focus)
		SetCharacterState(EErebeCharacterState::CHARSTATE_Focus);
}

void AErebeCharacter::SetDeadState()
{
	if (CharacterState != EErebeCharacterState::CHARSTATE_Dead)
		SetCharacterState(EErebeCharacterState::CHARSTATE_Dead);
}