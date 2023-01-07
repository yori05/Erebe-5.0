// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Character_Basic.h"
#include "Components/SceneComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CombatComponents/CombatComponent_Basic.h"
#include "Components/InteractiveComponent/InteractiveComponent_Basic.h"
#include "Components/InteractiveComponent/InteractiveComponent_Manager.h"
#include "Engine/CurveTable.h"

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
	if (IsValid(GetCapsuleComponent()))
	{
		GetCapsuleComponent()->InitCapsuleSize(40.f, 96.0f);
	}

	InteractionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("InteractionCapsule"));
	if (IsValid(InteractionCapsule))
	{
		InteractionCapsule->SetupAttachment(GetCapsuleComponent());
		InteractionCapsule->InitCapsuleSize(75.f, 100.f);
		InteractionCapsule->SetRelativeLocation(FVector(32.5f, 0.f, 0.f));
		InteractionCapsule->ComponentTags.Add(TEXT("Interaction"));
	}

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

	ZoomRatio = 0.f;

	// Create Third Person ViewPointCamera
	TPViewPointCamera = CreateDefaultSubobject<USceneComponent>(TEXT("TPViewPointCamera"));
	if (TPViewPointCamera != nullptr)
	{
		TPViewPointCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	}
	
	CombatComponent = CreateDefaultSubobject<UCombatComponent_Basic>(TEXT("CombatComponent"));
	CombatComponent->bEditableWhenInherited = true;
	bUseRootMotionAnimation = false;
}

/**-----------------	Actor Function Part		-----------------*/
// Called when the game starts or when spawned
void ACharacter_Basic::BeginPlay()
{
	Super::BeginPlay();

	//We have to set a default profil for the viewpoint set as true by default can be change in child
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
void ACharacter_Basic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!IsInFirstPerson())
	{
		if (IsValid(GetCameraBoom()))
		{
			//GetCameraBoom()->TargetOffset += FVector(MotionInput.X, -MotionInput.Z, MotionInput.Y) * 50.f;
		}
	}
}

/**-----------------	Character_Basic Part		-----------------*/
/**-----------------	Action Part		-----------------*/
/**-----------------	Movement Part		-----------------*/

/**
* Used to call action for MoveForward implemented in c++ and bp
*/
void ACharacter_Basic::CallMoveForward(float Value)
{
	MoveForward(Value);
	ReceiveMoveForward(Value);
}

