// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DoorComponent_Basic.generated.h"

class UCurveFloat;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDoorComponentEventSignature);

/**
 *	DoorStates
 *	Define the door state avaible used by IC_DoorOpenner
 */
UENUM(BlueprintType)
enum class EDoorStates : uint8
{
	/**
	*  Door closed, used by default to make the door as closed
	*/
	DOORSTATES_Closed = 0 UMETA(DisplayName = "Closed"),

	/**
	*  Door closing, to make the door as openned (not fully) and closing
	*/
	DOORSTATES_Closing UMETA(DisplayName = "Closing"),

	/**
	*  Door stuck, to make the door as not fully openned nor fully closed, and not in movement
	*  Should be used in case we want to stuck a door in this state
	*/
	DOORSTATES_StuckClosing UMETA(DisplayName = "HoldClosing"),

	/**
	*  Door stuck, to make the door as not fully openned nor fully closed, and not in movement
	*  Should be used in case we want to stuck a door in this state
	*/
	DOORSTATES_StuckOpening UMETA(DisplayName = "HoldOpening"),

	/**
	*  Door openning, to make the door as openned (not fully) and openning
	*/
	DOORSTATES_Openning UMETA(DisplayName = "Openning"),

	/**
	*  Door openned, to make the door as fully openned
	*/
	DOORSTATES_Openned UMETA(DisplayName = "Openned"),

	/**
	* Size of this enum can be used to know the number of state this enum give access to.
	* In case we want make a enum encapsulate this one could be used on the first one of the new enum
	*/
	DOORSTATES_Max UMETA(Hidden),

};

/**
 * Door Component Basic
 *
 * Used to manage logic for a door with a ratio of openning for the door
 * -> 0 when the door is close
 * -> 1 when the door is open
 * -> Will use the time value and the curve to get ratio value out of it
 * -> If you don't set the curve, RatioValue will use TimeValue ratio TimeMax as his value
 *
 * Time value will progress in UpdateState
 * -> Time min will use the curve (if set) to catch his value
 * -> Time max will use the curve (if set) to catch his value
 *
 * Use a enum to hold state for the door going to close, closing, stuck, openning, open
 * For now you can use a curve to set the time need to open your door and the ratio check the Blueprint of DoorCurve
 *
 * You can block the door, holding it as stuck, open, or close
 * Abort will make the door at stuck state, in case the door is moving (Openning or Closing) using the function to block will stuck the door
 *
 * You can set the UpdateDoor to be update when :
 * -> You call the function UpdateDoor
 * -> Set @bUpdateWithTick as true (usefull in case you don't want to use a IC_DoorOpenner)
 *
 * 
 *
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EREBE_API UDoorComponent_Basic : public UActorComponent
{
	GENERATED_BODY()

		/**-----------------	Variable Part		-----------------*/
