// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Trace.h"
#include "../Monster/MonsterAIController.h"
#include "../Monster/Monster.h"
#include "../Monster/MonsterAnimInstance.h"

UBTTask_Trace::UBTTask_Trace()
{
	NodeName = TEXT("Trace");
	bNotifyTick = true;
}
EBTNodeResult::Type UBTTask_Trace::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);
	
	AMonster* Monster = Cast<AMonster>(OwnerComp.GetAIOwner()->GetPawn());

	if (!Monster)
		return EBTNodeResult::Failed;

	AActor* Target = Cast<AActor>(OwnerComp.GetAIOwner()->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (!Target)
	{
		Monster->GetAnimInstnace()->ChangeAnimType(EmonsterAnim::Idle);

		OwnerComp.GetAIOwner()->StopMovement();

		return EBTNodeResult::Failed;
	}

	//타겟 위치로 이동한다.
	UAIBlueprintHelperLibrary::SimpleMoveToActor(OwnerComp.GetAIOwner(), Target);

	Monster->GetAnimInstnace()->ChangeAnimType(EmonsterAnim::Run);

	//애니메이션과 속도 맞추기
	Monster->GetCharacterMovement()->MaxWalkSpeed = Monster->GetMonsterInfo().MoveSpeed;

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_Trace::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::AbortTask(OwnerComp, NodeMemory);

	return result;
}

void UBTTask_Trace::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AMonster* Monster = Cast<AMonster>(OwnerComp.GetAIOwner()->GetPawn());

	if (!Monster)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}
	AActor* Target = Cast<AActor>(OwnerComp.GetAIOwner()->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (!Target)
	{
		Monster->GetAnimInstnace()->ChangeAnimType(EmonsterAnim::Idle);

		OwnerComp.GetAIOwner()->StopMovement();

		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	//몬스터와 Target의 위치를 얻어온다.
	FVector MonsterLoc = Monster->GetActorLocation();
	FVector TargetLoc = Target->GetActorLocation();

	//두 위치 사이의 높이를 맞춰준다,
	MonsterLoc.Z = TargetLoc.Z;

	//두 위치 사이의 거리를 구한다.
	float Distance = FVector::Distance(MonsterLoc, TargetLoc);

	if (Distance <= Monster->GetMonsterInfo().AttackDistance)
	{
		OwnerComp.GetAIOwner()->StopMovement();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}
}
