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

	//�����带 �����Ѵ�.
	if(mAIBlackboard)
		UseBlackboard(mAIBlackboard, Blackboard);

	//�ൿƮ�� ����
	if (mAITree)
	{
		if (!RunBehaviorTree(mAITree))
		{
			//����ó��
		}
	}	
}

void AMonsterAIController::OnUnPossess()
{
	Super::OnUnPossess();
}
