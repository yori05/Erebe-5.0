// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Common/InteractionTypes.h"
#include "Manager_InteractiveComponent.generated.h"

class UInteractiveComponent_Basic;
class AActor;

/**
 *  Manager_InteractiveComponent
 *  A Component used to manage every interaction on the Basic_InteractiveComponent on a actor
 *  It's recommanded to use this component to secure the interaction on a actor rather than just use a Basic_InteractiveComponent
 *  Will save InteractiveComponent and the InteractionType of this component in a map to have a easier access in need
 *  Recommanded to register interactive component on this component after the creation
 *  You can set @bSearchInteraction to true to automatically search all Interactive Component on the actor on begin play.
 *  It's recommanded to not have only one Interactive Component with a specified Interaction Type :
 *  - In case a seccond Interactive Component with the same InteractiveType is registred he will be ignored my the Manager
 * 
*/
UCLASS( ClassGroup=(Interaction), meta=(BlueprintSpawnableComponent) )
class EREBE_API UManager_InteractiveComponent : public UActorComponent
{
	GENERATED_BODY()

	/**-----------------	Variable Part		-----------------*/
private:

	/** Map used to save the different Interactive Component on the actor owner */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		TMap<EInteractionTypes, TObjectPtr<UInteractiveComponent_Basic>> InteractivesActions;

	/** pointer of the main Interactive Component on the actor owner*/
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UInteractiveComponent_Basic> MainInteractiveAction;

	/** Interaction Type of the main Interactive Component on the actor owner*/
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		EInteractionTypes MainCatergorieAction;

	/** pointer of the actor owner*/
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<AActor> Owner;

	/** Used to define if the component should search for interactivecomponent on the actor owner*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
		bool bSearchInteraction = false;

	/** Used to define if the component should set the first interactivecomponent added as a the main interaction*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
		bool bSetFirstAsMain = false;

	/**-----------------	Constructor Part		-----------------*/
public:	

	// Sets default values for this component's properties
	UManager_InteractiveComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called On post Load
	virtual void PostLoad() override;

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
	* Return nullptr if none interactive component is saved with this type
	*/
	UFUNCTION(BlueprintCallable)
	UInteractiveComponent_Basic* GetInteractionByType(EInteractionTypes _InteractionType);
};