protected:

	/** A curve used to catch the time and ratio used for the opening of the door */
	UPROPERTY(Category = Parameter, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UCurveFloat> UsedCurve;

	/** ID used by this door, should be edit on instance */
	UPROPERTY(Category = Time, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32 ID = 0;

	/** Time Value Saved */
	UPROPERTY(Category = Time, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float TimeValue = 0.f;

	/** Time Value Saved */
	UPROPERTY(Category = Time, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float TimeMin = 0.f;

	/** Time Value Saved */
	UPROPERTY(Category = Time, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float TimeMax = 1.f;

	/** Current ratio value goes between 0.f > 1.f */
	UPROPERTY(Category = Parameter, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float RatioValue = 0.f;

	/** Direction of the interactor saved by a dot product 1 if the interactor was in front -1 if in the back, will be 1 if none interactor is send when interacting */
	UPROPERTY(Category = Parameter, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float RatioDirection = 1.f;

	/** Save the state of the door */
	UPROPERTY(Category = Door, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		EDoorStates DoorState;

	/** Save if the door is openning / openned in opposite direction */
	UPROPERTY(Category = Door, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		bool bIsOpposedDirection = false;

	/** Save if the door can be open in both direction (like to 90° and -90°) */
	UPROPERTY(Category = Door, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool bCanGetOppositeDirection = true;

	/** define if we can we can try to change the door state   */
	UPROPERTY(Category = Door, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool bIsBlocked = false;

	/** Should the door move when unblock when stuck */
	UPROPERTY(Category = Door, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool bMoveOnUnblockStuck = false;

	/** Action to take when the door is stuck and we try to move it if it should open or close in case it's false will keep the previous movement */
	UPROPERTY(Category = Door, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool bOppositeAfterStuck = false;

	/** define if the state should be update with the tick */
	UPROPERTY(Category = Door, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool bUpdateWithTick = false;

	/**-----------------	Callback Part		-----------------*/
public:

	// Called when the door is open
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FDoorComponentEventSignature OnDoorOpen;

	// Called when the door is close
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FDoorComponentEventSignature OnDoorClose;

	// Called when the door is stuck
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FDoorComponentEventSignature OnDoorStuck;

	// Called when the door is blocked
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FDoorComponentEventSignature OnDoorBlock;

	// Called when the door is unblocked
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FDoorComponentEventSignature OnDoorUnblock;

	// Called when the door is moving
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FDoorComponentEventSignature OnDoorMoving;

	// Called when the door finish his movement
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FDoorComponentEventSignature OnDoorFinish;

/**-----------------	Constructor Part		-----------------*/
public:

	// Sets default values for this component's properties
	UDoorComponent_Basic();

/**-----------------	Actor Component Function Part		-----------------*/
protected:

	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

/**-----------------	State Function Part		-----------------*/
public:
	
	/** Call the function update door by external */
	virtual void CallUpdateDoor(float DeltaTime);

protected:

	/** Begin interact action used to have callback launch after the gameplay implementation even when override */
	virtual void ChangeState();

	/** Update interact action used to have callback launch after the gameplay implementation even when override */
	virtual void UpdateDoor(float DeltaTime);

/**-----------------	Door Component Function Part		-----------------*/
protected:

	/** Update the ratio value using @UsedCurve if valid or @_TimeValue */
	UFUNCTION()
		virtual void UpdateRatioValue(float _TimeValue);

public:

	/** Set the door ID */
	UFUNCTION(BlueprintCallable)
		void SetDoorID(int32 NewID) { ID = NewID; }

	/** Send a new opposite direction */
	UFUNCTION(BlueprintCallable)
		void SetOppositeDirection(bool bNewOppositeDirection) { bIsOpposedDirection = bNewOppositeDirection; }

	/** Call the interaction for the door trying to close or open it */
	UFUNCTION(BlueprintCallable)
		virtual void OpenCloseDoor();

	/** Call the interaction for the door trying open it (work only if the door is closed / closing) */
	UFUNCTION(BlueprintCallable)
		virtual void OpenDoor();

	/** Call the interaction for the door trying open it in the default direction (work only if the door is closed / closing) */
	UFUNCTION(BlueprintCallable)
		virtual void OpenDoorDefault();

	/** Call the interaction for the door trying open it in the opposite direction (work only if the door is closed / closing) */
	UFUNCTION(BlueprintCallable)
		virtual void OpenDoorReverse();

	/** Call the interaction for the door trying to close it (work only if the door is openned / openning) */
	UFUNCTION(BlueprintCallable)
		virtual void CloseDoor();

	/** Call the interaction for the door trying to get stuck will abort the current interaction */
	UFUNCTION(BlueprintCallable)
		virtual void StuckDoor();

	/** Call the interaction for the door trying to block the door if she is openning or closing will get in stuck state */
	UFUNCTION(BlueprintCallable)
		virtual void BlockDoor();

	/** Call the interaction for the door trying to unblock the door will let her in her current state (should be open / close / stuck) */
	UFUNCTION(BlueprintCallable)
		virtual void UnblockDoor();

	/**-----------------	Accessor Part		-----------------*/
public:

	/** Returns Time current value interaction */
	FORCEINLINE float GetTimeValue() const { return TimeValue; }
	/** Returns Time Min Interaction */
	FORCEINLINE float GetTimeMinValue() const { return TimeMin; }
	/** Returns Time Max Interaction */
	FORCEINLINE float GetTimeMaxValue() const { return TimeMax; }
	/** Returns openning's ratio */
	FORCEINLINE float GetOpenningRatioValue() const { return RatioValue; }
	/** Returns if the door is openning */
	FORCEINLINE bool GetIsOpenning() const { return DoorState == EDoorStates::DOORSTATES_Openning; }
	/** Returns if the door is open */
	FORCEINLINE bool GetIsOpen() const { return DoorState == EDoorStates::DOORSTATES_Openned; }
	/** Returns if the door is closing */
	FORCEINLINE bool GetIsClosing() const { return DoorState == EDoorStates::DOORSTATES_Closing; }
	/** Returns if the door is close */
	FORCEINLINE bool GetIsClose() const { return DoorState == EDoorStates::DOORSTATES_Closed; }
	/** Returns if the door is close */
	FORCEINLINE bool GetIsStuck() const { return DoorState == EDoorStates::DOORSTATES_StuckOpening || DoorState == EDoorStates::DOORSTATES_StuckClosing; }
	/** Returns if the door have to move */
	FORCEINLINE bool GetGoOppositeDirection() const { return bCanGetOppositeDirection; }
	/** Returns if the door have to move */
	FORCEINLINE bool GetIsOppositeDirection() const { return bIsOpposedDirection; }
	/** Returns if the door have to move */
	FORCEINLINE bool GetIsBlocked() const { return bIsBlocked; }
	/** Returns door state */
	FORCEINLINE EDoorStates GetDoorState() const { return DoorState; }
	/** Returns door state */
	FORCEINLINE int32 GetDoorID() const { return ID; }

};
