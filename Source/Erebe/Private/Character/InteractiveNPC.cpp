// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/InteractiveNPC.h"
#include "Components/SphereComponent.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Structures/Dialogues/DialogueStruct.h"
#include "Engine/DataTable.h"
#include "Components/DialogueComponent/DialogueComponent.h"


AInteractiveNPC::AInteractiveNPC()
{
	PrimaryActorTick.bCanEverTick = true;

	DetectionHitbox = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionHitbox"));
	DetectionHitbox->SetupAttachment(GetRootComponent());
	
	DialogueComponent = CreateDefaultSubobject<UDialogueComponent>(TEXT("DialogueComp"));

	if (InteractiveComponent != nullptr)
	{
		InteractiveComponent->TryToAddAInteraction(DialogueComponent, INTERACTIVE_Dialogue, true);
	}
}

void AInteractiveNPC::BeginPlay()
{
	Super::BeginPlay();

	DetectionHitbox->OnComponentEndOverlap.AddDynamic(this, &AInteractiveNPC::OnHitboxOverlapEnd);

}

void AInteractiveNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (DialogueComponent != nullptr && DialogueComponent->IsInDialogue())
	{
		this->SetActorRotation((DialogueComponent->GetSavedDialoger()->GetOwner()->GetActorLocation() - GetActorLocation()).Rotation());
	}
}

void AInteractiveNPC::OnHitboxOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (DialogueComponent != nullptr && DialogueComponent->IsInDialogue())
	{
		if (DialogueComponent->GetSavedDialoger()->GetOwner() == OtherActor)
		DialogueComponent->StopConversation_Interaction(nullptr);
	}
}


void AInteractiveNPC::StartConversation_Interaction(APawn* Interactor)
{
	if (DialogueComponent != nullptr)
	{
		UDialogueComponent* OtherDialoguer = Cast<UDialogueComponent>(Interactor->GetComponentByClass(UDialogueComponent::StaticClass()));

		if (OtherDialoguer != nullptr)
		{
			DialogueComponent->StartConversation_Interaction(OtherDialoguer);
		}
	}
}

bool AInteractiveNPC::CanStartConversation_Interaction(APawn* Interactor)
{
	if (DialogueComponent != nullptr)
	{
		UDialogueComponent* OtherDialoguer = Cast<UDialogueComponent>(Interactor->GetComponentByClass(UDialogueComponent::StaticClass()));

		if (OtherDialoguer != nullptr)
		{
			return DialogueComponent->CanStartConversation_Interaction(OtherDialoguer);
		}
	}

	return false;
}

FName AInteractiveNPC::GetStartConversationActionName_Interaction()
{
	if (DialogueComponent != nullptr)
		return DialogueComponent->GetStartConversationActionName_Interaction();

	return FName(TEXT("ErrorName"));
}

void AInteractiveNPC::MainInteract_Implementation(APawn* Interactor)
{
	if (!CanMainInteract_Implementation(Interactor))
	{
		return;
	}

	StartConversation_Interaction(Interactor);
	// Call the interaction callback 
	Super::MainInteract_Implementation(Interactor);

}

bool AInteractiveNPC::CanMainInteract_Implementation(APawn* Interactor)
{
	return CanStartConversation_Interaction(Interactor);
}

FName AInteractiveNPC::MainInteractionName_Implementation()
{
	return GetStartConversationActionName_Interaction();
}

bool AInteractiveNPC::HasAInteraction_Implementation()
{
	return true;
}

uint8 AInteractiveNPC::GetMainInteractionID_Implementation()
{
	return uint8(0);
}

void AInteractiveNPC::InteractByID_Implementation(uint8 ID, APawn* Interactor)
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
		StartConversation_Interaction(Interactor);
		break;
	default:
		break;
	}

	// Call the interaction callback 
	// Watch out need to be sure that you don't use a parent id in this switch or will call two interaction
	Super::InteractByID_Implementation(ID, Interactor);

}

bool AInteractiveNPC::CanInteractByID_Implementation(uint8 ID, APawn* Interactor)
{
	// Recommend to override this function the same way in child class to not lose parent interaction
	// Will call the check link to our interaction
	switch (ID)
	{
	case 0:
		return CanStartConversation_Interaction(Interactor);
		break;
	default:
		return Super::CanInteractByID_Implementation(ID, Interactor);
		break;
	}
	return false;
}

TArray<FName> AInteractiveNPC::GetInteractionsName_Implementation()
{
	// Take all the previous interaction name and add it to our interaction table to return all the interaction possible for this class and their parent
	auto Names = Super::GetInteractionsName_Implementation();

	Names.Add(GetStartConversationActionName_Interaction());

	return Names;
}

FName AInteractiveNPC::GetInteractionNameByID_Implementation(uint8 ID)
{
	if (ID == 0)
	{
		return GetStartConversationActionName_Interaction();
	}

	return FName();
}

bool AInteractiveNPC::HasMultipleInteraction_Implementation()
{
	// Need to be true if we add at last two interaction in this class or parent class.
	return NumberOfInteractions_Implementation() > 1;
}

uint8 AInteractiveNPC::NumberOfInteractions_Implementation()
{
	// Add the number of interaction implemented in this class the same way as this one.
	return Super::NumberOfInteractions_Implementation() + uint8(1);
}
