// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Character_Basic.generated.h"

class USceneComponent;
class UCameraComponent;
class USkeletalMeshComponent;
class USpringArmComponent;
class UCombatComponent_Basic;
class UCapsuleComponent;
class UCurveTable;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCharacterBasicEventSignature);

/**
*  Character_Basic
*	Character used by the player with a PlayerController_Basic
*  Should be used to make child if need default movement
*	Can move, jump
*  Contain two camera and two skeletal mesh (for the first and third person viewpoint) + one spring arm for the third person camera
*  The default viewpoint is set as a First Person
*  The First person skeletal mesh is attach to the First person camera if you want it in a other way you have to change it the child constructor
*  The Third Person skeletal mesh is attach the root and the SpringArm of the Third Person Camera is attach to the Third person Skeletal mesh
* 
*  Every action use a function to call Implementation for gameplay on c++ then on blueprint implementable event
* 
* Featur :
* Done :
* 
* ToDo :
*  -> Add handle to lock / unlock call for C++ / BP gameplay implementation (will use a enum to set as both / only one)
* 
*/
UCLASS(config = Game)
class EREBE_API ACharacter_Basic : public ACharacter
{
	GENERATED_BODY()

		/**-----------------	Component Part		-----------------*/
private:

	/** Interaction Capsule Colider */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Interaction, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UCapsuleComponent> InteractionCapsule;

	/**	First Person Camera Location */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		TObjectPtr <USceneComponent> FPViewPointCamera;

	/**	Character Camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		TObjectPtr <UCameraComponent> CharCamera;

	/** Third person mesh  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<USkeletalMeshComponent> TPMesh;

	/**	Third Person Camera Location */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<USceneComponent> TPViewPointCamera;

	/**	Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<USpringArmComponent> CameraBoom;

	/** CombatComponent */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UCombatComponent_Basic> CombatComponent;


	/**-----------------	Variable Part		-----------------*/
protected:

	/** Spring Arm Camera Boom Curve */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCurveTable> CameraBoomCurve;

	/** The direction of the movement for this character used in case we use the root motion animation */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Input")
		FVector MotionInput;

	/** Zoom Ratio used to change CameraBoom Param */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zoom", meta = (AllowPrivateAccess = "true"))
		float ZoomRatio;

	/** Check if the character is in first person or third */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FirstP/ThirdP", meta = (AllowPrivateAccess = "true"))
		bool bIsInFP = true;

	/** Should the character use root motion animation to move */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		bool bUseRootMotionAnimation;

	/**-----------------	Callback Part		-----------------*/
public:

	// Called when the interaction begin
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FCharacterBasicEventSignature OnUseFirstPersonView;

	// Called when the interaction begin
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FCharacterBasicEventSignature OnUseThirdPersonView;

	/**-----------------	Constructor Part		-----------------*/
public:

