// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

UCLASS()
class ACTIONRPG_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileBase();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USphereComponent* mSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* mMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* mParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* mProjectile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float mDistance;

	AController* mOwnerController;

	FVector mPrevLoc;

	float mDamage;

	AActor* mTarget;

public:
	void SetTarget(AActor* Target)
	{
		mTarget = Target;
	}

	void SetDamage(float Damage)
	{
		mDamage = Damage;
	}

	void SetOwnerController(AController* Controller)
	{
		mOwnerController = Controller;
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
	void CollisionBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity);

	UFUNCTION()
	void CollisionStop(const FHitResult& ImpactResult);

protected:
	virtual void ProjectileStop(const FHitResult& ImpactResult);
	virtual void ProjectileBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity);

};
