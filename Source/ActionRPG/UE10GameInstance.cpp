// Fill out your copyright notice in the Description page of Project Settings.


#include "UE10GameInstance.h"

UUE10GameInstance::UUE10GameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> MonsterTableAsset(TEXT("DataTable'/Game/Monster/DTMonsterInfo.DTMonsterInfo'"));

	if (MonsterTableAsset.Succeeded())
		mMonsterInfoTable = MonsterTableAsset.Object;


	static ConstructorHelpers::FObjectFinder<UDataTable> PlayerTableAsset(TEXT("DataTable'/Game/Player/DTPlayerInfo.DTPlayerInfo'"));

	if (PlayerTableAsset.Succeeded())
		mPlayerInfoTable = PlayerTableAsset.Object;
}

void UUE10GameInstance::init()
{
	Super::Init();
}

const FMonsterDataTableInfo* UUE10GameInstance::FindMonsterInfo(const FString& Name)
{
	return mMonsterInfoTable->FindRow<FMonsterDataTableInfo>(*Name, TEXT(""));
}

const FPlayerDataTableInfo* UUE10GameInstance::FindPlayerInfo(const FString& Name)
{
	return mPlayerInfoTable->FindRow<FPlayerDataTableInfo>(*Name, TEXT(""));

}