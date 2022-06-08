// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UENUM(BlueprintType)
enum class EPlayerSkill : uint8
{
	Skill1,
	Skill2,
	Skill3,
	Skill4,
	None = 255
};

UCLASS()
class ACTIONRPG_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* mCameraArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* mCamera;

	//메쉬가 생성해둔 애니메이션 인스턴스 객체를 얻어와서 저장할 포인터 변수 생성
	class UPlayerAnimInstance* mAnim;

	float mMoveDir;

	//공격용 애니메이션 몽타주 저장
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> mAttackMontageArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* mFallRecoveryMontage;

	//스킬 애니메이션 몽타주 저장
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> mSkillMontageArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EPlayerSkill mUseSkillType;

	int32 mAttackMontageIndex;

	//현재 공격이 가능한 상태인지를 판단하는 변수
	bool mAttackEnable;

	//죽은 상태인지를 판단하는 변수
	bool mDeath;

	FString mDataTablekey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FPlayerInfo mInfo;

public:
	const FPlayerInfo& GetPlayerInfo() const
	{
		return mInfo;
	}

public:
	void EnableAttack(bool Enable)
	{
		mAttackEnable = Enable;
	}

	void ClearSkillType()
	{
		mUseSkillType = EPlayerSkill::None;
	}
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

public:
	void MoveFront(float Scale);
	void MoveRight(float Scale);
	void RotationYaw(float Scale);

public:
	void AttackKey();
	void JumpKey();
	void PlayFallRecovery();
	void Skill1Key();
protected:
	virtual void Attack();
	virtual void Skill1();
	virtual void Skill2();
	virtual void Skill3();
	virtual void Skill4();

public:
	virtual void AttackEnd();
	virtual void UseSkill();
	virtual void NormalAttack();

public:
	void Death();
};
