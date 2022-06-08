// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Patrol.h"
#include "../Monster/MonsterAIController.h"
#include "../Monster/Monster.h"
#include "../Monster/MonsterAnimInstance.h"

UBTTask_Patrol::UBTTask_Patrol()
{
	NodeName = TEXT("Patrol");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AMonster* Monster = Cast<AMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Monster)
		return EBTNodeResult::Succeeded;

	if (Monster->GetPatrolPointCount() == 1)
		return EBTNodeResult::Succeeded;

	AActor* Target = Cast<AActor>(OwnerComp.GetAIOwner()->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (Target)
		return EBTNodeResult::Succeeded;

	Monster->GetAnimInstnace()->ChangeAnimType(EmonsterAnim::Walk);

	//애니메이션속도와 이동 속도 맞추기 위한 작업 + 애님 블프에서 Play Rate 바꿔도 된다.
	Monster->GetCharacterMovement()->MaxWalkSpeed = Monster->GetMonsterInfo().MoveSpeed * 0.5f;

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_Patrol::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::AbortTask(OwnerComp, NodeMemory);

	return result;
}

void UBTTask_Patrol::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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
		Monster->GetAnimInstnace()->ChangeAnimType(EmonsterAnim::Idle);

		OwnerComp.GetAIOwner()->StopMovement();

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	FVector Point = Monster->GetPatrolPoint();
	FVector MonsterLoc = Monster->GetActorLocation();

	UAIBlueprintHelperLibrary::SimpleMoveToLocation(OwnerComp.GetAIOwner(), Point);
	

	float CapsuleHalfHeight = Monster->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	float CapsuleRadius = Monster->GetCapsuleComponent()->GetScaledCapsuleRadius();

	MonsterLoc.Z -= CapsuleHalfHeight;
	float Distance = FVector::Distance(Point, MonsterLoc);

	if (Distance <= CapsuleRadius + 10.f)
	{
		Monster->NextPatrolPoint();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
