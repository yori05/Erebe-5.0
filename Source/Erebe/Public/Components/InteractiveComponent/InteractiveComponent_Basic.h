// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Common/InteractionTypes.h"
#include "InteractiveComponent_Basic.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInteractiveBasicEventSignature);

UCLASS( ClassGroup=("Interaction"), meta = (BlueprintSpawnableComponent), Blueprintable)
class EREBE_API UInteractiveComponent_Basic : public UActorComponent
{
	GENERATED_BODY()

/**-----------------	Variable Part		-----------------*/
protected:

	/** Used to define the kind of interaction of this InteractiveComponent */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
		EInteractionTypes EInteractionType;

	/** Used to define if the component is the main component on actor owner */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
		bool bIsMainInteraction = false;
	
	/** Used to define if the interaction has to be updated on the component tick */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
		bool bCanUpdateOnTick = false;

	/** 
	* Used to define if the update has to be called just after the begin
	* Could call twice the update sequence if the update is called after by the tick in the same frame 
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
		bool bCallUpdateInBegin = false;

	/**
	* Used to define if the update has to be called just after the begin
	* Could call twice the update sequence if the update is called after by the tick in the same frame
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
		bool bCanEndInBegin = false;

	/** Used to define if the component is the main component on actor owner */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
		bool bCanEnd = false;

	/** Used to define if the component is the main component on actor owner */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
		bool bInteractionOngoing = false;

	/**-----------------	Callback Part		-----------------*/
public :

	// Called when the interaction begin
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FInteractiveBasicEventSignature OnBeginInteract;

	// Called when the interaction update
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FInteractiveBasicEventSignature OnUpdateInteract;

	// Called when the interaction end
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FInteractiveBasicEventSignature OnEndInteract;

	// Called when the interaction is called but failled
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FInteractiveBasicEventSignature OnCantInteract;

	// Called when the interaction is begin but is abort
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FInteractiveBasicEventSignature OnAbortInteract;

	// Called when the component is set as main interaction
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FInteractiveBasicEventSignature OnSetAsMainInteraction;

	// Called when the component is unset as main interaction
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FInteractiveBasicEventSignature OnUnsetAsMainInteraction;

/**-----------------	Constructor Part		-----------------*/
public:	

	// Sets default values for this component's properties
	UInteractiveComponent_Basic();

/**-----------------	Inherit Function Part		-----------------*/
protected:

	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

/**-----------------	Interactive Function Part		-----------------*/
public:

	/**
	* Set or unset this Interactive Component as the main interaction of his actor owner
	* @bNewIsMain : define if the Component is the main interaction
	* @bForceIt : will change it (and call event and all) even if the component is already in New state asked
	*/
	UFUNCTION(BlueprintCallable)
		void SetIsMainInteraction(bool bNewIsMain, bool bForceIt = false);

	/**
	* Launch interaction 
	* Return true if the interaction launched
	* Will use CanInteract to check if the component can launch is interaction sequence
	* If succesfull will call BeginInteract_Sequence()
	* If unsuccesfull will call CantInteract_Sequence()
	*/
	UFUNCTION(BlueprintCallable)
		bool CallInteraction();

	/**
	* Stop interaction if still on going
	* Return true if the interaction is on going and stopped
	* If the interaction is on going will call EndInteract_Sequence()
	*/
	UFUNCTION(BlueprintCallable)
		bool StopInteraction();

	/**
	* Let the interaction have an other update before stopping it at the end of it,
	* need @bCanUpdate to be set as true, 
	* Will change @bCanEnd
	*/
	UFUNCTION(BlueprintCallable)
		void CanStopOnNextUpdate();

	/**
	* Abort interaction if still on going
	* Return true if the interaction is on going and stopped
	* If the interation is on going will call AbortInteract_Sequence()
	* Should be used in case we want a different kind of action as if the interaction is stopped
	* Like if this interaction end but not succesfully
	* In case the InteractiveComponent is destroy will call this function
	*/
	UFUNCTION(BlueprintCallable)
		bool AbortInteraction();

	/**
	* Check if it's possible to start a interaction
	* Return false in case a interaction is on going
	*/
	UFUNCTION(BlueprintCallable)
	virtual	bool CanInteract() const;

protected:

	/** Begin interact action used to have callback launch after the gameplay implementation even when override */
		virtual void BeginInteract();

	/** Event when Interact begins for this component. */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "BeginInteract"))
	void ReceiveBeginInteract();

	/** Update interact action used to have callback launch after the gameplay implementation even when override */
		virtual void UpdateInteract();

	/** Event when Interact updates for this component. */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "UpdateInteract"))
		void ReceiveUpdateInteract();

	/** End interact action used to have callback launch after the gameplay implementation even when override */
		virtual void EndInteract();

	/** Event when Interact ends for this component. */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "EndInteract"))
		void ReceiveEndInteract();

	/** Abort interact action used to have callback launch after the gameplay implementation even when override */
		virtual void AbortInteract();

	/** Event when Interact aborts for this component. */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "AbortInteract"))
		void ReceiveAbortInteract();

	/** Can't interact action used to have callback launch after the gameplay implementation even when override */
		virtual void CantInteract();

	/** Event when can't Interact for this component. */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "CantInteract"))
		void ReceiveCantInteract();

/**-----------------	Accessor Part		-----------------*/
public:

	// Return the interaction type
	FORCEINLINE EInteractionTypes GetInteractionType() const { return EInteractionType; }
	// Return if the interactive component is the main interactive comp on his actor owner
	FORCEINLINE bool GetIsMainInteraction() const { return bIsMainInteraction; }

};
