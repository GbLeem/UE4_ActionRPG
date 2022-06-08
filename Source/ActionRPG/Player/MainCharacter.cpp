// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "PlayerAnimInstance.h"
#include "../Effect/EffectBase.h"
#include "../Monster/Monster.h"


AMainCharacter::AMainCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//메쉬를 읽어오기
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("SkeletalMesh'/Game/ParagonGideon/Characters/Heroes/Gideon/Meshes/Gideon.Gideon'"));
	if (MeshAsset.Succeeded())
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);

	//Relative : 부모에 대해서 상대적인 움직임 값
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -88.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	////애니메이션 블루프린트 클래스를 로딩해서 지정
	static ConstructorHelpers::FClassFinder<UPlayerAnimInstance> AnimClass(TEXT("AnimBlueprint'/Game/Player/MainCharacter/ABMainCharacter.ABMainCharacter_C'"));
	if (AnimClass.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimClass.Class);

	static ConstructorHelpers::FObjectFinder<UAnimMontage> Attack1Asset(TEXT("AnimMontage'/Game/Player/MainCharacter/AMMainAttack1.AMMainAttack1'"));
	if (Attack1Asset.Succeeded())
		mAttackMontageArray.Add(Attack1Asset.Object);
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Attack2Asset(TEXT("AnimMontage'/Game/Player/MainCharacter/AMMainAttack2.AMMainAttack2'"));
	if (Attack2Asset.Succeeded())
		mAttackMontageArray.Add(Attack2Asset.Object);
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Attack3Asset(TEXT("AnimMontage'/Game/Player/MainCharacter/AMMainAttack3.AMMainAttack3'"));
	if (Attack3Asset.Succeeded())
		mAttackMontageArray.Add(Attack3Asset.Object);
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Attack4Asset(TEXT("AnimMontage'/Game/Player/MainCharacter/AMMainAttack4.AMMainAttack4'"));
	if (Attack4Asset.Succeeded())
		mAttackMontageArray.Add(Attack4Asset.Object);
	static ConstructorHelpers::FObjectFinder<UAnimMontage> RecoveryAsset(TEXT("AnimMontage'/Game/Player/MainCharacter/AMMainFallRecovery.AMMainFallRecovery'"));
	if (RecoveryAsset.Succeeded())
		mFallRecoveryMontage = RecoveryAsset.Object;

	mDataTablekey = TEXT("Main");

}
// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

}
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMainCharacter::Attack()
{
	Super::Attack();
}

void AMainCharacter::Skill1()
{

}

void AMainCharacter::Skill2()
{

}
void AMainCharacter::Skill3()
{

}
void AMainCharacter::Skill4()
{

}
void AMainCharacter::NormalAttack()
{
	FVector Start = GetActorLocation() + GetActorForwardVector() * 50.f;
	FVector End = GetActorLocation() + GetActorForwardVector() * 250.f;

	FCollisionQueryParams params(NAME_None, false, this);

	TArray<FHitResult> HitArray;
	bool Hit = GetWorld()->SweepMultiByChannel(HitArray, Start, End, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel3,
		FCollisionShape::MakeSphere(50.f), params);

	PrintViewport(1.f, FColor::Blue, FString::Printf(TEXT("Collision : %d"), HitArray.Num()));
	if (Hit)
	{
		for (int32 i = 0; i < HitArray.Num(); ++i)
		{
			FActorSpawnParameters param;
			param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			AEffectBase* Effect = GetWorld()->SpawnActor<AEffectBase>(HitArray[i].ImpactPoint,
				HitArray[i].ImpactNormal.Rotation(), param);

			Effect->SetParticle(TEXT("ParticleSystem'/Game/RPGEffects/Particles/P_Warrior_sWIPE.P_Warrior_Swipe'"));
			Effect->SetSound(TEXT("SoundWave'/Game/Sound/sword.sword'"), 0.5f);


			//상대방에게 데미지를 준다,
			//HitArray[i].GetActor() : 부딪힌 액터를 가져온다.
			//TakeDamage 에서 -1이 리턴되면 몬스터가 죽은 것으로 인식한다.
			if (HitArray[i].GetActor()->TakeDamage(mInfo.Attack, FDamageEvent(), GetController(), this) == -1.f)
			{
				AMonster* Monster = Cast<AMonster>(HitArray[i].GetActor());

				if (Monster)
				{
					const FMonsterInfo& MonsterInfo = Monster->GetMonsterInfo();
					mInfo.Exp += MonsterInfo.Exp;
					mInfo.Gold += MonsterInfo.Gold;
				}
			}
		}
	}

#if ENABLE_DRAW_DEBUG

	FColor DrawColor = Hit ? FColor::Red : FColor::Green;

	DrawDebugCapsule(GetWorld(), (Start + End) / 2.f, 150.f, 50.f, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 0.5f);
#endif
}
