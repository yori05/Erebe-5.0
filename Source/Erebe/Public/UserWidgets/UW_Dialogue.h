// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Structures/Dialogues/DialogueStruct.h"
#include "Widgets/SWidget.h"
#include "Components/Widget.h"

#include "UW_Dialogue.generated.h"

class UDialogueComponent_Basic;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWidgetDialogueEventSignature, FDialogueStruct, DialogueData);

/**
 *	User widget Dialogue
 *
 *	Base for a UW_Dialogue blueprint
 *	Used to save IC_Dialogue_Basic components and bind event on it
 *	This class is not build to manage action on the widget, just to add and remove link with Dialogue Component
 *	Action for now should stay on blueprint
 */
UCLASS(Abstract, editinlinenew, BlueprintType, Blueprintable, meta = (DontUseGenericSpawnObject = "True", DisableNativeTick))
class EREBE_API UUW_Dialogue : public UUserWidget
{
	GENERATED_BODY()

		friend class SObjectWidget;

	/**-----------------	Variable Part		-----------------*/
protected:

	/** Array of Dialogue Component */
	UPROPERTY(Category = "Management", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<TObjectPtr<UDialogueComponent_Basic>> DialogueComponents;

	/** Array of Dialogue Component */
	UPROPERTY(Category = "Management", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool bSearchForDialogueComp;

	/**-----------------	Callback Part		-----------------*/
public:

	// Called when a signpost link request a display call
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FWidgetDialogueEventSignature OnReceiveDisplayCall;

	// Called when a signpost link request a display call
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FWidgetDialogueEventSignature OnReceiveDialogueChangeCall;

	// Called when a signpost link request a hide call
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FWidgetDialogueEventSignature OnReceiveHideCall;

	/**-----------------	Constructor Part		-----------------*/
public:
	UUW_Dialogue(const FObjectInitializer& ObjectInitializer);

	/**-----------------	User Widget Function Part		-----------------*/
public:

	virtual bool Initialize() override;

	/**
	* Called when a widget is no longer referenced causing the slate resource to destroyed.  Just like
	* Construct this event can be called multiple times.
	*/
	virtual void BeginDestroy() override;

	/**-----------------	Management Part		-----------------*/
public:

	/** Search for dialogue component on the scene */
	UFUNCTION(BlueprintCallable, Category = "Management")
		virtual void SearchForDialogueComps();

		/** Add a Dialogue component to the array and bind event to this component's event */
	UFUNCTION(BlueprintCallable, Category = "Management")
		virtual bool AddDialogueComponent(UDialogueComponent_Basic* _DialogueComp);

	/** Remove a Dialogue component to the array and unbind event to this component's event */
	UFUNCTION(BlueprintCallable, Category = "Management")
		virtual bool RemoveDialogueComponent(UDialogueComponent_Basic* _DialogueComp);

protected:

	/** Unbind event to a dialogue component's event */
	UFUNCTION(BlueprintCallable, Category = "Management")
		virtual void BindDialogueComponent(UDialogueComponent_Basic* _DialogueComp);

	/** Unbind event to a dialogue component's event */
	UFUNCTION(BlueprintCallable, Category = "Management")
		virtual void UnbindDialogueComponent(UDialogueComponent_Basic* _DialogueComp);


	/**-----------------	UW Dialogue Part		-----------------*/

		/** Action when receive display call */
	UFUNCTION()
		virtual void ReceiveDisplayCall(FDialogueStruct _DialogueData);

	/** Blueprint function for the ReceiveDisplayCall */
	UFUNCTION(BlueprintImplementableEvent, Category = "Dialogue", meta = (DisplayName = "ReceiveDisplayCall"))
		void _ReceiveDisplayCall(FDialogueStruct _DialogueData);

	/** Action when receive dialogue change call */
	UFUNCTION()
		virtual void ReceiveDialogueChangeCall(FDialogueStruct _DialogueData);

	/** Blueprint function for the ReceiveDialogueChangeCall */
	UFUNCTION(BlueprintImplementableEvent, Category = "Dialogue", meta = (DisplayName = "ReceiveDialogueChangeCall"))
		void _ReceiveDialogueChangeCall(FDialogueStruct _DialogueData);

	/** Action when receive display call */
	UFUNCTION()
		virtual void ReceiveHideCall(FDialogueStruct _DialogueData);

	/** Blueprint function for the ReceiveHideCall */
	UFUNCTION(BlueprintImplementableEvent, Category = "Dialogue", meta = (DisplayName = "ReceiveHideCall"))
		void _ReceiveHideCall(FDialogueStruct _DialogueData);

};
