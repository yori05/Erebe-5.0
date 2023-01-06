// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DialogueComponent/DialogueComponent_Basic.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Structures/Dialogues/DialogueStruct.h"
#include "Engine/DataTable.h"

// Sets default values for this component's properties
UDialogueComponent_Basic::UDialogueComponent_Basic()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	EInteractionType = EInteractionTypes::INTERACTTYPES_Dialogue;

	ComponentTags.Add(FName(TEXT("Dialogue")));
	// ...
}


// Called when the game starts
void UDialogueComponent_Basic::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();

	if (World != nullptr)
	{

		APlayerController* PC = GetWorld()->GetFirstPlayerController();

		if (IsValid(PC))
		{
			if (DialogueWidgetClass != nullptr)
			{
				DialogueWidget = UUserWidget::CreateWidgetInstance(*PC, DialogueWidgetClass, FName(TEXT("DialogueWidget")));
				DialogueWidget->AddToViewport();
				DialogueWidget->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}

	if (DialogueTable != nullptr)
	{
		TArray<FName> RowNames = DialogueTable->GetRowNames();
		FDialogueStruct* CurrentDialog = nullptr;
		FString ContextString;

		if (RowNames.Num() > 0)
		{
			for (auto& Name : RowNames)
			{
				CurrentDialog = DialogueTable->FindRow<FDialogueStruct>(Name, ContextString);

				if (CurrentDialog != nullptr && NPC_ID == CurrentDialog->NPC_ID)
				{
					DialoguesName.Add(Name);
				}
			}
		}
	}
}


// Called every frame
void UDialogueComponent_Basic::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UDialogueComponent_Basic::ChangeVisibiltyWidget(ESlateVisibility Visibility, UDialogueComponent_Basic* OtherDialoguer)
{
	DialogueWidget->SetVisibility(Visibility);
	SavedDialoguer = OtherDialoguer;
}

FText UDialogueComponent_Basic::GetCurrentDialogueLine()
{
	return CurrentText;
}

void UDialogueComponent_Basic::InitializeDialogue(UDialogueComponent_Basic* OtherDialoguer)
{
	CurrentLine = 1;
	ChangeVisibiltyWidget(ESlateVisibility::Visible, OtherDialoguer);
	UpdateDialogue(OtherDialoguer);
}

void UDialogueComponent_Basic::IncrementeDialogue(UDialogueComponent_Basic* OtherDialoguer)
{
	CurrentLine++;

	UpdateDialogue(OtherDialoguer);
}

void UDialogueComponent_Basic::UpdateDialogue(UDialogueComponent_Basic* OtherDialoguer)
{
	if (DialogueTable != nullptr)
	{
		if (CurrentLine <= 0)
		{
			InitializeDialogue(OtherDialoguer);
		}

		FDialogueStruct* SelectedDialogue = nullptr;

		for (auto& DialogueName : DialoguesName)
		{
			SelectedDialogue = DialogueTable->FindRow<FDialogueStruct>(DialogueName, "");
			if (SelectedDialogue->Conversation_ID == Conversation_ID && SelectedDialogue->Line_ID == CurrentLine)
			{
				CurrentText = SelectedDialogue->Dialogue;
				OnDialogueLineChange.Broadcast(CurrentText);
				return;
			}
		}

		StopDialogue();
	}
}

void UDialogueComponent_Basic::StopDialogue()
{
	CurrentLine = 0;
	CurrentText = FText();
	ChangeVisibiltyWidget(ESlateVisibility::Collapsed, nullptr);
}

void UDialogueComponent_Basic::StartConversation_Interaction(UDialogueComponent_Basic* OtherDialoguer)
{
	if (OtherDialoguer != nullptr)
	{
		if (IsValid(DialogueWidget))
		{
			if (DialogueWidget->IsVisible())
			{
				IncrementeDialogue(OtherDialoguer);
			}
			else
			{
				InitializeDialogue(OtherDialoguer);
			}
		}
	}
}

bool UDialogueComponent_Basic::CanStartConversation_Interaction(UDialogueComponent_Basic* OtherDialoguer)
{
	if (OtherDialoguer != nullptr && OtherDialoguer == SavedDialoguer)
	{
		return true;
	}
	else if (OtherDialoguer != nullptr)
	{
		return true;
	}

	return false;
}

FName UDialogueComponent_Basic::GetStartConversationActionName_Interaction()
{
	return FName(TEXT("Conversation"));
}

void UDialogueComponent_Basic::StopConversation_Interaction(UDialogueComponent_Basic* OtherDialoguer)
{
	StopDialogue();
}

bool UDialogueComponent_Basic::TryConversation_Interaction(UDialogueComponent_Basic* OtherDialoguer)
{
	if (CanStartConversation_Interaction(OtherDialoguer))
	{
		StartConversation_Interaction(OtherDialoguer);
		return true;
	}

	return false;
}
