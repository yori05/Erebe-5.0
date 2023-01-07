// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/GameMode_TimeManipulator.h"
#include "Components/TimeManipulatorComponent/TimeManipulatorComponent_Manager.h"
#include "Character/Character_TimeManipulator.h"
#include "PlayerController/PlayerController_TimeManipulator.h"
#include "Engine/World.h"


AGameMode_TimeManipulator::AGameMode_TimeManipulator()
{
	DefaultPawnClass = ACharacter_TimeManipulator::StaticClass();
	PlayerControllerClass = APlayerController_TimeManipulator::StaticClass();

	// Create Time Manipulator Manager Component 
	TimeManagerComponent = CreateDefaultSubobject<UTimeManipulatorComponent_Manager>(TEXT("TimeManagerComponent"));
}
