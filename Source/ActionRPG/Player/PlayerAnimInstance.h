// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

UENUM(BlueprintType)
enum class EPlayerAnim : uint8
{
	Ground,
	Jump,
	Fall,
	Death,
};

UCLASS()
class ACTIONRPG_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPlayerAnimInstance();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EPlayerAnim mAnimType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float mSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float mDir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float mFallRecoveryAdditive;

	//땅을 밟고 있는지를 체크하는 변수이다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float mGround;

	bool mCheckState;
public:
	void SetMoveDir(float Dir)
	{
		mDir = Dir;
	}

	EPlayerAnim GetAnimType() const
	{
		return mAnimType;
	}

	UFUNCTION(BlueprintCallable)
	void ChangeAnimType(EPlayerAnim Type)
	{
		mAnimType = Type;
	}
public:
	virtual void NativeInitializeAnimation();
	virtual void NativeUpdateAnimation(float DeltaSeconds);

public:
	UFUNCTION()
	void AnimNotify_Attack();

	UFUNCTION()
	void AnimNotify_AttackCombo();

	UFUNCTION()
	void AnimNotify_AttackEnd();

	UFUNCTION()
	void AnimNotify_JumpApexEnd();

	UFUNCTION()
	void AnimNotify_FallRecoveryEnd();

	UFUNCTION()
		void AnimNotify_Skill();

	UFUNCTION()
		void AnimNotify_SkillEnd();

	UFUNCTION()
		void AnimNotify_DeathEnd();
};
