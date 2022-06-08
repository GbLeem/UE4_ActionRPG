// Fill out your copyright notice in the Description page of Project Settings.


#include "Kwang.h"
#include "PlayerAnimInstance.h"
#include "../Projectile/KwangSkill1.h"
#include "../Effect/EffectBase.h"
#include "../Monster/Monster.h"

// Sets default values
AKwang::AKwang()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//�޽��� �о����
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("SkeletalMesh'/Game/ParagonKwang/Characters/Heroes/Kwang/Meshes/KwangSunrise.KwangSunrise'"));
	if (MeshAsset.Succeeded())
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);

	//Relative : �θ� ���ؼ� ������� ������ ��
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -88.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	//�ִϸ��̼� �������Ʈ Ŭ������ �ε��ؼ� ����
	static ConstructorHelpers::FClassFinder<UPlayerAnimInstance> AnimClass(TEXT("AnimBlueprint'/Game/Player/ABKwang.ABKwang_C'"));
	if (AnimClass.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimClass.Class);

	static ConstructorHelpers::FObjectFinder<UAnimMontage> Attack1Asset(TEXT("AnimMontage'/Game/Player/AMKwangAttack1.AMKwangAttack1'"));
	if (Attack1Asset.Succeeded())
		mAttackMontageArray.Add(Attack1Asset.Object);
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Attack2Asset(TEXT("AnimMontage'/Game/Player/AMKwangAttack2.AMKwangAttack2'"));
	if (Attack2Asset.Succeeded())
		mAttackMontageArray.Add(Attack2Asset.Object);
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Attack3Asset(TEXT("AnimMontage'/Game/Player/AMKwangAttack3.AMKwangAttack3'"));
	if (Attack3Asset.Succeeded())
		mAttackMontageArray.Add(Attack3Asset.Object);
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Attack4Asset(TEXT("AnimMontage'/Game/Player/AMKwangAttack4.AMKwangAttack4'"));
	if (Attack4Asset.Succeeded())
		mAttackMontageArray.Add(Attack4Asset.Object);
	static ConstructorHelpers::FObjectFinder<UAnimMontage> RecoveryAsset(TEXT("AnimMontage'/Game/Player/AMKwangFallRecovery.AMKwangFallRecovery'"));
	if (RecoveryAsset.Succeeded())
		mFallRecoveryMontage = RecoveryAsset.Object;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Skill1Asset(TEXT("AnimMontage'/Game/Player/AMKwangSkill1.AMKwangSkill1'"));
	if (Skill1Asset.Succeeded())
		mSkillMontageArray.Add(Skill1Asset.Object);

	//������ ���̺� Ű �������ֱ�
	mDataTablekey = TEXT("Kwang");
}

// Called when the game starts or when spawned
void AKwang::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AKwang::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AKwang::Attack()
{
	Super::Attack();
}

void AKwang::Skill1()
{
	//GetWorld() : AActor Ŭ�������� �����ϴ� ���� Ŭ���� �޸� �ּ��̴�.
	//World�� ���õ� �۾��� �� �� �� �Լ��� �̿��ؼ� �� �� �ִ�. �����ڿ����� ��� �Ұ���
	//SpawnActor() : ���忡 ���͸� ���������ش�.
	FVector Loc = GetActorLocation() + GetActorForwardVector() * 100.f;

	FActorSpawnParameters param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	AKwangSkill1* Skill1Actor = GetWorld()->SpawnActor<AKwangSkill1>(Loc, GetActorRotation(), param);

	Skill1Actor->SetOwnerController(GetController());
}

void AKwang::Skill2()
{

}

void AKwang::Skill3()
{

}

void AKwang::Skill4()
{

}

void AKwang::NormalAttack()
{
	FVector Start = GetActorLocation() + GetActorForwardVector() * 50.f;
	FVector End = GetActorLocation() + GetActorForwardVector() * 250.f;

	FCollisionQueryParams params(NAME_None, false, this);

	TArray<FHitResult> HitArray;
	bool Hit = GetWorld()->SweepMultiByChannel(HitArray, Start, End, FQuat::Identity,ECollisionChannel::ECC_GameTraceChannel3,
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


			//���濡�� �������� �ش�,
			//HitArray[i].GetActor() : �ε��� ���͸� �����´�.
			//TakeDamage ���� -1�� ���ϵǸ� ���Ͱ� ���� ������ �ν��Ѵ�.
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