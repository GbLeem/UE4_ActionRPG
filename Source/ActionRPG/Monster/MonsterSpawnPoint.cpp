// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterSpawnPoint.h"
#include "Monster.h"

// Sets default values
AMonsterSpawnPoint::AMonsterSpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	SetRootComponent(mRoot);

	mRoot->bVisualizeComponent = true;

	mSpawnMonster = nullptr;
	mSpawnTimeAcc = 0.f;

	//-1�� ��� �ѹ��� ������ �Ѵ�.
	mSpawnTime = -1.f;

	mPatrolType = EPatroltype::Progress;
}

// Called when the game starts or when spawned
void AMonsterSpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
	//ó�� ���۽ÿ��� ���� �̸� �ϳ� ����
	if (IsValid(mMonsterClass))
	{
		FActorSpawnParameters param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		mSpawnMonster = GetWorld()->SpawnActor<AMonster>(mMonsterClass, GetActorLocation(), GetActorRotation(), param);

		//������ ���Ϳ��� SpawnPoint�� �˷��ֱ�
		mSpawnMonster->SetSpawnPoint(this, mPatrolPointArray, mPatrolType);
	}
}

// Called every frame
void AMonsterSpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//���Ͱ� �׾��� ��� ����ؼ� Spawnó���� ���־�� �� ���
	if (mSpawnTime > -1.f)
	{
		//���Ͱ� �׾����� �Ǵ�
		if (mSpawnMonster == nullptr)
		{
			mSpawnTimeAcc += DeltaTime;

			if (mSpawnTimeAcc >= mSpawnTime)
			{
				mSpawnTimeAcc = 0;

				FActorSpawnParameters param;
				param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

				mSpawnMonster = GetWorld()->SpawnActor<AMonster>(mMonsterClass, GetActorLocation(), GetActorRotation(), param);

				//������ ���Ϳ��� SpawnPoint�� �˷��ֱ�
				mSpawnMonster->SetSpawnPoint(this, mPatrolPointArray, mPatrolType);
			}
		}
	}
}

