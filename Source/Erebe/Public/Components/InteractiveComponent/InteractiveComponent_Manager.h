// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Common/InteractionTypes.h"

#include "InteractiveComponent_Manager.generated.h"

class UInteractiveComponent_Basic;
class AActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FActorOverlapInteractionEventSignature, AActor*, OverlapActor);

/**
 *  Manager_InteractiveComponent
 *  Part of the Interactive System
 * 
 *  A Component used to manage every interaction on the InteractiveComponent_Basic on a actor
 *  It's recommanded to use this component to secure the interaction on a actor rather than just use a Basic_InteractiveComponent
 *  Will save InteractiveComponent and the InteractionType of this component in a map to have a easier access in need
 *  Recommanded to register interactive component on this component after the creation
 *  You can set @bSearchInteraction to true to automatically search all Interactive Component on the actor on begin play.
 * 
 *  It's recommanded to not have only one Interactive Component with a specified Interaction Type :
 *  -> In case a seccond Interactive Component with the same InteractiveType is registred he will be ignored my the Manager
 *  -> In case you want to use a custom interactive component you can have multiple component registred with the same interactive type,
 *  but you should use a Custom interactive type with it
 * 
*/
UCLASS( ClassGroup=(Interaction), meta=(BlueprintSpawnableComponent) )
class EREBE_API UInteractiveComponent_Manager : public UActorComponent
{
	GENERATED_BODY()

	/**-----------------	Variable Part		-----------------*/
private:

	/** Map used to save the different Interactive Component on the actor owner */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		TMap<uint8, TObjectPtr<UInteractiveComponent_Basic>> InteractivesActions;

	/** pointer of the main Interactive Component on the actor owner*/
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UInteractiveComponent_Basic> MainInteractiveAction;

	/** pointer of the Interactive Colision Component on the actor owner*/
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UPrimitiveComponent> InteractionColision;

	/** Interaction Type of the main Interactive Component on the actor owner*/
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		EInteractionTypes MainCatergorieAction;

	/** Custom Interaction Type of the main Interactive Component on the actor owner*/
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		uint8 MainCustomCatergorieAction;

	/** pointer of the actor owner*/
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<AActor> Owner;

	/** Used to define if the component should search for interactivecomponent on the begin play on the actor owner*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
		bool bSearchInteractiveComponents = true;

	/** Used to define if the component should search for primitive component with the tag "Interaction" on the begin play on the actor owner*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
		bool bSearchInteractiveCollider = true;

	/** Used to define if the component should set the first interactivecomponent added as a the main interaction*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
		bool bSetFirstAsMain = true;

	/** Should the interactive component launch main interaction when player begin overlap with it */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	bool bPlayerBegOverlLaunchMain = false;

	/** Should the interactive component stop main interaction when player end overlap with it */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
		bool bPlayerEndOverlStopMain = false;

	/** Should the interactive component launch main interaction when a actor begin overlap with it */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
		bool bActorBegOverlLaunchMain = false;

	/** Should the interactive component stop main interaction when a actor end overlap with it */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
		bool bActorEndOverlStopMain = false;

	/**-----------------	Callback Part		-----------------*/
public:

	/** Called when the Player begin to overlap with the interaction colision component */
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FActorOverlapInteractionEventSignature OnInteractionPlayerBeginOverlap;

	/** Called when the Player stop to overlap with the interaction colision component */
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FActorOverlapInteractionEventSignature OnInteractionPlayerEndOverlap;


	/** Called when a Actor begin to overlap with the interaction colision component */
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FActorOverlapInteractionEventSignature OnInteractionActorBeginOverlap;

	/** Called when a Actor stop to overlap with the interaction colision component */
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FActorOverlapInteractionEventSignature OnInteractionActorEndOverlap;

	/**-----------------	Constructor Part		-----------------*/
public:	