	// Sets default values for this character's properties
	ACharacter_Basic(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


	/**-----------------	Actor Function Part		-----------------*/
protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/**-----------------	Character_Basic Part		-----------------*/
	/**-----------------	Action Part		-----------------*/
	/**-----------------	Movement Part		-----------------*/
public:

	/** 
	* Used to call action for MoveForward implemented in c++ and bp
	*/
	UFUNCTION(BlueprintCallable)
		virtual void CallMoveForward(float Value);

	/**
	* Used to call action for MoveRight implemented in c++ and bp
	*/
	UFUNCTION(BlueprintCallable)
		virtual void CallMoveRight(float Value);

	/**
	* Used to call action for MoveUp implemented in c++ and bp
	*/
	UFUNCTION(BlueprintCallable)
		virtual void CallMoveUp(float Value);

	/**
	* Used to call action for MoveForwardCamera implemented in c++ and bp
	*/
	UFUNCTION(BlueprintCallable)
		virtual void CallMoveForwardCamera(float Value);

	/**
	* Used to call action for MoveRightCamera implemented in c++ and bp
	*/
	UFUNCTION(BlueprintCallable)
		virtual void CallMoveRightCamera(float Value);

	/**
	* Used to call action for MoveUpCamera implemented in c++ and bp
	*/
	UFUNCTION(BlueprintCallable)
		virtual void CallMoveUpCamera(float Value);

protected:

	// Called to make the character move in the forward direction
	virtual void MoveForward(float Value);

	/** Event Called to make the character move in the forward direction. */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "MoveForward"), Category = "Movement")
		void ReceiveMoveForward(float Value);

	// Called to make the character move in the right direction
	virtual void MoveRight(float Value);

	/** Event Called to make the character move in the right direction. */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "MoveRight"), Category = "Movement")
		void ReceiveMoveRight(float Value);

	// Called to make the character move in the up direction
	virtual void MoveUp(float Value);

	/** Event Called to make the character move in the up direction. */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "MoveUp"), Category = "Movement")
		void ReceiveMoveUp(float Value);

	// Called to make the character move in the forward direction of the camera (used while swimming)
	virtual void MoveForwardCamera(float Value);

	/** Event Called to make the character move in the forward direction of the camera (used while swimming). */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "MoveForwardCamera"), Category = "Movement")
		void ReceiveMoveForwardCamera(float Value);

	// Called to make the character move in the right direction of the camera (used while swimming)
	virtual void MoveRightCamera(float Value);

	/** Event Called to make the character move in the right direction of the camera (used while swimming). */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "MoveRightCamera"), Category = "Movement")
		void ReceiveMoveRightCamera(float Value);

	// Called to make the character move in the up direction of the camera (used while swimming)
	virtual void MoveUpCamera(float Value);

	/** Event Called to make the character move in the up direction of the camera (used while swimming). */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "MoveUpCamera"), Category = "Movement")
		void ReceiveMoveUpCamera(float Value);

	/**-----------------	Jump Part		-----------------*/
public:

	/**
	* Used to call action for Jump implemented in c++ and bp
	*/
	UFUNCTION(BlueprintCallable)
		virtual void CallJump();

	/**
	* Used to call action for StopJumping implemented in c++ and bp
	*/
	UFUNCTION(BlueprintCallable)
		virtual void CallStopJumping();

protected:

	/** 
	* It's better to call "CallJump" rather than "Jump" and let call jump called it
	* Will call the character jump
	*/
	virtual void Jump() override;

	/** Event Called to make the character jump. */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "JumpBP"), Category = "Movement")
		void ReceiveJump();

	/**
	* It's better to call "CallStopJumping" rather than "StopJumping" and let call stop jumping called it
	* Will call the character to stop jumping
	*/
	virtual void StopJumping() override;

	/** Event Called to make the character stop jump. */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "StopJumpingBP"), Category = "Movement")
		void ReceiveStopJumping();

	/**-----------------	Orientation Part		-----------------*/
public:

	/**
	* Used to call action for Orient Rotation To Camera implemented in c++ and bp
	*/
	UFUNCTION(BlueprintCallable)
		virtual void CallOrientRotationToCamera(bool bIfInThirdPerson = true);

	/**
	* Used to call action for Allign Yaw With Camera implemented in c++ and bp
	*/
	UFUNCTION(BlueprintCallable)
		virtual void CallAllignYawWithCamera();

protected:

	/**
	* Called to make the character orient to the camera
	* @bIfInThirdPerson make the character orient only if he is in third person -> set by default has true
	* This function is direct action she wont uptade the orientation every frame
	*/
	virtual void OrientRotationToCamera(bool bIfInThirdPerson = true);

	/** Event Called by OrtienRotationToCamera. */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OrientRotationToCamera"), Category = "Camera")
		void ReceiveOrientRotationToCamera(bool bIfInThirdPerson = true);

	/**
	* Called to orrient this character's yaw with the camera's yaw
	* Recommand to use it when the character is in ThirdPerson
	* Will use the @CharCamera
	*/
	virtual void AllignYawWithCamera();

	/** Event Called by allign yaw with camera. */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "AllignYawWithCamera"), Category = "Camera")
		void ReceiveAllignYawWithCamera();

	/**-----------------	Interact Part		-----------------*/
