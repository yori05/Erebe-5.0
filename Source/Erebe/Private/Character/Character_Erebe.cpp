// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Character_Erebe.h"
#include "Components/BoxComponent.h"
#include "Components/CharacterMovementComponent/ErebeCharacterMovementComponent.h"
#include "Components/HealthComponent/BasicHealthComponent.h"
#include "Weapon/Weapon_Base.h"
#include "Weapon/Weapon_Shooting_Base.h"

DEFINE_LOG_CATEGORY_STATIC(LogCharacterState, Log, All);

FName ACharacter_Erebe::HealthComponentName(TEXT("HealthComponent"));

// Sets default values
ACharacter_Erebe::ACharacter_Erebe(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UErebeCharacterMovementComponent>(ACharacter_Erebe::CharacterMovementComponentName)) // Used to edit the CharactermovementComponent
{
	bCanFly = false;

	// Configure Character Movement Component
	ErebeCharacterMovement = Cast<UErebeCharacterMovementComponent>(GetCharacterMovement());
	if (ErebeCharacterMovement)
	{
		GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of the input ...
		GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f); // ... at this rotation rate
		GetCharacterMovement()->JumpZVelocity = 600.f;
		GetCharacterMovement()->AirControl = 0.2f;
	}

	// Create a Health component
	HealthComponent = CreateDefaultSubobject<UBasicHealthComponent>(HealthComponentName);

	//Create a Damage Hitbox component
	DamageHitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("DamageHitbox"));
	if (DamageHitbox != nullptr)
	{
		DamageHitbox->SetupAttachment(RootComponent);					// We attach it to the root component so he follow the character
		DamageHitbox->SetCollisionProfileName(TEXT("OverlapAllDynamic")); //We make it available to target all dynamic
	}
}

// Called when the game starts or when spawned
void ACharacter_Erebe::BeginPlay()
{
	Super::BeginPlay();

	auto World = GetWorld();

	if (World && WeaponClass != nullptr)
	{
		EquipedWeapon = World->SpawnActor<AWeapon_Base>(WeaponClass, GetActorTransform());

		if (EquipedWeapon != nullptr)
		{
			EquipedWeapon->AssociateToActorOwner(this, GetFPViewPointCamera());
			EquipedWeapon->AttachMeshToComponent(GetMesh(), TEXT("grip_r"));
			EquipedWeapon->UseMesh3P();
		}
	}

	if (bIsInFP)
	{
		CallUseFirstPerson();
	}
	else
	{
		CallUseThirdPerson();
	}
}

// Called every frame
void ACharacter_Erebe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PerformState(DeltaTime);
}

void ACharacter_Erebe::Jump()
{
	if (ErebeCharacterMovement != nullptr)
	{
		if (ErebeCharacterMovement->IsRushing())
		{
			ErebeCharacterMovement->StopRush(true);
		}
	}

	Super::Jump();
}

void ACharacter_Erebe::DebugFly()
{
	if (GetCharacterState() != ECharacterState::CHARSTATE_Flying)
	{
		SetCharacterState(ECharacterState::CHARSTATE_Flying);
	}
	else
	{
		SetCharacterState(ECharacterState::CHARSTATE_Free);
	}
}

