// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon_Base.generated.h"

class USceneComponent;
class UCapsuleComponent;
class USkeletalMeshComponent;

/**
 * Base for weapon
 * Get Two skeletal Mesh to associate with First Person and Third Person Pawn
 * Can use a viewpoint or a impact point of a other actor
 */
UCLASS()
class EREBE_API AWeapon_Base : public AActor
{
	GENERATED_BODY()
	
		/**-----------------	Component Part		-----------------*/
private:
	/** Capsule  */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;

	/**-----------------	Variable Part		-----------------*/
protected:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	AActor* ActorOwner;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	USceneComponent* OwnerViewpont;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	USceneComponent* OwnerImpactPoint;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bUseOwnerViewpoint = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bUseOwnerImpactpoint = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bUse1P = false;

	/**-----------------	Constructor Part		-----------------*/
public:
	// Sets default values for this actor's properties
	AWeapon_Base(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**-----------------	Inherit Function Part		-----------------*/
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/**-----------------	Weapon_Base Part		-----------------*/
	/**-----------------	Owning Actor Part		-----------------*/
public :
	/**
	 *  Link to an actor this weapon 
	 *  If @NewActorOwner is null don't do anything
	 *  Call DissociatesToActorOwner if we have an actor owner
	 *  Call systematically SetOwnerViewpoint and SetOwnerImpactPoint
	 */
	UFUNCTION(BlueprintCallable)
	virtual void AssociateToActorOwner(AActor* NewActorOwner, USceneComponent* NewOwnerViewpoint = nullptr, USceneComponent* NewOwnerImpactpoint = nullptr);

	/**
	 * Change the current OwnerViewpoint with the new one,
	 * if @NewOwnerViewpont is null @bUseOwnerViewpoint will be set to false
	 */
	UFUNCTION(BlueprintCallable)
	virtual void SetOwnerViewpoint(USceneComponent* NewOwnerViewpoint = nullptr, bool bShouldUseIt = true);

	/**
 	 * Change the current OwnerViewpoint with the new one,
	 * if @NewOwnerViewpont is null @bUseOwnerViewpoint will be set to false
	 */
	UFUNCTION(BlueprintCallable)
	virtual void SetOwnerImpactpoint(USceneComponent* NewOwnerImpactpoint = nullptr, bool bShouldUseIt = true);

	/**
	 * Set the owning actor to null and break all the link with component or actor
	 */
	UFUNCTION(BlueprintCallable)
	virtual void DissociatesToActorOwner();

	/**-----------------	Mesh Part		-----------------*/
public :
	UFUNCTION(BlueprintCallable)
	virtual void AttachMeshToComponent(USceneComponent* NewParentComponent, FName SocketName = NAME_None);

	UFUNCTION(BlueprintCallable)
	virtual void AttachMesh1PToComponent(USceneComponent* NewParentComponent, FName SocketName = NAME_None);

	UFUNCTION(BlueprintCallable)
		virtual void UseMesh1P();

	UFUNCTION(BlueprintCallable)
		virtual void UseMesh3P();

	/**-----------------	Variable Accessor Part		-----------------*/
public :
	FORCEINLINE USceneComponent* GetOwnerViewpoint() { return OwnerViewpont; }

	FORCEINLINE USceneComponent* GetOwnerImpactpoint() { return OwnerImpactPoint; }

	FORCEINLINE void SetShouldUseOwnerViewpoint(bool bNewOwnerViewpoint) { bUseOwnerViewpoint = bNewOwnerViewpoint; }

	FORCEINLINE bool GetShouldUseOwnerViewpoint() const { return bUseOwnerViewpoint; }

	FORCEINLINE void SetShouldUseOwnerImpactpoint(bool bNewOwnerImpactpoint) { bUseOwnerImpactpoint = bNewOwnerImpactpoint; }

	FORCEINLINE bool GetShouldUseOwnerImpactpoint() const { return bUseOwnerImpactpoint; }

	FORCEINLINE	AActor* GetActorOwner() { return ActorOwner; }

	FORCEINLINE bool IsUsing1P() { return bUse1P; }

	/**-----------------	Component Accessor Part		-----------------*/
public :
	/** Return the Mesh Subobject */
	FORCEINLINE USkeletalMeshComponent* GetMesh3P() const { return Mesh; }
	/** Return the Mesh1P Subobject */
	FORCEINLINE USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Return the Mesh Subobject Currently in use */
	FORCEINLINE USkeletalMeshComponent* GetMesh() const { return bUse1P ? Mesh1P : Mesh; }
	/** Return the Capsule Subobject */
	FORCEINLINE UCapsuleComponent* GetCapsuleComponent() const { return CapsuleComponent; }
};
