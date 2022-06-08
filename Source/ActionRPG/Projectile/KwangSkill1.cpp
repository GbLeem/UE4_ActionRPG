// Fill out your copyright notice in the Description page of Project Settings.


#include "KwangSkill1.h"
#include "../Effect/EffectBase.h"
#include "../Effect/EffectDecal.h"
#include "../Player/PlayerCharacter.h"
#include "../Monster/Monster.h"

AKwangSkill1::AKwangSkill1()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("ParticleSystem'/Game/ParagonKwang/FX/Particles/Abilities/Sword/FX/P_Kwang_Sword_Attach.P_Kwang_Sword_Attach'"));
	
	if (ParticleAsset.Succeeded())
		mParticle->SetTemplate(ParticleAsset.Object);

	mProjectile->InitialSpeed = 1000.f;
	mProjectile->MaxSpeed = 1000.f;
	mProjectile->ProjectileGravityScale = 0.f;

	mSphere->SetCollisionProfileName(TEXT("PlayerObject"));

	//overlap event
	mSphere->SetGenerateOverlapEvents(false);

	//hit event
	mSphere->SetNotifyRigidBodyCollision(true);

	mDistance = 2000.f;
	mDamage = 50.f;

}

// Called when the game starts or when spawned
void AKwangSkill1::BeginPlay()
{
	Super::BeginPlay();

	mPrevLoc = GetActorLocation();
}

// Called every frame
void AKwangSkill1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AKwangSkill1::ProjectileStop(const FHitResult& ImpactResult)
{
	Super::ProjectileStop(ImpactResult);

	//폭발 처리
	FActorSpawnParameters param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	AEffectBase* Effect = GetWorld()->SpawnActor<AEffectBase>(GetActorLocation(), GetActorRotation(), param);

	Effect->SetParticle(TEXT("ParticleSystem'/Game/RPGEffects/Particles/P_Warrior_Shout.P_Warrior_Shout'"));
	Effect->SetSound(TEXT("SoundWave'/Game/Sound/thunder.thunder'"));
	
	FVector DecalLoc = GetActorLocation();
	DecalLoc.Z -= 88.f;
	FRotator Rot(-90.f, 0.f, 0.f);

	AEffectDecal* Decal = GetWorld()->SpawnActor<AEffectDecal>(GetActorLocation(), GetActorRotation(), param);

	Decal->SetMaterial(TEXT("MaterialInstanceConstant'/Game/Effect/MTDecalInst.MTDecalInst'"));

	//스킬 데미지 추가
	ImpactResult.GetActor()->TakeDamage(mDamage, FDamageEvent(), mOwnerController, this);
	Destroy();
	
}

void AKwangSkill1::ProjectileBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity)
{
	Super::ProjectileBounce(ImpactResult, ImpactVelocity);
}