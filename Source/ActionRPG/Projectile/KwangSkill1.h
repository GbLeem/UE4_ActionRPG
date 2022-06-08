// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectileBase.h"
#include "KwangSkill1.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONRPG_API AKwangSkill1 : public AProjectileBase
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AKwangSkill1();

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
