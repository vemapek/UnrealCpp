// Fill out your copyright notice in the Description page of Project Settings.


#include "TestActor.h"

// Sets default values
ATestActor::ATestActor()
{
	// Set this actor to call Tick() every frame.  
	// You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Data1 = 20;

	// USceneComponent 만들어서 루트로 설정
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	// UStaticMeshComponent 타입으로 디폴트 서브오브젝트 생성하고 MainMesh변수에 저장
	// 에디터 창에서 보이는 이름은 "MainMesh"로 설정
	MainMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainMesh"));
	MainMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATestActor::BeginPlay()
{
	Super::BeginPlay();	// Super = 부모 클래스

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("언리얼 시작"));
	UE_LOG(LogTemp, Log, TEXT("BeginPlay : Log"));
	UE_LOG(LogTemp, Warning, TEXT("BeginPlay : Warning"));
	UE_LOG(LogTemp, Error, TEXT("BeginPlay : Error"));

	Test_NormalFunction();
	Test_UFunction();

	Test_ImplementableFunction();
	Test_NativeEventFunction();
}

// Called every frame
void ATestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MainMesh->AddRelativeLocation(FVector(0, 0, MoveSpeed * DeltaTime));
}

void ATestActor::Test_NormalFunction()
{
	UE_LOG(LogTemp, Warning, TEXT("Test_NormalFunction : 함수 실행"));
}

void ATestActor::Test_UFunction()
{
	UE_LOG(LogTemp, Warning, TEXT("Test_UFunction : 함수 실행"));
}

void ATestActor::Test_NativeEventFunction_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Test_NativeEventFunction : 함수 실행"));
}

// 있으면 안된다.
//void ATestActor::Test_ImplementableFunction()
//{
//}