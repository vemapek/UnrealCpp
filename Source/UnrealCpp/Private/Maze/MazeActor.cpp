// Fill out your copyright notice in the Description page of Project Settings.


#include "Maze/MazeActor.h"
#include "Maze/CellActor.h"
#include "NavigationSystem.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "DrawDebugHelpers.h"


// Sets default values
AMazeActor::AMazeActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(Root);

	FloorHISM = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("FloorHISM"));
	FloorHISM->SetupAttachment(Root);
	FloorHISM->SetCollisionProfileName(TEXT("BlockAll"));

	WallHISM = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("WallHISM"));
	WallHISM->SetupAttachment(Root);
	WallHISM->SetCollisionProfileName(TEXT("BlockAll"));

	GateHISM = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("GateHISM"));
	GateHISM->SetupAttachment(Root);
	GateHISM->SetCollisionProfileName(TEXT("BlockAllDynamic"));
}

void AMazeActor::GenerateMaze()
{
	OnPreMazeGenerate();

	ClearMaze();	// 이전에 만들어진 미로가 있으면 삭제

	TUniquePtr<FMazeData> Maze = MakeMazeData();
	if (bUseHISM)
	{
		BuildMazeHISM(Maze.Get());
	}
	else
	{
		SpawnCells(Maze.Get());
	}

	OnPostMazeGenerate();
}

void AMazeActor::ClearMaze()
{
	if (bUseHISM)
	{
		ClearHISMInstances();
	}
	else
	{
		ClearSpawnedCells();
	}
}

// Called when the game starts or when spawned
void AMazeActor::BeginPlay()
{
	Super::BeginPlay();
	GenerateMaze();
}

void AMazeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMazeActor::OnPreMazeGenerate()
{
	UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (NavSystem)
	{
		NavSystem->SetNavigationAutoUpdateEnabled(false, NavSystem);	// NavMesh 자동 업데이트 막기
	}
}

void AMazeActor::OnPostMazeGenerate()
{
	UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (NavSystem)
	{
		NavSystem->Build();	// NavMesh 수동 생성
		NavSystem->SetNavigationAutoUpdateEnabled(true, NavSystem);	// NavMesh 자동 업데이트 재개
	}
}

TUniquePtr<FMazeData> AMazeActor::MakeMazeData()
{
	TUniquePtr<FMazeData> Maze = MakeUnique<FMazeData>();
	Maze->MakeMaze(static_cast<uint8>(Width), static_cast<uint8>(Height), Algorithm, RandomSeed);
	return Maze;
}

void AMazeActor::SpawnCells(FMazeData* InMaze)
{
	if (!CellActorClass) return;

	SpawnedCells.Reserve(Width * Height);

	const ACellActor* DefaultCell = CellActorClass->GetDefaultObject<ACellActor>();

	CellHalfSize = DefaultCell->GetCellHalfSize();
	float CellSize = CellHalfSize * 2.0f;

	// UnrealCpp 좌표계: Y가 증가할수록 북쪽(+X)이므로, Height축 시작점을 남쪽 끝에 둔다
	FVector StartLocation = FVector(-Height * CellHalfSize + (Height % 2) * CellHalfSize, -Width * CellHalfSize + (Width % 2) * CellHalfSize, 0);

	UWorld* World = GetWorld();
	if (!World) return;
	// x, y는 MazeData의 좌표계 기준
	for (uint8 y = 0; y < Height; y++)		// y는 북쪽으로 증가함을 나타냄 (UnrealCpp 좌표계)
	{
		for (uint8 x = 0; x < Width; x++)	// X는 동쪽(오른쪽)으로 증가함을 나타냄
		{
			FCellData* Cell = InMaze->GetCell(x, y);
			if (!Cell) continue;

			// 셀의 위치 결정
			FVector CellLoction = StartLocation + FVector(y * CellSize, x * CellSize, 0.0f);
			ACellActor* CellActor = World->SpawnActor<ACellActor>(
				CellActorClass,
				CellLoction,
				FRotator::ZeroRotator
			);
			if (CellActor)
			{
				// 스폰 후 MazeActor의 자식으로 붙인 후 초기화
				CellActor->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
				CellActor->InitializeCell(Cell);
				SpawnedCells.Add(CellActor);
			}
		}
	}
}

void AMazeActor::ClearSpawnedCells()
{
	for (ACellActor* Cell : SpawnedCells)
	{
		if (IsValid(Cell))
		{
			Cell->Destroy();
		}
	}
	SpawnedCells.Empty();
}

