// Fill out your copyright notice in the Description page of Project Settings.


#include "Maze/CellActor.h"
#include "Components/ArrowComponent.h"

// Sets default values
ACellActor::ACellActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	FloorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Floor"));
	SetRootComponent(FloorMesh);
	FloorMesh->SetCollisionProfileName(TEXT("BlockAll"));

	UStaticMeshComponent* Wall;
	Wall = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallNorth"));
	Wall->SetupAttachment(GetRootComponent());
	Wall->SetCollisionProfileName(TEXT("BlockAll"));
	Wall->SetRelativeLocationAndRotation(
		FVector::ForwardVector * (CellHalfSize - WallHalfThickness),
		FRotator(0, 0, 0));
	WallMeshes.Add(Wall);

	Wall = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallEast"));
	Wall->SetupAttachment(GetRootComponent());
	Wall->SetCollisionProfileName(TEXT("BlockAll"));
	Wall->SetRelativeLocationAndRotation(
		FVector::RightVector * (CellHalfSize - WallHalfThickness),
		FRotator(0, 90, 0));
	WallMeshes.Add(Wall);

	Wall = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallSouth"));
	Wall->SetupAttachment(GetRootComponent());
	Wall->SetCollisionProfileName(TEXT("BlockAll"));
	Wall->SetRelativeLocationAndRotation(
		FVector::BackwardVector * (CellHalfSize - WallHalfThickness),
		FRotator(0, 180, 0));
	WallMeshes.Add(Wall);

	Wall = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallWest"));
	Wall->SetupAttachment(GetRootComponent());
	Wall->SetCollisionProfileName(TEXT("BlockAll"));
	Wall->SetRelativeLocationAndRotation(
		FVector::LeftVector * (CellHalfSize - WallHalfThickness),
		FRotator(0, 270, 0));
	WallMeshes.Add(Wall);

	UStaticMeshComponent* Gate;
	Gate = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GateNorth"));
	Gate->SetupAttachment(GetRootComponent());
	Gate->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	Gate->SetRelativeLocationAndRotation(
		FVector::ForwardVector * (CellHalfSize - WallHalfThickness),
		FRotator(0, 0, 0));
	GateMeshes.Add(Gate);

	Gate = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GateEast"));
	Gate->SetupAttachment(GetRootComponent());
	Gate->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	Gate->SetRelativeLocationAndRotation(
		FVector::RightVector * (CellHalfSize - WallHalfThickness),
		FRotator(0, 90, 0));
	GateMeshes.Add(Gate);

	Gate = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GateSouth"));
	Gate->SetupAttachment(GetRootComponent());
	Gate->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	Gate->SetRelativeLocationAndRotation(
		FVector::BackwardVector * (CellHalfSize - WallHalfThickness),
		FRotator(0, 180, 0));
	GateMeshes.Add(Gate);

	Gate = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GateWest"));
	Gate->SetupAttachment(GetRootComponent());
	Gate->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	Gate->SetRelativeLocationAndRotation(
		FVector::LeftVector * (CellHalfSize - WallHalfThickness),
		FRotator(0, 270, 0));
	GateMeshes.Add(Gate);

	UArrowComponent* Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("NorthArrow"));
	Arrow->SetupAttachment(GetRootComponent());
	Arrow->SetRelativeLocation(FVector(0, 0, 200));

	this->SetCanBeDamaged(false);
}

void ACellActor::InitializeCell(FCellData* InCellData)
{
	if (!InCellData) return;

	Path = static_cast<int32>(InCellData->Path);
	OpenGate();
}

// Called when the game starts or when spawned
void ACellActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ACellActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACellActor::TestPath()
{
	TArray<FString> OpenDirections;
	if (IsPath(EDirectionType::North)) { OpenDirections.Add(TEXT("North")); }
	if (IsPath(EDirectionType::East)) { OpenDirections.Add(TEXT("East")); }
	if (IsPath(EDirectionType::South)) { OpenDirections.Add(TEXT("South")); }
	if (IsPath(EDirectionType::West)) { OpenDirections.Add(TEXT("West")); }
	FString PathNames = OpenDirections.Num() > 0 ? FString::Join(OpenDirections, TEXT(" | ")) : TEXT("None");
	UE_LOG(LogTemp, Log, TEXT("[TestPath] Open : %s (Raw: %d)"), *PathNames, Path);
	OpenGate();
}

void ACellActor::OpenGate()
{
	constexpr int DirectionCount = 4;
	for (int i = 0; i < DirectionCount; i++)
	{
		EDirectionType Dir = static_cast<EDirectionType>(1 << i);
		if (IsPath(Dir))
		{
			GateMeshes[i]->SetVisibility(false);
			GateMeshes[i]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		else
		{
			GateMeshes[i]->SetVisibility(true);
			GateMeshes[i]->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		}
	}
}

bool ACellActor::IsPath(EDirectionType InDirection)
{
	return (static_cast<EDirectionType>(Path) & InDirection) != EDirectionType::None;
}
