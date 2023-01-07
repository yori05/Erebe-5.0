// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/InteractiveComponent/InteractiveComponent_Basic.h"
#include "IC_DoorOpenner_Basic.generated.h"

class UDoorComponent_Basic;

/**
 * Interactive Component DoorOpenner Basic
 * Part of the interactive system
 * Child of Interactive Component Basic
 * Linked with a DoorComponent
 * 
 * Need a requestor
 * Will link with a door component on begin play
 * Can send an other door component on run if need
 * You can still use the Callback even of Interactive Component Basic
 * 
 */
UCLASS(ClassGroup = ("Door"), meta = (BlueprintSpawnableComponent), Blueprintable, BlueprintType)
class EREBE_API UIC_DoorOpenner_Basic : public UInteractiveComponent_Basic
{
	GENERATED_BODY()
	
/**-----------------	Variable Part		-----------------*/
protected:

	UPROPERTY(Category = "Door", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UDoorComponent_Basic> SavedDoorComponent;

	/** Should this component search for the dialogue component on begin play */
	UPROPERTY(Category = "Door", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool bSearchDoorComp = true;

/**-----------------	Constructor Part		-----------------*/
public:

	// Sets default values for this component's properties
	UIC_DoorOpenner_Basic();

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
	* Check if the door is blocked
	* Check if the requestor is in dot limite (if send)
	*/
	virtual	bool CanInteract(AActor* _Requestor = nullptr) const override;

protected:

	/** Begin interact action used to have callback launch after the gameplay implementation even when override */
	virtual void BeginInteract() override;
	
	/** Update interact action used to have callback launch after the gameplay implementation even when override */
	virtual void UpdateInteract(float DeltaTime) override;

	/** Abort interact action used to have callback launch after the gameplay implementation even when override */
	virtual void AbortInteract() override;

/**-----------------	Door Openner Function Part		-----------------*/
public:

	/** Receive a dialogue component will link to it */
	UFUNCTION(BlueprintCallable)
		virtual void ReceiveDoorComponent(UDoorComponent_Basic* _NewComp);

protected:

	/** Search for the dialogue component on the actor owner */
	UFUNCTION(BlueprintCallable)
		virtual void SearchForDoorComponent();

	/** Link with the @SavedDialogueComponent */
	UFUNCTION(BlueprintCallable)
		virtual void LinkDoorComponent();

	/** Break link with the @SavedDialogueComponent */
	UFUNCTION(BlueprintCallable)
		virtual void BreakLinkDoorComponent();

	/**-----------------	Accessor Part		-----------------*/
public:

	/** Return the signpost struct */
	FORCEINLINE UDoorComponent_Basic* GetLinkedDoorComp() const { return SavedDoorComponent; }

};
