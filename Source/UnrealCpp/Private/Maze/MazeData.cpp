// Fill out your copyright notice in the Description page of Project Settings.


#include "Maze/MazeData.h"
#include "Maze/CellData.h"

const FIntPoint FMazeData::Direction[DirectionCount] =
{
	FIntPoint(0,1), FIntPoint(1,0), FIntPoint(0,-1), FIntPoint(-1,0)
};

FMazeData::~FMazeData()
{
	ClearMaze();	// 안전장치
}

void FMazeData::MakeMaze(uint8 InWidth, uint8 InHeight, int32 InSeed)
{
	ClearMaze();					// 이전에 만들어진 데이터 삭제하기

	Width = InWidth;
	Height = InHeight;

	if (InSeed == RandomSeed)
	{
		// 랜덤하게 시드값 설정
		RandomStream.GenerateNewSeed();
	}
	else
	{
		// 정해진 값으로 시드값 설정
		RandomStream.Initialize(InSeed);
	}

	Cells.SetNum(Width * Height);	// 배열 초기화(SetNum으로 실제 배열 요소도 생성)

	WillsonAlgorithmExecute();
}

void FMazeData::ClearMaze()
{
	Width = 0;
	Height = 0;
	Cells.Empty();
}

void FMazeData::WillsonAlgorithmExecute()
{
	// 기본 초기화
	TArray<FCellData*> NotInMazeCells;
	NotInMazeCells.Reserve(Width * Height);
	for (int32 y = 0; y < Height; y++)
	{
		for (int32 x = 0; x < Width; x++)
		{
			FCellData* Cell = GetCell(x, y);	// 기본 값으로 들어있는 셀을 가져와서
			Cell->X = x;						// 위치 설정하고
			Cell->Y = y;
			NotInMazeCells.Add(Cell);			// 미로에 포함되지 않은 셀 목록에 추가
		}
	}
	ShuffleArray(NotInMazeCells);

	// 윌슨 알고리즘 시작
	FCellData* InitCell = NotInMazeCells.Pop();	// 미로가 아닌 셀 중 하나를 초기셀로 지정하고 셀 목록에서 제거
	InitCell->bInMaze = true;

	while (NotInMazeCells.Num() > 0)	// 미로에 포함되지 않은 셀이 남아있으면 계속 반복
	{
		FCellData* StartCell = NotInMazeCells.Pop();	// 미로에 포함되지 않은 셀 중 하나를 랜덤으로 선택
		if (StartCell->bInMaze)
		{
			continue;	// 이미 미로에 포함된 셀은 스킵(NotInMazeCells에서 제거하는 역할)
		}

		FCellData* CurrentCell = StartCell;
		do
		{
			// 랜덤 워크 진행
			FCellData* NeighborCell = GetRandomNeighborCell(*CurrentCell);	// 랜덤한 이웃 셀 구해서
			CurrentCell->NextCell = NeighborCell;	// 다음 셀로 저장하고
			CurrentCell = NeighborCell;				// 이웃 셀 기준으로 계속 진행
		} while (!CurrentCell->bInMaze);

		// 경로에 따라 미로에 포함시키기
		FCellData* Path = StartCell;
		while (Path != CurrentCell)	// 미로에 포함되어 있는 셀에 도달할 때까지 반복
		{
			Path->bInMaze = true;					// 미로에 포함시키고
			ConnectCells(Path, Path->NextCell);		// 경로를 뚫고
			Path = Path->NextCell;					// 다음 셀 처리
		}
	}
}

FCellData* FMazeData::GetCell(uint8 InX, uint8 InY)
{
	if (!IsValidLocation(InX, InY)) return nullptr;

	return &(Cells[LocationToIndex(InX, InY)]);
}

void FMazeData::ConnectCells(FCellData* InFrom, FCellData* InTo)
{
	// From과 To가 모두 있어야 하고, 서로 다른 셀이어야 한다.
	if (!InFrom || !InTo || (InFrom == InTo)) return;

	if (InFrom->X < InTo->X)
	{
		// From의 동쪽에 To가 있다.
		InFrom->AddPath(EDirectionType::East);
		InTo->AddPath(EDirectionType::West);
	}
	else if (InFrom->X > InTo->X)
	{
		// From의 서쪽에 To가 있다.
		InFrom->AddPath(EDirectionType::West);
		InTo->AddPath(EDirectionType::East);
	}
	else if (InFrom->Y < InTo->Y)
	{
		// From의 북쪽에 To가 있다. (UnrealCpp 좌표계: Y가 클수록 북쪽)
		InFrom->AddPath(EDirectionType::North);
		InTo->AddPath(EDirectionType::South);
	}
	else //if (InFrom->Y > InTo->Y)
	{
		// From의 남쪽에 To가 있다.
		InFrom->AddPath(EDirectionType::South);
		InTo->AddPath(EDirectionType::North);
	}
}

FCellData* FMazeData::GetRandomNeighborCell(const FCellData& InCell)
{
	FIntPoint NeighborLoc;

	do
	{
		int32 Index = RandomStream.RandRange(0, DirectionCount - 1); // 0,1,2,3 중 하나가 랜덤으로 결정
		NeighborLoc = InCell.GetLocation() + Direction[Index];
	} while (!IsValidLocation(NeighborLoc.X, NeighborLoc.Y));	// 미로 밖을 선택하는 일을 방지

	return GetCell(static_cast<uint8>(NeighborLoc.X), static_cast<uint8>(NeighborLoc.Y));
}

void FMazeData::ShuffleArray(TArray<FCellData*>& InOutArray)
{
	for (int i = InOutArray.Num() - 1; i > 0; i--)
	{
		int32 Index = RandomStream.RandRange(0, i);
		InOutArray.Swap(i, Index);
	}
}
