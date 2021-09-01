// Fill out your copyright notice in the Description page of Project Settings.


#include "Interfaces/InteractiveInterface.h"

// Add default functionality here for any IInteractibleInterface functions that are not pure virtual.

void IInteractiveInterface::MainInteract_Implementation(APawn* Interactor)
{
	return;
}

bool IInteractiveInterface::CanMainInteract_Implementation(APawn* Interactor)
{
	return false;
}

FName IInteractiveInterface::MainInteractionName_Implementation()
{
	return FName();
}

bool IInteractiveInterface::HasAInteraction_Implementation()
{
	return false;
}

uint8 IInteractiveInterface::GetMainInteractionID_Implementation()
{
	return uint8(0);
}

void IInteractiveInterface::InteractByID_Implementation(uint8 ID, APawn* Interactor)
{
	return;
}

bool IInteractiveInterface::CanInteractByID_Implementation(uint8 ID, APawn* Interactor)
{
	return false;
}

TArray<FName> IInteractiveInterface::GetInteractionsName_Implementation()
{
	TArray<FName> InteractionsName;
	
	return InteractionsName;
}

FName IInteractiveInterface::GetInteractionNameByID_Implementation(uint8 ID)
{
	return FName();
}

bool IInteractiveInterface::HasMultipleInteraction_Implementation()
{
	return false;
}

uint8 IInteractiveInterface::NumberOfInteractions_Implementation()
{
	return uint8(0);
}

