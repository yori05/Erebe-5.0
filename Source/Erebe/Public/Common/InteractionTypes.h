// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 *	InteractionTypes
 *	Define the interactions categories used by a interactive component
 *
 */
UENUM(BlueprintType)
enum class EInteractionTypes : uint8
{
	/**
	*  Undefinned default value shoulnd be used as it
	*/
	INTERACTTYPES_Undefinned = 0 UMETA(DisplayName = "Undefinned"),

	/**
	* Signpost
	*/
	INTERACTTYPES_Signpost UMETA(DisplayName = "Signpost"),

	/**
	* Dialogue
	*/
	INTERACTTYPES_Dialogue UMETA(DisplayName = "Dialogue"),

	/**
	* Open Door
	*/
	INTERACTTYPES_OpenDoor UMETA(DisplayName = "OpenDoor"),

	/** Size of this enum can be used to know the number of state this enum give access to. In case we want make a enum encapsulate this one could be used on the first one of the new enum */
	INTERACTTYPES_Max UMETA(Hidden),
};