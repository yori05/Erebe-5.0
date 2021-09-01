// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ammo_Base.generated.h"

class UStaticMeshComponent;
class USceneComponent;
class UProjectileMovementComponent;
class AWeapon_Shooting_Base;
class USphereComponent;

UCLASS()
class EREBE_API AAmmo_Base : public AActor
{
	GENERATED_BODY()
	
	/**-----------------	Component Part		-----------------*/
private :
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	USceneComponent* SceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
		USphereComponent* SphereCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UProjectileMovementComponent* MovementComponent;


	/**-----------------	Variable Part		-----------------*/
protected :
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 ProjectileIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AWeapon_Shooting_Base* WeaponOwner;


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
	UFUNCTION(BlueprintCallable)
		virtual void OnSphereColliderBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	/**-----------------	Projectile Function Part		-----------------*/
public :
	UFUNCTION(BlueprintCallable)
	virtual void Inactive(bool bHide = true);

	UFUNCTION(BlueprintCallable)
	virtual void Active(bool bShow = true);

	UFUNCTION(BlueprintCallable)
	virtual void Hide();

	UFUNCTION(BlueprintCallable)
	virtual void Show();


	/**-----------------	Variable Accessor Part			-----------------*/
public : 
	
	FORCEINLINE	void SetProjectileIndex(int32 NewProjectileIndex) { ProjectileIndex = NewProjectileIndex; }

	FORCEINLINE	int32 GetProjectileIndex() { return ProjectileIndex; }

	FORCEINLINE	void SetWeaponOwner(AWeapon_Shooting_Base* NewWeaponOwner) { WeaponOwner = NewWeaponOwner; }

	FORCEINLINE	AWeapon_Shooting_Base* GetWeaponOwner() { return WeaponOwner; }


	/**-----------------	Component Accessor Part		-----------------*/
public :
	/** Return the Mesh Subobject */
	FORCEINLINE UStaticMeshComponent* GetStaticMeshComponent() const { return StaticMesh; }
	/** Return the SceneRoot Subobject */
	FORCEINLINE USceneComponent* GetSceneRootComponent() const { return SceneRoot; }
	/** Return sphere collider  subobject */
	FORCEINLINE USphereComponent* GetSphereColliderComponent() { return SphereCollider; }

};
