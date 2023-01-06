// Fill out your copyright notice in the Description page of Project Settings.


#include "ConstructionElements/SignpostElement.h"
#include "GameFramework/PlayerController.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Engine/World.h"
#include "Components/SignpostComponent/SignpostComponent.h"


ASignpostElement::ASignpostElement()
{
	PrimaryActorTick.bCanEverTick = true;

	SignpostMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SignpostMesh"));
	SignpostMesh->SetupAttachment(RootComponent);

	InteractiveHitbox = CreateDefaultSubobject<UCapsuleComponent>(TEXT("InteractiveHitbox"));
	InteractiveHitbox->SetupAttachment(SignpostMesh);

	SignpostComponent = CreateDefaultSubobject<USignpostComponent>(TEXT("SignpostComp"));
	if (InteractiveComponent != nullptr)
	{
		InteractiveComponent->TryToAddAInteraction(SignpostComponent, EInteractiveCategorie::INTERACTIVE_Signpost, true);
	}
}

void ASignpostElement::BeginPlay()
{
	Super::BeginPlay();

	InteractiveHitbox->OnComponentEndOverlap.AddDynamic(this, &ASignpostElement::OnHitboxOverlapEnd);

}

void ASignpostElement::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASignpostElement::OnHitboxOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (SignpostComponent != nullptr && SignpostComponent->IsReading())
	{
		if (OtherActor == SignpostComponent->GetSavedPawn())
		{
			SignpostComponent->StopReadSignpost_Interaction(nullptr);
		}
	}
}

void ASignpostElement::ReadSignpost_Interaction(APawn* Interactor)
{
	if (SignpostComponent != nullptr)
	{
		SignpostComponent->ReadSignpost_Interaction(Interactor);
	}
}

bool ASignpostElement::CanReadSignpost_Interaction(APawn* Interactor)
{
	if (SignpostComponent != nullptr)
	{
		return SignpostComponent->CanReadSignpost_Interaction(Interactor);
	}

	return false;
}

FName ASignpostElement::GetReadSignpostActionName_Interaction()
{
	if (SignpostComponent != nullptr)
		return SignpostComponent->GetReadSignpostActionName_Interaction();

	return FName(TEXT("Error Signpost comp"));
}

void ASignpostElement::MainInteract_Implementation(APawn* Interactor)
{
	if (!CanMainInteract_Implementation(Interactor))
	{
		return;
	}

	ReadSignpost_Interaction(Interactor);
	// Call the interaction callback 
	Super::MainInteract_Implementation(Interactor);

}

bool ASignpostElement::CanMainInteract_Implementation(APawn* Interactor)
{
	return CanReadSignpost_Interaction(Interactor);
}

FName ASignpostElement::MainInteractionName_Implementation()
{
	return GetReadSignpostActionName_Interaction();
}

bool ASignpostElement::HasAInteraction_Implementation()
{
	return true;
}

uint8 ASignpostElement::GetMainInteractionID_Implementation()
{
	return uint8(0);
}

void ASignpostElement::InteractByID_Implementation(uint8 ID, APawn* Interactor)
{
	// Recommend to override this function the same way in child class to not lose parent interaction
	if (!CanInteractByID_Implementation(ID, Interactor))
	{
		return;
	}

	// Will class the action link to our interaction
	switch (ID)
	{
	case 0:
		// Our current main interaction
		ReadSignpost_Interaction(Interactor);
		break;
	default:
		break;
	}

	// Call the interaction callback 
	// Watch out need to be sure that you don't use a parent id in this switch or will call two interaction
	Super::InteractByID_Implementation(ID, Interactor);

}

bool ASignpostElement::CanInteractByID_Implementation(uint8 ID, APawn* Interactor)
{
	// Recommend to override this function the same way in child class to not lose parent interaction
	// Will call the check link to our interaction
	switch (ID)
	{
	case 0:
		return CanReadSignpost_Interaction(Interactor);
		break;
	default:
		return Super::CanInteractByID_Implementation(ID, Interactor);
		break;
	}
	return false;
}

TArray<FName> ASignpostElement::GetInteractionsName_Implementation()
{
	// Take all the previous interaction name and add it to our interaction table to return all the interaction possible for this class and their parent
	auto Names = Super::GetInteractionsName_Implementation();

	Names.Add(GetReadSignpostActionName_Interaction());

	return Names;
}

FName ASignpostElement::GetInteractionNameByID_Implementation(uint8 ID)
{
	if (ID == 0)
	{
		return GetReadSignpostActionName_Interaction();
	}

	return FName();
}

bool ASignpostElement::HasMultipleInteraction_Implementation()
{
	// Need to be true if we add at last two interaction in this class or parent class.
	return NumberOfInteractions_Implementation() > 1;
}

uint8 ASignpostElement::NumberOfInteractions_Implementation()
{
	// Add the number of interaction implemented in this class the same way as this one.
	return Super::NumberOfInteractions_Implementation() + uint8(1);
}
