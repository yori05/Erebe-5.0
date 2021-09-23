// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon_Base.generated.h"

class USceneComponent;
class UCapsuleComponent;
class USkeletalMeshComponent;

/**
 * Universal base for weapon
 */
UCLASS()
class EREBE_API AWeapon_Base : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;

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
public:
	// Sets default values for this actor's properties
	AWeapon_Base(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetActorOwner(AActor* NewActorOwner);

	UFUNCTION(BlueprintCallable)
	AActor* GetActorOwner();

	UFUNCTION(BlueprintCallable)
	virtual void SetOwnerViewpoint(USceneComponent* NewOwnerViewpoint = nullptr, bool bShouldUseIt = true);

	UFUNCTION(BlueprintCallable)
	virtual USceneComponent* GetOwnerViewpoint();

	UFUNCTION(BlueprintCallable)
	virtual void SetOwnerImpactpoint(USceneComponent* NewOwnerImpactpoint = nullptr, bool bShouldUseIt = true);

	UFUNCTION(BlueprintCallable)
	virtual USceneComponent* GetOwnerImpactpoint();

	UFUNCTION(BlueprintCallable)
	virtual void AssociateToActorOwner(AActor* NewActorOwner, USceneComponent* NewOwnerViewpoint = nullptr, USceneComponent* NewOwnerImpactpoint = nullptr);

	UFUNCTION(BlueprintCallable)
	virtual void DissociatesToActorOwner();

	UFUNCTION(BlueprintCallable)
	virtual void AttachMeshToComponent(USceneComponent* NewParentComponent, FName SocketName = NAME_None);

	UFUNCTION(BlueprintCallable)
	virtual void AttachMesh1PToComponent(USceneComponent* NewParentComponent, FName SocketName = NAME_None);

	UFUNCTION(BlueprintCallable)
		virtual void UseMesh1P();

	UFUNCTION(BlueprintCallable)
		virtual void UseMesh3P();

	UFUNCTION(BlueprintCallable)
		virtual bool IsUsing1P();


public :

	FORCEINLINE void SetShouldUseOwnerViewpoint(bool bNewOwnerViewpoint) { bUseOwnerViewpoint = bNewOwnerViewpoint; }

	FORCEINLINE bool GetShouldUseOwnerViewpoint() const { return bUseOwnerViewpoint; }

	FORCEINLINE void SetShouldUseOwnerImpactpoint(bool bNewOwnerImpactpoint) { bUseOwnerImpactpoint = bNewOwnerImpactpoint; }

	FORCEINLINE bool GetShouldUseOwnerImpactpoint() const { return bUseOwnerImpactpoint; }

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
