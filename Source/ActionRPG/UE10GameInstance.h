// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameInfo.h"
#include "Engine/GameInstance.h"
#include "UE10GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONRPG_API UUE10GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UUE10GameInstance();
	
private:
	UPROPERTY()
	UDataTable* mPlayerInfoTable;

	UPROPERTY()
	UDataTable* mMonsterInfoTable;

public:
	virtual void init();

public:
	const FMonsterDataTableInfo* FindMonsterInfo(const FString& Name);
	const FPlayerDataTableInfo* FindPlayerInfo(const FString& Name);
};
