// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/InteractiveCharacter.h"

// Sets default values
AInteractiveCharacter::AInteractiveCharacter(const FObjectInitializer& ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InteractiveComponent = CreateDefaultSubobject<UInteractiveComponent>(TEXT("InteractiveComp"));
}

// Called when the game starts or when spawned
void AInteractiveCharacter::BeginPlay()
{
	ACharacter::BeginPlay();
	
}

// Called every frame
void AInteractiveCharacter::Tick(float DeltaTime)
{
	ACharacter::Tick(DeltaTime);

}

// Called to bind functionality to input
void AInteractiveCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	ACharacter::SetupPlayerInputComponent(PlayerInputComponent);

}

// Called to call the callback function in case interaction can happen
void AInteractiveCharacter::MainInteract_Implementation(APawn* Interactor)
{
	if (CanMainInteract_Implementation(Interactor))
	{
		OnInteract.Broadcast(this, Interactor, GetMainInteractionID_Implementation(), MainInteractionName_Implementation());
	}

	return;
}

// Called to verify if the main interaction is enable 
bool AInteractiveCharacter::CanMainInteract_Implementation(APawn* Interactor)
{
	return false;
}

FName AInteractiveCharacter::MainInteractionName_Implementation()
{
	return FName();
}

bool AInteractiveCharacter::HasAInteraction_Implementation()
{
	return false;
}

uint8 AInteractiveCharacter::GetMainInteractionID_Implementation()
{
	return uint8(0);
}

void AInteractiveCharacter::InteractByID_Implementation(uint8 ID, APawn* Interactor)
{
	if (CanInteractByID_Implementation(ID, Interactor))
	{
		OnInteract.Broadcast(this, Interactor, ID, GetInteractionNameByID_Implementation(ID));
	}

	return;
}

bool AInteractiveCharacter::CanInteractByID_Implementation(uint8 ID, APawn* Interactor)
{
	return false;
}

TArray<FName> AInteractiveCharacter::GetInteractionsName_Implementation()
{
	TArray<FName> InteractionsName;

	return InteractionsName;
}

FName AInteractiveCharacter::GetInteractionNameByID_Implementation(uint8 ID)
{
	return FName();
}

bool AInteractiveCharacter::HasMultipleInteraction_Implementation()
{
	return false;
}

uint8 AInteractiveCharacter::NumberOfInteractions_Implementation()
{
	return uint8(0);
}

