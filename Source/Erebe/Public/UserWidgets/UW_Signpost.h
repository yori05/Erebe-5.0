// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Structures/Signposts/SignpostStructure.h"
#include "Widgets/SWidget.h"
#include "Components/Widget.h"

#include "UW_Signpost.generated.h"

class UIC_Signpost_Basic;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWidgetSignpostEventSignature, FSignpostStructure, SignpostData);

/**
 *	User widget Signpost
 * 
 *	Base for a UW_Signpost blueprint
 *	Used to save IC_Signpost_Basic components and bind event on it
 *	This class is not build to manage action on the widget, just to add and remove link with Signpost Component
 *	Action for now should stay on blueprint 
 */
UCLASS(Abstract, editinlinenew, BlueprintType, Blueprintable, meta = (DontUseGenericSpawnObject = "True", DisableNativeTick))
class EREBE_API UUW_Signpost : public UUserWidget
{
	GENERATED_BODY()
	
friend class SObjectWidget;

/**-----------------	Variable Part		-----------------*/
protected:

	/** Array of Signpost Component */
	UPROPERTY(Category = "Management", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UIC_Signpost_Basic>> SignpostComponents;
	
/**-----------------	Callback Part		-----------------*/
public:

	// Called when a signpost link request a display call
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FWidgetSignpostEventSignature OnReceiveDisplayCall;

	// Called when a signpost link request a hide call
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FWidgetSignpostEventSignature OnReceiveHideCall;

/**-----------------	Constructor Part		-----------------*/
public:
	UUW_Signpost(const FObjectInitializer& ObjectInitializer);

/**-----------------	User Widget Function Part		-----------------*/
public:

	virtual bool Initialize() override;

	/**
	* Called when a widget is no longer referenced causing the slate resource to destroyed.  Just like
	* Construct this event can be called multiple times.
	*/
	virtual void BeginDestroy() override;

/**-----------------	Management Part		-----------------*/
public:

	/** Add a Signpost component to the array and bind event to this component's event */
	UFUNCTION(BlueprintCallable, Category = "Management")
	virtual bool AddSignpostComponent(UIC_Signpost_Basic* _SignpostComp = nullptr);

	/** Remove a Signpost component to the array and unbind event to this component's event */
	UFUNCTION(BlueprintCallable, Category = "Management")
	virtual bool RemoveSignpostComponent(UIC_Signpost_Basic* _New_SignpostCompSignpost = nullptr);

protected:

	/** Unbind event to a signpost component's event */
	UFUNCTION(BlueprintCallable, Category = "Management")
		virtual void BindSignpostComponent(UIC_Signpost_Basic* _SignpostComp = nullptr);

	/** Unbind event to a signpost component's event */
	UFUNCTION(BlueprintCallable, Category = "Management")
		virtual void UnbindSignpostComponent(UIC_Signpost_Basic* _SignpostComp = nullptr);

	
/**-----------------	UW Signpost Part		-----------------*/

	/** Action when receive display call */
	UFUNCTION()
	virtual void ReceiveDisplayCall(FSignpostStructure _SignpostData);

	/** Blueprint function for the ReceiveDisplayCall */
	UFUNCTION(BlueprintImplementableEvent, Category = "Signpost", meta = (DisplayName = "ReceiveDisplayCall"))
	void _ReceiveDisplayCall(FSignpostStructure _SignpostData);

	/** Action when receive display call */
	UFUNCTION()
	virtual void ReceiveHideCall(FSignpostStructure _SignpostData);

	/** Blueprint function for the ReceiveHideCall */
	UFUNCTION(BlueprintImplementableEvent, Category = "Signpost", meta = (DisplayName = "ReceiveHideCall"))
		void _ReceiveHideCall(FSignpostStructure _SignpostData);

};
