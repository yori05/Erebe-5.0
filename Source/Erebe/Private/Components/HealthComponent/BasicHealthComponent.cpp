// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HealthComponent/BasicHealthComponent.h"

// Sets default values for this component's properties
UBasicHealthComponent::UBasicHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ComponentTags.Add(FName(TEXT("Health")));

	// ...
}


// Called when the game starts
void UBasicHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// The default function to change the components health
void UBasicHealthComponent::ChangeHealth(int32 NewValue)
{
	// Will clamp the NewValue between 0 and HealthMax
	NewValue = (NewValue > HealthMax) ? HealthMax : ((NewValue < 0) ? 0 : NewValue);

	if (NewValue == Health)
		return;

	Health = NewValue;
	OnHealthChange.Broadcast(Health, HealthMax);

	if (Health == HealthMax)
	{
		OnHealthToTheMax.Broadcast();
	}
	else if (Health == 0)
	{
		OnHealthToZero.Broadcast();
	}
}

// Called every frame
void UBasicHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// Call to give damage 
void UBasicHealthComponent::ReceiveDamage(int32 DamageValue)
{
	if (DamageValue <= 0 || Health <= 0)
		return;

	ChangeHealth(Health - DamageValue);

	OnReceiveDamge.Broadcast(DamageValue);
}

// Call to give heal 
void UBasicHealthComponent::ReceiveHeal(int32 HealValue)
{
	if (HealValue <= 0 || Health >= HealthMax)
		return;

	ChangeHealth(Health + HealValue);

	OnReceiveDamge.Broadcast(HealValue);
}

// Call to restore all the health 
void UBasicHealthComponent::RestorFullHealth()
{
	if (Health >= HealthMax)
		return;

	ChangeHealth(HealthMax);

	OnRestoreFullHealth.Broadcast();
}

// Call to lose all the health 
void UBasicHealthComponent::LoseAllHealth()
{
	if (Health <= 0)
		return;

	ChangeHealth(0);

	OnLoseAllHealth.Broadcast();
}

