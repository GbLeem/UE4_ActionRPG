// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_DetectTarget.h"
#include "../Monster/Monster.h"
#include "../Monster/MonsterAIController.h"

UBTService_DetectTarget::UBTService_DetectTarget()
{
	NodeName = TEXT("Detect");
	
	//특정 노드 실행 사이의 간격을 0.5초 사이의 랜덤한 값으로 지정
	Interval = 0.5f;
}

void UBTService_DetectTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AMonsterAIController* Controller = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());

	if (!Controller)
		return;

	//GetPawn  : 이 AI Controller가 빙의되어 있는 Pawn을 얻어온다.
	AMonster* Monster = Cast<AMonster>(Controller->GetPawn());

	if (!Monster)
		return;

	const FMonsterInfo& Info = Monster->GetMonsterInfo();

	//주변에 플레이어가 있는지 판단한다.
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