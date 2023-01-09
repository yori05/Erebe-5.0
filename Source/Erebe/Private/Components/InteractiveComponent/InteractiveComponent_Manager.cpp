// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InteractiveComponent/InteractiveComponent_Manager.h"
#include "Components/InteractiveComponent/InteractiveComponent_Basic.h"
#include "Components/PrimitiveComponent.h"

// Sets default values for this component's properties
UInteractiveComponent_Manager::UInteractiveComponent_Manager(const FObjectInitializer& ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInteractiveComponent_Manager::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<AActor>(GetOwner());

	if (bSearchInteractiveComponents)
	{
		SearchInteractionComponents();
	}
	if (bSearchInteractiveCollider)
	{
		SearchInteractionPrimitive();
	}
	// ...
	
}


// Called on the post Load
void UInteractiveComponent_Manager::PostLoad()
{
	Super::PostLoad();

	// ...

}

void UInteractiveComponent_Manager::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);


	BreakLinkToInteractionPrimitive();
}

// Called every frame
void UInteractiveComponent_Manager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UInteractiveComponent_Manager::RequestSaveInteractiveComponent(UInteractiveComponent_Basic* NewInteractiveComp)
{
	if (!IsValid(NewInteractiveComp) || NewInteractiveComp->GetInteractionType() == EInteractionTypes::INTERACTTYPES_Undefinned)
	{
		return false;
	}
	
	if (InteractivesActions.Contains(NewInteractiveComp->GetInteractionID()))
	{
		return false;
	}

	InteractivesActions.Add(NewInteractiveComp->GetInteractionID(), NewInteractiveComp);
	
	if (NewInteractiveComp->GetIsMainInteraction() || (bSetFirstAsMain && !IsValid(MainInteractiveAction)))
	{
		if (!SetNewMainInteraction(NewInteractiveComp))
		{
			NewInteractiveComp->SetAsMainInteraction(false);
		}
	}

	return true;
}

bool UInteractiveComponent_Manager::SetNewMainInteraction(UInteractiveComponent_Basic* NewMainInteraction)
{
	if (IsValid(NewMainInteraction))
	{
		ResetMainInteraction();

		MainCatergorieAction = NewMainInteraction->GetInteractionType();
		MainCustomCatergorieAction = NewMainInteraction->GetCustomInteractionType();
		MainInteractiveAction = NewMainInteraction;
		
		return true;
	}

	return false;
}

void UInteractiveComponent_Manager::ResetMainInteraction(bool bResetInteractiveComp)
{
	if (bResetInteractiveComp && IsValid(MainInteractiveAction))
	{
		MainInteractiveAction->SetAsMainInteraction(false);
	}

	MainInteractiveAction = nullptr;
	MainCatergorieAction = EInteractionTypes::INTERACTTYPES_Undefinned;
	MainCustomCatergorieAction = 0;
}

UInteractiveComponent_Basic* UInteractiveComponent_Manager::GetInteractionByType(EInteractionTypes _InteractionType, uint8 _CustomInteractionType)
{
	uint8 InteractionID = (uint8)_InteractionType + (_InteractionType == EInteractionTypes::INTERACTTYPES_CustomInteraction)
													? _CustomInteractionType 
													: 0;

	return GetInteractionByID(InteractionID);
}

UInteractiveComponent_Basic* UInteractiveComponent_Manager::GetInteractionByID(uint8 _InteractionID)
{
	if (InteractivesActions.Contains(_InteractionID))
	{
		return InteractivesActions.Find(_InteractionID)->Get();
	}

	return nullptr;
}

void UInteractiveComponent_Manager::ReceiveInteractionPrimitive(UPrimitiveComponent* _NewPrimitiveComponent)
{
	if (IsValid(_NewPrimitiveComponent) && _NewPrimitiveComponent != InteractionColision)
	{
		if (IsValid(InteractionColision))
		{
			BreakLinkToInteractionPrimitive();
		}

		InteractionColision = _NewPrimitiveComponent;
		LinkToInteractionPrimitive();
	}
}

void UInteractiveComponent_Manager::SearchInteractionComponents()
{
	if (IsValid(Owner))
	{
		TObjectPtr<UInteractiveComponent_Basic> ICBase = nullptr;
		TArray<UActorComponent*> ICArray;
		Owner->GetComponents(UInteractiveComponent_Basic::StaticClass(), ICArray);

		for (auto& i : ICArray)
		{
			ICBase = Cast<UInteractiveComponent_Basic>(i);

			if (IsValid(ICBase))
			{
				RequestSaveInteractiveComponent(ICBase);
			}

			ICBase = nullptr;
		}
	}
}

void UInteractiveComponent_Manager::SearchInteractionPrimitive()
{
	if (IsValid(Owner))
	{
		TObjectPtr<UPrimitiveComponent> Prim = nullptr;

		for (auto& i : Owner->GetComponentsByTag(UPrimitiveComponent::StaticClass(), TEXT("Interaction")))
		{
			Prim = Cast<UPrimitiveComponent>(i);

			if (IsValid(Prim))
			{
				InteractionColision = Prim;
				LinkToInteractionPrimitive();
				return;
			}

			Prim = nullptr;
		}
	}
}


