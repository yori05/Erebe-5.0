// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameFramework/Pawn.h"
#include "Internationalization/Text.h"
#include "InteractiveInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable, BlueprintType)
class UInteractiveInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *  Interactive Interface
 *  Will be implemented by every class whom use interaction.
 *  Contain a fast interaction.
 *  And multiple interaction.
 *  The fast interaction is the default interaction like one to open a door
 *  Multiple interaction contain other interaction but for the same object like watch through the keyhole of a door
 * 
 * 
 * 
 *  Maybe on the future version will add a variable for the interaction name that will be filled in the child class's Constructor
 * 
 * 
 *  Depreciate use a interactivecomponent
 */
class EREBE_API IInteractiveInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//
	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	//void Interact(class UBasicInteractiveComponent);

	/** The main interaction send the Interactor if needed to valid or test element */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void MainInteract(APawn* Interactor);

	/** Can use the main interaction */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool CanMainInteract(APawn* Interactor);

	/** Name of the main interaction */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	FName MainInteractionName();

	/** Posses at last one interaction */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool HasAInteraction();

	/** Return the main interaction ID need to be override if different of 0 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	uint8 GetMainInteractionID();

	/**
	 *  Use interaction by ID used if implement secondary interaction
	 *  To make it usable with an Interface will use a switch case inside, the switch case will call good function in the child class
	 *  This pattern let the user implement an unlimited number of interaction (in fact no more than 255)
	 *  When override Recommended to call the Super:: of this function to keep previous interaction
	 *  (If seems useful maybe will implement a find function by name)
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void InteractByID(uint8 ID, APawn* Interactor);

	/** Can use an interaction */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool CanInteractByID(uint8 ID, APawn* Interactor);

	/**
	 *  Return in a Array name of every interaction available.
	 *  When override Recommended to call the Super:: of this function to keep previous interaction
	 *  The default form of this function will return a empty TArray
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	TArray<FName> GetInteractionsName();

	/** Return the interaction name corresponding to the ID. Return 'None' if not an valid ID*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		FName GetInteractionNameByID(uint8 ID);

	/**
	 *  Return true if possess more than one interaction
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool HasMultipleInteraction();

	/**
	 * Return the number of interaction implemented.
	 * When override recommanded to call the Super:: and add the result to the number of new interaction implemented
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	uint8 NumberOfInteractions();

public :
	// Default implementation for interface 

	/** By default don't do anything */
	virtual void MainInteract_Implementation(APawn* Interactor);

	/** By default return false */
	virtual bool CanMainInteract_Implementation(APawn* Interactor);

	/** By default return a empty text */
	virtual FName MainInteractionName_Implementation();

	/** By default return false */
	virtual bool HasAInteraction_Implementation();

	/** By default return 0 */
	virtual uint8 GetMainInteractionID_Implementation();

	/** By default return "Nothing" */
	virtual void InteractByID_Implementation(uint8 ID, APawn* Interactor);

	/** By default return false */
	virtual bool CanInteractByID_Implementation(uint8 ID, APawn* Interactor);

	/** By default return a empty array */
	virtual TArray<FName> GetInteractionsName_Implementation();

	/** By default return a empty text */
	virtual FName GetInteractionNameByID_Implementation(uint8 ID);

	/** By default return false */
	virtual bool HasMultipleInteraction_Implementation();

	/** By default return 0 */
	virtual uint8 NumberOfInteractions_Implementation();

};
