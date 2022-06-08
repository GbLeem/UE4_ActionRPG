// Fill out your copyright notice in the Description page of Project Settings.


#include "Minion.h"
#include "MonsterAnimInstance.h"
#include "../Effect/EffectBase.h"

// Sets default values
AMinion::AMinion()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("SkeletalMesh'/Game/ParagonMinions/Characters/Minions/Down_Minions/Meshes/Minion_Lane_Melee_Core_Dawn.Minion_Lane_Melee_Core_Dawn'"));
	if (MeshAsset.Succeeded())
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);

	GetCapsuleComponent()->SetCapsuleHalfHeight(78.f);
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -78.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	//�ִϸ��̼� �������Ʈ Ŭ������ �ε��ؼ� ����
	static ConstructorHelpers::FClassFinder<UMonsterAnimInstance> AnimClass(TEXT("AnimBlueprint'/Game/Monster/BPMinionAnim.BPMinionAnim_C'"));
	if (AnimClass.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimClass.Class);

	mDataTablekey = TEXT("Minion");

}

// Called when the game starts or when spawned
void AMinion::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMinion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMinion::Attack()
{
	FVector Start = GetActorLocation() + GetActorForwardVector() * 50.f;
	FVector End = GetActorLocation() + GetActorForwardVector() * (50.f + mInfo.AttackDistance);

	FCollisionQueryParams params(NAME_None, false, this);

	FHitResult Result;
	bool Hit = GetWorld()->SweepSingleByChannel(Result, Start, End, FQuat::Identity, 
		ECollisionChannel::ECC_GameTraceChannel6,
		FCollisionShape::MakeSphere(50.f), params);

	if (Hit)
	{
		FActorSpawnParameters param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AEffectBase* Effect = GetWorld()->SpawnActor<AEffectBase>(Result.ImpactPoint,
			Result.ImpactNormal.Rotation(), param);

		Effect->SetParticle(TEXT("ParticleSystem'/Game/RPGEffects/Particles/P_Warrior_sWIPE.P_Warrior_Swipe'"));
		Effect->SetSound(TEXT("SoundWave'/Game/Sound/sword.sword'"), 0.5f);


		//���濡�� �������� �ش�,
		//HitArray[i].GetActor() : �ε��� ���͸� �����´�.
		//TakeDamage ���� -1�� ���ϵǸ� ���Ͱ� ���� ������ �ν��Ѵ�.
		if (Result.GetActor()->TakeDamage(mInfo.Attack, FDamageEvent(), GetController(), this) == -1.f)
		{
			
		}
	}

#if ENABLE_DRAW_DEBUG

	FColor DrawColor = Hit ? FColor::Red : FColor::Green;

	DrawDebugCapsule(GetWorld(), (Start + End) / 2.f, mInfo.AttackDistance, 50.f, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 0.5f);
#endif
}