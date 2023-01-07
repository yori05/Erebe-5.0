// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidgets/UW_Signpost.h"
#include "Components/SignpostComponent/IC_Signpost_Basic.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

/**-----------------	Constructor Part		-----------------*/

UUW_Signpost::UUW_Signpost(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{

}

/**-----------------	User Widget Function Part		-----------------*/

bool UUW_Signpost::Initialize()
{
	if (Super::Initialize())
	{
		TArray<TObjectPtr<AActor>> SignpostActors;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Signpost"), SignpostActors);
		TObjectPtr<UIC_Signpost_Basic> SignpostComp = nullptr;

		for (auto& i : SignpostActors)
		{
			if (IsValid(i))
			{
				SignpostComp = Cast<UIC_Signpost_Basic>(i->GetComponentByClass(UIC_Signpost_Basic::StaticClass()));

				if (IsValid(SignpostComp))
				{
					AddSignpostComponent(SignpostComp);
				}
			}

			SignpostComp = nullptr;
		}

		return true;
	}

	return false;
}

void UUW_Signpost::BeginDestroy()
{
	Super::BeginDestroy();

	for (auto& i : SignpostComponents)
	{
		UnbindSignpostComponent(i);
	}
}

/**-----------------	Management Part		-----------------*/

bool UUW_Signpost::AddSignpostComponent(UIC_Signpost_Basic* _SignpostComp)
{
	if (!IsValid(_SignpostComp))
	{
		return false;
	}

	if (SignpostComponents.Contains(_SignpostComp))
	{
		return false;
	}

	SignpostComponents.Add(_SignpostComp);
	BindSignpostComponent(_SignpostComp);

	return true;
}

bool UUW_Signpost::RemoveSignpostComponent(UIC_Signpost_Basic* _SignpostComp)
{
	if (!IsValid(_SignpostComp))
	{
		return false;
	}

	if (!SignpostComponents.Contains(_SignpostComp))
	{
		return false;
	}

	SignpostComponents.Remove(_SignpostComp);
	UnbindSignpostComponent(_SignpostComp);

	return true;
}

void UUW_Signpost::BindSignpostComponent(UIC_Signpost_Basic* _SignpostComp)
{
	if (IsValid(_SignpostComp))
	{
		_SignpostComp->OnSignpostDisplayCall.AddUniqueDynamic(this, &UUW_Signpost::ReceiveDisplayCall);
		_SignpostComp->OnSignpostHideCall.AddUniqueDynamic(this, &UUW_Signpost::ReceiveHideCall);
	}
}

void UUW_Signpost::UnbindSignpostComponent(UIC_Signpost_Basic* _SignpostComp)
{
	if (IsValid(_SignpostComp))
	{
		if (_SignpostComp->OnSignpostDisplayCall.IsAlreadyBound(this, &UUW_Signpost::ReceiveDisplayCall))
		{
			_SignpostComp->OnSignpostDisplayCall.RemoveDynamic(this, &UUW_Signpost::ReceiveDisplayCall);
		}
		if (_SignpostComp->OnSignpostDisplayCall.IsAlreadyBound(this, &UUW_Signpost::ReceiveDisplayCall))
		{
			_SignpostComp->OnSignpostHideCall.RemoveDynamic(this, &UUW_Signpost::ReceiveHideCall);
		}
	}
}

/**-----------------	UW Signpost Part		-----------------*/

void UUW_Signpost::ReceiveDisplayCall(FSignpostStructure _SignpostData)
{
	_ReceiveDisplayCall(_SignpostData);
	OnReceiveDisplayCall.Broadcast(_SignpostData);
}

void UUW_Signpost::ReceiveHideCall(FSignpostStructure _SignpostData)
{
	_ReceiveHideCall(_SignpostData);
	OnReceiveHideCall.Broadcast(_SignpostData);
}
