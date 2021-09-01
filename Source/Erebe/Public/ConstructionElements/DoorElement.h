// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/InteractiveActor.h"
#include "DoorElement.generated.h"


/**
 *	A Interactive Actor used like a basic Door
 *  Contain a Timeline Component used to modifies the angle of the door mesh.
 *  Can set the opening angle and if the door can be open in both way. 
 *  The Dot sensitivity change how much the pawn need to be aligned with the door to open it.
 *  Mesh are choice in the editor or on the child blueprint class 
 *	Same as the CurveFloat
 */
UCLASS()
class EREBE_API ADoorElement : public AInteractiveActor
{
	GENERATED_BODY()
	
protected:

	/** Mesh for the door frame */
	UPROPERTY(Category = Skin, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* DoorFrameMesh;

	/** Mesh for the door panel */
	UPROPERTY(Category = Skin, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* DoorMesh;

	/** Health component for the door */
	UPROPERTY(Category = Health, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UBasicHealthComponent* HealthComponent;

	UPROPERTY(Category = Movement, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class URotatingMeshComponent* RotatingMeshComponent;

public :
	// Class constructor
	ADoorElement();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	/** Function called by the InteractiveInterface will be useful in case a child class on this one want to change the order for the interaction, or the interaction effect */

	/** This is the main interaction of this class ID : 0 */

	//Open the door
	UFUNCTION(BlueprintCallable)
	virtual void OpenDoor_Interaction(APawn* Interactor);

	//Check if the door can be opened
	UFUNCTION(BlueprintCallable)
	virtual bool CanOpenDoor_Interaction(APawn* Interactor) const;

	//Return the action name interaction
	UFUNCTION(BlueprintCallable)
	virtual FName GetOpenDoorActionName_Interaction() const;


public :
	/** InteractiveInterface Implementation */

	virtual void MainInteract_Implementation(APawn* Interactor) override;
	virtual bool CanMainInteract_Implementation(APawn* Interactor) override;
	virtual FName MainInteractionName_Implementation() override;
	virtual bool HasAInteraction_Implementation() override;
	virtual uint8 GetMainInteractionID_Implementation() override;
	virtual void InteractByID_Implementation(uint8 ID, APawn* Interactor) override;
	virtual bool CanInteractByID_Implementation(uint8 ID, APawn* Interactor) override;
	virtual TArray<FName> GetInteractionsName_Implementation() override;
	virtual FName GetInteractionNameByID_Implementation(uint8 ID) override;
	virtual bool HasMultipleInteraction_Implementation() override;
	virtual uint8 NumberOfInteractions_Implementation() override;

public :
	/** Returns DoorFrame mesh subobject */
	FORCEINLINE class UStaticMeshComponent* GetDoorFrameMesh() const { return DoorFrameMesh; }
	/** Returns Door mesh subobject */
	FORCEINLINE class UStaticMeshComponent* GetDoorMesh() const { return DoorMesh; }
	/** Returns HealthComponent subobject */
	FORCEINLINE class UBasicHealthComponent* GetHealthComponent() const { return HealthComponent; }
	/** Returns RotatingMeshComp */
	FORCEINLINE class URotatingMeshComponent* GetRotatingMeshComp() const { return RotatingMeshComponent; }

};
