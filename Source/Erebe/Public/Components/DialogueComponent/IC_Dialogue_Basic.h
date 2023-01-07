// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/InteractiveComponent/InteractiveComponent_Basic.h"
#include "Structures/Dialogues/DialogueStruct.h"

#include "IC_Dialogue_Basic.generated.h"


class UDialogueComponent_Basic;

/**
 * Interactive Component Dialogue Basic
 * Link of the interactive system and dialogue system
 * Child of UInteractiveComponent_Basic
 *
 * Set the EInteractionType as INTERACTTYPES_Dialogue
 * By default value are set so when interact is called it will update the interaction if ongoing
 * Need to send a actor as requestor when you want to use this interactive component
 * Check UInteractiveComponent_Basic if you want more info.
 * 
 */
UCLASS(ClassGroup = ("Dialogue"), meta = (BlueprintSpawnableComponent), Blueprintable, BlueprintType)
class EREBE_API UIC_Dialogue_Basic : public UInteractiveComponent_Basic
{
	GENERATED_BODY()

		/**-----------------	Variable Part		-----------------*/
protected:
	
	UPROPERTY(Category = "Dialogue", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDialogueComponent_Basic> SavedDialogueComponent;

	/** Should this component search for the dialogue component on begin play */
	UPROPERTY(Category = "Dialogue", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool bSearchDialogueComp = true;

	/**-----------------	Callback Part		-----------------*/
public:

	/**-----------------	Constructor Part		-----------------*/
public:

	// Sets default values for this component's properties
	UIC_Dialogue_Basic();

	/**-----------------	Actor Component Function Part		-----------------*/
protected:

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called when the game starts
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	/**-----------------	Interactive Function Part		-----------------*/
public:

	/**
	* Check if it's possible to start a interaction
	*/
	virtual	bool CanInteract(AActor* _Requestor = nullptr) const override;

protected:

	/** Begin interact action used to have callback launch after the gameplay implementation even when override */
	virtual void BeginInteract() override;

	/** Update interact action used to have callback launch after the gameplay implementation even when override */
	virtual void UpdateInteract(float DeltaTime) override;

	/** End interact action used to have callback launch after the gameplay implementation even when override */
	virtual void EndInteract() override;

	/** Abort interact action used to have callback launch after the gameplay implementation even when override */
	virtual void AbortInteract() override;

	/**-----------------	Dialogue Function Part		-----------------*/
public:

	/** Receive a dialogue component will link to it */
	UFUNCTION(BlueprintCallable)
	virtual void ReceiveDialogueComponent(UDialogueComponent_Basic* _NewDialogueComp);

protected:

	/** Search for the dialogue component on the actor owner */
	UFUNCTION(BlueprintCallable)
		virtual void SearchForDialogueComponent();

	/** Link with the @SavedDialogueComponent */
	UFUNCTION(BlueprintCallable)
	virtual void LinkDialogueComponent();

	/** Break link with the @SavedDialogueComponent */
	UFUNCTION(BlueprintCallable)
	virtual void BreakLinkDialogueComponent();

	/** Action take when linked dialogue component disengage */
	UFUNCTION(BlueprintCallable)
		virtual void WhenDialogueDisengage(FDialogueStruct DialogueStruct);

	/**-----------------	Accessor Part		-----------------*/
public:

	/** Return the signpost struct */
	FORCEINLINE UDialogueComponent_Basic* GetLinkedDialogueComp() const { return SavedDialogueComponent; }

};
