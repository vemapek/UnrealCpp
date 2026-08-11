// Fill out your copyright notice in the Description page of Project Settings.


#include "Maze/MazeActor.h"
#include "Maze/CellActor.h"
#include "Maze/CellData.h"

AMazeActor::AMazeActor()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);
}

void AMazeActor::GenerateMaze()
{
	ClearCells();

	MazeData.MakeMaze(Width, Height, Seed);

	SpawnCells();
}

void AMazeActor::BeginPlay()
{
	Super::BeginPlay();

	GenerateMaze();
}

void AMazeActor::SpawnCells()
{
	if (!CellActorClass) return;

	UWorld* World = GetWorld();
	if (!World) return;

	const float CellSize = CellActorClass.GetDefaultObject()->GetCellSize();

	FActorSpawnParameters SpawnParam;
	SpawnParam.Owner = this;

	for (FCellData& Cell : MazeData.GetCells())
	{
		//CellData.X는 동쪽으로, CellData.Y는 북쪽으로 증가하고
		//CellActor는 북쪽=+X, 동쪽=+Y로 벽을 배치하므로 좌표를 맞바꿔서 위치를 계산한다
		const FVector SpawnLocation = GetActorLocation() + FVector(Cell.Y * CellSize, Cell.X * CellSize, 0.0f);
		const FTransform SpawnTransform(GetActorRotation(), SpawnLocation);

		if (ACellActor* NewCell = World->SpawnActor<ACellActor>(CellActorClass, SpawnTransform, SpawnParam))
		{
			NewCell->InitializeCell(&Cell);
			CellActors.Add(NewCell);
		}
	}
}

void AMazeActor::ClearCells()
{
	for (ACellActor* CellActor : CellActors)
	{
		if (CellActor)
		{
			CellActor->Destroy();
		}
	}
	CellActors.Empty();

	MazeData.ClearMaze();
}