void UInteractiveComponent_Manager::LinkToInteractionPrimitive()
{
	if (IsValid(InteractionColision))
	{
		InteractionColision->OnComponentBeginOverlap.AddUniqueDynamic(this, &UInteractiveComponent_Manager::OnInteractionPrimitiveBeginOverlap);
		InteractionColision->OnComponentEndOverlap.AddUniqueDynamic(this, &UInteractiveComponent_Manager::OnInteractionPrimitiveEndOverlap);
	}
}

void UInteractiveComponent_Manager::BreakLinkToInteractionPrimitive()
{
	if (IsValid(InteractionColision))
	{
		if (InteractionColision->OnComponentBeginOverlap.IsAlreadyBound(this, &UInteractiveComponent_Manager::OnInteractionPrimitiveBeginOverlap))
		{
			InteractionColision->OnComponentBeginOverlap.RemoveDynamic(this, &UInteractiveComponent_Manager::OnInteractionPrimitiveBeginOverlap);
		}
		if (InteractionColision->OnComponentEndOverlap.IsAlreadyBound(this, &UInteractiveComponent_Manager::OnInteractionPrimitiveEndOverlap))
		{
			InteractionColision->OnComponentEndOverlap.RemoveDynamic(this, &UInteractiveComponent_Manager::OnInteractionPrimitiveEndOverlap);
		}
	}
}

void UInteractiveComponent_Manager::OnInteractionPrimitiveBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor) && IsValid(OtherComp) && OtherComp->ComponentHasTag(TEXT("Interaction")))
	{
		if (Cast<APlayerController>(OtherActor->GetOwner()))
		{
			CallInteractionPlayerBeginOverlap(OtherActor);
		}

		if (OtherActor != Owner)
		{
			CallInteractionActorBeginOverlap(OtherActor);
		}
	}
}

void UInteractiveComponent_Manager::OnInteractionPrimitiveEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IsValid(OtherActor) && IsValid(OtherComp) && OtherComp->ComponentHasTag(TEXT("Interaction")))
	{
		if (Cast<APlayerController>(OtherActor->GetOwner()))
		{
			CallInteractionPlayerEndOverlap(OtherActor);
		}

		if (OtherActor != Owner)
		{
			CallInteractionActorEndOverlap(OtherActor);
		}
	}
}


void UInteractiveComponent_Manager::CallInteractionPlayerBeginOverlap(AActor* PlayerActor)
{
	InteractionPlayerBeginOverlap(PlayerActor);
	ReceiveInteractionPlayerBeginOverlap(PlayerActor);
	OnInteractionPlayerBeginOverlap.Broadcast(PlayerActor);
}

void UInteractiveComponent_Manager::InteractionPlayerBeginOverlap(AActor* PlayerActor)
{
	if (bPlayerBegOverlLaunchMain)
	{
		if (IsValid(MainInteractiveAction))
		{
			MainInteractiveAction->CallInteraction(PlayerActor);
		}
	}
}

void UInteractiveComponent_Manager::CallInteractionActorBeginOverlap(AActor* Actor)
{
	InteractionActorBeginOverlap(Actor);
	ReceiveInteractionActorBeginOverlap(Actor);
	OnInteractionActorBeginOverlap.Broadcast(Actor);
}

void UInteractiveComponent_Manager::InteractionActorBeginOverlap(AActor* Actor)
{
	if (bActorBegOverlLaunchMain)
	{
		if (IsValid(MainInteractiveAction))
		{
			MainInteractiveAction->CallInteraction(Actor);
		}
	}
}

void UInteractiveComponent_Manager::CallInteractionPlayerEndOverlap(AActor* PlayerActor)
{
	InteractionPlayerEndOverlap(PlayerActor);
	ReceiveInteractionPlayerEndOverlap(PlayerActor);
	OnInteractionPlayerEndOverlap.Broadcast(PlayerActor);
}

void UInteractiveComponent_Manager::InteractionPlayerEndOverlap(AActor* PlayerActor)
{
	if (bPlayerEndOverlStopMain)
	{
		if (IsValid(MainInteractiveAction) && MainInteractiveAction->GetRequestor() == PlayerActor)
		{
			MainInteractiveAction->StopInteraction();
		}
	}
}

void UInteractiveComponent_Manager::CallInteractionActorEndOverlap(AActor* Actor)
{
	InteractionActorEndOverlap(Actor);
	ReceiveInteractionActorEndOverlap(Actor);
	OnInteractionActorEndOverlap.Broadcast(Actor);
}

void UInteractiveComponent_Manager::InteractionActorEndOverlap(AActor* Actor)
{
	if (bActorEndOverlStopMain)
	{
		if (IsValid(MainInteractiveAction) && MainInteractiveAction->GetRequestor() == Actor)
		{
			MainInteractiveAction->StopInteraction();
		}
	}
}
