#include "FloatingActor.h"

AFloatingActor::AFloatingActor()
{
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	MainMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainMesh"));
	MainMesh->SetupAttachment(RootComponent);
}

void AFloatingActor::BeginPlay()
{
	Super::BeginPlay();

	// 시작 시점의 위치를 기준점으로 저장
	InitialLocation = GetActorLocation();
}

void AFloatingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TickTock(DeltaTime);
}

void AFloatingActor::TickTock(float DeltaTime)
{
	RunningTime += DeltaTime;

	float DeltaHeight = Amplitude * FMath::Cos(Frequency * RunningTime);
	FVector NewLocation = InitialLocation;
	NewLocation.Z += DeltaHeight;

	SetActorLocation(NewLocation);
}

