// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/TestTimer.h"

// Sets default values
ATestTimer::ATestTimer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATestTimer::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void ATestTimer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ATestTimer::Test_TimerOnce()
{
	UWorld* World = GetWorld();
	FTimerManager& TimerManager = World->GetTimerManager();

	TimerManager.SetTimer(
		TestOnce,
		this,
		&ATestTimer::Test_MessagePrint1,
		3.0f,
		false
	);

}

void ATestTimer::Test_TimerRepeat()
{
	UWorld* World = GetWorld();
	FTimerManager& TimerManager = World->GetTimerManager();

	TimerManager.SetTimer(
		TestRepeat,
		this,
		&ATestTimer::Test_MessagePrint2,
		3.0f,
		true
	);
}

void ATestTimer::Test_TimerDelayRepeat()
{
	UWorld* World = GetWorld();
	FTimerManager& TimerManager = World->GetTimerManager();

	TimerManager.SetTimer(
		TestDelay, // TestDelay가 핸들인 타이머
		this, // 이 객채의 
		&ATestTimer::Test_MessagePrint3, // 이함수를 
		3.0f, // 3초간격으로 시행
		true, // 반복여부
		5.0f // 처음에 5초 대기 후 실행 
	);
}

void ATestTimer::Test_TimerAllClear()
{
	UWorld* World = GetWorld();
	FTimerManager& TimerManager = World->GetTimerManager();

	TimerManager.ClearTimer(TestOnce);
	TimerManager.ClearTimer(TestRepeat);
	TimerManager.ClearTimer(TestDelay);

}




void ATestTimer::Test_MessagePrint1()
{
	UE_LOG(LogTemp, Log, TEXT("Test_TimerOnce: Invoke"));
}

void ATestTimer::Test_MessagePrint2()
{
	UE_LOG(LogTemp, Log, TEXT("Test_TimerRepeat: Invoke"));
}

void ATestTimer::Test_MessagePrint3()
{
	UE_LOG(LogTemp, Log, TEXT("Test_TimerDelayRepeat: Invoke"));
}