// Called to make the character move in the forward direction
void ACharacter_Basic::MoveForward(float Value)
{
	MotionInput.Z = Value;

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

/**
* Used to call action for MoveRight implemented in c++ and bp
*/
void ACharacter_Basic::CallMoveRight(float Value)
{
	MoveRight(Value);
	ReceiveMoveRight(Value);
}

// Called to make the character move in the right direction
void ACharacter_Basic::MoveRight(float Value)
{
	MotionInput.X = Value;

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

/**
* Used to call action for MoveUp implemented in c++ and bp
*/
void ACharacter_Basic::CallMoveUp(float Value)
{
	MoveUp(Value);
	ReceiveMoveUp(Value);
}

// Called to make the character move in the up direction
void ACharacter_Basic::MoveUp(float Value)
{
	MotionInput.Y = Value;

	if ((Controller != nullptr) && (Value != 0.f))
	{
		// Use the controller rotation ...
		const FRotator Rotation = Controller->GetControlRotation();
		// ... To catch the yaw direction using the controller rotation
		const FRotator YawDirection(0, Rotation.Yaw, 0);

		// Get up direction vector
		const FVector Direction = FRotationMatrix(YawDirection).GetUnitAxis(EAxis::Y);

		// Add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

/**
* Used to call action for MoveForwardCamera implemented in c++ and bp
*/
void ACharacter_Basic::CallMoveForwardCamera(float Value)
{
	MoveForwardCamera(Value);
	ReceiveMoveForwardCamera(Value);
}

// Called to make the character move in the forward direction of the camera (used while swimming)
void ACharacter_Basic::MoveForwardCamera(float Value)
{
	if (IsValid(CharCamera))
	{
		MotionInput.Z = Value;

		if (IsValid(Controller) && (Value != 0.f))
		{
			// Add movement in that direction
			AddMovementInput(CharCamera->GetForwardVector(), Value);
		}
	}
	else
	{
		MoveForward(Value);
	}
}


/**
* Used to call action for MoveRightCamera implemented in c++ and bp
*/
void ACharacter_Basic::CallMoveRightCamera(float Value)
{
	MoveRightCamera(Value);
	ReceiveMoveRightCamera(Value);
}

// Called to make the character move in the right direction of the camera (used while swimming)
void ACharacter_Basic::MoveRightCamera(float Value)
{
	if (IsValid(CharCamera))
	{
		MotionInput.X = Value;

		if (IsValid(Controller) && (Value != 0.f))
		{
			// Add movement in that direction
			AddMovementInput(CharCamera->GetRightVector(), Value);
		}
	}
	else
	{
		MoveRight(Value);
	}
}

/**
* Used to call action for MoveUpCamera implemented in c++ and bp
*/
void ACharacter_Basic::CallMoveUpCamera(float Value)
{
	MoveUpCamera(Value);
	ReceiveMoveUpCamera(Value);
}

// Called to make the character move in the up direction of the camera (used while swimming)
void ACharacter_Basic::MoveUpCamera(float Value)
{
	if (IsValid(CharCamera))
	{
		MotionInput.Y = Value;

		if (IsValid(Controller) && (Value != 0.f))
		{
			// Add movement in that direction
			AddMovementInput(CharCamera->GetUpVector(), Value);
		}
	}
	else
	{
		MoveUp(Value);
	}
}

/**-----------------	Jump Part		-----------------*/

/**
* Used to call action for Jump implemented in c++ and bp
*/
void ACharacter_Basic::CallJump()
{
	Jump();
	ReceiveJump();
}

// Called to make the character jump
void ACharacter_Basic::Jump()
{
	Super::Jump();
}

/**
* Used to call action for StopJumping implemented in c++ and bp
*/
void ACharacter_Basic::CallStopJumping()
{
	StopJumping();
	ReceiveStopJumping();
}

// Called to make the character stop jump
void ACharacter_Basic::StopJumping()
{
	Super::StopJumping();
}


/**
* Used to call action for Orient Rotation To Camera implemented in c++ and bp
*/
void ACharacter_Basic::CallOrientRotationToCamera(bool _bIfInThirdPerson)
{
	OrientRotationToCamera(_bIfInThirdPerson);
	ReceiveOrientRotationToCamera(_bIfInThirdPerson);
}

void ACharacter_Basic::OrientRotationToCamera(bool _bIfInThirdPerson)
{
	if (_bIfInThirdPerson && IsInFirstPerson())
	{
		return;
	}

	if (IsValid(CharCamera))
	{
		FRotator CamRotator(CharCamera->GetRelativeRotation());
		SetActorRotation(FRotator(0.f, CamRotator.Yaw, CamRotator.Roll));
	}
}

/**
* Used to call action for Allign Yaw With Camera implemented in c++ and bp
*/
void ACharacter_Basic::CallAllignYawWithCamera()
{
	AllignYawWithCamera();
	ReceiveAllignYawWithCamera();
}

void ACharacter_Basic::AllignYawWithCamera()
{
	if (IsValid(CharCamera))
	{
		//Align actor's yaw with the camera
		FRotator CamRotator(CharCamera->GetComponentRotation());
		SetActorRotation(FRotator(0.f, CamRotator.Yaw, 0.f));
	}
}

/**-----------------	Interact Part		-----------------*/

void ACharacter_Basic::CallInteract()
{
	Interact();
	ReceiveInteract();
}

	// Called to Interact with a InteractiveComponent
void ACharacter_Basic::Interact()
{
	if (IsValid(InteractionCapsule))
	{
		//Check actor in colision the interaction capsule
		TArray<AActor*> OverlappingActor;
		InteractionCapsule->GetOverlappingActors(OverlappingActor);
		UInteractiveComponent_Basic* IC_Basic = nullptr;
		UInteractiveComponent_Manager* IC_Mana = nullptr;

		for (auto& i : OverlappingActor)
		{
			//In case the actor does use a interactive component manager use it to have the main interaction
			IC_Mana = Cast<UInteractiveComponent_Manager>(i->GetComponentByClass(UInteractiveComponent_Manager::StaticClass()));

			if (IsValid(IC_Mana))
			{
				IC_Basic = IC_Mana->GetMainInteraction();
			}
			else
			{
				//In case the actor does not use a interactive component manager, we cath the first interactive component
				IC_Basic = Cast<UInteractiveComponent_Basic>(i->GetComponentByClass(UInteractiveComponent_Basic::StaticClass()));
			}

			//Launch interaction with interactive component saved
			if (IsValid(IC_Basic))
			{
				IC_Basic->CallInteraction(this);
				break;
			}
		}
	}
}

/**-----------------	Combat Part		-----------------*/

void ACharacter_Basic::CallAttack()
{
	Attack();
	ReceiveAttack();
}

	// Called to Attack with the Combat Component
void ACharacter_Basic::Attack()
{
	if (IsInFirstPerson() == false)
	{
		AllignYawWithCamera();
	}
	if (CombatComponent != nullptr)
	{
		CombatComponent->Attack();
	}
}

/**-----------------	ViewPoint Selection Part		-----------------*/

void ACharacter_Basic::CallSwapViewMode()
{
	if (IsInFirstPerson())
	{
		CallUseThirdPerson();
	}
	else
	{
		CallUseFirstPerson();
	}
}

void ACharacter_Basic::CallUseFirstPerson()
{
	UseFirstPerson();
	ReceiveUseFirstPerson();
	OnUseFirstPersonView.Broadcast();
}

// Called to set the First Person Components in use for this character
void ACharacter_Basic::UseFirstPerson()
{
	if (GetFPMesh() != nullptr)
	{
		//Show the first person mesh
		GetFPMesh()->SetOwnerNoSee(false);
		GetFPMesh()->SetCastShadow(false);
	}
	if (GetTPMesh() != nullptr)
	{
		//Hide the third person mesh but use his shadow
		GetTPMesh()->SetHiddenInGame(true);
		GetTPMesh()->SetCastHiddenShadow(true);
	//	GetTPMesh()->SetOwnerNoSee(true);
	//	GetTPMesh()->SetCastShadow(true);
	}
	if (CharCamera != nullptr && FPViewPointCamera != nullptr)
	{
		bIsInFP = true;
		bUseControllerRotationYaw = true;

		//Attach the camera to the end of the First Person ViewPoint Scene Object
		CharCamera->AttachToComponent(FPViewPointCamera, FAttachmentTransformRules::KeepRelativeTransform);

		//Reset actor rotation and use the pawn rotation for the camera
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


void ACharacter_Basic::CallUseThirdPerson()
{
	UseThirdPerson();
	ReceiveUseThirdPerson();
	OnUseThirdPersonView.Broadcast();
}

// Called to set the Third Person Components in use for this character
void ACharacter_Basic::UseThirdPerson()
{
	if (GetFPMesh() != nullptr)
	{
		//Show the first person mesh
		GetFPMesh()->SetOwnerNoSee(true);
		GetFPMesh()->SetCastShadow(false);
	}
	if (GetTPMesh() != nullptr)
	{
		GetTPMesh()->SetHiddenInGame(false);
//		GetTPMesh()->SetCastHiddenShadow(true);
		//Hide the third person mesh
//		GetTPMesh()->SetOwnerNoSee(false);
//		GetTPMesh()->SetCastShadow(true);
	}
	if (CharCamera != nullptr && TPViewPointCamera != nullptr)
	{
		bIsInFP = false;

		//Attach the camera to the end of the Third Person ViewPoint Scene Object
		CharCamera->AttachToComponent(TPViewPointCamera, FAttachmentTransformRules::KeepRelativeTransform);
		CharCamera->bUsePawnControlRotation = false;
		bUseControllerRotationYaw = false;

		//Reset the camera rotation
		CharCamera->SetRelativeRotation(FRotator());
	}
	if (GetCharacterMovement() != nullptr)
	{
		// Character doe moves in the direction of the input in case we don't use the root motion animation
		GetCharacterMovement()->bOrientRotationToMovement = !bUseRootMotionAnimation;
	}

	ZoomRatio = 0.75f;
}

/**-----------------	Zoom Part		-----------------*/
void ACharacter_Basic::CallZoomAction(float Value)
{
	ZoomAction(Value);
	ReceiveZoomAction(Value);
}

void ACharacter_Basic::ZoomAction(float Value)
{
	if (IsValid(GetCameraBoom()))
	{
		if (IsInFirstPerson())
		{
			if (Value > 0.f)
			{
				CallUseThirdPerson();
			}
			else
			{
				return;
			}

			ZoomRatio = 0.f;
		}
		else
		{
			ZoomRatio += Value;

			if (ZoomRatio < 0.f)
			{
				ZoomRatio = 0.f;
				CallUseFirstPerson();
			}
			else if (ZoomRatio > 1.f)
			{
				ZoomRatio = 1.f;
			}

			if (IsValid(CameraBoomCurve))
			{
				FRichCurve* CurveUsed = nullptr;

				CurveUsed = CameraBoomCurve->FindRichCurve(TEXT("ArmLength"),"");

				if (CurveUsed != nullptr)
				{
					GetCameraBoom()->TargetArmLength = CurveUsed->Eval(ZoomRatio);
				}

				CurveUsed = CameraBoomCurve->FindRichCurve(TEXT("SocketOffset"), "");

				if (CurveUsed != nullptr)
				{
					GetCameraBoom()->SocketOffset = FVector(0.f, 0.f, CurveUsed->Eval(ZoomRatio));
				}

				CurveUsed = CameraBoomCurve->FindRichCurve(TEXT("TargetOffset"), "");

				if (CurveUsed != nullptr)
				{
					GetCameraBoom()->TargetOffset = FVector(0.f, 0.f, CurveUsed->Eval(ZoomRatio));
				}
			}
		}
	}
}
