// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

struct FCellData;
struct FHuntAndKillCellData;

// 미로 생성에 사용할 알고리즘 종류
UENUM(BlueprintType)
enum class EMazeAlgorithmType : uint8
{
	Wilson			UMETA(DisplayName = "Wilson (Loop-Erased Random Walk)"),
	HuntAndKill		UMETA(DisplayName = "Hunt and Kill"),
};

/**
 *
 */
class UNREALCPP_API FMazeData
{
public:
	FMazeData();

	// Cells가 TUniquePtr를 담고 있어 복사가 불가능하므로 명시적으로 복사를 막는다
	// (UNREALCPP_API로 내보내지는 클래스는 암시적 복사 생성자/대입 연산자가 즉시 생성되려고 시도하기 때문에,
	//  삭제하지 않으면 TUniquePtr의 삭제된 복사 생성자 때문에 컴파일 에러가 발생한다)
	FMazeData(const FMazeData&) = delete;
	FMazeData& operator=(const FMazeData&) = delete;

	~FMazeData();	// FMazeData가 상속이 된다면 반드시 가상 소멸자를 사용해야 한다.

	// 미로를 생성하는 함수
	void MakeMaze(uint8 InWidth, uint8 InHeight, EMazeAlgorithmType InAlgorithm, int32 InSeed = RandomSeed);

	// 생성된 미로를 제거하는 함수
	void ClearMaze();

	// 특정 위치의 셀을 리턴하는 함수
	FCellData* GetCell(uint8 InX, uint8 InY);

private:
	// 윌슨 알고리즘을 실행하는 함수
	void WillsonAlgorithmExecute();

	// 헌트 앤 킬 알고리즘을 실행하는 함수
	void HuntAndKillAlgorithmExecute();

	// InCell 기준 아직 방문하지 않은 이웃 셀 중 하나를 랜덤으로 리턴하는 함수(헌트 앤 킬 전용, 없으면 nullptr)
	FHuntAndKillCellData* GetRandomUnvisitedNeighbor(const FCellData& InCell);

	// 방문한 이웃을 가진, 아직 방문하지 않은 셀을 순회하며 찾는 함수(헌트 앤 킬 전용, 없으면 nullptr)
	FHuntAndKillCellData* HuntForNextCell();

	// From셀과 To셀 사이의 문을 제거하는 함수
	void ConnectCells(FCellData* InFrom, FCellData* InTo);

	// InCell 주변의 셀 중 하나를 랜덤으로 리턴하는 함수(윌슨 전용, 방문 여부와 상관없이 무작위)
	FCellData* GetRandomNeighborCell(const FCellData& InCell);

	// 배열의 순서를 섞는 함수
	template<typename T>
	void ShuffleArray(TArray<T*>& InOutArray)
	{
		for (int32 i = InOutArray.Num() - 1; i > 0; i--)
		{
			int32 Index = RandomStream.RandRange(0, i);
			InOutArray.Swap(i, Index);
		}
	}

	// 위치를 인덱스로 변경하는 함수
	inline uint16 LocationToIndex(uint8 InX, uint8 InY) const { return InX + InY * Width; }

	// 위치가 미로 범위 안인지 확인하는 함수
	inline bool IsValidLocation(uint8 InX, uint8 InY) const { return InX < Width && InY < Height; }

public:
	// 랜덤하게 시드를 선택한다는 상수
	static constexpr int32 RandomSeed = -1;

private:
	// 미로의 가로 칸(Cell) 수
	uint8 Width = 0;
	// 미로의 세로 칸(Cell) 수
	uint8 Height = 0;

	// 미로의 모든 칸(Cell)이 들어있는 배열. 실제 타입은 선택한 알고리즘에 따라 FWilsonCellData 또는 FHuntAndKillCellData
	TArray<TUniquePtr<FCellData>> Cells;

	// 랜덤한 수를 뽑기 위한 스트림
	FRandomStream RandomStream;

	// 이웃 방향의 개수(4방향)
	static constexpr int DirectionCount = 4;
	// 이웃 방향의 상대 좌표(북동남서 순서)
	static const FIntPoint Direction[DirectionCount];
};
