// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InteractiveComponent/InteractiveComponent_Basic.h"

/**-----------------	Constructor Part		-----------------*/

// Sets default values for this component's properties
UInteractiveComponent_Basic::UInteractiveComponent_Basic()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ComponentTags.Add(FName(TEXT("Interactive")));

	// ...
}


/**-----------------	Inherit Function Part		-----------------*/

// Called when the game starts
void UInteractiveComponent_Basic::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInteractiveComponent_Basic::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (bInteractionOngoing && bCanUpdateOnTick)
	{
		UpdateInteract();
	}
}

/**-----------------	Interactive Function Part		-----------------*/

void UInteractiveComponent_Basic::SetIsMainInteraction(bool bNewIsMainInteract, bool bForceIt)
{
	if (!bForceIt && bNewIsMainInteract == bIsMainInteraction)
	{
		return;
	}

	bIsMainInteraction = bNewIsMainInteract;

	OnSetAsMainInteraction.Broadcast();
}

bool UInteractiveComponent_Basic::CallInteraction()
{
	if (CanInteract())
	{
		BeginInteract();

		return true;
	}

	CantInteract();

	return false;
}

bool UInteractiveComponent_Basic::StopInteraction()
{
	if (bInteractionOngoing)
	{
		EndInteract();

		return true;
	}

	return false;
}

void UInteractiveComponent_Basic::CanStopOnNextUpdate()
{
	if (bInteractionOngoing)
	{
		bCanEnd = true;
	}
}

bool UInteractiveComponent_Basic::AbortInteraction()
{
	if (bInteractionOngoing)
	{
		AbortInteract();

		return true;
	}

	return false;
}

bool UInteractiveComponent_Basic::CanInteract() const
{
	return false;
}

void UInteractiveComponent_Basic::BeginInteract()
{
	bInteractionOngoing = true;

	OnBeginInteract.Broadcast();
	ReceiveBeginInteract();

	if (bCallUpdateInBegin)
	{
		UpdateInteract();
	}
	if (bCanEndInBegin && bCanEnd)
	{
		EndInteract();
	}
}

void UInteractiveComponent_Basic::UpdateInteract()
{
	if (!bInteractionOngoing)
	{
		return;
	}

	OnUpdateInteract.Broadcast();
	ReceiveUpdateInteract();

	if (bCanEnd)
	{
		EndInteract();
	}
}

void UInteractiveComponent_Basic::EndInteract()
{
	bInteractionOngoing = false;
	bCanEnd = false;

	OnEndInteract.Broadcast();
	ReceiveEndInteract();
}

void UInteractiveComponent_Basic::AbortInteract()
{
	bInteractionOngoing = false;
	bCanEnd = false;

	OnAbortInteract.Broadcast();
	ReceiveAbortInteract();
}

void UInteractiveComponent_Basic::CantInteract()
{
	OnCantInteract.Broadcast();
	ReceiveCantInteract();
}