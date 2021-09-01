// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractiveComponent.generated.h"

/**
 *	InteractiveCategorie
 *	Define the interactive categories used to save interactive component
 *
 */
UENUM(BlueprintType)
enum EInteractiveCategories
{
	/**
	* Signpost
	*/
	INTERACTIVE_Signpost UMETA(DisplayName = "Signpost"),

	/**
	* Dialogue
	*/
	INTERACTIVE_Dialogue UMETA(DisplayName = "Dialogue"),

	/**
	* Open Door
	*/
	INTERACTIVE_OpenDoor UMETA(DisplayName = "OpenDoor"),

	/** Size of this enum can be used to know the number of state this enum give access to. In case we want make a enum encapsulate this one could be used on the first one of the new enum */
	INTERACTIVE_Max UMETA(Hidden),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FInteractiveComponentCallback, class UInteractiveComponent*, InteractiveComponent, class UInteractiveComponent*, OtherComponent, EInteractiveCategories, InteractionCategorie);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EREBE_API UInteractiveComponent : public UActorComponent
{
	GENERATED_BODY()

private :
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, meta= (AllowPrivateAccess = "true"))
		TMap<TEnumAsByte<EInteractiveCategories>, UActorComponent*> InteractivesActions;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		UActorComponent* MainInteractiveAction;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		TEnumAsByte<EInteractiveCategories> MainCatergorieAction;

public:
	/** Called in the MainInteract and InteractByID function, in case you override on these function and don't call the super you need to call this callback */
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FInteractiveComponentCallback OnInteract;

public:	
	// Sets default values for this component's properties
	UInteractiveComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public :
	virtual bool TryToAddAInteraction(UActorComponent* NewComp, EInteractiveCategories NewCategorie, bool IsMainAction = false);
	virtual UActorComponent* GetInteractionComponent(EInteractiveCategories NewCategorie);
	virtual void Interact(EInteractiveCategories Categorie, UInteractiveComponent* OtherComponent);
	virtual void MainInteract(UInteractiveComponent* OtherComponent);
};
