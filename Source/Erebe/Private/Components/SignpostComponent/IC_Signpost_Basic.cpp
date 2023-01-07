// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SignpostComponent/IC_Signpost_Basic.h"
#include "Engine/World.h"
#include "GameFramework/GameModeBase.h"
#include "Components/WidgetComponent/WidgetsComponent_Manager.h"
#include "UserWidgets/UW_Signpost.h"
#include "Engine/DataTable.h"

/**-----------------	Constructor Part		-----------------*/

// Sets default values for this component's properties
UIC_Signpost_Basic::UIC_Signpost_Basic()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ComponentTags.Add(FName(TEXT("Signpost")));

	EInteractionType = EInteractionTypes::INTERACTTYPES_Signpost;

	bCanInteractIfOngoing = true;
	bStopIfCalledOngoing = true;
	bNeedRequestor = true;

	// ...
}

/**-----------------	Actor Component Function Part		-----------------*/

// Called when the game starts
void UIC_Signpost_Basic::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();

	if (SignpostTable != nullptr)
	{
		TArray<FName> RowNames = SignpostTable->GetRowNames();
		FSignpostStructure* CurrentSignpost = nullptr;
		FString ContextString;

		if (RowNames.Num() > 0)
		{
			for (auto& Name : RowNames)
			{
				CurrentSignpost = SignpostTable->FindRow<FSignpostStructure>(Name, ContextString);

				if (CurrentSignpost != nullptr && Signpost_ID == CurrentSignpost->Signpost_ID)
				{
					SavedStructure = (*CurrentSignpost);
					break;
				}
			}
		}
	}

	if (bSearchForUI)
	{
		RequestLinkToWidget();
	}
}

void UIC_Signpost_Basic::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (bSearchForUI)
	{
		RemoveLinkToWidget();
	}
}

/**-----------------	Interactive Function Part		-----------------*/

bool UIC_Signpost_Basic::CanInteract(AActor* _Requestor) const
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

void UIC_Signpost_Basic::BeginInteract()
{
	Super::BeginInteract();

	OnSignpostDisplayCall.Broadcast(SavedStructure);
}

void UIC_Signpost_Basic::EndInteract()
{
	Super::EndInteract();

	Requestor = nullptr;
	OnSignpostHideCall.Broadcast(SavedStructure);
}

void UIC_Signpost_Basic::AbortInteract()
{
	Super::AbortInteract();

	Requestor = nullptr;
	OnSignpostHideCall.Broadcast(SavedStructure);
}

/**-----------------	Signpost Function Part		-----------------*/

void UIC_Signpost_Basic::RequestLinkToWidget()
{
	if (bSearchForUI && GetWorld() != nullptr)
	{
		auto GM = GetWorld()->GetAuthGameMode();

		if (IsValid(GM))
		{
			UWidgetsComponent_Manager* WidgetManager = Cast<UWidgetsComponent_Manager>(GM->GetComponentByClass(UWidgetsComponent_Manager::StaticClass()));

			if (IsValid(WidgetManager))
			{
				UUW_Signpost* WidgetSignpost = WidgetManager->GetSignpostWidget();

				if (IsValid(WidgetSignpost))
				{
					WidgetSignpost->AddSignpostComponent(this);
				}
			}
		}
	}
}

void UIC_Signpost_Basic::RemoveLinkToWidget()
{
	if (bSearchForUI && GetWorld() != nullptr)
	{
		auto GM = GetWorld()->GetAuthGameMode();

		if (IsValid(GM))
		{
			UWidgetsComponent_Manager* WidgetManager = Cast<UWidgetsComponent_Manager>(GM->GetComponentByClass(UWidgetsComponent_Manager::StaticClass()));

			if (IsValid(WidgetManager))
			{
				UUW_Signpost* WidgetSignpost = WidgetManager->GetSignpostWidget();

				if (IsValid(WidgetSignpost))
				{
					WidgetSignpost->RemoveSignpostComponent(this);
				}
			}
		}
	}
}