void AMazeActor::BuildMazeHISM(FMazeData* InMaze)
{
	if (!InMaze) return;
	if (!FloorHISM->GetStaticMesh() || !WallHISM->GetStaticMesh() || !GateHISM->GetStaticMesh())
	{
		UE_LOG(LogTemp, Log, TEXT("HISM 컴포넌트에 스태틱 메시가 없습니다."));
		return;
	}

	ClearHISMInstances();

	float CellSize = CellHalfSize * 2.0f;
	// UnrealCpp 좌표계: Y가 증가할수록 북쪽(+X)이므로, Height축 시작점을 남쪽 끝에 둔다
	FVector StartLocation = FVector(-Height * CellHalfSize + (Height % 2) * CellHalfSize, -Width * CellHalfSize + (Width % 2) * CellHalfSize, 0);

	for (uint8 y = 0; y < Height; y++)		// y는 북쪽으로 증가함을 나타냄 (UnrealCpp 좌표계)
	{
		for (uint8 x = 0; x < Width; x++)	// X는 동쪽(오른쪽)으로 증가함을 나타냄
		{
			FCellData* Cell = InMaze->GetCell(x, y);
			if (!Cell) continue;

			// 셀의 위치 결정
			FVector CellLoction = StartLocation + FVector(y * CellSize, x * CellSize, 0.0f);
			MakeCellHISM(Cell, CellLoction);
		}
	}
}

void AMazeActor::ClearHISMInstances()
{
	if (FloorHISM)
	{
		FloorHISM->ClearInstances();
	}
	if (WallHISM)
	{
		WallHISM->ClearInstances();
	}
	if (GateHISM)
	{
		GateHISM->ClearInstances();
	}
	FlushPersistentDebugLines(GetWorld());	// 디버그 박스 지우기
}

void AMazeActor::MakeCellHISM(const FCellData* InCell, const FVector& InLocation)
{
	if (!InCell) return;

	// 바닥 추가
	FloorHISM->AddInstance(FTransform(FRotator::ZeroRotator, InLocation));

	// 벽과 문 추가
	// UnrealCpp 좌표계(Y 증가=북쪽)에서는 South/West 벽이 이웃 셀과 공유되므로 매 셀마다 무조건 추가하고,
	// North/East는 미로의 북쪽/동쪽 끝단 셀에서만 경계 벽으로 추가한다.
	FVector Location;
	FRotator Rotate;
	Location = InLocation + FVector::BackwardVector * CellHalfSize;
	Rotate = FRotator(0.0f, 180.0f, 0.0f);
	WallHISM->AddInstance(FTransform(Rotate, Location));		// 남쪽 벽 만들기
	if (InCell->IsWall(EDirectionType::South))
	{
		GateHISM->AddInstance(FTransform(Rotate, Location));	// 남쪽 문 만들기
	}

	Location = InLocation + FVector::LeftVector * CellHalfSize;
	Rotate = FRotator(0.0f, -90.0f, 0.0f);
	WallHISM->AddInstance(FTransform(Rotate, Location));
	if (InCell->IsWall(EDirectionType::West))
	{
		GateHISM->AddInstance(FTransform(Rotate, Location));
	}

	if (InCell->X == Width - 1)	// 동쪽 끝
	{
		Location = InLocation + FVector::RightVector * CellHalfSize;
		Rotate = FRotator(0.0f, 90.0f, 0.0f);
		WallHISM->AddInstance(FTransform(Rotate, Location));
		GateHISM->AddInstance(FTransform(Rotate, Location));	// 동쪽 끝은 막혀있는 것이 확정
	}
	if (InCell->Y == Height - 1) // 북쪽 끝 (UnrealCpp 좌표계: Y가 클수록 북쪽)
	{
		Location = InLocation + FVector::ForwardVector * CellHalfSize;
		Rotate = FRotator::ZeroRotator;
		WallHISM->AddInstance(FTransform(Rotate, Location));
		GateHISM->AddInstance(FTransform(Rotate, Location));	// 북쪽 끝은 막혀있는 것이 확정
	}

#if WITH_EDITOR
	DrawDebugBox(
		GetWorld(),
		InLocation,
		FVector(CellHalfSize, CellHalfSize, 400.0f),
		FColor::Green,
		true,
		-1.0f,
		0,
		3.0f
	);
#endif
}
