// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DoorComponent/IC_DoorOpenner_Basic.h"
#include "Components/DoorComponent/DoorComponent_Basic.h"


// Sets default values for this component's properties
UIC_DoorOpenner_Basic::UIC_DoorOpenner_Basic()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	ComponentTags.Add(FName(TEXT("Door")));
	EInteractionType = EInteractionTypes::INTERACTTYPES_OpenDoor;

	bCanUpdateOnTick = true;
	bCanInteractIfOngoing = true;
	
	bNeedRequestor = true;
	//bCanGetOppositeDirection = true;
}

// Called when the game starts
void UIC_DoorOpenner_Basic::BeginPlay()
{
	Super::BeginPlay();

	if (bSearchDoorComp)
	{
		SearchForDoorComponent();
	}
}

void UIC_DoorOpenner_Basic::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	BreakLinkDoorComponent();
}

bool UIC_DoorOpenner_Basic::CanInteract(AActor* _Requestor) const
{
	if (!Super::CanInteract(_Requestor))
	{
		return false;
	}

	if (SavedDoorComponent->GetIsBlocked())
	{
		return false;
	}

	if (SavedDoorComponent->GetIsClose())
	{
		return OwnerLocByRequestorFrwAlign(_Requestor);
	}

	return true;
}

void UIC_DoorOpenner_Basic::BeginInteract()
{
	Super::BeginInteract();

	if (bInteractionOngoing && IsValid(SavedDoorComponent))
	{
		if (SavedDoorComponent->GetIsClose())
		{
			if (SavedDoorComponent->GetGoOppositeDirection() && IsValid(Requestor))
			{
				// Check actor direction from the door and see he how is align with her.
				float InteractorProjection = FVector::DotProduct(Requestor->GetActorForwardVector(), Owner->GetActorForwardVector());

				// Using the actor projection to check if the actor is behind the door and he push it.
				if (InteractorProjection < 0)
				{
					SavedDoorComponent->OpenDoorReverse();
					return;
				}
			}

			SavedDoorComponent->OpenDoorDefault();
		}
		else
		{
			SavedDoorComponent->OpenCloseDoor();
		}
	}
}

void UIC_DoorOpenner_Basic::UpdateInteract(float DeltaTime)
{
	Super::UpdateInteract(DeltaTime);

	if (bInteractionOngoing)
	{
		SavedDoorComponent->CallUpdateDoor(DeltaTime);
	}
}

void UIC_DoorOpenner_Basic::AbortInteract()
{
	Super::AbortInteract();

	if (bInteractionOngoing)
	{
		SavedDoorComponent->StuckDoor();
	}
}

/**-----------------	Dialogue Function Part		-----------------*/

void UIC_DoorOpenner_Basic::ReceiveDoorComponent(UDoorComponent_Basic* _NewDoorComp)
{
	if (SavedDoorComponent != _NewDoorComp)
	{
		if (IsValid(SavedDoorComponent))
		{
			BreakLinkDoorComponent();
		}

		SavedDoorComponent = _NewDoorComp;

		if (IsValid(_NewDoorComp))
		{
			LinkDoorComponent();
		}
	}
}

void UIC_DoorOpenner_Basic::SearchForDoorComponent()
{
	if (IsValid(Owner))
	{
		UDoorComponent_Basic* Comp = Cast<UDoorComponent_Basic>(Owner->GetComponentByClass(UDoorComponent_Basic::StaticClass()));

		if (IsValid(Comp))
		{
			SavedDoorComponent = Comp;
			LinkDoorComponent();
		}
	}
}

void UIC_DoorOpenner_Basic::LinkDoorComponent()
{
	if (IsValid(SavedDoorComponent))
	{
		SavedDoorComponent->OnDoorFinish.AddUniqueDynamic(this, &UIC_DoorOpenner_Basic::StopInteraction);
	}
}

void UIC_DoorOpenner_Basic::BreakLinkDoorComponent()
{
	if (IsValid(SavedDoorComponent))
	{
		if (SavedDoorComponent->OnDoorFinish.IsAlreadyBound(this, &UIC_DoorOpenner_Basic::StopInteraction))
		{
			SavedDoorComponent->OnDoorFinish.RemoveDynamic(this, &UIC_DoorOpenner_Basic::StopInteraction);
		}
	}
}
