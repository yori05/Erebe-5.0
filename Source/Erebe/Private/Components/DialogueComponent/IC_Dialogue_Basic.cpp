// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DialogueComponent/IC_Dialogue_Basic.h"
#include "Components/DialogueComponent/DialogueComponent_Basic.h"

/**-----------------	Constructor Part		-----------------*/

// Sets default values for this component's properties
UIC_Dialogue_Basic::UIC_Dialogue_Basic()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	ComponentTags.Add(FName(TEXT("Dialogue")));

	EInteractionType = EInteractionTypes::INTERACTTYPES_Dialogue;

	bCanInteractIfOngoing = true;
	bUpdateIfCalledOngoing = true;
	bStopIfCalledOngoing = false;
	bNeedRequestor = true;
}

/**-----------------	Actor Component Function Part		-----------------*/

// Called when the game starts
void UIC_Dialogue_Basic::BeginPlay()
{
	Super::BeginPlay();

	if (bSearchDialogueComp)
	{
		SearchForDialogueComponent();
	}
}

void UIC_Dialogue_Basic::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	BreakLinkDialogueComponent();
}

/**-----------------	Interactive Function Part		-----------------*/

bool UIC_Dialogue_Basic::CanInteract(AActor* _Requestor) const
{
	if (Super::CanInteract(_Requestor))
	{
		if (_Requestor == Requestor)
		{
			return true;
		}
		else
		{
			return FrontAndForwardAlignment(_Requestor);
		}
	}

	return false;
}

void UIC_Dialogue_Basic::BeginInteract()
{
	Super::BeginInteract();

	if (bInteractionOngoing && IsValid(SavedDialogueComponent))
	{
		SavedDialogueComponent->ChangeLine(1);
	}
}

void UIC_Dialogue_Basic::UpdateInteract(float DeltaTime)
{
	Super::UpdateInteract(DeltaTime);

	if (bInteractionOngoing && IsValid(SavedDialogueComponent))
	{
		if (!SavedDialogueComponent->NextLine())
		{
			StopInteraction();
		}
	}
}

void UIC_Dialogue_Basic::EndInteract()
{
	Super::EndInteract();

	if (IsValid(SavedDialogueComponent))
	{
		SavedDialogueComponent->DisengageDialogue();
	}
}

void UIC_Dialogue_Basic::AbortInteract()
{
	Super::AbortInteract();

	if (IsValid(SavedDialogueComponent))
	{
		SavedDialogueComponent->DisengageDialogue();
	}
}

/**-----------------	Dialogue Function Part		-----------------*/

void UIC_Dialogue_Basic::ReceiveDialogueComponent(UDialogueComponent_Basic* _NewDialogueComp)
{
	if (SavedDialogueComponent != _NewDialogueComp)
	{
		if (IsValid(SavedDialogueComponent))
		{
			BreakLinkDialogueComponent();
		}

		SavedDialogueComponent = _NewDialogueComp;

		if (IsValid(_NewDialogueComp))
		{
			LinkDialogueComponent();
		}
	}
}

void UIC_Dialogue_Basic::SearchForDialogueComponent()
{
	if (IsValid(Owner))
	{
		UDialogueComponent_Basic* Comp = Cast<UDialogueComponent_Basic>(Owner->GetComponentByClass(UDialogueComponent_Basic::StaticClass()));

		if (IsValid(Comp))
		{
			SavedDialogueComponent = Comp;
			LinkDialogueComponent();
		}
	}
}

void UIC_Dialogue_Basic::LinkDialogueComponent()
{
	if (IsValid(SavedDialogueComponent))
	{
		SavedDialogueComponent->OnDialogueDisengagedCall.AddUniqueDynamic(this, &UIC_Dialogue_Basic::WhenDialogueDisengage);
	}	
}

void UIC_Dialogue_Basic::BreakLinkDialogueComponent()
{
	if (IsValid(SavedDialogueComponent))
	{
		if (SavedDialogueComponent->OnDialogueDisengagedCall.IsAlreadyBound(this, &UIC_Dialogue_Basic::WhenDialogueDisengage))
		{
			SavedDialogueComponent->OnDialogueDisengagedCall.RemoveDynamic(this, &UIC_Dialogue_Basic::WhenDialogueDisengage);
		}
	}
}

void UIC_Dialogue_Basic::WhenDialogueDisengage(FDialogueStruct DialogueStruct)
{
	if (bInteractionOngoing)
	{
		StopInteraction();
	}
}