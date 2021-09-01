// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon_Base.generated.h"

class USceneComponent;
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
	USceneComponent* SceneRoot;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh;
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
	virtual void SetOwnerViewpoint(USceneComponent* NewOwnerViewpoint);

	UFUNCTION(BlueprintCallable)
	virtual USceneComponent* GetOwnerViewpoint();

	UFUNCTION(BlueprintCallable)
	virtual void SetOwnerImpactpoint(USceneComponent* NewOwnerImpactpoint);

	UFUNCTION(BlueprintCallable)
	virtual USceneComponent* GetOwnerImpactpoint();

public :

	FORCEINLINE void SetShouldUseOwnerViewpoint(bool bNewOwnerViewpoint) { bUseOwnerViewpoint = bNewOwnerViewpoint; }

	FORCEINLINE bool GetShouldUseOwnerViewpoint() const { return bUseOwnerViewpoint; }

	FORCEINLINE void SetShouldUseOwnerImpactpoint(bool bNewOwnerImpactpoint) { bUseOwnerImpactpoint = bNewOwnerImpactpoint; }

	FORCEINLINE bool GetShouldUseOwnerImpactpoint() const { return bUseOwnerImpactpoint; }

public :
	/** Return the Mesh Subobject */
	FORCEINLINE USkeletalMeshComponent* GetSkeletalMeshComponent() const { return Mesh; }
	/** Return the DefaultSceneRoot Subobject */
	FORCEINLINE USceneComponent* GetSceneRootComponent() const { return SceneRoot; }
};
