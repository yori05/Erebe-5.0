// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/InteractiveActor.h"
#include "Components/InteractiveComponent/InteractiveComponent.h"

// Sets default values
AInteractiveActor::AInteractiveActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InteractiveComponent = CreateDefaultSubobject<UInteractiveComponent>(TEXT("InteractiveComp"));
}

// Called when the game starts or when spawned
void AInteractiveActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractiveActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractiveActor::MainInteract_Implementation(APawn* Interactor)
{
	if (CanMainInteract_Implementation(Interactor))
	{
		OnInteract.Broadcast(this, Interactor, GetMainInteractionID_Implementation(), MainInteractionName_Implementation());
	}

	return;
}

bool AInteractiveActor::CanMainInteract_Implementation(APawn* Interactor)
{
	return false;
}

FName AInteractiveActor::MainInteractionName_Implementation()
{
	return FName();
}

bool AInteractiveActor::HasAInteraction_Implementation()
{
	return false;
}

uint8 AInteractiveActor::GetMainInteractionID_Implementation()
{
	return uint8(0);
}

void AInteractiveActor::InteractByID_Implementation(uint8 ID, APawn* Interactor)
{
	if (CanInteractByID_Implementation(ID, Interactor))
	{
		OnInteract.Broadcast(this, Interactor, ID, GetInteractionNameByID_Implementation(ID));
	}

	return;
}

bool AInteractiveActor::CanInteractByID_Implementation(uint8 ID, APawn* Interactor)
{
	return false;
}

TArray<FName> AInteractiveActor::GetInteractionsName_Implementation()
{
	TArray<FName> InteractionsName;

	return InteractionsName;
}

FName AInteractiveActor::GetInteractionNameByID_Implementation(uint8 ID)
{
	return FName();
}

bool AInteractiveActor::HasMultipleInteraction_Implementation()
{
	return false;
}

uint8 AInteractiveActor::NumberOfInteractions_Implementation()
{
	return uint8(0);
}