void ACharacter_Erebe::Rush()
{
	if (ErebeCharacterMovement != nullptr)
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

void ACharacter_Erebe::Dodge()
{
	if (ErebeCharacterMovement != nullptr)
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

void ACharacter_Erebe::Attack()
{
	if (EquipedWeapon != nullptr)
	{
		AWeapon_Shooting_Base* Weapon = Cast<AWeapon_Shooting_Base>(EquipedWeapon);
	
		if (EquipedWeapon != nullptr && Weapon != nullptr)
		{
			if (Weapon->GetFire())
			{
				Weapon->FirePress();
			}
			else
			{
				Weapon->FireRelease();
			}
		}
	}
	if (DamageHitbox != nullptr)
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
	}
}


void ACharacter_Erebe::Run()
{

}


void ACharacter_Erebe::StopRunning()
{

}

void ACharacter_Erebe::PerformState(float DeltaTime)
{
	switch (CharacterState)
	{
	case ECharacterState::CHARSTATE_Free:

		break;
	case ECharacterState::CHARSTATE_Flying:

		break;
	case ECharacterState::CHARSTATE_Focus:

		break;
	case ECharacterState::CHARSTATE_Dead:

		break;
	default:
		UE_LOG(LogCharacterState, Warning, TEXT("%s has unsupported state mode %d"), *GetName(), int32(CharacterState));
		SetCharacterState(ECharacterState::CHARSTATE_Free);
		break;
	}
}

void ACharacter_Erebe::StartFreeState(ECharacterState PreviousCharacterState)
{
	if (PreviousCharacterState >= ECharacterState::CHARSTATE_Max && IsValid(ErebeCharacterMovement))
	{
		ErebeCharacterMovement->bOrientRotationToMovement = true;	// Character moves in the direction of the input ...
		bUseControllerRotationYaw = false;							// The controller don't change the character orientation
	}
	if ((PreviousCharacterState == ECharacterState::CHARSTATE_Focus || PreviousCharacterState == ECharacterState::CHARSTATE_Dead) && IsValid(ErebeCharacterMovement))
	{
		ErebeCharacterMovement->bOrientRotationToMovement = true;	// Character moves in the direction of the input ...
		bUseControllerRotationYaw = false;							// The controller don't change the character orientation
	}

	ErebeCharacterMovement->SetMovementMode(MOVE_Walking);

}

void ACharacter_Erebe::StartFlyingState(ECharacterState PreviousCharacterState)
{
	if ((PreviousCharacterState == ECharacterState::CHARSTATE_Focus || PreviousCharacterState == ECharacterState::CHARSTATE_Dead) && IsValid(ErebeCharacterMovement))
	{
		ErebeCharacterMovement->bOrientRotationToMovement = true;	// Character moves in the direction of the input ...
		bUseControllerRotationYaw = false;							// The controller don't change the character orientation
	}

	ErebeCharacterMovement->SetMovementMode(MOVE_Flying);
}

void ACharacter_Erebe::StartFocusState(ECharacterState PreviousCharacterState)
{
	if ((PreviousCharacterState == ECharacterState::CHARSTATE_Free || PreviousCharacterState == ECharacterState::CHARSTATE_Flying) && IsValid(ErebeCharacterMovement))
	{
		ErebeCharacterMovement->bOrientRotationToMovement = false;	// Character don't moves in the direction of the input ...
		bUseControllerRotationYaw = true;							// The controller change the character orientation

	}
}

void ACharacter_Erebe::StartDeadState(ECharacterState PreviousCharacterState)
{
	if ((PreviousCharacterState == ECharacterState::CHARSTATE_Free || PreviousCharacterState == ECharacterState::CHARSTATE_Flying) && IsValid(ErebeCharacterMovement))
	{
		ErebeCharacterMovement->bOrientRotationToMovement = false; // Character moves in the direction of the input ...
	}
	else if (PreviousCharacterState == ECharacterState::CHARSTATE_Focus && IsValid(ErebeCharacterMovement))
	{
		ErebeCharacterMovement->bOrientRotationToMovement = true; // Character moves in the direction of the input ...
		bUseControllerRotationYaw = false;
	}

}

void ACharacter_Erebe::SetCharacterState(ECharacterState NewCharacterState)
{
	if (NewCharacterState == CharacterState)
		return;

	ECharacterState PreviousState = CharacterState;
	CharacterState = NewCharacterState;

	switch (CharacterState)
	{
	case ECharacterState::CHARSTATE_Free:
		StartFreeState(PreviousState);
		break;
	case ECharacterState::CHARSTATE_Flying:
		StartFlyingState(PreviousState);
		break;
	case ECharacterState::CHARSTATE_Focus:
		StartFocusState(PreviousState);
		break;
	case ECharacterState::CHARSTATE_Dead:
		StartDeadState(PreviousState);
	default:
		//In case we are not in logic state, Call again this function to set it to free
		SetCharacterState(ECharacterState::CHARSTATE_Free);
		break;
	}

}

ECharacterState ACharacter_Erebe::GetCharacterState()
{
	return CharacterState;
}


void ACharacter_Erebe::SetFreeState()
{
	if (CharacterState != ECharacterState::CHARSTATE_Free)
		SetCharacterState(ECharacterState::CHARSTATE_Free);
}

void ACharacter_Erebe::SetFlyingState()
{
	if (CharacterState != ECharacterState::CHARSTATE_Flying)
		SetCharacterState(ECharacterState::CHARSTATE_Flying);
}

void ACharacter_Erebe::SetFocusState()
{
	if (CharacterState != ECharacterState::CHARSTATE_Focus)
		SetCharacterState(ECharacterState::CHARSTATE_Focus);
}

void ACharacter_Erebe::SetDeadState()
{
	if (CharacterState != ECharacterState::CHARSTATE_Dead)
		SetCharacterState(ECharacterState::CHARSTATE_Dead);
}