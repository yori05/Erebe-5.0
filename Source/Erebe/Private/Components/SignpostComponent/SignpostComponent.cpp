// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SignpostComponent/SignpostComponent.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"


// Sets default values for this component's properties
USignpostComponent::USignpostComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ComponentTags.Add(FName(TEXT("Signpost")));

	// ...
}


// Called when the game starts
void USignpostComponent::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();

	if (World != nullptr)
	{

		APlayerController* PC = GetWorld()->GetFirstPlayerController();

		if (IsValid(PC))
		{
			if (SignpostWidgetClass != nullptr)
			{
				SignpostWidget = UUserWidget::CreateWidgetInstance(*PC, SignpostWidgetClass, FName(TEXT("DialogueWidget")));
				SignpostWidget->AddToViewport();
				SignpostWidget->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
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
}


// Called every frame
void USignpostComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USignpostComponent::ReadSignpost_Interaction(APawn* Interactor)
{
	if (Interactor != nullptr && Cast<APlayerController>(Interactor->GetController()))
	{
		if (IsValid(SignpostWidget))
		{
			if (SignpostWidget->IsVisible())
			{
				StopReadSignpost_Interaction(Interactor);
			}
			else
			{
				SignpostWidget->SetVisibility(ESlateVisibility::Visible);
				SavedPawn = Interactor;
				OnSignpostLineChange.Broadcast(CurrentSubtext);
			}
		}
	}
}

bool USignpostComponent::CanReadSignpost_Interaction(APawn* Interactor)
{
	if (SavedPawn != nullptr && Interactor == SavedPawn)
	{
		return true;
	}
	else if (Interactor != nullptr)
	{
		// the forward direction of this object
		auto ForwardVector = GetOwner()->GetActorForwardVector();
		// The location of the interactor based on this object
		auto InteractorLocation = Interactor->GetActorLocation() - GetOwner()->GetActorLocation();
		// Dot of both object forward. If they make face of each other should be -1 if they look in the same direction 1 
		float OrientationProjection = FVector::DotProduct(Interactor->GetActorForwardVector(), ForwardVector);
		// Dot product of this object forward and the interactor position, if he is in front of this object it's more than 0 and if he is behind less than 0
		float LocationProjection = FVector::DotProduct(InteractorLocation, ForwardVector);

		// Check if the interactor is in front of this object and look in his direction
		// Maybe later will use the controller direction more than the pawn
		if (OrientationProjection < -0.1f && LocationProjection > 0.1f)
		{
			return true;
		}
	}

	return false;
}

FName USignpostComponent::GetReadSignpostActionName_Interaction()
{
	return FName(TEXT("Read"));
}

void USignpostComponent::StopReadSignpost_Interaction(APawn* Interactor)
{
	SignpostWidget->SetVisibility(ESlateVisibility::Collapsed);
	SavedPawn = nullptr;
}

bool USignpostComponent::TryReadSignpost_Interaction(APawn* Interactor)
{
	if (CanReadSignpost_Interaction(Interactor))
	{
		ReadSignpost_Interaction(Interactor);
		return true;
	}

	return false;
}
