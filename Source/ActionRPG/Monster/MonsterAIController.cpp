// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAIController.h"

AMonsterAIController::AMonsterAIController()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> AIAsset(TEXT("BehaviorTree'/Game/Monster/BTMonster.BTMonster'"));
	if (AIAsset.Succeeded())
		mAITree = AIAsset.Object;

	static ConstructorHelpers::FObjectFinder<UBlackboardData> AIBlackboardAsset(TEXT("BlackboardData'/Game/Monster/BBMonster.BBMonster'"));
	if (AIBlackboardAsset.Succeeded())
		mAIBlackboard = AIBlackboardAsset.Object;
}

void AMonsterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	//블랙보드를 지정한다.
	if(mAIBlackboard)
		UseBlackboard(mAIBlackboard, Blackboard);

	//행동트리 동작
	if (mAITree)
	{
		if (!RunBehaviorTree(mAITree))
		{
			//예외처리
		}
	}	
}

void AMonsterAIController::OnUnPossess()
{
	Super::OnUnPossess();
}
