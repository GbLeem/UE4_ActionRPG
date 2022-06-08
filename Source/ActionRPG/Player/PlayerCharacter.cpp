// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "PlayerAnimInstance.h"
#include "../UE10GameInstance.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
	:mMoveDir(0.f)
	,mAttackMontageIndex(0)
	,mAttackEnable(true)
	, mDeath(false)
	, mUseSkillType(EPlayerSkill::None)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//사용할 컴포넌트 생성
	//CreateDefaultSubobject : 컴포넌트를 생성할 때 사용한다.
	mCameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Arm"));
	mCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	//Arm의 부모 컴포넌트 지정
	mCameraArm->SetupAttachment(GetCapsuleComponent());

	//camera의 부모 컴포넌트 지정
	mCamera->SetupAttachment(mCameraArm);

	//카메라 각도 조정
	mCameraArm->TargetArmLength = 400.f;
	mCameraArm->TargetOffset = FVector(0.f, 0.f, 80.f);
	mCameraArm->SetRelativeRotation(FRotator(-15.f, 0.f, 0.f));

	GetCharacterMovement()->JumpZVelocity = 600.f;

	GetMesh()->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));

	GetMesh()->bReceivesDecals = false;

	SetCanBeDamaged(true);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay() 
{
	Super::BeginPlay();
	
	//애님 인스턴스는 생성자에서 생성 불가능
	//메쉬가 생성해둔 애님 인스턴스 객체를 얻어온다.
	//GetAnimInstance : 메쉬가 생성한 애님 인스턴스 객체를 얻어오는 함수
	//but UAnimInstance 타입을 얻어온다(형변환 필요 - Cast 이용)
	mAnim = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());

	//플레이어의 정보를 얻어온다
	UUE10GameInstance* GameInst = Cast<UUE10GameInstance>(GetWorld()->GetGameInstance());

	if (GameInst)
	{
		const FPlayerDataTableInfo* Info = GameInst->FindPlayerInfo(mDataTablekey);

		if (Info)
		{
			mInfo.Name = Info->Name;
			mInfo.Job = Info->Job;
			mInfo.Attack = Info->Attack;
			mInfo.Armor = Info->Armor;
			mInfo.HP = Info->HP;
			mInfo.HPMax = Info->HP;
			mInfo.MP = Info->MP;
			mInfo.AttackSpeed = Info->AttackSpeed;
			mInfo.MoveSpeed = Info->MoveSpeed;
			mInfo.CriticalRatio = Info->CriticalRatio;
			mInfo.CriticalDamage = Info->CriticalDamage;
			mInfo.Level = 1;
			mInfo.Exp = 0;
			mInfo.Gold = 10000;

			GetCharacterMovement()->MaxWalkSpeed = Info->MoveSpeed;

		}
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis(TEXT("MoveFront"), this, &APlayerCharacter::MoveFront);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("RotationYaw"), this, &APlayerCharacter::RotationYaw);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed , this, &APlayerCharacter::AttackKey);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &APlayerCharacter::JumpKey);
	PlayerInputComponent->BindAction(TEXT("Skill1"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Skill1Key);

}
float APlayerCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	//무적 상태일 경우
	if (Damage == 0.f)
		return Damage;

	Damage = DamageAmount - mInfo.Armor;
	Damage = Damage < 1.f ? 1.f : Damage;

	mInfo.HP -= (int32)Damage;

	if (mInfo.HP <= 0)
	{
		mAnim->ChangeAnimType(EPlayerAnim::Death);
		Damage = -1.f;
	}

	PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("Damage : %2.f"), DamageAmount));


	return Damage;
}


void APlayerCharacter::MoveFront(float Scale)
{
	if (mDeath || !IsValid(mAnim))
		return;

	if(Scale != 0.f)
		mUseSkillType = EPlayerSkill::None;

	//CharacterMovementComponent를 이용해서 이동하는 기능 제공
	//GetActorForwardVector 해당 액터의 전방 방향 벡터를 얻어온다
	AddMovementInput(GetActorForwardVector(), Scale);

	mMoveDir = Scale;

	//앞으로 이동할때
	if (Scale > 0.f)
	{
		mAnim->SetMoveDir(0.f);
	}

	//뒤로 이동할때
	else if (Scale < 0.f)
	{
		mAnim->SetMoveDir(180.f);
	}
	
	//둘다 누르거나 안눌렀을 경우
	else
	{
		mAnim->SetMoveDir(0.f);
	}
}

