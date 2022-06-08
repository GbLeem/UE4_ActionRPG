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

	//����� ������Ʈ ����
	//CreateDefaultSubobject : ������Ʈ�� ������ �� ����Ѵ�.
	mCameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Arm"));
	mCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	//Arm�� �θ� ������Ʈ ����
	mCameraArm->SetupAttachment(GetCapsuleComponent());

	//camera�� �θ� ������Ʈ ����
	mCamera->SetupAttachment(mCameraArm);

	//ī�޶� ���� ����
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
	
	//�ִ� �ν��Ͻ��� �����ڿ��� ���� �Ұ���
	//�޽��� �����ص� �ִ� �ν��Ͻ� ��ü�� ���´�.
	//GetAnimInstance : �޽��� ������ �ִ� �ν��Ͻ� ��ü�� ������ �Լ�
	//but UAnimInstance Ÿ���� ���´�(����ȯ �ʿ� - Cast �̿�)
	mAnim = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());

	//�÷��̾��� ������ ���´�
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

	//���� ������ ���
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

	//CharacterMovementComponent�� �̿��ؼ� �̵��ϴ� ��� ����
	//GetActorForwardVector �ش� ������ ���� ���� ���͸� ���´�
	AddMovementInput(GetActorForwardVector(), Scale);

	mMoveDir = Scale;

	//������ �̵��Ҷ�
	if (Scale > 0.f)
	{
		mAnim->SetMoveDir(0.f);
	}

	//�ڷ� �̵��Ҷ�
	else if (Scale < 0.f)
	{
		mAnim->SetMoveDir(180.f);
	}
	
	//�Ѵ� �����ų� �ȴ����� ���
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

	//������ �̵�
	if (Scale > 0.f)
	{
		//������ + ������ �̵�
		if (mMoveDir > 0.f)
		{
			mAnim->SetMoveDir(45.f);
		}
		//������ + �ڷ� �̵�
		else if (mMoveDir < 0.f)
		{
			mAnim->SetMoveDir(135.f);
		}
		//������
		else
		{
			mAnim->SetMoveDir(90.f);
		}
	}

	//���� �̵�
	else if (Scale < 0.f)
	{
		//���� + ������ �̵�
		if (mMoveDir > 0.f)
		{
			mAnim->SetMoveDir(-45.f);
		}
		//���� + �ڷ� �̵�
		else if (mMoveDir < 0.f)
		{
			mAnim->SetMoveDir(-135.f);
		}
		//����
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
	//AddControllerYawInput: Pawn�� Use Controller Rotation Yaw�� üũ�� ��� �̿�����
	//AddActorWorldRotation(FRotator(0.f, 1.f, 0.f)); -> üũ �ȵȰ�� ��� ����
	AddControllerYawInput(Scale);
}

void APlayerCharacter::AttackKey()
{
	//Num �Լ��� �� �迭�� ��� �߰��Ǿ��ִ����� ��ȯ���ش�.
	if (mAttackMontageArray.Num() == 0)
		return;

	//������ ������ ���°� �ƴ� ���
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
		// Character Ŭ�������� �����Ǵ� �Լ��̴�.
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
	//�ٸ� ��ų�� ������� ��쿡�� ��ų ����� �Ұ����ϴ�
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
	//Montage_IsPlaying : ���ڷ� �� ��Ÿ�ְ� ���� ����������� �Ǵ��Ѵ�.
	if (!mAnim->Montage_IsPlaying(mAttackMontageArray[mAttackMontageIndex]))
	{
		//���ݿ� �ִϸ��̼� ��Ÿ�� ��� 
		//BlendIn : �ٸ� ��ǿ��� �ش� ������� ��ȯ�ɶ� �ڿ������� ���ֱ�->�׳� �������Ʈ���� ���ֱ�

		//Montage_SetPosition : ù��° ��Ǻ��� �����ϵ��� �ʱ�ȭ������(0���� ���� �ε��� ����)
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
