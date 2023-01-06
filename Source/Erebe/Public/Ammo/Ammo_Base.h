// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CollisionQueryParams.h"
#include "Ammo_Base.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAmmoHitValidActor, AActor*, AmmoHit, const FHitResult&, HitResult);

class UStaticMeshComponent;
class UProjectileMovementComponent;
class AWeapon_Shooting_Base;
class USphereComponent;

UCLASS()
class EREBE_API AAmmo_Base : public AActor
{
	GENERATED_BODY()
	
	/**-----------------	Component Part		-----------------*/
private :
	// Static mesh of this projectile 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMesh;

	// Collier used to handle collision on the projectile
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USphereComponent* SphereCollider;

	// Simple projectile movement component to move this projectile
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* MovementComponent;


	/**-----------------	Variable Part		-----------------*/
protected :
	// Index of the projectile in the weapon pool
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 ProjectileIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PhysicsInteration = 1;

	// Weapon that shot and possess this projectile
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AWeapon_Shooting_Base* WeaponOwner;

	// Current velocity of the projectile
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FVector Velocity;

	// Initial velocity of the projectile
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector InitialVelocity;

	FCollisionQueryParams CollisionQueryParams;

	// Array of every hit touch by the projectile from his last movement
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FHitResult HitMovingCollision;

	// scale of the gravity apply to this projectile
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GravityScale = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SweepTestRadius = 16.f;

	// should use gravity on this projectile (value of 9.81)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseGravity = false;

	// Can the projectile can move 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bCanMove = false;

	// Can the projectile stay active after hit a valid actor 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bInactiveOnHit = true;

	// Can the projectile stick to the actor hit 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bStickToActorOnHit = true;


	/**-----------------	Callback Declaration Part		-----------------*/
public :
	/** Called in the AmmoHitValideActor function */
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FAmmoHitValidActor OnAmmoHitValidActor;

	/**-----------------	Inherit Function Part		-----------------*/
public:	
	// Sets default values for this actor's properties
	AAmmo_Base(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	

	/**-----------------	Callback Implementation Part	-----------------*/
protected:
	// Implementation of the action to take if the projectile overlap with a actor
	UFUNCTION(BlueprintCallable)
		virtual void OnSphereColliderBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/**-----------------	Projectile Function Part		-----------------*/
public :
	// Define the action to take when the projectile is deactivate (can hide the projectile if necessary)
	UFUNCTION(BlueprintCallable)
	virtual void Inactive(bool bHide = true);

	// Define the action to take when the projectile is activate (can show the projectile if necessary)
	UFUNCTION(BlueprintCallable)
	virtual void Active(bool bShow = true);

	// Hide the mesh of the projectile (override this function if you have other mesh / fx to hide)
	UFUNCTION(BlueprintCallable)
	virtual void Hide();

	// Show the mesh of the projectile (override this function if you have other mesh / fx to show)
	UFUNCTION(BlueprintCallable)
	virtual void Show();

	// Reset the ammo to a default state (speed location etc)
	UFUNCTION(BlueprintCallable)
	virtual void Reset();

	// Associate the ammo to a weapon to make the weapon the ammo's owner
	UFUNCTION(BlueprintCallable)
	virtual void AssociateToWeapon(class AWeapon_Shooting_Base* NewWeaponOwner, int32 NewProjectileIndex);
protected:

	// Define the behavior of the projectile when he hit a valid actor
	UFUNCTION(BlueprintCallable)
		virtual void HitValideActor(const FHitResult &HitResult);

	UFUNCTION(BlueprintCallable)
	virtual void HandleCollision();
	/**-----------------	Variable Accessor Part			-----------------*/
public : 
	
	// Set the index of this projectile
	FORCEINLINE	void SetProjectileIndex(int32 NewProjectileIndex) { ProjectileIndex = NewProjectileIndex; }
	// Return the projectile index
	FORCEINLINE	int32 GetProjectileIndex() { return ProjectileIndex; }
	// Set the weapon owner of this projectile
	FORCEINLINE	void SetWeaponOwner(AWeapon_Shooting_Base* NewWeaponOwner) { WeaponOwner = NewWeaponOwner; }
	// Get the weapon owner of this projectile
	FORCEINLINE	AWeapon_Shooting_Base* GetWeaponOwner() { return WeaponOwner; }


	/**-----------------	Component Accessor Part		-----------------*/
public :
	/** Return the Mesh Subobject */
	FORCEINLINE UStaticMeshComponent* GetStaticMeshComponent() const { return StaticMesh; }
	/** Return sphere collider  subobject */
	FORCEINLINE USphereComponent* GetSphereColliderComponent() { return SphereCollider; }

};
