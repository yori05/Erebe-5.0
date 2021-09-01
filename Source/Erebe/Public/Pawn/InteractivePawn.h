// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Interfaces/InteractiveInterface.h"
#include "InteractivePawn.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FInteractivePawnCallback, class AInteractivePawn*, InteractiveActor, APawn*, InteractorPawn, uint8, InteractionID, FName, InteractionName);

UCLASS()
class EREBE_API AInteractivePawn : public APawn, public IInteractiveInterface
{
	GENERATED_BODY()

public:
	/** Called in the MainInteract and InteractByID function, in case you override on these function and don't call the super you need to call this callback */
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FInteractivePawnCallback OnInteract;

public:
	// Sets default values for this pawn's properties
	AInteractivePawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	// Default implementation for interface 

	/**
	 *  Will call the callback event.
	 *	If you override this function it's recommanded to call Super::MainInteract_Implementation.
	 *  In case you can't use the Super of this function call AInteractiveActor::MainInteract_Implementation or you will loss the callback call.
	 */
	virtual void MainInteract_Implementation(APawn* Interactor) override;

	/** By default return false */
	virtual bool CanMainInteract_Implementation(APawn* Interactor) override;

	/** By default return a empty text */
	virtual FName MainInteractionName_Implementation() override;

	/** By default return false */
	virtual bool HasAInteraction_Implementation() override;

	/** By default return 0 */
	virtual uint8 GetMainInteractionID_Implementation() override;

	/**
	 *  Will call the callback event.
	 *	If you override this function it's recommanded to call Super::InteractByID_Implementation.
	 *  In case you can't use the Super of this function call AInteractiveActor::InteractByID_Implementation or you will loss the callback call.
	 */
	virtual void InteractByID_Implementation(uint8 ID, APawn* Interactor) override;

	/** By default return false */
	virtual bool CanInteractByID_Implementation(uint8 ID, APawn* Interactor) override;

	/** By default return a empty array */
	virtual TArray<FName> GetInteractionsName_Implementation() override;

	/** By default return a empty text */
	virtual FName GetInteractionNameByID_Implementation(uint8 ID) override;

	/** By default return false */
	virtual bool HasMultipleInteraction_Implementation() override;

	/** By default return 0 */
	virtual uint8 NumberOfInteractions_Implementation() override;


};
