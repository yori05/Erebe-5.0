// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/InteractiveCharacter.h"
#include "InteractiveNPC.generated.h"


/**
 * 
 */
UCLASS()
class EREBE_API AInteractiveNPC : public AInteractiveCharacter
{
	GENERATED_BODY()
	
protected :

	/** Component used to handle Dialogue */
	UPROPERTY(Category = "Dialogue", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UDialogueComponent* DialogueComponent;

	/** Hit box used to trigger the detection of this NPC  */
	UPROPERTY(Category = "Interaction", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* DetectionHitbox;

public :
	// Class constructor
	AInteractiveNPC();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	UFUNCTION(BlueprintCallable)
		virtual void OnHitboxOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	/** Function called by the InteractiveInterface will be useful in case a child class on this one want to change the order for the interaction, or the interaction effect */

	/** Read the text displayed by this object widget / make it invisible if the text is already visible */
	UFUNCTION(BlueprintCallable)
		virtual void StartConversation_Interaction(APawn* Interactor);
	/** Block the reading if the character is behind the object or not in the front */
	UFUNCTION(BlueprintCallable)
		virtual bool CanStartConversation_Interaction(APawn* Interactor);
	/** Return a text with a 'Read' in content maybe will take localization in the future*/
	UFUNCTION(BlueprintCallable)
		virtual FName GetStartConversationActionName_Interaction();

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

public:

	/** Returns the DialogueComponent Subobject */
	FORCEINLINE class UDialogueComponent* GetDialogueComponent() { return DialogueComponent; };

};
