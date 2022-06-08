// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameInfo.h"
#include "GameFramework/GameModeBase.h"
#include "ActionRPGGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONRPG_API AActionRPGGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	AActionRPGGameModeBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
