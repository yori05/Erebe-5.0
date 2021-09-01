// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/InteractiveActor.h"
#include "Components/SlateWrapperTypes.h"
#include "Internationalization/Text.h"
#include "SignpostElement.generated.h"


/**
 *  
 */
UCLASS()
class EREBE_API ASignpostElement : public AInteractiveActor
{
	GENERATED_BODY()
	
protected:

	/** Signpost Component */
	UPROPERTY(Category = Hitbox, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class USignpostComponent* SignpostComponent;

	/**	Mesh for the Signpost */
	UPROPERTY(Category = Skin, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* SignpostMesh;

	/** Hit box used to trigger the interaction  */
	UPROPERTY(Category = Hitbox, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* InteractiveHitbox;

public :
	// Class constructor
	ASignpostElement();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	UFUNCTION()
	virtual void OnHitboxOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
protected:
	/** Function called by the InteractiveInterface will be useful in case a child class on this one want to change the order for the interaction, or the interaction effect */

	/** Read the text displayed by this object widget / make it invisible if the text is already visible */
	UFUNCTION(BlueprintCallable)
		virtual void ReadSignpost_Interaction(APawn* Interactor);
	/** Block the reading if the character is behind the object or not in the front */
	UFUNCTION(BlueprintCallable)
		virtual bool CanReadSignpost_Interaction(APawn* Interactor);
	/** Return a text with a 'Read' in content maybe will take localization in the future*/
	UFUNCTION(BlueprintCallable)
		virtual FName GetReadSignpostActionName_Interaction();


public:
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
	/** Return the signpost mesh component */
	FORCEINLINE class UStaticMeshComponent* GetSignpostMesh() const { return SignpostMesh; }
	/** Return the interactive hit box component */
	FORCEINLINE class UCapsuleComponent* GetInteractiveHitbox() const { return InteractiveHitbox; }
	/** Return the signpost mesh component */
	FORCEINLINE class USignpostComponent* GetSignpostComponent() const { return SignpostComponent; }

};
