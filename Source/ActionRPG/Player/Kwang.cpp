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

	//메쉬를 읽어오기
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("SkeletalMesh'/Game/ParagonKwang/Characters/Heroes/Kwang/Meshes/KwangSunrise.KwangSunrise'"));
	if (MeshAsset.Succeeded())
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);

	//Relative : 부모에 대해서 상대적인 움직임 값
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -88.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	//애니메이션 블루프린트 클래스를 로딩해서 지정
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

	//데이터 테이블 키 설정해주기
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
	//GetWorld() : AActor 클래스에서 지원하는 월드 클래스 메모리 주소이다.
	//World에 관련된 작업을 할 때 이 함수를 이용해서 할 수 있다. 생성자에서는 사용 불가능
	//SpawnActor() : 월드에 액터를 생성시켜준다.
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