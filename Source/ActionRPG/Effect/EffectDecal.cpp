// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectDecal.h"

// Sets default values
AEffectDecal::AEffectDecal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	mDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal"));
	SetRootComponent(mDecal);

	mDecal->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
	
}

// Called when the game starts or when spawned
void AEffectDecal::BeginPlay()
{
	Super::BeginPlay();

	mDecal->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));

	SetLifeSpan(3.f);
}

// Called every frame
void AEffectDecal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AEffectDecal::SetMaterial(const FString& path)
{
	UMaterialInstance* Asset = LoadObject<UMaterialInstance>(nullptr, *path);
	if (Asset)
	{
		mDecal->SetDecalMaterial(Asset);

		return true;
	}
	return false;
}
