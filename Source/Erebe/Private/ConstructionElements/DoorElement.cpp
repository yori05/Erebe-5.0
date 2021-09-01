// Fill out your copyright notice in the Description page of Project Settings.


#include "ConstructionElements/DoorElement.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TimelineComponent.h"
#include "Components/MovementComponent/RotatingMeshComponent.h"
#include "Curves/CurveFloat.h"
#include "Engine/World.h"
#include "Engine/Engine.h"

ADoorElement::ADoorElement()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create a Door frame mesh and attach it to the root component
	DoorFrameMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrameMesh"));
	DoorFrameMesh->SetupAttachment(RootComponent);

	// Create a Door mesh and attach it to the Door Frame Mesh to move it from this component point of view
	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->AttachToComponent(DoorFrameMesh, FAttachmentTransformRules::KeepRelativeTransform);

	// Create a RotatingMesh component to move the door mesh using a timeling
	RotatingMeshComponent = CreateDefaultSubobject<URotatingMeshComponent>(TEXT("RotatingMeshComp"));
	if (RotatingMeshComponent != nullptr)
	{
		RotatingMeshComponent->SeRotatingMesh(DoorMesh);
		if (InteractiveComponent != nullptr)
		{
			InteractiveComponent->TryToAddAInteraction(RotatingMeshComponent, INTERACTIVE_OpenDoor, true);
		}

	}
	// The mesh will be assign in the editor or in the blueprint class of this class. 
}

void ADoorElement::BeginPlay()
{
	Super::BeginPlay();
}

void ADoorElement::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADoorElement::OpenDoor_Interaction(APawn* Interactor)
{
	if (RotatingMeshComponent != nullptr)
	{
		RotatingMeshComponent->RotateMesh_Interaction(Interactor);
	}

	return;
}

bool ADoorElement::CanOpenDoor_Interaction(APawn* Interactor) const
{
	if (RotatingMeshComponent != nullptr)
	{
		return RotatingMeshComponent->CanRotateMesh_Interaction(Interactor);
	}

	return false;
}

FName ADoorElement::GetOpenDoorActionName_Interaction()const
{
	if (RotatingMeshComponent != nullptr)
	{
		return RotatingMeshComponent->GetRotateMeshActionName_Interaction();
	}

	// For now return a brute value, maybe latter will return one depend of the localization
	return FName(TEXT("Error RotatingMeshComp"));
}

void ADoorElement::MainInteract_Implementation(APawn* Interactor)
{
	if (!CanMainInteract_Implementation(Interactor))
	{
		return;
	}
	// Call the OpenDoor as the main action
	OpenDoor_Interaction(Interactor);

	// Call the call back for the interaction
	Super::MainInteract_Implementation(Interactor);

}

bool ADoorElement::CanMainInteract_Implementation(APawn* Interactor)
{
	//Call the check link to action to open the door
	return CanOpenDoor_Interaction(Interactor);
}

FName ADoorElement::MainInteractionName_Implementation()
{
	// Call the name link to the open door action
	return GetOpenDoorActionName_Interaction();
}

bool ADoorElement::HasAInteraction_Implementation()
{
	return true;
}

uint8 ADoorElement::GetMainInteractionID_Implementation()
{
	return uint8(0);
}

void ADoorElement::InteractByID_Implementation(uint8 ID, APawn* Interactor)
{
	if (!CanInteractByID_Implementation(ID, Interactor))
	{
		return;
	}


	// Recommend to override this function the same way in child class to not lose parent interaction
	// Will class the action link to our interaction
	switch (ID)
	{
	case 0:
		// Our current main interaction
		OpenDoor_Interaction(Interactor);
		break;
	default:
		break;
	}

	// Call the interaction callback 
	// Watch out need to be sure that you don't use a parent id in this switch or will call two interaction
	Super::InteractByID_Implementation(ID, Interactor);
}

bool ADoorElement::CanInteractByID_Implementation(uint8 ID, APawn* Interactor)
{
	// Recommend to override this function the same way in child class to not lose parent interaction
	// Will call the check link to our interaction
	switch (ID)
	{
	case 0:
		return CanOpenDoor_Interaction(Interactor);
		break;
	default:
		return Super::CanInteractByID_Implementation(ID, Interactor);
		break;
	}
	return false;
}

TArray<FName> ADoorElement::GetInteractionsName_Implementation()
{
	// Take all the previous interaction name and add it to our interaction table to return all the interaction possible for this class and their parent
	auto Names = Super::GetInteractionsName_Implementation();

	Names.Add(GetOpenDoorActionName_Interaction());

	return Names;
}

FName ADoorElement::GetInteractionNameByID_Implementation(uint8 ID)
{
	if (ID == 0)
	{
		return GetOpenDoorActionName_Interaction();
	}

	return FName();
}

bool ADoorElement::HasMultipleInteraction_Implementation()
{
	// Need to be true if we add at last two interaction in this class or parent class.
	return NumberOfInteractions_Implementation() > 1;
}

uint8 ADoorElement::NumberOfInteractions_Implementation()
{
	// Add the number of interaction implemented in this class the same way as this one.
	return Super::NumberOfInteractions_Implementation() + uint8(1);
}
