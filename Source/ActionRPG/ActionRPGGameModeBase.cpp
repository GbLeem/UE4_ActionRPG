// Copyright Epic Games, Inc. All Rights Reserved.


#include "ActionRPGGameModeBase.h"

AActionRPGGameModeBase::AActionRPGGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;

	//디폴트 폰 클래스를 원하느 클래스로 지정해준다.
	//블루프린트 클래스를 가져올때 include로 불러오기 불가능
	//FClassFinder 를 이용해서 원하는 클래스 타입을 얻어올 수 있다.(경로의 맨 끝에 C붙여주기)
	static ConstructorHelpers::FClassFinder<APawn>PlayerPawn(TEXT("Blueprint'/Game/Player/MainCharacter/BPMainCharacter.BPMainCharacter_C'"));

	//위에서 읽어온 클래스를 Default로 지정하여 게임 시작시 빙의된 폰을 지정할 수 있다.
	if (PlayerPawn.Succeeded())
		DefaultPawnClass = PlayerPawn.Class;

}

void AActionRPGGameModeBase::BeginPlay()
{
	Super::BeginPlay();

}

void AActionRPGGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
