// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayerCharacter.h"
#include "MainCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONRPG_API AMainCharacter : public APlayerCharacter
{
	GENERATED_BODY()
public:
	AMainCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void Attack();
	virtual void Skill1();
	virtual void Skill2();
	virtual void Skill3();
	virtual void Skill4();
	virtual void NormalAttack();
};
