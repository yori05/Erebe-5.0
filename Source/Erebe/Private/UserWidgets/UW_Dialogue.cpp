// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidgets/UW_Dialogue.h"
#include "Components/DialogueComponent/DialogueComponent_Basic.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

/**-----------------	Constructor Part		-----------------*/

UUW_Dialogue::UUW_Dialogue(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	bSearchForDialogueComp = true;
}

/**-----------------	User Widget Function Part		-----------------*/

bool UUW_Dialogue::Initialize()
{
	if (Super::Initialize())
	{
		if (bSearchForDialogueComp)
		{
			SearchForDialogueComps();
		}

		return true;
	}

	return false;
}

void UUW_Dialogue::BeginDestroy()
{
	Super::BeginDestroy();

	for (auto& i : DialogueComponents)
	{
		UnbindDialogueComponent(i);
	}
}

/**-----------------	Management Part		-----------------*/

void UUW_Dialogue::SearchForDialogueComps()
{
	TArray<TObjectPtr<AActor>> DialogueActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Dialogue"), DialogueActors);
	TObjectPtr<UDialogueComponent_Basic> DialogueComp = nullptr;

	for (auto& i : DialogueActors)
	{
		if (IsValid(i))
		{
			DialogueComp = Cast<UDialogueComponent_Basic>(i->GetComponentByClass(UDialogueComponent_Basic::StaticClass()));

			if (IsValid(DialogueComp))
			{
				AddDialogueComponent(DialogueComp);
			}
		}

		DialogueComp = nullptr;
	}
}

bool UUW_Dialogue::AddDialogueComponent(UDialogueComponent_Basic* _DialogueComp)
{
	if (!IsValid(_DialogueComp))
	{
		return false;
	}

	if (DialogueComponents.Contains(_DialogueComp))
	{
		return false;
	}

	DialogueComponents.Add(_DialogueComp);
	BindDialogueComponent(_DialogueComp);

	return true;
}

bool UUW_Dialogue::RemoveDialogueComponent(UDialogueComponent_Basic* _DialogueComp)
{
	if (!IsValid(_DialogueComp))
	{
		return false;
	}

	if (!DialogueComponents.Contains(_DialogueComp))
	{
		return false;
	}

	DialogueComponents.Remove(_DialogueComp);
	UnbindDialogueComponent(_DialogueComp);

	return true;
}

void UUW_Dialogue::BindDialogueComponent(UDialogueComponent_Basic* _DialogueComp)
{
	if (IsValid(_DialogueComp))
	{
		_DialogueComp->OnDialogueEngagedCall.AddUniqueDynamic(this, &UUW_Dialogue::ReceiveDisplayCall);
		_DialogueComp->OnDialogueChangeCall.AddUniqueDynamic(this, &UUW_Dialogue::ReceiveDialogueChangeCall);
		_DialogueComp->OnDialogueDisengagedCall.AddUniqueDynamic(this, &UUW_Dialogue::ReceiveHideCall);
	}
}

void UUW_Dialogue::UnbindDialogueComponent(UDialogueComponent_Basic* _DialogueComp)
{
	if (IsValid(_DialogueComp))
	{
		if (_DialogueComp->OnDialogueEngagedCall.IsAlreadyBound(this, &UUW_Dialogue::ReceiveDisplayCall))
		{
			_DialogueComp->OnDialogueEngagedCall.RemoveDynamic(this, &UUW_Dialogue::ReceiveDisplayCall);
		}
		if (_DialogueComp->OnDialogueChangeCall.IsAlreadyBound(this, &UUW_Dialogue::ReceiveDialogueChangeCall))
		{
			_DialogueComp->OnDialogueChangeCall.RemoveDynamic(this, &UUW_Dialogue::ReceiveDialogueChangeCall);
		}
		if (_DialogueComp->OnDialogueDisengagedCall.IsAlreadyBound(this, &UUW_Dialogue::ReceiveHideCall))
		{
			_DialogueComp->OnDialogueDisengagedCall.RemoveDynamic(this, &UUW_Dialogue::ReceiveHideCall);
		}
	}
}

/**-----------------	UW Dialogue Part		-----------------*/

void UUW_Dialogue::ReceiveDisplayCall(FDialogueStruct _DialogueData)
{
	_ReceiveDisplayCall(_DialogueData);
	OnReceiveDisplayCall.Broadcast(_DialogueData);
}

void UUW_Dialogue::ReceiveDialogueChangeCall(FDialogueStruct _DialogueData)
{
	_ReceiveDialogueChangeCall(_DialogueData);
	OnReceiveDialogueChangeCall.Broadcast(_DialogueData);
}

void UUW_Dialogue::ReceiveHideCall(FDialogueStruct _DialogueData)
{
	_ReceiveHideCall(_DialogueData);
	OnReceiveHideCall.Broadcast(_DialogueData);
}
