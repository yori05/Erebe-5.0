// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/WidgetComponent/WidgetsComponent_Manager.h"
#include "UserWidgets/UW_Signpost.h"
#include "UserWidgets/UW_Dialogue.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UWidgetsComponent_Manager::UWidgetsComponent_Manager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	UWorld* World = GetWorld();

	if (World != nullptr)
	{
		if (bSignpostWidgetHandle && SignpostWidgetClass != nullptr)
		{
			SignpostWidget = Cast<UUW_Signpost>(UUserWidget::CreateWidgetInstance(*World, SignpostWidgetClass, FName(TEXT("SignpostWidget"))));
		}
		if (bDialogueWidgetHandle && DialogueWidgetClass != nullptr)
		{
			DialogueWidget = Cast<UUW_Dialogue>(UUserWidget::CreateWidgetInstance(*World, DialogueWidgetClass, FName(TEXT("DialogueWidget"))));
		}
	}

	// ...
}


// Called when the game starts
void UWidgetsComponent_Manager::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();

	if (World != nullptr)
	{
		if (!IsValid(SignpostWidget) && bSignpostWidgetHandle)
		{
			if (SignpostWidgetClass != nullptr)
			{
				SignpostWidget = Cast<UUW_Signpost>(UUserWidget::CreateWidgetInstance(*World, SignpostWidgetClass, FName(TEXT("SignpostWidget"))));
			}
		}
		if (!IsValid(DialogueWidget) && bDialogueWidgetHandle)
		{
			if (DialogueWidgetClass != nullptr)
			{
				DialogueWidget = Cast<UUW_Dialogue>(UUserWidget::CreateWidgetInstance(*World, DialogueWidgetClass, FName(TEXT("DialogueWidget"))));
			}
		}
	}
	if (IsValid(SignpostWidget) && bSignpostWidgetHandle)
	{
		SignpostWidget->AddToViewport();
	}
	if (IsValid(DialogueWidget) && bDialogueWidgetHandle)
	{
		DialogueWidget->AddToViewport();
	}

}


// Called every frame
void UWidgetsComponent_Manager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

