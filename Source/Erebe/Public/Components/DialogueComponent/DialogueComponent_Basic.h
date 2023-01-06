// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/InteractiveComponent/InteractiveComponent_Basic.h"
#include "DialogueComponent_Basic.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDialogueBasicChange, FText, CurrentText);

UCLASS(ClassGroup = ("Dialogue"), meta = (BlueprintSpawnableComponent), Blueprintable, BlueprintType)
class EREBE_API UDialogueComponent_Basic : public UInteractiveComponent_Basic
{
	GENERATED_BODY()

protected:
	/** Dialogue widget build and used by this object maybe later will only use the owning by the player */
	UPROPERTY(Category = "Dialogue", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UUserWidget* DialogueWidget;

	/** Class of Dialogue widget used by this object need to be set in the editor or in the blueprint class */
	UPROPERTY(Category = "Dialogue", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class UUserWidget> DialogueWidgetClass;

	/** Dialogue table stock all NPC's lines need to be set in the editor or in the blueprint class */
	UPROPERTY(Category = "Dialogue", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UDataTable* DialogueTable;

	/** Saved interactor  */
	UPROPERTY(Category = "Dialogue", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UDialogueComponent_Basic* SavedDialoguer;

	/** Saved name for this NPC dialogues  */
	UPROPERTY(Category = "Dialogue", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<FName> DialoguesName;

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

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FDialogueBasicChange OnDialogueLineChange;

public:
	// Sets default values for this component's properties
	UDialogueComponent_Basic();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:

	UFUNCTION(BlueprintCallable)
		virtual void ChangeVisibiltyWidget(ESlateVisibility Visibility, UDialogueComponent_Basic* OtherDialoguer);

	UFUNCTION(BlueprintCallable)
		virtual FText GetCurrentDialogueLine();

	UFUNCTION(BlueprintCallable)
		virtual void InitializeDialogue(UDialogueComponent_Basic* OtherDialoguer);

	UFUNCTION(BlueprintCallable)
		virtual void IncrementeDialogue(UDialogueComponent_Basic* OtherDialoguer);

	UFUNCTION(BlueprintCallable)
		virtual void UpdateDialogue(UDialogueComponent_Basic* OtherDialoguer);

	UFUNCTION(BlueprintCallable)
		virtual void StopDialogue();

public:
	/** Function called by the InteractiveInterface will be useful in case a child class on this one want to change the order for the interaction, or the interaction effect */

	/** Read the text displayed by this object widget / make it invisible if the text is already visible */
	UFUNCTION(BlueprintCallable)
		virtual void StartConversation_Interaction(UDialogueComponent_Basic* OtherDialoguer);
	/** Block the reading if the character is behind the object or not in the front */
	UFUNCTION(BlueprintCallable)
		virtual bool CanStartConversation_Interaction(UDialogueComponent_Basic* OtherDialoguer);
	/** Return a text with a 'Read' in content maybe will take localization in the future*/
	UFUNCTION(BlueprintCallable)
		virtual FName GetStartConversationActionName_Interaction();
	/** Stop the text display and clear the SavedDialoguer */
	UFUNCTION(BlueprintCallable)
		virtual void StopConversation_Interaction(UDialogueComponent_Basic* OtherDialoguer);
	/** Try to start a conversation */
	UFUNCTION(BlueprintCallable)
		virtual bool TryConversation_Interaction(UDialogueComponent_Basic* OtherDialoguer);

	UFUNCTION(BlueprintCallable)
		bool IsInDialogue() { return SavedDialoguer != nullptr; }
public:

	FORCEINLINE UDialogueComponent_Basic* GetSavedDialoger() { return SavedDialoguer; };
	FORCEINLINE class UUserWidget* GetDialogueWidget() { return DialogueWidget; };

};
