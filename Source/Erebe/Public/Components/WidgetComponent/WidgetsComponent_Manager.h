// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WidgetsComponent_Manager.generated.h"

class UUW_Signpost;
class UUW_Dialogue;

/**
* A WidgetComponent_Manager
*
* This component should be add on a game mode because the components whome will search for him will try to access it from a game mode
* 
* A component used to manage widget when they should be shared by multiple component / object
* Can spawn / not spawn widget using Handle
* Add widget to this component only if widget should be shared and can't be manage by one actor
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EREBE_API UWidgetsComponent_Manager : public UActorComponent
{
	GENERATED_BODY()

/**-----------------	Variable Part		-----------------*/
protected:

	/** Dialogue widget build and used by this object maybe later will only use the owning by the player */
	UPROPERTY(Category = "Signpost", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UUW_Signpost> SignpostWidget;

	/** Class of Dialogue widget used by this object need to be set in the editor or in the blueprint class */
	UPROPERTY(Category = "Signpost", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UUW_Signpost> SignpostWidgetClass;

	/** Dialogue widget build and used by this object maybe later will only use the owning by the player */
	UPROPERTY(Category = "Dialogue", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UUW_Dialogue> DialogueWidget;

	/** Class of Dialogue widget used by this object need to be set in the editor or in the blueprint class */
	UPROPERTY(Category = "Dialogue", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UUW_Dialogue> DialogueWidgetClass;

	/**
	* Handle to spawn signpost widget instance, and add it to the viewport 
	* Set as true by default
	*/
	UPROPERTY(Category = "Signpost", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool bSignpostWidgetHandle = true;

	/**
	* Handle to spawn signpost widget instance, and add it to the viewport
	* Set as true by default
	*/
	UPROPERTY(Category = "Dialogue", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool bDialogueWidgetHandle = true;

/**-----------------	Constructor Part		-----------------*/
public:	

	// Sets default values for this component's properties
	UWidgetsComponent_Manager();

/**-----------------	Actor Component Function Part		-----------------*/
protected:

	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

/**-----------------	Accessor Part		-----------------*/
public:

	/** Returns Signpost widget instance saved **/
	FORCEINLINE UUW_Signpost* GetSignpostWidget() { return SignpostWidget; };
	/** Returns Dialogue widget instance saved **/
	FORCEINLINE UUW_Dialogue* GetDialogueWidget() { return DialogueWidget; };

};
