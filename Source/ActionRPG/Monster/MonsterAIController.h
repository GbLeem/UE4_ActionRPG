// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "BehaviorTree/BehaviorTree.h"
#include"BehaviorTree/BlackboardData.h"
#include"BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "MonsterAIController.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONRPG_API AMonsterAIController : public AAIController
{
	GENERATED_BODY()

public:
	AMonsterAIController();
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* mAITree;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UBlackboardData* mAIBlackboard;


protected:
	void OnPossess(APawn* InPawn);
	void OnUnPossess();
};
