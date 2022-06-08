// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"

// Sets default values
AProjectileBase::AProjectileBase()
	:mDistance(-1.f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	mMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	mProjectile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));

	SetRootComponent(mSphere);

	mMesh->SetupAttachment(mSphere);
	mParticle->SetupAttachment(mSphere);

	mProjectile->SetUpdatedComponent(mSphere);

	mProjectile->InitialSpeed = 200.f;
	mProjectile->MaxSpeed = 200.f;
	mProjectile->ProjectileGravityScale = 0.f;

	mProjectile->bShouldBounce = false;
	
	mDistance = -1.f;
	
	//Delegate
	mProjectile->OnProjectileStop.AddDynamic(this, &AProjectileBase::CollisionStop);
	mProjectile->OnProjectileBounce.AddDynamic(this, &AProjectileBase::CollisionBounce);

	mMesh->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	mMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	mDamage = 0.f;
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
	mPrevLoc = GetActorLocation();
}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (mDistance > -1.f)
	{
		FVector CurLoc = GetActorLocation();

		FVector Dist = CurLoc - mPrevLoc;;

		mDistance -= Dist.Size();
		mPrevLoc = CurLoc;

		if (mDistance <= 0.f)
		{
			Destroy();
		}
	}

	if (mTarget)
	{
		FVector Dir = mTarget->GetActorLocation() - GetActorLocation();
		Dir.Normalize();

		mProjectile->Velocity = Dir * mProjectile->MaxSpeed;

		SetActorRotation(FRotator(0.f, Dir.Rotation().Yaw, 0.f));
	}
}

void AProjectileBase::CollisionBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity)
{
	ProjectileBounce(ImpactResult, ImpactVelocity);
}

void AProjectileBase::CollisionStop(const FHitResult& ImpactResult)
{
	ProjectileStop(ImpactResult);
}

void AProjectileBase::ProjectileStop(const FHitResult& ImpactResult)
{
	
}

void AProjectileBase::ProjectileBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity)
{
	
}