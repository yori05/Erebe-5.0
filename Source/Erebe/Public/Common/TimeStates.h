// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 *	TimeStates
 *	Define the time state avaible used by TimeManipulator : components, manager, ... etc
 *
 */
UENUM(BlueprintType)
enum class ETimeStates : uint8
{
	/**
	* Fast ForwardTime
	*/
	TIMESTATES_FastForward = 4 UMETA(DisplayName = "FastForwardTime"),

	/**
	*  Forward time, used by default to make it progress as it should
	*/
	TIMESTATES_Forward = 0 UMETA(DisplayName = "ForwardTime"),

	/**
	* StopTime
	*/
	TIMESTATES_Stop UMETA(DisplayName = "StopTime"),

	/**
	* BackwardTime
	*/
	TIMESTATES_Backward UMETA(DisplayName = "BackwardTime"),

	/**
	* Fast BackwardTime
	*/
	TIMESTATES_FastBackward UMETA(DisplayName = "FastBackwardTime"),


	/** Size of this enum can be used to know the number of state this enum give access to. In case we want make a enum encapsulate this one could be used on the first one of the new enum */
	TIMESTATES_Max UMETA(Hidden),
};