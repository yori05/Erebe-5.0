// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structures/Signposts/SignpostStructure.h"
#include "SignpostComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSignpostChange, FText, CurrentText);


UCLASS( ClassGroup=("Signpost"), meta=(BlueprintSpawnableComponent), Blueprintable, BlueprintType )
class EREBE_API USignpostComponent : public UActorComponent
{
	GENERATED_BODY()

protected:

	/** Dialogue widget build and used by this object maybe later will only use the owning by the player */
	UPROPERTY(Category = "Signpost", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UUserWidget* SignpostWidget;

	/** Class of Dialogue widget used by this object need to be set in the editor or in the blueprint class */
	UPROPERTY(Category = "Signpost", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class UUserWidget> SignpostWidgetClass;

	/** Dialogue table stock all NPC's lines need to be set in the editor or in the blueprint class */
	UPROPERTY(Category = "Signpost", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UDataTable* SignpostTable;

	/** Saved interactor  */
	UPROPERTY(Category = "Signpost", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class APawn* SavedPawn;

	/** Saved name for this Signpost  */
	UPROPERTY(Category = "Signpost", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FSignpostStructure SavedStructure;

	/** Saved name for this Signpost  */
	UPROPERTY(Category = "Signpost", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FName SelfSignpostName;

	/** Saved subtext for the Signpost  */
	UPROPERTY(Category = "Signpost", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FText CurrentSubtext;

	/** ID of this Signpost */
	UPROPERTY(Category = "Signpost", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32 Signpost_ID;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FSignpostChange OnSignpostLineChange;

public:	
	// Sets default values for this component's properties
	USignpostComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
public :
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
	/** Stop the text display and clear the SavedDialoguer */
	UFUNCTION(BlueprintCallable)
		virtual void StopReadSignpost_Interaction(APawn* Interactor);
	/** Try to start a conversation */
	UFUNCTION(BlueprintCallable)
		virtual bool TryReadSignpost_Interaction(APawn* Interactor);

	UFUNCTION(BlueprintCallable)
		virtual bool IsReading() { return SavedPawn != nullptr; }

public:
	/** Return the signpost widget subobject */
	FORCEINLINE class UUserWidget* GetSignposttWidgetComponent() const { return SignpostWidget; }
	/** Return the signpost widget subobject */
	FORCEINLINE class APawn* GetSavedPawn() const { return SavedPawn; }
	/** Return the signpost struct */
	FORCEINLINE FSignpostStructure GetSavedStruct() const { return SavedStructure; }

};
