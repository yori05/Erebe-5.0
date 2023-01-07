// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DialogueComponent/DialogueComponent_Basic.h"
#include "Engine/World.h"
#include "GameFramework/GameModeBase.h"
#include "Components/WidgetComponent/WidgetsComponent_Manager.h"
#include "UserWidgets/UW_Dialogue.h"
#include "Engine/DataTable.h"

/**-----------------	Constructor Part		-----------------*/

// Sets default values for this component's properties
UDialogueComponent_Basic::UDialogueComponent_Basic()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ComponentTags.Add(FName(TEXT("Dialogue")));
}

/**-----------------	Actor Component Function Part		-----------------*/

// Called when the game starts
void UDialogueComponent_Basic::BeginPlay()
{
	Super::BeginPlay();

	if (bLoadDataInBegin)
	{
		LoadDataTable();
	}

	if (bSearchForUI)
	{
		RequestLinkToWidget();
	}
}

void UDialogueComponent_Basic::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (bSearchForUI)
	{
		RemoveLinkToWidget();
	}
}

/**-----------------	Data Function Part		-----------------*/

bool UDialogueComponent_Basic::LoadDataTable()
{
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
					DialoguesSaved.Add(*CurrentDialog);

					if (NbLineForConversation.Contains(CurrentDialog->Conversation_ID))
					{
						if (NbLineForConversation[CurrentDialog->Conversation_ID] < CurrentDialog->Line_ID)
						{
							NbLineForConversation[CurrentDialog->Conversation_ID] = CurrentDialog->Line_ID;
						}
					}
					else
					{
						NbLineForConversation.Add(CurrentDialog->Conversation_ID, CurrentDialog->Line_ID);
					}
				}
			}

			bLoaded = true;
			return true;
		}
	}

	return false;
}

void UDialogueComponent_Basic::ChangeDataTable(UDataTable* NewDataTable)
{
	if (IsValid(NewDataTable))
	{
		DialogueTable = NewDataTable;
		DialoguesSaved.Empty(DialoguesSaved.Num());
		NbLineForConversation.Empty(NbLineForConversation.Num());

		LoadDataTable();
	}
}

/**-----------------	Dialogue Function Part		-----------------*/

void UDialogueComponent_Basic::UpdateDialogue()
{
	if (bLoaded)
	{
		for (auto& it : DialoguesSaved)
		{
			if (it.Conversation_ID == Conversation_ID && it.Line_ID == CurrentLine)
			{
				if (CurrentDialogue.bIsSpeacialEvent)
				{
					OnDialogueSpecialEventUncall.Broadcast(CurrentDialogue);
				}

				CurrentDialogue = it;
				CurrentText = it.Dialogue;
				OnDialogueChangeCall.Broadcast(CurrentDialogue);

				if (CurrentDialogue.bIsSpeacialEvent)
				{
					OnDialogueSpecialEventCall.Broadcast(CurrentDialogue);
				}

				if (!IsEngaged())
				{
					EngageDialogue();
				}
				
				return;
			}
		}
	}
}

bool UDialogueComponent_Basic::ChangeDialogue(int32 NewConversationID, int32 NewLineNumber)
{
	if (CanChangeDialogue(NewConversationID, NewLineNumber))
	{
		Conversation_ID = NewConversationID;
		CurrentLine = NewLineNumber;

		UpdateDialogue();

		return true;
	}

	return false;
}

bool UDialogueComponent_Basic::ChangeLine(int32 NewLineNumber)
{
	return ChangeDialogue(Conversation_ID, NewLineNumber);
}

bool UDialogueComponent_Basic::NextLine()
{
	return ChangeLine(CurrentLine + 1);
}

bool UDialogueComponent_Basic::ChangeConversation(int32 NewConversationID)
{
	return ChangeDialogue(NewConversationID, 1);
}

bool UDialogueComponent_Basic::CanChangeDialogue(int32 NewConversationID, int32 NewLineNumber)
{
	if (bLoaded && NbLineForConversation.Contains(NewConversationID))
	{
		return NbLineForConversation[NewConversationID] >= NewLineNumber;
	}

	return false;
}

bool UDialogueComponent_Basic::CanChangeLine(int32 NewLineNumber)
{
	return CanChangeDialogue(Conversation_ID, NewLineNumber);
}

bool UDialogueComponent_Basic::CanNextLine()
{
	return CanChangeLine(CurrentLine + 1);
}

bool UDialogueComponent_Basic::CanChangeConversation(int32 NewConversationID)
{
	return CanChangeDialogue(NewConversationID, 1);
}

/**-----------------	Engage Function Part		-----------------*/

void UDialogueComponent_Basic::EngageDialogue()
{
	bIsEngaged = true;
	OnDialogueEngagedCall.Broadcast(CurrentDialogue);
}

void UDialogueComponent_Basic::DisengageDialogue()
{
	bIsEngaged = false;
	OnDialogueDisengagedCall.Broadcast(CurrentDialogue);
}

/**-----------------	Widget Function Part		-----------------*/

void UDialogueComponent_Basic::RequestLinkToWidget()
{
	if (bSearchForUI && GetWorld() != nullptr)
	{
		auto GM = GetWorld()->GetAuthGameMode();

		if (IsValid(GM))
		{
			UWidgetsComponent_Manager* WidgetManager = Cast<UWidgetsComponent_Manager>(GM->GetComponentByClass(UWidgetsComponent_Manager::StaticClass()));

			if (IsValid(WidgetManager))
			{
				UUW_Dialogue* WidgetDialogue = WidgetManager->GetDialogueWidget();

				if (IsValid(WidgetDialogue))
				{
					WidgetDialogue->AddDialogueComponent(this);
				}
			}
		}
	}
}

void UDialogueComponent_Basic::RemoveLinkToWidget()
{
	if (bSearchForUI && GetWorld() != nullptr)
	{
		auto GM = GetWorld()->GetAuthGameMode();

		if (IsValid(GM))
		{
			UWidgetsComponent_Manager* WidgetManager = Cast<UWidgetsComponent_Manager>(GM->GetComponentByClass(UWidgetsComponent_Manager::StaticClass()));

			if (IsValid(WidgetManager))
			{
				UUW_Dialogue* WidgetDialogue = WidgetManager->GetDialogueWidget();

				if (IsValid(WidgetDialogue))
				{
					WidgetDialogue->RemoveDialogueComponent(this);
				}
			}
		}
	}
}

