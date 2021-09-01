// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BasicDialogueComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDialogueStateSignature, class UBasicDialogueComponent*, DialogueComponent, class UBasicDialogueComponent*, OtherComponent);

UCLASS( ClassGroup=(Custom), Blueprintable, BlueprintType, meta=(BlueprintSpawnableComponent) )
class EREBE_API UBasicDialogueComponent : public UActorComponent
{
	GENERATED_BODY()

protected:

	/** Dialogue table stock all NPC's lines need to be set in the editor or in the blueprint class */
	UPROPERTY(Category = "Interaction", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UDataTable* DialogueTable;

	/** Saved interactor  */
	UPROPERTY(Category = "Interaction", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		APawn* SavedInteractor;

	/** Saved name for this NPC dialogues  */
	UPROPERTY(Category = "Interaction", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<FName> DialoguesName;

	/** Saved line for the NPC  */
	UPROPERTY(Category = "Interaction", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FText CurrentText;

	/** ID of this NPC */
	UPROPERTY(Category = "Interaction", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32 NPC_ID;

	/** Conversation ID of this NPC */
	UPROPERTY(Category = "Interaction", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32 Conversation_ID;

	/** Current line of the current conversation */
	UPROPERTY(Category = "Interaction", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32 CurrentLine;

public :
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FDialogueStateSignature OnDialogueStart;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FDialogueStateSignature OnDialogueEnd;

public:	
	// Sets default values for this component's properties
	UBasicDialogueComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
