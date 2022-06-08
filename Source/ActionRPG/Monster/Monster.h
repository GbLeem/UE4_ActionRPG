// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "MonsterAnimInstance.h"
#include "GameFramework/Character.h"
#include "Monster.generated.h"

UCLASS()
class ACTIONRPG_API AMonster : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMonster();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FMonsterInfo mInfo;

	TArray<FVector> mPatrolPointArray;
	EPatroltype mPatrolType;
	int32 mPatrolIndex;
	int32 mPatrolNext;

	class UMonsterAnimInstance* mAnim;
	
	class AMonsterSpawnPoint* mSpawnPoint;

	FString mDataTablekey;

	bool mAttackEnd;

public:

	int32 GetPatrolPointCount() const
	{
		return mPatrolPointArray.Num();
	}

	FVector GetPatrolPoint()
	{
		return mPatrolPointArray[mPatrolIndex];
	}

	bool GetAttackEnd() const
	{
		return mAttackEnd;
	}

	void SetAttackEnd(bool End)
	{
		mAttackEnd = End;
	}
	class UMonsterAnimInstance* GetAnimInstnace() const
	{
		return mAnim;
	}

	const FMonsterInfo& GetMonsterInfo() const
	{
		return mInfo;
	}

public:
	void SetSpawnPoint(class AMonsterSpawnPoint* Point, 
		const TArray<class APatrolPoint*>& PatrolPointArray,
		EPatroltype PatrolType);
	void NextPatrolPoint();

	/*void ChangeAnimType(EmonsterAnim AnimType)
	{
		mAnim->ChangeAnimType(AnimType);
	}*/

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

public:
	void Death();

public:
	virtual void Attack();
};
