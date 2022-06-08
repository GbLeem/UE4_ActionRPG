// Copyright Epic Games, Inc. All Rights Reserved.


#include "ActionRPGGameModeBase.h"

AActionRPGGameModeBase::AActionRPGGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;

	//����Ʈ �� Ŭ������ ���ϴ� Ŭ������ �������ش�.
	//�������Ʈ Ŭ������ �����ö� include�� �ҷ����� �Ұ���
	//FClassFinder �� �̿��ؼ� ���ϴ� Ŭ���� Ÿ���� ���� �� �ִ�.(����� �� ���� C�ٿ��ֱ�)
	static ConstructorHelpers::FClassFinder<APawn>PlayerPawn(TEXT("Blueprint'/Game/Player/MainCharacter/BPMainCharacter.BPMainCharacter_C'"));

	//������ �о�� Ŭ������ Default�� �����Ͽ� ���� ���۽� ���ǵ� ���� ������ �� �ִ�.
	if (PlayerPawn.Succeeded())
		DefaultPawnClass = PlayerPawn.Class;

}

void AActionRPGGameModeBase::BeginPlay()
{
	Super::BeginPlay();

}

void AActionRPGGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
