// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_DetectTarget.h"
#include "../Monster/Monster.h"
#include "../Monster/MonsterAIController.h"

UBTService_DetectTarget::UBTService_DetectTarget()
{
	NodeName = TEXT("Detect");
	
	//Ư�� ��� ���� ������ ������ 0.5�� ������ ������ ������ ����
	Interval = 0.5f;
}

void UBTService_DetectTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AMonsterAIController* Controller = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());

	if (!Controller)
		return;

	//GetPawn  : �� AI Controller�� ���ǵǾ� �ִ� Pawn�� ���´�.
	AMonster* Monster = Cast<AMonster>(Controller->GetPawn());

	if (!Monster)
		return;

	const FMonsterInfo& Info = Monster->GetMonsterInfo();

	//�ֺ��� �÷��̾ �ִ��� �Ǵ��Ѵ�.
	FHitResult result;
	FCollisionQueryParams params(NAME_None, false, Monster);

	bool Hit = GetWorld()->SweepSingleByChannel(result, Monster->GetActorLocation(), Monster->GetActorLocation(), FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel5, FCollisionShape::MakeSphere(Info.TraceDistance), params);

#if ENABLE_DRAW_DEBUG

	FColor DrawColor = Hit ? FColor::Red : FColor::Green;

	DrawDebugSphere(GetWorld(), Monster->GetActorLocation(), Info.TraceDistance, 20, DrawColor, false, 0.3f);
#endif

	if (Hit)
		Controller->GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), result.GetActor());
	else
		Controller->GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), nullptr);
}