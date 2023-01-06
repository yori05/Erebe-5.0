// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Common/InteractionTypes.h"
#include "InteractiveComponent_Basic.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInteractiveBasicEventSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractiveBasicUpdateEventSignature, float, DeltaTime);

class AActor;

/**
* Interactive Component Basic
* Part of the interactive system
* 
* Base for all interactive component
* Can be used as it is to have interaction with actor using callback for event and action
* It should be more effective to override function like : BeginInteract, UpdateInteract, EndInteract, etc ... to implement your gameplay,
* Doing so you can still have every callback event, blueprint function, called rightly by the component
* You should set the EInteractionType of your component, to access it easly 
* And to have multiple interaction and even use the InteractiveComponent_Manager on your actor to work rightly and automatically in case you want to use it
*/
UCLASS( ClassGroup=("Interaction"), meta = (BlueprintSpawnableComponent), Blueprintable)
class EREBE_API UInteractiveComponent_Basic : public UActorComponent
{
	GENERATED_BODY()

/**-----------------	Variable Part		-----------------*/
protected:

	/** pointer of the actor owner*/
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<AActor> Owner;

	/** pointer of the requester actor for this interaction*/
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<AActor> Requestor;


	/** Sensitivity for the dot product with the forward alignement. Between 0 and 1. The smaller is this number the easiest is it to interact without being aligned with the actor */
	UPROPERTY(Category = Parameter, VisibleAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0", ClampMax = "1", UIMax = "1", AllowPrivateAccess = "true"))
		float DotForwdAlignSensitivity = 0.1f;

	/** Sensitivity for the dot product with the front alignement. Between 0 and 1. The smaller is this number the easiest is it to interact without being in front of the actor */
	UPROPERTY(Category = Parameter, VisibleAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0", ClampMax = "1", UIMax = "1", AllowPrivateAccess = "true"))
		float DotFrontAlignSensitivity = 0.1f;

	/** Used to define the kind of interaction of this InteractiveComponent */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Type", meta = (AllowPrivateAccess = "true"))
		EInteractionTypes EInteractionType;

	/** 
	* Used to define the kind of custom interaction of this InteractiveComponent 
	* Recommand to use it only if you can't set the type of interaction in code
	* In case you want to use it by a child of this class in Blueprint
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Type", meta = (AllowPrivateAccess = "true"))
		uint8 CustomInteractionType;

	/** Used to define if the component is the main component on actor owner */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
		bool bIsMainInteraction = false;
	
	/** Used to define if the component need Actor requester to interact */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
		bool bNeedRequestor = false;

	/** Used to define if the interaction has to be updated on the component tick */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
		bool bCanUpdateOnTick = false;

	/** 
	* Used to define if the update has to be called just after the begin
	* Could call twice the update sequence if the update is called after by the tick in the same frame 
	* Update need a DeltaTime, for now it's send with a value 0.f in the begin when called
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
		bool bCallUpdateInBegin = false;

	/**
	* Used to define if the end has to be called just after the begin
	* Could call twice the end sequence if the end is called after by the update in the tick on the same frame
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
		bool bCanEndInBegin = false;

	/** Used to define if the interaction can end on next update */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
		bool bCanEnd = false;

	/** Used to save if interaction is ongoing */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
		bool bInteractionOngoing = false;

	/** Used to define if the interaction can happen if ongoing */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
		bool bCanInteractIfOngoing = false;

	/** Used to define if the interaction can update if CallInteract is call when ongoing */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
		bool bUpdateIfCalledOngoing = false;

	/** Used to define if the interaction can stop if CallInteract is call when ongoing */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
		bool bStopIfCalledOngoing = false;

	/**-----------------	Callback Part		-----------------*/
public :

	// Called when the interaction begin
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FInteractiveBasicEventSignature OnBeginInteract;

	// Called when the interaction update
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FInteractiveBasicUpdateEventSignature OnUpdateInteract;

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

/**-----------------	Actor Component Function Part		-----------------*/
protected:

	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Called when the game starts
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

/**-----------------	Interactive Function Part		-----------------*/
public:

	/**
	* Set or unset this Interactive Component as the main interaction of his actor owner
	* @bNewIsMain : define if the Component is the main interaction
	* @bForceIt : will change it (and call event and all) even if the component is already in New state asked
	*/
		//UFUNCTION() //Uncomment in case we need it to be a ufunction (to link it to a callback call or smthg like that)
		void SetAsMainInteraction(bool bNewIsMain, bool bForceIt = false);

	/** 
	* Called by SetIsMainInteraction
	* Happen before the callback broadcast 
	*/
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "SetAsMainInteraction"), Category = "Interaction")
		void ReceiveSetAsMainInteraction(bool bNewIsMain, bool bForceIt = false);

	/**
	* Launch interaction 
	* Return true if the interaction launched
	* Will use CanInteract to check if the component can launch is interaction sequence
	* If succesfull will call BeginInteract_Sequence()
	* If unsuccesfull will call CantInteract_Sequence()
	*/
	UFUNCTION(BlueprintCallable, Category = "Interaction")
		virtual bool CallInteraction(AActor* _Requestor = nullptr);

