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

//실시간으로 호출된다
void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	//첫 프레임 상태 체크
	if (!mCheckState)
	{
		mCheckState = true;
		return;
	}

	//이 애님 인스턴스를 사용하는 플레이어 캐릭터 객체를 얻어온다
	//TryGetPawnOwner : 이 애님 인스턴스 객체를 생성해서 가지고 있는 Pawn을 얻어온다.
	//APawn타입으로 반환하므로 형변환 필요
	APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (IsValid(Player))
	{
		//CharacterMovementComponent를 이용하여 이동을 진행하므로 CharacterMovementComponent가 가지고있는
		//현재의 이동속도 가져올 수 있다.
		UCharacterMovementComponent* Movement = Player->GetCharacterMovement();

		mSpeed = Movement->Velocity.Size();

		mGround = Movement->IsMovingOnGround();

		//공중에 떠있는 상태일 경우 Jump아니면 Fall 둘중 하나의 상태이다.
		//그러므로 Jump 상태 아니면 Fall 로 만들어주기
		if (!mGround && mAnimType != EPlayerAnim::Jump)
		{
			mAnimType = EPlayerAnim::Fall;
			mFallRecoveryAdditive = 0.f;
		}
		else if (mGround && (mAnimType == EPlayerAnim::Fall || mAnimType == EPlayerAnim::Jump))
		{
			mAnimType = EPlayerAnim::Ground;
			mFallRecoveryAdditive = 1.f;

			//FallRecovery 몽타주를 재생시켜 준다.
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
	//사용해야 할 스킬이 어떤 스킬인지 판단한다.
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
