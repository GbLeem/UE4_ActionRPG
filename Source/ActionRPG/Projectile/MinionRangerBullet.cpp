// Fill out your copyright notice in the Description page of Project Settings.


#include "MinionRangerBullet.h"

#include "../Effect/EffectBase.h"
#include "../Effect/EffectDecal.h"


AMinionRangerBullet::AMinionRangerBullet()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("ParticleSystem'/Game/RPGEffects/Particles/P_Mage_Fire_Shield.P_Mage_Fire_Shield'"));

	if (ParticleAsset.Succeeded())
		mParticle->SetTemplate(ParticleAsset.Object);

	mProjectile->InitialSpeed = 1000.f;
	mProjectile->MaxSpeed = 1000.f;
	mProjectile->ProjectileGravityScale = 0.f;

	mSphere->SetCollisionProfileName(TEXT("MonsterObject"));

	//overlap event
	mSphere->SetGenerateOverlapEvents(true);

	//hit event
	mSphere->SetNotifyRigidBodyCollision(true);

	mDistance = 10000.f;  
}

// Called when the game starts or when spawned
void AMinionRangerBullet::BeginPlay()
{
	Super::BeginPlay();

	mPrevLoc = GetActorLocation();
}

// Called every frame
void AMinionRangerBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMinionRangerBullet::ProjectileStop(const FHitResult& ImpactResult)
{
	FActorSpawnParameters param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AEffectBase* Effect = GetWorld()->SpawnActor<AEffectBase>(GetActorLocation(), GetActorRotation(), param);

	Effect->SetParticle(TEXT("ParticleSystem'/Game/RPGEffects/Particles/P_Warrior_Shout.P_Warrior_Shout'"));
	Effect->SetSound(TEXT("SoundWave'/Game/Sound/thunder.thunder'"));

	ImpactResult.GetActor()->TakeDamage(mDamage, FDamageEvent(), mOwnerController, this);

	Destroy();
}

void AMinionRangerBullet::ProjectileBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity)
{
	Super::ProjectileBounce(ImpactResult, ImpactVelocity);
}