// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"
#include "../UE10GameInstance.h"
#include "MonsterAnimInstance.h"
#include "MonsterSpawnPoint.h"
#include "MonsterAIController.h"
#include "../AI/PatrolPoint.h"

// Sets default values
AMonster::AMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Monster"));

	GetMesh()->bReceivesDecals = false;

	SetCanBeDamaged(true);

	mSpawnPoint = nullptr;

	AIControllerClass = AMonsterAIController::StaticClass();

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	mAttackEnd = false;

	mPatrolIndex = 1;
	mPatrolNext = 1;
}

void AMonster::SetSpawnPoint(class AMonsterSpawnPoint* Point, const TArray<class APatrolPoint*>& PatrolPointArray, EPatroltype PatrolType)
{
	mSpawnPoint = Point;

	mPatrolType = PatrolType;

	mPatrolPointArray.Add(Point->GetActorLocation());

	for (auto& PatrolPoint : PatrolPointArray)
	{
		mPatrolPointArray.Add(PatrolPoint->GetActorLocation());

	}
}

void AMonster::NextPatrolPoint()
{
	mPatrolIndex += mPatrolNext;

	switch (mPatrolType)
	{
	case EPatroltype::Progress:
		if (mPatrolIndex == mPatrolPointArray.Num())
			mPatrolIndex = 0;
		break;
	case EPatroltype::PingPong:
		if (mPatrolIndex == mPatrolPointArray.Num())
		{
			mPatrolIndex = mPatrolPointArray.Num() - 1;
			mPatrolNext = -1;
		}
		else if (mPatrolIndex == -1)
		{
			mPatrolIndex = 1;
			mPatrolNext = 1;
		}
		break;
	}
}


// Called when the game starts or when spawned
void AMonster::BeginPlay()
{
	Super::BeginPlay();
	
	mAnim = Cast<UMonsterAnimInstance>(GetMesh()->GetAnimInstance());


	UUE10GameInstance* GameInst = Cast<UUE10GameInstance>(GetWorld()->GetGameInstance());

	if (GameInst)
	{
		const FMonsterDataTableInfo* Info = GameInst->FindMonsterInfo(mDataTablekey);

		if (Info)
		{
			mInfo.Name = Info->Name;
			mInfo.Attack = Info->Attack;
			mInfo.Armor = Info->Armor;
			mInfo.HP = Info->HP;
			mInfo.HPMax = Info->HP;
			mInfo.MP = Info->MP;
			mInfo.AttackSpeed = Info->AttackSpeed;
			mInfo.MoveSpeed = Info->MoveSpeed;
			mInfo.CriticalRatio = Info->CriticalRatio;
			mInfo.CriticalDamage = Info->CriticalDamage;
			mInfo.Level = Info->Level;
			mInfo.Exp = Info->Exp;
			mInfo.Gold = Info->Gold;
			mInfo.TraceDistance = Info->TraceDistance;
			mInfo.AttackDistance = Info->AttackDistance;

			GetCharacterMovement()->MaxWalkSpeed = Info->MoveSpeed;
		}
	}

}

// Called every frame
void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	mAttackEnd = false;
}

float AMonster::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	//무적 상태일 경우
	if (Damage == 0.f)
		return Damage;

	Damage = DamageAmount - mInfo.Armor;
	Damage = Damage < 1.f ? 1.f : Damage;

	mInfo.HP -= (int32)Damage;

	if (mInfo.HP <= 0)
	{
		mAnim->ChangeAnimType(EmonsterAnim::Death);
		Damage = -1.f;
	}

	PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("Damage : %2.f"), DamageAmount));

	return Damage;
}

//죽는 모션이 끝나면 호출되는 함수
void AMonster::Death()
{
	mSpawnPoint->MonsterDeath();
	Destroy();
}

void AMonster::Attack()
{

}