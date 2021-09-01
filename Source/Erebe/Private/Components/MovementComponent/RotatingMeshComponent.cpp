// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/MovementComponent/RotatingMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Pawn.h"
#include "Curves/CurveFloat.h"


// Sets default values for this component's properties
URotatingMeshComponent::URotatingMeshComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ComponentTags.Add(FName(TEXT("RotatingMesh")));

	// ...
}


// Called when the game starts
void URotatingMeshComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
		// Check if we have a curve
	if (RotationTimelineFloatCurve)
	{
		/** Float track signature to handle the update of the curve */
		FOnTimelineFloat RotationCurveTrack;

		//Bind our float track to our TimelineMoveDoorTick function
		RotationCurveTrack.BindDynamic(this, &URotatingMeshComponent::RotateMeshTick);

		// If we have a float curve, bind it's graph to our update function
		RotationTimeline.AddInterpFloat(RotationTimelineFloatCurve, RotationCurveTrack);
	}

}


// Called every frame
void URotatingMeshComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	RotationTimeline.TickTimeline(DeltaTime);
	// ...
}

void URotatingMeshComponent::RotateMeshTick(float Output)
{
	// We take the output of the curve that varies to 0 to 1 multiply it by the max angle that can have the door and the opening sens.
	// This value will always been at 1 if the boolean @CanGetOppositeAngle is set as false.
	// Maybe in next version will add a check to hold the door if she collide with something when opening or closing it.

	if (RotatingMesh != nullptr)
	{
		FRotator NewRotation = FRotator(0.0f, Output * OpenAngle * AngleSens, 0.f);
		RotatingMesh->SetRelativeRotation(NewRotation);
	}
}

void URotatingMeshComponent::RotateMesh_Interaction(APawn* Interactor)
{
	// Don't possess check to close the door cause we can interact with the moving part when she isn't aligned with static part
	if (bIsOpen)
	{
		// We don't check anything to close the door 
		bIsOpen = false;
		RotationTimeline.Reverse();
	}
	else
	{
		// Check actor direction from the door and see he how is align with her.
		float InteractorProjection = FVector::DotProduct(Interactor->GetActorForwardVector(), GetOwner()->GetActorForwardVector());

		// If the actor isn't aligned enough quit the function.
		if (DotSensitivity * DotSensitivity > InteractorProjection * InteractorProjection)
		{
			return;
		}

		//Check if the door can be opened in the opposite direction
		if (bCanGetOppositeAngle)
		{
			// Using the actor projection to check if the actor is behind the door and he push it.
			if (InteractorProjection > 0)
			{
				AngleSens = 1.f;
			}
			else
			{
				AngleSens = -1.f;
			}
		}

		//Launch the Timeline component to open the door
		bIsOpen = true;
		RotationTimeline.Play();
	}
}

bool URotatingMeshComponent::CanRotateMesh_Interaction(APawn* Interactor) const
{
	// Check if the door is opened if she is open can close the door any way
	if (!bIsOpen && RotatingMesh != nullptr)
	{
		//If the door is close check if the player is aligned enough with the door.
		float InteractorProjection = FVector::DotProduct(Interactor->GetActorForwardVector(), GetOwner()->GetActorForwardVector());

		if (DotSensitivity * DotSensitivity > InteractorProjection * InteractorProjection)
		{
			return false;
		}
	}

	return true;
}

FName URotatingMeshComponent::GetRotateMeshActionName_Interaction()const
{
	// For now return a brute value, maybe latter will return one depend of the localization
	return FName(TEXT("Open"));
}
