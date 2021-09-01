// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawn/InteractivePawn.h"

// Sets default values
AInteractivePawn::AInteractivePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AInteractivePawn::BeginPlay()
{
	APawn::BeginPlay();
	
}

// Called every frame
void AInteractivePawn::Tick(float DeltaTime)
{
	APawn::Tick(DeltaTime);

}

// Called to bind functionality to input
void AInteractivePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	APawn::SetupPlayerInputComponent(PlayerInputComponent);

}

void AInteractivePawn::MainInteract_Implementation(APawn* Interactor)
{
	if (CanMainInteract_Implementation(Interactor))
	{
		OnInteract.Broadcast(this, Interactor, GetMainInteractionID_Implementation(), MainInteractionName_Implementation());
	}

	return;
}

bool AInteractivePawn::CanMainInteract_Implementation(APawn* Interactor)
{
	return false;
}

FName AInteractivePawn::MainInteractionName_Implementation()
{
	return FName();
}

bool AInteractivePawn::HasAInteraction_Implementation()
{
	return false;
}

uint8 AInteractivePawn::GetMainInteractionID_Implementation()
{
	return uint8(0);
}

void AInteractivePawn::InteractByID_Implementation(uint8 ID, APawn* Interactor)
{
	if (CanInteractByID_Implementation(ID, Interactor))
	{
		OnInteract.Broadcast(this, Interactor, ID, GetInteractionNameByID_Implementation(ID));
	}

	return;
}

bool AInteractivePawn::CanInteractByID_Implementation(uint8 ID, APawn* Interactor)
{
	return false;
}

TArray<FName> AInteractivePawn::GetInteractionsName_Implementation()
{
	TArray<FName> InteractionsName;

	return InteractionsName;
}

FName AInteractivePawn::GetInteractionNameByID_Implementation(uint8 ID)
{
	return FName();
}

bool AInteractivePawn::HasMultipleInteraction_Implementation()
{
	return false;
}

uint8 AInteractivePawn::NumberOfInteractions_Implementation()
{
	return uint8(0);
}

