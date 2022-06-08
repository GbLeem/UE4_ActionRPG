// Fill out your copyright notice in the Description page of Project Settings.


#include "MinionRanger.h"
#include "MonsterAnimInstance.h"
#include "../Projectile/MinionRangerBullet.h"
#include "MonsterAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

AMinionRanger::AMinionRanger()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("SkeletalMesh'/Game/ParagonMinions/Characters/Minions/Down_Minions/Meshes/Minion_Lane_Ranged_Core_Dawn.Minion_Lane_Ranged_Core_Dawn'"));
	if (MeshAsset.Succeeded())
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);

	GetCapsuleComponent()->SetCapsuleHalfHeight(78.f);
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -78.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	//애니메이션 블루프린트 클래스를 로딩해서 지정
	static ConstructorHelpers::FClassFinder<UMonsterAnimInstance> AnimClass(TEXT("AnimBlueprint'/Game/Monster/BPRangerAnim.BPRangerAnim_C'"));
	if (AnimClass.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimClass.Class);

	mDataTablekey = TEXT("Ranger");

}

// Called when the game starts or when spawned
void AMinionRanger::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMinionRanger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMinionRanger::Attack()
{
	FVector Loc = GetActorLocation() + GetActorForwardVector() * 100.f;

	FActorSpawnParameters param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	AMinionRangerBullet* BulletActor = GetWorld()->SpawnActor<AMinionRangerBullet>(Loc, GetActorRotation(), param);

	BulletActor->SetOwnerController(GetController());
	BulletActor->SetDamage((float)mInfo.Attack);

	//AIController를 이용해서 Blackboard의 타겟 얻어오기
	AAIController* AIController = Cast<AAIController>(GetController());
	AActor* Target = Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (Target)
		BulletActor->SetTarget(Target);
}