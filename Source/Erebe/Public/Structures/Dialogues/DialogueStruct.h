// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "DialogueStruct.generated.h"

/**
 *	Struct to organize the Dialogue Data 
 */
USTRUCT(BlueprintType)
struct EREBE_API FDialogueStruct : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 NPC_ID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Conversation_ID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Line_ID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText Dialogue;
};
