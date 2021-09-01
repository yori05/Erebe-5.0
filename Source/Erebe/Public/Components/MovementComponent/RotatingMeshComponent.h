// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "RotatingMeshComponent.generated.h"


UCLASS( ClassGroup=("Movement"), meta=(BlueprintSpawnableComponent), Blueprintable, BlueprintType )
class EREBE_API URotatingMeshComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY()
		FTimeline RotationTimeline;

	UPROPERTY(Category = Interaction, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* RotatingMesh;

	UPROPERTY(Category = Interaction, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UCurveFloat* RotationTimelineFloatCurve;

	/** Angle for the door to open. In can you want the door to open in both sens use a positive angle or the door won't be push by the actor but pull*/
	UPROPERTY(Category = Interaction, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float OpenAngle = 90.f;

	/** Save if the door is opened in a opposite angle */
	UPROPERTY(Category = Interaction, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float AngleSens = 1.f;

	/** Sensitivity to open the door.Between 0 and 1. The smaller is this number the easiest is it to open the door without being aligned with her */
	UPROPERTY(Category = Interaction, VisibleAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0", ClampMax = "1", UIMax = "1", AllowPrivateAccess = "true"))
		float DotSensitivity = 0.1f;

	/** Save if the door is opened */
	UPROPERTY(Category = Interaction, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		bool bIsOpen = false;

	/** Save if the door can be open in both sens (like to 90° and -90°) */
	UPROPERTY(Category = Interaction, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool bCanGetOppositeAngle = true;

	UPROPERTY(Category = Interaction, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool bIsBlocked = false;

public:	
	// Sets default values for this component's properties
	URotatingMeshComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
		void SeRotatingMesh(class UStaticMeshComponent* NewRotatingMesh) { RotatingMesh = NewRotatingMesh; };

protected:
	//Door movement update called by the TimelineComponent
	UFUNCTION(BlueprintCallable)
		virtual void RotateMeshTick(float Output);

public:
	/** Function called by the InteractiveInterface will be useful in case a child class on this one want to change the order for the interaction, or the interaction effect */

	/** This is the main interaction of this class ID : 0 */

	//Open the door
	UFUNCTION(BlueprintCallable)
		virtual void RotateMesh_Interaction(APawn* Interactor);

	//Check if the door can be opened
	UFUNCTION(BlueprintCallable)
		virtual bool CanRotateMesh_Interaction(APawn* Interactor) const;

	//Return the action name interaction
	UFUNCTION(BlueprintCallable)
		virtual FName GetRotateMeshActionName_Interaction() const;

public:
	/** Returns door's angle */
	FORCEINLINE float GetAngle() const { return OpenAngle; }
	/** Returns if the door is open or no */
	FORCEINLINE bool GetIsOpen() const { return bIsOpen; }
	/** Returns if the door have to move */
	FORCEINLINE bool GetCanGetOppositeAngle() const { return bCanGetOppositeAngle; }
	/** Returns if the door have to move */
	FORCEINLINE bool GetIsOppositeAngle() const { return AngleSens < 0.f; }
	/** Returns if the door have to move */
	FORCEINLINE bool GetIsBlocked() const { return bIsBlocked; }

};
