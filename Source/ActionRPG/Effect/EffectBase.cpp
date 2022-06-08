// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectBase.h"

// Sets default values
AEffectBase::AEffectBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));

	SetRootComponent(mParticle);
}

// Called when the game starts or when spawned
void AEffectBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEffectBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AEffectBase::SetParticle(const FString& path)
{
	UParticleSystem* Asset = LoadObject<UParticleSystem>(nullptr, *path);
	if (Asset)
	{
		mParticle->SetTemplate(Asset);
		mParticle->OnSystemFinished.AddDynamic(this,&AEffectBase::ParticleFinish); //종료된 파티클 시스템 인식

		return true;
	}
	return false;
}

bool AEffectBase::SetParticle(UParticleSystem* particle)
{
	if (particle)
	{
		mParticle->SetTemplate(particle);
		mParticle->OnSystemFinished.AddDynamic(this, &AEffectBase::ParticleFinish); 

		return true;
	}
	return false;
}

void AEffectBase::ParticleFinish(UParticleSystemComponent* Particle)
{
	Destroy();
}

bool AEffectBase::SetSound(const FString& path, float Volume)
{
	USoundBase* Sound = LoadObject<USoundBase>(nullptr, *path);
	if (Sound)
	{
		mSound = Sound;
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), mSound, GetActorLocation(), Volume);//사운드 재생은 해당 함수 호출해줘야함
		return true;
	}
	return false;
}

bool AEffectBase::SetSound(USoundBase* sound, float Volume)
{
	if (sound)
	{
		mSound = sound;
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), mSound, GetActorLocation(), Volume);
		return true;
	}
	return false;
}