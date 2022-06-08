// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "../Monster/MonsterAIController.h"
#include "../Monster/Monster.h"
#include "../Monster/MonsterAnimInstance.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = TEXT("Attack");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AMonster* Monster = Cast<AMonster>(OwnerComp.GetAIOwner()->GetPawn());

	if (!Monster)
		return EBTNodeResult::Succeeded;

	AActor* Target = Cast<AActor>(OwnerComp.GetAIOwner()->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (!Target)
	{
		Monster->GetAnimInstnace()->ChangeAnimType(EmonsterAnim::Idle);

		OwnerComp.GetAIOwner()->StopMovement();

		return EBTNodeResult::Succeeded;
	}

	//공격 모션으로 바꾼다
	Monster->GetAnimInstnace()->ChangeAnimType(EmonsterAnim::Attack);

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_Attack::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::AbortTask(OwnerComp, NodeMemory);

	return result;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AMonster* Monster = Cast<AMonster>(OwnerComp.GetAIOwner()->GetPawn());

	if (!Monster)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}
	AActor* Target = Cast<AActor>(OwnerComp.GetAIOwner()->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (!Target)
	{
		Monster->GetAnimInstnace()->ChangeAnimType(EmonsterAnim::Idle);

		OwnerComp.GetAIOwner()->StopMovement();

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	// < 타겟도 있고 몬스터도 정상적일때 > 
	
	//몬스터와 Target의 위치를 얻어온다.
	FVector MonsterLoc = Monster->GetActorLocation();
	FVector TargetLoc = Target->GetActorLocation();

	//두 위치 사이의 높이를 맞춰준다,
	MonsterLoc.Z = TargetLoc.Z;

	//두 위치 사이의 거리를 구한다.
	float Distance = FVector::Distance(MonsterLoc, TargetLoc);

	if (Monster->GetAttackEnd())
	{
		if (Distance > Monster->GetMonsterInfo().AttackDistance)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		}
		else
		{
			//몬스터가 타겟을 바라보는 방향을 만든다.
			FVector Dir = TargetLoc - MonsterLoc;
			Dir.Normalize();

			Monster->SetActorRotation(FRotator(0.f, Dir.Rotation().Yaw, 0.f));
		}
		Monster->SetAttackEnd(false);
	}
}