public:

	/**
	* Used to call action for Interact implemented in c++ and bp
	*/
	UFUNCTION(BlueprintCallable)
		virtual void CallInteract();

protected:

	// Called to Interact with a InteractiveComponent
	virtual void Interact();

	/** Event Called to Interact with a InteractiveComponent. */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Interact"), Category = "Interaction")
		void ReceiveInteract();

	/**-----------------	Combat Part		-----------------*/
public:

	/**
	* Used to call action for Attack implemented in c++ and bp
	*/
	UFUNCTION(BlueprintCallable)
		virtual void CallAttack();

protected:

	// Called to Attack with the Combat Component
	virtual void Attack();

	/** Event Called to Attack with the Combat Component. */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Attack"), Category = "Attack")
		void ReceiveAttack();

	/**-----------------	ViewPoint Selection Part		-----------------*/
public:

	/**
	* Used to call action to change view mode
	*/
	UFUNCTION(BlueprintCallable)
		virtual void CallSwapViewMode();

	/**
	* Used to call action for Use First Person implemented in c++ and bp
	*/
	UFUNCTION(BlueprintCallable)
		virtual void CallUseFirstPerson();

	/**
	* Used to call action for Use Third Person implemented in c++ and bp
	*/
	UFUNCTION(BlueprintCallable)
		virtual void CallUseThirdPerson();

protected:

	// Called to set the First Person Components in use for this character
	virtual void UseFirstPerson();

	/** Event Called to set the First Person Components in use for this character. */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "UseFirstPerson"), Category = "Camera")
		void ReceiveUseFirstPerson();

	// Called to set the Third Person Components in use for this character
	virtual void UseThirdPerson();

	/** Event Called to set the First Person Components in use for this character. */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "UseThirdPerson"), Category = "Camera")
		void ReceiveUseThirdPerson();

	/**-----------------	Zoom Part		-----------------*/
public:
	
	/**
	* Used to call action to zoom using the value send
	*/
	UFUNCTION(BlueprintCallable)
		virtual void CallZoomAction(float Value);

protected:


	/** Event Called to change camera boom lengh for this character. */
	virtual void ZoomAction(float Value);

	/** Event Called to change camera boom lengh for this character. */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "ZoomAction"), Category = "Camera")
		void ReceiveZoomAction(float Value);


	/**-----------------	Component Accessor Part		-----------------*/
public:

	/** Returns Interaction Capsule subobject **/
	FORCEINLINE UCapsuleComponent* GetInteractionCapsule() const { return InteractionCapsule; }
	/** Returns First person Camera subobject **/
	FORCEINLINE UCameraComponent* GetCharCamera() const { return CharCamera; }
	/** Returns First person ViewPoint Camera subobject **/
	FORCEINLINE USceneComponent* GetFPViewPointCamera() const { return FPViewPointCamera; }
	/** Returns Third person ViewPoint Camera subobject **/
	FORCEINLINE USceneComponent* GetTPViewPointCamera() const { return TPViewPointCamera; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns First person mesh subobject **/
	FORCEINLINE USkeletalMeshComponent* GetFPMesh() const { return GetMesh(); }
	/** Returns Third person mesh subobject **/
	FORCEINLINE USkeletalMeshComponent* GetTPMesh() const { return TPMesh; }
	/** Returns Used mesh subobject **/
	FORCEINLINE	USkeletalMeshComponent* GetUsedMesh() const { return (IsInFirstPerson() ? GetFPMesh() : GetTPMesh()); }
	/** Returns Used Camera subobject **/
	FORCEINLINE	USceneComponent* GetUsedViewPointCamera() const { return (IsInFirstPerson() ? GetFPViewPointCamera() : GetTPViewPointCamera()); }
	/** Returns Used CombatComponent subobject **/
	FORCEINLINE UCombatComponent_Basic* GetCombatComponent() const { return CombatComponent; }


	/**-----------------	Variable Accessor Part		-----------------*/
public:

	// Called to know if this character use the first person of third person components
	FORCEINLINE bool IsInFirstPerson() const { return bIsInFP; }

};
