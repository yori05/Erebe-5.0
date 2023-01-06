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

/**
 *  Character_Basic
 *	Character used by the player with a PlayerController_Basic
 *  Should be used to make child if need default movement
 *	Can move, jump
 *  Contain two camera and two skeletal mesh (for the first and third person viewpoint) + one spring arm for the third person camera
 *  The default viewpoint is set as a First Person
 *  The First person skeletal mesh is attach to the First person camera if you want it in a other way you have to change it the child constructor
 *  The Third Person skeletal mesh is attach the root and the SpringArm of the Third Person Camera is attach to the Third person Skeletal mesh
*/
UCLASS(config = Game)
class EREBE_API ACharacter_Basic : public ACharacter
{
	GENERATED_BODY()

/**-----------------	Component Part		-----------------*/
private:

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

	/** Check if the character is in first person or third */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FirstP/ThirdP", meta = (AllowPrivateAccess = "true"))
		bool bIsInFP;

/**-----------------	Constructor Part		-----------------*/
public:

	// Sets default values for this character's properties
	ACharacter_Basic(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


/**-----------------	Inherit Function Part		-----------------*/
protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

/**-----------------	Character_Basic Part		-----------------*/
/**-----------------	Action Part		-----------------*/
public:

	// Called to make the character move in the forward direction
	UFUNCTION(BlueprintCallable)
		void MoveForward(float Value);

	// Called to make the character move in the right direction
	UFUNCTION(BlueprintCallable)
		void MoveRight(float Value);

	// Called to make the character move in the forward direction of the camera (used while swimming)
	UFUNCTION(BlueprintCallable)
		void MoveForwardCamera(float Value);

	// Called to make the character move in the right direction of the camera (used while swimming)
	UFUNCTION(BlueprintCallable)
		void MoveRightCamera(float Value);

	// Called to make the character jump
	virtual void Jump() override;

	// Called to make the character stop jump
	virtual void StopJumping() override;

	/** 
	* Called to make the character orient to the camera
	* @bIfInThirdPerson make the character orient only if he is in third person -> set by default has true 
	* This function is direct action she wont uptade the orientation every frame
	*/
	UFUNCTION(BlueprintCallable)
	virtual void OrientRotationToCamera(bool bIfInThirdPerson = true);

	/**
	* Called to orrient this character's yaw with the camera's yaw
	* Recommand to use it when the character is in ThirdPerson
	* Will use the @CharCamera
	*/
	UFUNCTION(BlueprintCallable)
	virtual void AllignYawWithCamera();

/**-----------------	Combat Part		-----------------*/
	// Called to Attack with the Combat Component
	UFUNCTION(BlueprintCallable)
	virtual void Attack();

/**-----------------	ViewPoint Selection Part		-----------------*/
public:
	// Called to set the First Person Components in use for this character
	UFUNCTION(BlueprintCallable)
		void UseFirstPerson();

	// Called to set the Third Person Components in use for this character
	UFUNCTION(BlueprintCallable)
		void UseThirdPerson();

	// Called to know if this character use the first person of third person components
	UFUNCTION(BlueprintCallable)
		bool GetIsInFirstPerson() const;


/**-----------------	Component Accessor Part		-----------------*/
public:

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
	FORCEINLINE	USkeletalMeshComponent* GetUsedMesh() const {	return (GetIsInFirstPerson() ? GetFPMesh() : GetTPMesh());	}
	/** Returns Used Camera subobject **/
	FORCEINLINE	USceneComponent* GetUsedViewPointCamera() const {	return (GetIsInFirstPerson() ? GetFPViewPointCamera() : GetTPViewPointCamera()); }
	/** Returns Used CombatComponent subobject **/
	FORCEINLINE UCombatComponent_Basic* GetCombatComponent() const { return CombatComponent; }

};

FORCEINLINE bool ACharacter_Basic::GetIsInFirstPerson() const
{
	return bIsInFP;
}