// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "ProjectileBase.h"
#include "MinionRangerBullet.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONRPG_API AMinionRangerBullet : public AProjectileBase
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AMinionRangerBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void ProjectileStop(const FHitResult& ImpactResult);
	virtual void ProjectileBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity);
};
