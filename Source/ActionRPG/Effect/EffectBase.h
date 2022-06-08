// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/Actor.h"
#include "EffectBase.generated.h"

UCLASS()
class ACTIONRPG_API AEffectBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEffectBase();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* mParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USoundBase* mSound;
public:
	bool SetParticle(const FString& path);
	bool SetParticle(UParticleSystem* particle);
	bool SetSound(const FString& path, float Volume = 1.f);
	bool SetSound(USoundBase* sound, float Volume = 1.f);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void ParticleFinish(UParticleSystemComponent* Particle);
};