void APlayerCharacter::MoveRight(float Scale)
{
	if (mDeath || !IsValid(mAnim))
		return;

	if (Scale != 0.f)
		mUseSkillType = EPlayerSkill::None;

	AddMovementInput(GetActorRightVector(), Scale);

	//오른쪽 이동
	if (Scale > 0.f)
	{
		//오른쪽 + 앞으로 이동
		if (mMoveDir > 0.f)
		{
			mAnim->SetMoveDir(45.f);
		}
		//오른쪽 + 뒤로 이동
		else if (mMoveDir < 0.f)
		{
			mAnim->SetMoveDir(135.f);
		}
		//오른쪽
		else
		{
			mAnim->SetMoveDir(90.f);
		}
	}

	//왼쪽 이동
	else if (Scale < 0.f)
	{
		//왼쪽 + 앞으로 이동
		if (mMoveDir > 0.f)
		{
			mAnim->SetMoveDir(-45.f);
		}
		//왼쪽 + 뒤로 이동
		else if (mMoveDir < 0.f)
		{
			mAnim->SetMoveDir(-135.f);
		}
		//왼쪽
		else
		{
			mAnim->SetMoveDir(-90.f);
		}
	}
}

void APlayerCharacter::RotationYaw(float Scale)
{
	if (mDeath || !IsValid(mAnim))
		return;
	//AddControllerYawInput: Pawn의 Use Controller Rotation Yaw가 체크된 경우 이용하자
	//AddActorWorldRotation(FRotator(0.f, 1.f, 0.f)); -> 체크 안된경우 사용 가능
	AddControllerYawInput(Scale);
}

void APlayerCharacter::AttackKey()
{
	//Num 함수는 이 배열에 몇개가 추가되어있는지를 반환해준다.
	if (mAttackMontageArray.Num() == 0)
		return;

	//공격이 가능한 상태가 아닌 경우
	else if (!mAttackEnable || mDeath || mUseSkillType != EPlayerSkill::None)
		return;

	else if (mAnim->GetAnimType() != EPlayerAnim::Ground)
		return;

	mAttackEnable = false;

	Attack();
}

void APlayerCharacter::JumpKey()
{
	if (mDeath || !IsValid(mAnim))
		return;

	if (mAnim->GetAnimType() == EPlayerAnim::Ground)
	{
		// Character 클래스에서 제공되는 함수이다.
		Jump();
		mAnim->ChangeAnimType(EPlayerAnim::Jump);
	}
}

void APlayerCharacter::PlayFallRecovery()
{
	if (!IsValid(mFallRecoveryMontage))
		return;
	
	AttackEnd();

	if (!mAnim -> Montage_IsPlaying(mFallRecoveryMontage))
	{
		mAnim->Montage_SetPosition(mFallRecoveryMontage, 0.f);
		mAnim->Montage_Play(mFallRecoveryMontage);
	}
}

void APlayerCharacter::Skill1Key()
{
	//다른 스킬을 사용중일 경우에는 스킬 사용이 불가능하다
	if (mUseSkillType != EPlayerSkill::None || mSkillMontageArray.Num() < 1 || 
		!GetCharacterMovement()->IsMovingOnGround())
		return;

	AttackEnd();

	mUseSkillType = EPlayerSkill::Skill1;

	if (!mAnim->Montage_IsPlaying(mSkillMontageArray[0]))
	{
		mAnim->Montage_SetPosition(mSkillMontageArray[0], 0.f);
		mAnim->Montage_Play(mSkillMontageArray[0]);
	}
}


void APlayerCharacter::Attack()
{
	//Montage_IsPlaying : 인자로 들어간 몽타주가 현재 재생중인지를 판단한다.
	if (!mAnim->Montage_IsPlaying(mAttackMontageArray[mAttackMontageIndex]))
	{
		//공격용 애니메이션 몽타주 재생 
		//BlendIn : 다른 모션에서 해당 모션으로 전환될때 자연스럽게 해주기->그냥 블루프린트에서 해주기

		//Montage_SetPosition : 첫번째 모션부터 동작하도록 초기화시켜줌(0으로 시작 인덱스 지정)
		mAnim->Montage_SetPosition(mAttackMontageArray[mAttackMontageIndex], 0.f);
		mAnim->Montage_Play(mAttackMontageArray[mAttackMontageIndex]);

		mAttackMontageIndex = (mAttackMontageIndex + 1) % mAttackMontageArray.Num();
	}
}

void APlayerCharacter::AttackEnd()
{
	mAttackEnable = true;
	mAttackMontageIndex = 0;
}
void APlayerCharacter::UseSkill()
{
	if (mUseSkillType == EPlayerSkill::None)
		return;
	switch (mUseSkillType)
	{
	case EPlayerSkill::Skill1:
		Skill1();
		break;
	case EPlayerSkill::Skill2:
		Skill2();
		break;
	case EPlayerSkill::Skill3:
		Skill3();
		break;
	case EPlayerSkill::Skill4:
		Skill4();
		break;
	}
}

void APlayerCharacter::Skill1()
{

}

void APlayerCharacter::Skill2()
{

}

void APlayerCharacter::Skill3()
{

}

void APlayerCharacter::Skill4()
{

}

void APlayerCharacter::NormalAttack()
{

}

void APlayerCharacter::Death()
{
	Destroy();
}
