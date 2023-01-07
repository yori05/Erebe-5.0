// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CombatComponents/CombatComponent_Basic.h"
#include "Character/Character_Basic.h"

/**-----------------	Constructor Part		-----------------*/
// Sets default values for this component's properties
UCombatComponent_Basic::UCombatComponent_Basic(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

/**-----------------	Inherit Function Part		-----------------*/
// Called when the game starts
void UCombatComponent_Basic::BeginPlay()
{
	Super::BeginPlay();


	// ...
	
}

// Called on post Load
void UCombatComponent_Basic::PostLoad()
{
	Super::PostLoad();

	CharacterOwner = Cast<ACharacter_Basic>(GetOwner());
}

// Called every frame
void UCombatComponent_Basic::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCombatComponent_Basic::Attack()
{
	if (IsValid(CharacterOwner))
	{
		bool Test(CharacterOwner->IsInFirstPerson());

		if (!Test)
		{
			CharacterOwner->CallAllignYawWithCamera();
		}
	}
}

