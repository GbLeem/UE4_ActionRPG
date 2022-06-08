// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAnimInstance.h"
#include "Monster.h"

UMonsterAnimInstance::UMonsterAnimInstance()
	:mAnimType(EmonsterAnim::Idle) 
{

}

void UMonsterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

//실시간으로 호출된다
void UMonsterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	AMonster* Monster = Cast<AMonster>(TryGetPawnOwner());

	if (IsValid(Monster))
	{
		UCharacterMovementComponent* Movement = Monster->GetCharacterMovement();
		mGround = Movement->IsMovingOnGround();
	}
}
void UMonsterAnimInstance::AnimNotify_DeathEnd()
{
	AMonster* Monster = Cast<AMonster>(TryGetPawnOwner());

	if(IsValid(Monster))
		Monster->Death();
}
void UMonsterAnimInstance::AnimNotify_AttackEnd()
{
	AMonster* Monster = Cast<AMonster>(TryGetPawnOwner());

	if (IsValid(Monster))
		Monster->SetAttackEnd(true);
}

void UMonsterAnimInstance::AnimNotify_Attack()
{
	AMonster* Monster = Cast<AMonster>(TryGetPawnOwner());

	if (IsValid(Monster))
		Monster->Attack();
}