	/**
	* Stop interaction if still on going
	* Return true if the interaction is on going and stopped
	* If the interaction is on going will call EndInteract_Sequence()
	*/
	UFUNCTION(BlueprintCallable, Category = "Interaction")
		virtual void StopInteraction();

	/**
	* Let the interaction have an other update before stopping it at the end of it,
	* need @bCanUpdate to be set as true, 
	* Will change @bCanEnd
	*/
	UFUNCTION(BlueprintCallable, Category = "Interaction")
		virtual void CanStopOnNextUpdate();

	/**
	* Abort interaction if still on going
	* Return true if the interaction is on going and stopped
	* If the interation is on going will call AbortInteract_Sequence()
	* Should be used in case we want a different kind of action as if the interaction is stopped
	* Like if this interaction end but not succesfully
	* In case the InteractiveComponent is destroy will call this function
	*/
	UFUNCTION(BlueprintCallable, Category = "Interaction")
		virtual void AbortInteraction();

	/**
	* Check if it's possible to start a interaction
	* Don't use the interactor in case 
	*/
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual	bool CanInteract(AActor* _Requestor = nullptr) const;

protected:

	/** Begin interact action used to have callback launch after the gameplay implementation even when override */
		//UFUNCTION() //Uncomment in case we need it to be a ufunction (to link it to a callback call or smthg like that)
		virtual void BeginInteract();

	/** Event when Interact begins for this component. */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "BeginInteract"), Category = "Interaction")
	void ReceiveBeginInteract();

	/** Update interact action used to have callback launch after the gameplay implementation even when override */
		//UFUNCTION() //Uncomment in case we need it to be a ufunction (to link it to a callback call or smthg like that)
		virtual void UpdateInteract(float DeltaTime = 0.f);

	/** Event when Interact updates for this component. */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "UpdateInteract"), Category = "Interaction")
		void ReceiveUpdateInteract(float DeltaTime = 0.f);

	/** End interact action used to have callback launch after the gameplay implementation even when override */
		//UFUNCTION() //Uncomment in case we need it to be a ufunction (to link it to a callback call or smthg like that)
		virtual void EndInteract();

	/** Event when Interact ends for this component. */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "EndInteract"), Category = "Interaction")
		void ReceiveEndInteract();

	/** Abort interact action used to have callback launch after the gameplay implementation even when override */
		//UFUNCTION() //Uncomment in case we need it to be a ufunction (to link it to a callback call or smthg like that)
		virtual void AbortInteract();

	/** Event when Interact aborts for this component. */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "AbortInteract"), Category = "Interaction")
		void ReceiveAbortInteract();

	/** Can't interact action used to have callback launch after the gameplay implementation even when override */
		//UFUNCTION() //Uncomment in case we need it to be a ufunction (to link it to a callback call or smthg like that)
		virtual void CantInteract();

	/** Event when can't Interact for this component. */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "CantInteract"), Category = "Interaction")
		void ReceiveCantInteract();

/**-----------------	Check Interaction Function Part		-----------------*/
protected:
	
	/** 
	* Use a dot product with the actor owner and the requestor forward's vectors to see if the actor is align enough
	* Use @DotForwdAlignSensitivity to see the two actor are align enough 
	*/
	UFUNCTION(BlueprintCallable)
		bool ForwardAlignment(AActor* _Requestor) const;

	/**
	* Use a dot product with the actor requestor and the position of the owner by the requestor to see if the requestor is looking at the owner
	* Use @DotForwdAlignSensitivity to see the two actor are align enough
	*/
	UFUNCTION(BlueprintCallable)
		bool OwnerLocByRequestorFrwAlign(AActor* _Requestor) const;

	/**
	* Check if the requestor is in front and looking at the actor owner of this interaction object
	* Use @DotForwdAlignSensitivity to see the two actor are align enough
	* Use @DotFrontSensitivity to see if the actor is in front of the owner
	*/
	UFUNCTION(BlueprintCallable)
		bool FrontAndForwardAlignment(AActor* _Requestor) const;

/**-----------------	Accessor Part		-----------------*/
public:

	// Return the interaction type
	FORCEINLINE EInteractionTypes GetInteractionType() const { return EInteractionType; }
	// Return the custom interaction type
	FORCEINLINE uint8 GetCustomInteractionType() const { return CustomInteractionType; }
	// Return the InteractionID of this component, used by the InteractiveComponent_Manager arrange InteractionType on a uint8 value (max 255)
	FORCEINLINE uint8 GetInteractionID() const { return (uint8) EInteractionType + (EInteractionType == EInteractionTypes::INTERACTTYPES_CustomInteraction) 
																					? CustomInteractionType 
																					: 0; }
	// Return if the interactive component is the main interactive comp on his actor owner
	FORCEINLINE bool GetIsMainInteraction() const { return bIsMainInteraction; }
	/** Return the actual requestor of this component */
	FORCEINLINE AActor* GetRequestor() const { return Requestor; }
};
