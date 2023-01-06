// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InteractiveComponent/Manager_InteractiveComponent.h"
#include "Components/InteractiveComponent/InteractiveComponent_Basic.h"

// Sets default values for this component's properties
UManager_InteractiveComponent::UManager_InteractiveComponent(const FObjectInitializer& ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UManager_InteractiveComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called on the post Load
void UManager_InteractiveComponent::PostLoad()
{
	Super::PostLoad();

	Owner = Cast<AActor>(GetOwner());

	// ...

}

// Called every frame
void UManager_InteractiveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UManager_InteractiveComponent::RequestSaveInteractiveComponent(UInteractiveComponent_Basic* NewInteractiveComp)
{
	if (!IsValid(NewInteractiveComp) || NewInteractiveComp->GetInteractionType() == EInteractionTypes::INTERACTTYPES_Undefinned)
	{
		return false;
	}

	if (InteractivesActions.Contains(NewInteractiveComp->GetInteractionType()))
	{
		return false;
	}

	InteractivesActions.Add(NewInteractiveComp->GetInteractionType(), NewInteractiveComp);
	
	if (NewInteractiveComp->GetIsMainInteraction())
	{
		if (!SetNewMainInteraction(NewInteractiveComp))
		{
			NewInteractiveComp->SetIsMainInteraction(false);
		}
	}

	return true;
}

bool UManager_InteractiveComponent::SetNewMainInteraction(UInteractiveComponent_Basic* NewMainInteraction)
{
	if (IsValid(NewMainInteraction))
	{
		ResetMainInteraction();

		MainCatergorieAction = NewMainInteraction->GetInteractionType();
		MainInteractiveAction = NewMainInteraction;
		
		return true;
	}

	return false;
}

void UManager_InteractiveComponent::ResetMainInteraction(bool bResetInteractiveComp)
{
	if (bResetInteractiveComp && IsValid(MainInteractiveAction))
	{
		MainInteractiveAction->SetIsMainInteraction(false);
	}

	MainInteractiveAction = nullptr;
	MainCatergorieAction = EInteractionTypes::INTERACTTYPES_Undefinned;
}

UInteractiveComponent_Basic* UManager_InteractiveComponent::GetInteractionByType(EInteractionTypes _InteractionType)
{
	//if (InteractivesActions.Contains(_InteractionType))
	//{
	//	return InteractivesActions[_InteractionType].Get();
	//}

	return nullptr;
}
