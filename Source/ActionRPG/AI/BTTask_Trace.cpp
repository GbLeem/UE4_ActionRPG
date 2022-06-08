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

	//Ÿ�� ��ġ�� �̵��Ѵ�.
	UAIBlueprintHelperLibrary::SimpleMoveToActor(OwnerComp.GetAIOwner(), Target);

	Monster->GetAnimInstnace()->ChangeAnimType(EmonsterAnim::Run);

	//�ִϸ��̼ǰ� �ӵ� ���߱�
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

	//���Ϳ� Target�� ��ġ�� ���´�.
	FVector MonsterLoc = Monster->GetActorLocation();
	FVector TargetLoc = Target->GetActorLocation();

	//�� ��ġ ������ ���̸� �����ش�,
	MonsterLoc.Z = TargetLoc.Z;

	//�� ��ġ ������ �Ÿ��� ���Ѵ�.
	float Distance = FVector::Distance(MonsterLoc, TargetLoc);

	if (Distance <= Monster->GetMonsterInfo().AttackDistance)
	{
		OwnerComp.GetAIOwner()->StopMovement();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}
}
