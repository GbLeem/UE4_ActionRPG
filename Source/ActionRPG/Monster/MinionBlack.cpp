// Fill out your copyright notice in the Description page of Project Settings.


#include "MinionBlack.h"
#include "MonsterAnimInstance.h"
#include "../Effect/EffectBase.h"

// Sets default values
AMinionBlack::AMinionBlack()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("SkeletalMesh'/Game/ParagonMinions/Characters/Minions/Dusk_Minions/Meshes/Minion_Lane_Melee_Dusk.Minion_Lane_Melee_Dusk'"));
	if (MeshAsset.Succeeded())
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);

	GetCapsuleComponent()->SetCapsuleHalfHeight(78.f);
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -78.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	//애니메이션 블루프린트 클래스를 로딩해서 지정
	static ConstructorHelpers::FClassFinder<UMonsterAnimInstance> AnimClass(TEXT("AnimBlueprint'/Game/Monster/BPMinionBlackAnim.BPMinionBlackAnim_C'"));
	if (AnimClass.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimClass.Class);

	mDataTablekey = TEXT("Black");

}

// Called when the game starts or when spawned
void AMinionBlack::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMinionBlack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMinionBlack::Attack()
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


		//상대방에게 데미지를 준다,
		//HitArray[i].GetActor() : 부딪힌 액터를 가져온다.
		//TakeDamage 에서 -1이 리턴되면 몬스터가 죽은 것으로 인식한다.
		if (Result.GetActor()->TakeDamage(mInfo.Attack, FDamageEvent(), GetController(), this) == -1.f)
		{

		}
	}

#if ENABLE_DRAW_DEBUG

	FColor DrawColor = Hit ? FColor::Red : FColor::Green;

	DrawDebugCapsule(GetWorld(), (Start + End) / 2.f, mInfo.AttackDistance, 50.f, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 0.5f);
#endif
}