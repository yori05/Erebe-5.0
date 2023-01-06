// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InteractiveComponent/InteractiveComponent.h"
#include "Components/SignpostComponent/SignpostComponent.h"
#include "Components/DialogueComponent/DialogueComponent.h"
#include "Components/MovementComponent/RotatingMeshComponent.h"

// Sets default values for this component's properties
UInteractiveComponent::UInteractiveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInteractiveComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInteractiveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UInteractiveComponent::TryToAddAInteraction(UActorComponent* NewComp, EInteractiveCategorie NewCategorie, bool IsMainAction)
{
	if (!InteractivesActions.Contains(NewCategorie))
	{
		InteractivesActions.Add(NewCategorie, NewComp);

		if (IsMainAction)
		{
			MainInteractiveAction = NewComp;
			MainCatergorieAction = NewCategorie;
		}

		return true;
	}

	return false;
}

UActorComponent* UInteractiveComponent::GetInteractionComponent(EInteractiveCategorie NewCategorie)
{
	if (InteractivesActions.Contains(NewCategorie))
	{
		return InteractivesActions[NewCategorie];
	}

	return nullptr;
}

void UInteractiveComponent::Interact(EInteractiveCategorie Categorie, UInteractiveComponent* OtherComponent)
{
	if (!InteractivesActions.Contains(Categorie))
	{
		return;
	}

	UActorComponent* InteractiveComp = InteractivesActions[Categorie];
	if (InteractiveComp)
	{
		if (Categorie == INTERACTIVE_Signpost)
		{
			USignpostComponent* CompSign = nullptr;
			CompSign = Cast<USignpostComponent>(InteractiveComp);
			CompSign->ReadSignpost_Interaction(OtherComponent->GetOwner<APawn>());
			OnInteract.Broadcast(this, OtherComponent, Categorie);
		}
		else if (Categorie == INTERACTIVE_Dialogue)
		{
			UDialogueComponent* CompDial = Cast<UDialogueComponent>(InteractiveComp);
			UDialogueComponent* CompOtherDial = Cast<UDialogueComponent>(OtherComponent->GetInteractionComponent(INTERACTIVE_Dialogue));
			CompDial->StartConversation_Interaction(CompOtherDial);
			OnInteract.Broadcast(this, OtherComponent, Categorie);
		}
		else if (Categorie == INTERACTIVE_OpenDoor)
		{
			URotatingMeshComponent* CompRotate = Cast<URotatingMeshComponent>(InteractiveComp);
			CompRotate->RotateMesh_Interaction(OtherComponent->GetOwner<APawn>());
			OnInteract.Broadcast(this, OtherComponent, Categorie);
		}
	}
}

void UInteractiveComponent::MainInteract(UInteractiveComponent* OtherComponent)
{
	Interact(MainCatergorieAction, OtherComponent);
}