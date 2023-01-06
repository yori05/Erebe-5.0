// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Character_Basic.h"
#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CombatComponents/CombatComponent_Basic.h"

DEFINE_LOG_CATEGORY_STATIC(LogChararacterBasic, Warning, All);

/**-----------------	Constructor Part		-----------------*/
// Sets default values
ACharacter_Basic::ACharacter_Basic(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Create First Person Camera 
	CharCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CharCamera"));
	if (CharCamera != nullptr)
	{
		CharCamera->SetupAttachment(GetCapsuleComponent());
		CharCamera->bUsePawnControlRotation = true;
	}

	// Create First Person ViewPointCamera
	FPViewPointCamera = CreateDefaultSubobject<USceneComponent>(TEXT("FPViewPointCamera"));
	if (FPViewPointCamera != nullptr)
	{
		FPViewPointCamera->SetupAttachment(GetCapsuleComponent());
		FPViewPointCamera->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	}

	// Check Default Mesh and Make It First Person Mesh
	USkeletalMeshComponent* FPSkeletalMesh = GetMesh();
	if (FPSkeletalMesh != nullptr)
	{
		//FPSkeletalMesh->Rename(TEXT("FPMesh"));
		FPSkeletalMesh->SetupAttachment(CharCamera);
		FPSkeletalMesh->SetRelativeLocation(FVector(49.547775f, -30.845685f, -149.520031f)); //(X=49.547775,Y=-30.845685,Z=-149.520031)
		FPSkeletalMesh->SetRelativeRotation(FRotator(21.831325f, -17.965423f, 5.694401f));	 //(Pitch=21.831325,Yaw=-17.965423,Roll=5.694401)
	}

	// Create a Third person Skeletal Mesh 
	TPMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TPMesh"));
	if (TPMesh != nullptr)
	{
		TPMesh->SetupAttachment(RootComponent);
		TPMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
		TPMesh->SetRelativeLocation(FVector(0.f, 0.f, -95.f));
	}

	// Create a camera boom (pulls in towards the player if there is collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	if (CameraBoom != nullptr)
	{
		CameraBoom->SetupAttachment(TPMesh);
		CameraBoom->TargetArmLength = 300.f; // The camera follow at this distance behind the player
		CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
		CameraBoom->SetRelativeLocation(FVector(0.f, 0.f, 110.f));
	}

	// Create Third Person ViewPointCamera
	TPViewPointCamera = CreateDefaultSubobject<USceneComponent>(TEXT("TPViewPointCamera"));
	if (TPViewPointCamera != nullptr)
	{
		TPViewPointCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	}

	//We have to set a default profil for the viewpoint
	UseFirstPerson();
	
	CombatComponent = CreateDefaultSubobject<UCombatComponent_Basic>(TEXT("CombatComponent"));
	CombatComponent->bEditableWhenInherited = true;
}

/**-----------------	Inherit Function Part		-----------------*/
// Called when the game starts or when spawned
void ACharacter_Basic::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ACharacter_Basic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/**-----------------	Character_Basic Part		-----------------*/
/**-----------------	Action Part		-----------------*/
// Called to make the character move in the forward direction
void ACharacter_Basic::MoveForward(float Value)
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
void ACharacter_Basic::MoveRight(float Value)
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

// Called to make the character move in the forward direction of the camera (used while swimming)
void ACharacter_Basic::MoveForwardCamera(float Value)
{
	if ((Controller != nullptr) && (Value != 0.f))
	{
		if (CharCamera != nullptr)
		{
			// Add movement in that direction
			AddMovementInput(CharCamera->GetForwardVector(), Value);

		}
		else
		{
			MoveForward(Value);
		}
	}
}

// Called to make the character move in the right direction of the camera (used while swimming)
void ACharacter_Basic::MoveRightCamera(float Value)
{
	if ((Controller != nullptr) && (Value != 0.f))
	{
		if (CharCamera != nullptr)
		{
			// Add movement in that direction
			AddMovementInput(CharCamera->GetRightVector(), Value);

		}
		else
		{
			MoveRight(Value);
		}
	}
}

// Called to make the character jump
void ACharacter_Basic::Jump()
{
	Super::Jump();
}


// Called to make the character stop jump
void ACharacter_Basic::StopJumping()
{
	Super::StopJumping();
}

void ACharacter_Basic::OrientRotationToCamera(bool _bIfInThirdPerson)
{
	if (_bIfInThirdPerson && GetIsInFirstPerson())
	{
		return;
	}

	if (IsValid(CharCamera))
	{
		FRotator CamRotator(CharCamera->GetRelativeRotation());
		SetActorRotation(FRotator(double(0), CamRotator.Yaw, CamRotator.Roll));
	}
}

void ACharacter_Basic::AllignYawWithCamera()
{
	if (IsValid(CharCamera))
	{
		FRotator CamRotator(CharCamera->GetComponentRotation());
		SetActorRotation(FRotator(double(0), CamRotator.Yaw, double(0)));
	}
}

/**-----------------	Combat Part		-----------------*/
	// Called to Attack with the Combat Component
void ACharacter_Basic::Attack()
{
	if (GetIsInFirstPerson() == false)
	{
		AllignYawWithCamera();
	}
/*
	if (CombatComponent != nullptr)
	{
		CombatComponent->Attack();
	}
*/
}

/**-----------------	ViewPoint Selection Part		-----------------*/
// Called to set the First Person Components in use for this character
void ACharacter_Basic::UseFirstPerson()
{
	if (GetFPMesh() != nullptr)
	{
		GetFPMesh()->SetOwnerNoSee(false);
	}
	if (GetTPMesh() != nullptr)
	{
		GetTPMesh()->SetOwnerNoSee(true);
	}
	if (CharCamera != nullptr && FPViewPointCamera != nullptr)
	{
		bIsInFP = true;
		bUseControllerRotationYaw = true;
		//Attach the camera to the end of the First Person ViewPoint Scene Object
		CharCamera->AttachToComponent(FPViewPointCamera, FAttachmentTransformRules::KeepRelativeTransform);

		SetActorRotation(FRotator());
		CharCamera->bUsePawnControlRotation = true;
		//Don't need to reset the camera rotation cause she will follow the controller rotation
	}
	if (GetCharacterMovement() != nullptr)
	{
		// Character does not moves in the direction of the input ...
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}

}

// Called to set the Third Person Components in use for this character
void ACharacter_Basic::UseThirdPerson()
{
	if (GetFPMesh() != nullptr)
	{
		GetFPMesh()->SetOwnerNoSee(true);
	}
	if (GetTPMesh() != nullptr)
	{
		GetTPMesh()->SetOwnerNoSee(false);
	}
	if (CharCamera != nullptr && TPViewPointCamera != nullptr)
	{
		bIsInFP = false;
		//Attach the camera to the end of the Third Person ViewPoint Scene Object
		CharCamera->AttachToComponent(TPViewPointCamera, FAttachmentTransformRules::KeepRelativeTransform);
		CharCamera->bUsePawnControlRotation = false;
		bUseControllerRotationYaw = false;

		//Orient Character
		//OrientRotationToCamera(false);

		//Reset the camera rotation
		CharCamera->SetRelativeRotation(FRotator());
	}
	if (GetCharacterMovement() != nullptr)
	{
		// Character does not moves in the direction of the input ...
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}

}
