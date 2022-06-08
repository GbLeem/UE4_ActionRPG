// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_PatrolWait.h"
#include "../Monster/MonsterAIController.h"
#include "../Monster/Monster.h"
#include "../Monster/MonsterAnimInstance.h"

UBTTask_PatrolWait::UBTTask_PatrolWait()
{
	NodeName = TEXT("PatrolWait");
	bNotifyTick = true;

	mWaitTime = 1.f;
	mTime = 0.f;
}

EBTNodeResult::Type UBTTask_PatrolWait::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AMonster* Monster = Cast<AMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Monster)
		return EBTNodeResult::Succeeded;

	if(Monster->GetPatrolPointCount() == 1)
		return EBTNodeResult::Succeeded;

	Monster->GetAnimInstnace()->ChangeAnimType(EmonsterAnim::Idle);
	OwnerComp.GetAIOwner()->StopMovement();
	mTime = 0.f;

	AActor* Target = Cast<AActor>(OwnerComp.GetAIOwner()->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (Target)
	{
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_PatrolWait::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::AbortTask(OwnerComp, NodeMemory);

	return result;
}

void UBTTask_PatrolWait::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AMonster* Monster = Cast<AMonster>(OwnerComp.GetAIOwner()->GetPawn());

	if (!Monster)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}
	AActor* Target = Cast<AActor>(OwnerComp.GetAIOwner()->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (Target)
	{
		mTime = 0.f;
		Monster->GetAnimInstnace()->ChangeAnimType(EmonsterAnim::Idle);

		OwnerComp.GetAIOwner()->StopMovement();

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	//시간을 체크한다.
	mTime += DeltaSeconds;
	if (mTime >= mWaitTime)
	{
		mTime = 0.f;
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}





}
