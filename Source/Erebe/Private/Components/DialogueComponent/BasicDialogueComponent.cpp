// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DialogueComponent/BasicDialogueComponent.h"
#include "Structures/Dialogues/DialogueStruct.h"

// Sets default values for this component's properties
UBasicDialogueComponent::UBasicDialogueComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBasicDialogueComponent::BeginPlay()
{
	Super::BeginPlay();

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
void UBasicDialogueComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

