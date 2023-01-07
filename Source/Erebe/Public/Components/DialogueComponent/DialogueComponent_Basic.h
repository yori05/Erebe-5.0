// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structures/Dialogues/DialogueStruct.h"

#include "DialogueComponent_Basic.generated.h"

class UDataTable;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDialogueCallSignature, FDialogueStruct, DialogueData);

/**
 * Dialogue Component Basic
 * Part of the Dialogue system
 * Child of UActorComponent
 *
 * Use a Widget to display a text for the player
 * The data used by this component is load from the data table set in @DialogueTable
 *
 */
UCLASS(ClassGroup = ("Dialogue"), meta = (BlueprintSpawnableComponent), Blueprintable, BlueprintType)
class EREBE_API UDialogueComponent_Basic : public UActorComponent
{
	GENERATED_BODY()

		/**-----------------	Variable Part		-----------------*/
protected:

	/** Dialogue table stock all NPC's lines need to be set in the editor or in the blueprint class */
	UPROPERTY(Category = "Dialogue", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UDataTable> DialogueTable;

	/** Saved name for this Signpost  */
	UPROPERTY(Category = "Dialogue", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FDialogueStruct CurrentDialogue;

	/** Saved name for this NPC dialogues  */
	UPROPERTY(Category = "Dialogue", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<FDialogueStruct> DialoguesSaved;

	UPROPERTY(Category = "Dialogue", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TMap<int32, int32> NbLineForConversation;

	/** Saved line for the NPC  */
	UPROPERTY(Category = "Dialogue", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FText CurrentText;

	/** ID of this NPC */
	UPROPERTY(Category = "Dialogue", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32 NPC_ID;

	/** Conversation ID of this NPC */
	UPROPERTY(Category = "Dialogue", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32 Conversation_ID;

	/** Current line of the current conversation */
	UPROPERTY(Category = "Dialogue", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32 CurrentLine;

	/** Should this component try to load data table in begin */
	UPROPERTY(Category = "Dialogue", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool bLoadDataInBegin = true;

	/** Did this component load his dialogue from his table */
	UPROPERTY(Category = "Dialogue", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool bLoaded = false;

	/** Should this component try to link himself to the ui */
	UPROPERTY(Category = "Dialogue", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool bSearchForUI = false;

	/** Did this component is engaged in a discussion */
	UPROPERTY(Category = "Dialogue", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool bIsEngaged = false;


	/**-----------------	Callback Part		-----------------*/
public:

	/** Called when the signpost line change, for now it's used in the begin interaction */
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FDialogueCallSignature OnDialogueChangeCall;

	/** Called when a dialogue have the special event handle set as true is call */
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FDialogueCallSignature OnDialogueSpecialEventCall;

	/** Called when a dialogue have the special event handle set as true is uncall */
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FDialogueCallSignature OnDialogueSpecialEventUncall;

	/** Called when the signpost call for being display, for now it's used in the begin interaction */
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FDialogueCallSignature OnDialogueEngagedCall;

	/** Called when the signpost call for being hide, for now it's used in the end / abort interaction */
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FDialogueCallSignature OnDialogueDisengagedCall;


	/**-----------------	Constructor Part		-----------------*/
public:

	// Sets default values for this component's properties
	UDialogueComponent_Basic();

	/**-----------------	Actor Component Function Part		-----------------*/
protected:

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called when the game starts
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	/**-----------------	Data Function Part		-----------------*/
public:

	UFUNCTION(BlueprintCallable)
	virtual bool LoadDataTable();

	UFUNCTION(BlueprintCallable)
		virtual void ChangeDataTable(UDataTable* NewDataTable);

	/**-----------------	Dialogue Function Part		-----------------*/
protected:

	/** Update saved struct using the Dialogue Table, DialoguesNames and Current Line  */
	UFUNCTION(BlueprintCallable)
		virtual void UpdateDialogue();

public:

	UFUNCTION(BlueprintCallable)
		virtual bool ChangeDialogue(int32 NewConversationID, int32 NewLineNumber);

	UFUNCTION(BlueprintCallable)
		virtual bool ChangeLine(int32 NewLineNumber);

	UFUNCTION(BlueprintCallable)
		virtual bool NextLine();

	UFUNCTION(BlueprintCallable)
		virtual bool ChangeConversation(int32 NewConversationID);

	UFUNCTION(BlueprintCallable)
		virtual bool CanChangeDialogue(int32 NewConversationID, int32 NewLineNumber);

	UFUNCTION(BlueprintCallable)
		virtual bool CanChangeLine(int32 NewLineNumber);

	UFUNCTION(BlueprintCallable)
		virtual bool CanNextLine();

	UFUNCTION(BlueprintCallable)
		virtual bool CanChangeConversation(int32 NewConversationID);

	/**-----------------	Engage Function Part		-----------------*/
public:

	UFUNCTION(BlueprintCallable)
		virtual void EngageDialogue();

	UFUNCTION(BlueprintCallable)
		virtual void DisengageDialogue();

	FORCEINLINE bool IsEngaged() { return bIsEngaged; }


	/**-----------------	Widget Function Part		-----------------*/
protected:

	/**
	* Will try to link himself to the ui
	* Need to have @bSearchForUI as true
	* The gameplay should'nt try to call the ui so it's better if set as false
	*/
	UFUNCTION(BlueprintCallable)
		virtual void RequestLinkToWidget();

	/**
	* Will try to break link himself to the ui
	* Need to have @bSearchForUI as true
	* The gameplay should'nt try to call the ui so it's better if set as false
	*/
	UFUNCTION(BlueprintCallable)
		virtual void RemoveLinkToWidget();

	/**-----------------	Accessor Part		-----------------*/
public:

	/** Return the signpost struct */
	FORCEINLINE FDialogueStruct GetCurrentDialogue() const { return CurrentDialogue; }

};
