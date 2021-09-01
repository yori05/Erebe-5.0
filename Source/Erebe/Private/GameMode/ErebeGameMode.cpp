// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/ErebeGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "PlayerController/ErebePlayerController.h"
#include "Character/ErebeCharacter.h"

AErebeGameMode::AErebeGameMode()
{
	/** Set ErebeCharacter blueprint as default class for the pawn */
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/BlueprintClasses/Character/ErebeCharacter_BP"));
	
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	/** Set ErebePlayerController blueprint as default class for the player controller */
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/BlueprintClasses/PlayerController/ErebePlayerController_BP"));

	if (PlayerControllerBPClass.Class != nullptr)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}

}
