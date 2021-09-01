// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "SignpostStructure.generated.h"


/**
 * 
 */
USTRUCT(BlueprintType)
struct EREBE_API FSignpostStructure : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 Signpost_ID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FText Signpost_Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FText Signpost_Subtext;

};
