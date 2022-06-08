// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Animation/AnimInstance.h"
#include "MonsterAnimInstance.generated.h"


UENUM(BlueprintType)
enum class EmonsterAnim : uint8
{
	Idle,
	Walk,
	Run,
	Attack,
	Death,
	Stun,
	Skill1,
	Skill2,
	Skill3,
	Skill4
};

UCLASS()
class ACTIONRPG_API UMonsterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UMonsterAnimInstance();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EmonsterAnim mAnimType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float mGround;

public:
	EmonsterAnim GetAnimType() const
	{
		return mAnimType;
	}

	UFUNCTION(BlueprintCallable)
	void ChangeAnimType(EmonsterAnim Type)
	{
		mAnimType = Type;
	}

public:
	virtual void NativeInitializeAnimation();
	virtual void NativeUpdateAnimation(float DeltaSeconds);

public:
	UFUNCTION()
	void AnimNotify_DeathEnd();
	UFUNCTION()
	void AnimNotify_AttackEnd();
	UFUNCTION()
	void AnimNotify_Attack();
};
