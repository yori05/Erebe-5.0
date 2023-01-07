// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/InteractiveComponent/InteractiveComponent_Basic.h"
#include "Structures/Signposts/SignpostStructure.h"

#include "IC_Signpost_Basic.generated.h"

class UDataTable;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSignpostChangeSignature, FText, CurrentText);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSignpostDisplayCallSignature, FSignpostStructure, SignpostData);

/**
 * Interactive Component Signpost Basic
 * Part of the interactive system
 * Child of UInteractiveComponent_Basic
 * 
 * Set the EInteractionType as INTERACTTYPES_Signpost
 * Use a Widget to display a text for the player
 * The data used by this component is load from the data table set in @SignpostTable
 * By default value are set so when interact is called it will stop the interaction if ongoing
 * Need to send a actor as requestor when you want to use this interactive component
 * Check UInteractiveComponent_Basic if you want more info.
 */
UCLASS(ClassGroup = ("Signpost"), meta = (BlueprintSpawnableComponent), Blueprintable, BlueprintType)
class EREBE_API UIC_Signpost_Basic : public UInteractiveComponent_Basic
{
	GENERATED_BODY()
	
/**-----------------	Variable Part		-----------------*/
protected:

	/** Dialogue table stock all NPC's lines need to be set in the editor or in the blueprint class */
	UPROPERTY(Category = "Signpost", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UDataTable> SignpostTable;

	/** Saved name for this Signpost  */
	UPROPERTY(Category = "Signpost", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FSignpostStructure SavedStructure;

	/** ID of this Signpost */
	UPROPERTY(Category = "Signpost", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32 Signpost_ID;

	/** Should this component try to link himself to the ui */
	UPROPERTY(Category = "Signpost", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool bSearchForUI = false;

/**-----------------	Callback Part		-----------------*/
public:

	/** Called when the signpost line change, for now it's used in the begin interaction */
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FSignpostChangeSignature OnSignpostLineChange;

	/** Called when the signpost call for being display, for now it's used in the begin interaction */
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FSignpostDisplayCallSignature OnSignpostDisplayCall;

	/** Called when the signpost call for being hide, for now it's used in the end / abort interaction */
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FSignpostDisplayCallSignature OnSignpostHideCall;

/**-----------------	Constructor Part		-----------------*/
public:

	// Sets default values for this component's properties
	UIC_Signpost_Basic();

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

	/** End interact action used to have callback launch after the gameplay implementation even when override */
	virtual void EndInteract() override;

	/** Abort interact action used to have callback launch after the gameplay implementation even when override */
	virtual void AbortInteract() override;

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
	FORCEINLINE FSignpostStructure GetSavedStruct() const { return SavedStructure; }

};