	// Sets default values for this component's properties
	UInteractiveComponent_Manager(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

/**-----------------	Inherit Function Part		-----------------*/
protected:

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called On post Load
	virtual void PostLoad() override;

	// Called when the game starts
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

public:	

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

/**-----------------	Manage Interaction Part		-----------------*/
public :

	/** 
	* Add a InteractiveComponent in the InteractivesActions Map
	* Return false :
	* - In case a other InteractiveComponent with the same EInteractionTypes is already added in the Map
	* - If the InteractiveComponent pointer is not valid
	* - If the InteractiveComponent categories is set as Undefinned
	* Return true in other case
	* If the InteractiveComponent is set as MainInteraction will use the Function SetNewMainInteraction
	*/
	UFUNCTION(BlueprintCallable)
	bool RequestSaveInteractiveComponent(UInteractiveComponent_Basic* NewInteractiveComp);

	/**
	* Set a InteractiveComp as the new main interaction
	* Check if the @InteractivesActions contain @NewMainInteraction
	* @MainCategorieAction : will take the EInteractionTypes of the @NewMainInteraction
	* @MainInteractiveAction : will point @NewMainInteraction
	* Return true in case the action is succesfull
	* Return false in case the @NewMainInteraction is not part of the @InteractivesActions
	* And keep the previous InteractiveComponent as the MainInteractiveAction
	*/
	UFUNCTION(BlueprintCallable)
		bool SetNewMainInteraction(UInteractiveComponent_Basic* NewMainInteraction);

	/**
	* Clear the main categories action
	* @bResetInteractiveComp : Will unset the InteractiveComp as a main interaction
	* Need for it to have a valid @MainInteractiveAction
	*/
	UFUNCTION(BlueprintCallable)
	void ResetMainInteraction(bool bResetInteractiveComp = true);

	/**
	* Return the InteractiveComponent saved by this type
	* Will need @_CustomInteractionType in the interaction type is set as custom, will ignore it other way
	* Return nullptr if none interactive component is saved with this type
	*/
	UFUNCTION(BlueprintCallable)
	UInteractiveComponent_Basic* GetInteractionByType(EInteractionTypes _InteractionType, uint8 _CustomInteractionType = 0);

	/**
	* Return the InteractiveComponent saved by this ID
	* Return nullptr if none interactive component is saved with this ID
	*/
	UFUNCTION(BlueprintCallable)
		UInteractiveComponent_Basic* GetInteractionByID(uint8 _InteractionID);

protected:
	
	/** Search for interaction component on the actor owner */
	UFUNCTION(BlueprintCallable)
		virtual void SearchInteractionComponents();


	/**-----------------	Interaction Colision Part		-----------------*/
public :

	/** Receive a new interaction primitive won't do anything in case he is same as the one used or void */
	UFUNCTION(BlueprintCallable)
		virtual void ReceiveInteractionPrimitive(UPrimitiveComponent* _NewPrimitiveComponent);

protected:

	/** Search for a interaction colision component on the actor owner */
	UFUNCTION(BlueprintCallable)
	virtual void SearchInteractionPrimitive();

	/** Link overlaping event to begin overlap and end overlap function of this component */
	UFUNCTION(BlueprintCallable)
	virtual void LinkToInteractionPrimitive();

	/** Break link overlaping event to begin overlap and end overlap function of this component */
	UFUNCTION(BlueprintCallable)
		virtual void BreakLinkToInteractionPrimitive();

	/** 
	* Action take when a actor begin overlaping with the primitive component link to this component
	* Will call player function in case the actor is a player
	* Will call actor function in every case
	*/
	UFUNCTION(BlueprintCallable)
		virtual void OnInteractionPrimitiveBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/**
	* Action take when a actor end overlaping with the primitive component link to this component
	* Will call player function in case the actor is a player
	* Will call actor function in every case
	*/
	UFUNCTION(BlueprintCallable)
		virtual void OnInteractionPrimitiveEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:

	void CallInteractionPlayerBeginOverlap(AActor* PlayerActor);

	void CallInteractionActorBeginOverlap(AActor* Actor);

	void CallInteractionPlayerEndOverlap(AActor* PlayerActor);

	void CallInteractionActorEndOverlap(AActor* Actor);

protected:

	virtual void InteractionPlayerBeginOverlap(AActor* PlayerActor);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "InteractionPlayerBeginOverlap"), Category = "Interaction")
		void ReceiveInteractionPlayerBeginOverlap(AActor* PlayerActor);

	virtual void InteractionActorBeginOverlap(AActor* Actor);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "InteractionActorBeginOverlap"), Category = "Interaction")
		void ReceiveInteractionActorBeginOverlap(AActor* Actor);


	virtual void InteractionPlayerEndOverlap(AActor* PlayerActor);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "InteractionPlayerEndOverlap"), Category = "Interaction")
		void ReceiveInteractionPlayerEndOverlap(AActor* PlayerActor);

	virtual void InteractionActorEndOverlap(AActor* Actor);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "InteractionActorEndOverlap"), Category = "Interaction")
		void ReceiveInteractionActorEndOverlap(AActor* Actor);

/**-----------------	Accessor Part		-----------------*/
public:

	// Return the main interaction
	FORCEINLINE UInteractiveComponent_Basic* GetMainInteraction() const { return MainInteractiveAction; }
	// Return the main interaction type
	FORCEINLINE EInteractionTypes GetMainInteractionType() const { return MainCatergorieAction; }
	// Return the main interaction custom type
	FORCEINLINE uint8 GetCustomInteractionType() const { return MainCustomCatergorieAction; }

};
