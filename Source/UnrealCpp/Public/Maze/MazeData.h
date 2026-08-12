// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

struct FCellData;
/**
 *
 */
class UNREALCPP_API FMazeData
{
public:
	~FMazeData();	// FMazeData가 상속이 된다면 반드시 가상 소멸자를 사용해야 한다.

	// 미로를 생성하는 함수
	void MakeMaze(uint8 InWidth, uint8 InHeight, int32 InSeed = RandomSeed);

	// 생성된 미로를 제거하는 함수
	void ClearMaze();

	// 특정 위치의 셀을 리턴하는 함수
	FCellData* GetCell(uint8 InX, uint8 InY);

private:
	// 윌슨 알고리즘을 실행하는 함수
	void WillsonAlgorithmExecute();

	// From셀과 To셀 사이의 문을 제거하는 함수
	void ConnectCells(FCellData* InFrom, FCellData* InTo);

	// InCell 주변의 셀 중 하나를 랜덤으로 리턴하는 함수
	FCellData* GetRandomNeighborCell(const FCellData& InCell);

	// 배열의 순서를 섞는 함수
	void ShuffleArray(TArray<FCellData*>& InOutArray);

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

	// 미로의 모든 칸(Cell)이 들어있는 배열
	TArray<FCellData> Cells;

	// 랜덤한 수를 뽑기 위한 스트림
	FRandomStream RandomStream;

	// 이웃 방향의 개수(4방향)
	static constexpr int DirectionCount = 4;
	// 이웃 방향의 상대 좌표(북동남서 순서)
	static const FIntPoint Direction[DirectionCount];
};
