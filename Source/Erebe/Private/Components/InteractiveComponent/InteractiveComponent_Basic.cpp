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

	Owner = Cast<AActor>(GetOwner());

	// ...

}


// Called every frame
void UInteractiveComponent_Basic::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (bInteractionOngoing && bCanUpdateOnTick)
	{
		UpdateInteract(DeltaTime);
	}
}

void UInteractiveComponent_Basic::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);


	if (bInteractionOngoing)
	{
		AbortInteraction();
	}
}

/**-----------------	Interactive Function Part		-----------------*/

void UInteractiveComponent_Basic::SetAsMainInteraction(bool bNewIsMainInteract, bool bForceIt)
{
	if (!bForceIt && bNewIsMainInteract == bIsMainInteraction)
	{
		return;
	}

	bIsMainInteraction = bNewIsMainInteract;

	ReceiveSetAsMainInteraction(bNewIsMainInteract, bForceIt);
	OnSetAsMainInteraction.Broadcast();
}

bool UInteractiveComponent_Basic::CallInteraction(AActor* _Requestor)
{
	if (CanInteract(_Requestor))
	{
		if (bInteractionOngoing && bStopIfCalledOngoing)
		{
			EndInteract();
		}
		else if (bInteractionOngoing && bUpdateIfCalledOngoing)
		{
			UpdateInteract();
		}
		else
		{
			Requestor = _Requestor;
			BeginInteract();
		}

		return true;
	}

	CantInteract();

	return false;
}

void UInteractiveComponent_Basic::StopInteraction()
{
	if (bInteractionOngoing)
	{
		EndInteract();
	}
}

void UInteractiveComponent_Basic::CanStopOnNextUpdate()
{
	if (bInteractionOngoing)
	{
		bCanEnd = true;
	}
}

void UInteractiveComponent_Basic::AbortInteraction()
{
	if (bInteractionOngoing)
	{
		AbortInteract();
	}
}

bool UInteractiveComponent_Basic::CanInteract(AActor* _Requestor) const
{
	bool bResult = (bNeedRequestor) ? IsValid(_Requestor) : true;
	bResult = bResult && ((bInteractionOngoing) ? bCanInteractIfOngoing : true);

	return (bNeedRequestor) ? IsValid(_Requestor) : true
			&& (bInteractionOngoing) ? bCanInteractIfOngoing : true;
}

void UInteractiveComponent_Basic::BeginInteract()
{
	bInteractionOngoing = true;

	ReceiveBeginInteract();
	OnBeginInteract.Broadcast();

	if (bCallUpdateInBegin)
	{
		UpdateInteract(0.f);
	}
	if (bCanEndInBegin && bCanEnd)
	{
		EndInteract();
	}
}

void UInteractiveComponent_Basic::UpdateInteract(float DeltaTime)
{
	if (!bInteractionOngoing)
	{
		return;
	}

	ReceiveUpdateInteract(DeltaTime);
	OnUpdateInteract.Broadcast(DeltaTime);

	if (bCanEnd)
	{
		EndInteract();
	}
}

void UInteractiveComponent_Basic::EndInteract()
{
	bInteractionOngoing = false;
	bCanEnd = false;
	Requestor = nullptr;

	ReceiveEndInteract();
	OnEndInteract.Broadcast();
}

void UInteractiveComponent_Basic::AbortInteract()
{
	bInteractionOngoing = false;
	bCanEnd = false;
	Requestor = nullptr;

	ReceiveAbortInteract();
	OnAbortInteract.Broadcast();
}

void UInteractiveComponent_Basic::CantInteract()
{
	ReceiveCantInteract();
	OnCantInteract.Broadcast();
}

/**-----------------	Check Interaction Function Part		-----------------*/

bool UInteractiveComponent_Basic::ForwardAlignment(AActor* _Requestor) const
{
	if (IsValid(_Requestor) && IsValid(Owner))
	{
		//If the door is close check if the player is aligned enough with the door.
		float InteractorProjection = FVector::DotProduct(_Requestor->GetActorForwardVector(), Owner->GetActorForwardVector());

		if (DotForwdAlignSensitivity * DotForwdAlignSensitivity <= InteractorProjection * InteractorProjection)
		{
			return true;
		}
	}

	return false;
}

bool UInteractiveComponent_Basic::OwnerLocByRequestorFrwAlign(AActor* _Requestor) const
{
	if (IsValid(_Requestor) && IsValid(Owner))
	{
		auto OwnerByRequestorLocation = Owner->GetActorLocation() - _Requestor->GetActorLocation();
		float ForwardLocationProjection = FVector::DotProduct(_Requestor->GetActorForwardVector(), OwnerByRequestorLocation);

		if (ForwardLocationProjection >= DotForwdAlignSensitivity)
		{
			return true;
		}
	}

	return false;
}

bool UInteractiveComponent_Basic::FrontAndForwardAlignment(AActor* _Requestor) const
{
	if (IsValid(_Requestor) && IsValid(Owner))
	{
		// the forward direction of this object
		auto ForwardVector = Owner->GetActorForwardVector();
		// The location of the interactor based on this object
		auto InteractorLocation = _Requestor->GetActorLocation() - Owner->GetActorLocation();
		// Dot of both object forward. If they make face of each other should be -1 if they look in the same direction 1 
		float OrientationProjection = FVector::DotProduct(_Requestor->GetActorForwardVector(), ForwardVector);
		// Dot product of this object forward and the interactor position, if he is in front of this object it's more than 0 and if he is behind less than 0
		float LocationProjection = FVector::DotProduct(InteractorLocation, ForwardVector);

		// Check if the interactor is in front of this object and look in his direction
		// Maybe later will use the controller direction more than the pawn
		if (OrientationProjection < -DotForwdAlignSensitivity && LocationProjection > DotFrontAlignSensitivity)
		{
			return true;
		}
	}

	return false;
}

