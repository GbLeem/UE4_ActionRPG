// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "PlayerCharacter.h"

UPlayerAnimInstance::UPlayerAnimInstance()
	:mSpeed(0.f)
	,mDir(0.f)
	,mAnimType(EPlayerAnim::Ground)
	,mFallRecoveryAdditive(0.f)
	,mCheckState(false)
{

}

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

//�ǽð����� ȣ��ȴ�
void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	//ù ������ ���� üũ
	if (!mCheckState)
	{
		mCheckState = true;
		return;
	}

	//�� �ִ� �ν��Ͻ��� ����ϴ� �÷��̾� ĳ���� ��ü�� ���´�
	//TryGetPawnOwner : �� �ִ� �ν��Ͻ� ��ü�� �����ؼ� ������ �ִ� Pawn�� ���´�.
	//APawnŸ������ ��ȯ�ϹǷ� ����ȯ �ʿ�
	APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (IsValid(Player))
	{
		//CharacterMovementComponent�� �̿��Ͽ� �̵��� �����ϹǷ� CharacterMovementComponent�� �������ִ�
		//������ �̵��ӵ� ������ �� �ִ�.
		UCharacterMovementComponent* Movement = Player->GetCharacterMovement();

		mSpeed = Movement->Velocity.Size();

		mGround = Movement->IsMovingOnGround();

		//���߿� ���ִ� ������ ��� Jump�ƴϸ� Fall ���� �ϳ��� �����̴�.
		//�׷��Ƿ� Jump ���� �ƴϸ� Fall �� ������ֱ�
		if (!mGround && mAnimType != EPlayerAnim::Jump)
		{
			mAnimType = EPlayerAnim::Fall;
			mFallRecoveryAdditive = 0.f;
		}
		else if (mGround && (mAnimType == EPlayerAnim::Fall || mAnimType == EPlayerAnim::Jump))
		{
			mAnimType = EPlayerAnim::Ground;
			mFallRecoveryAdditive = 1.f;

			//FallRecovery ��Ÿ�ָ� ������� �ش�.
			Player->PlayFallRecovery();
		}
	}
}

void UPlayerAnimInstance::AnimNotify_Attack()
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());
	
	if (IsValid(Player))
		Player->NormalAttack();
}

void UPlayerAnimInstance::AnimNotify_AttackCombo()
{
	//LOG(TEXT("AttackCombo"));
	APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (IsValid(Player))
		Player->EnableAttack(true);
}

void UPlayerAnimInstance::AnimNotify_AttackEnd()
{
	//LOG(TEXT("AttackEnd"));
	APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());
	
	if (IsValid(Player))
		Player->AttackEnd();
}

void UPlayerAnimInstance::AnimNotify_JumpApexEnd()
{
	mAnimType = EPlayerAnim::Fall;	
}

void UPlayerAnimInstance::AnimNotify_FallRecoveryEnd()
{
	mFallRecoveryAdditive = 0.f;
}

void UPlayerAnimInstance::AnimNotify_Skill()
{
	//����ؾ� �� ��ų�� � ��ų���� �Ǵ��Ѵ�.
	APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (IsValid(Player))
		Player->UseSkill();
}

void UPlayerAnimInstance::AnimNotify_SkillEnd()
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (IsValid(Player))
		Player->ClearSkillType();
}
void UPlayerAnimInstance::AnimNotify_DeathEnd()
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (IsValid(Player))
		Player->Death();
}
