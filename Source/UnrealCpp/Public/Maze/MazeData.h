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
	~FMazeData();


	//미로를 생성하는 함수
	void MakeMaze(uint8 InWidth, uint8 InHeight, int32 InSeed = RandomSeed);

	//생성된 미로를 제거하는 함수
	void ClearMaze();

	//생성된 미로의 가로 칸 수를 가져오는 함수
	inline uint8 GetWidth() const { return Width; }

	//생성된 미로의 세로 칸 수를 가져오는 함수
	inline uint8 GetHeight() const { return Height; }

	//생성된 미로의 모든 셀 데이터를 가져오는 함수
	inline TArray<FCellData>& GetCells() { return Cells; }

private:
	//윌슨 알고리즘 실행 함수
	void WillsonAlgorithmExexute();

	// 특정 위치 셀을 리턴하는 함수
	FCellData* GetCell(uint8 InX, uint8 InY);

	//From과 To셀 사이의 문을 제거하느 함수
	void ConnectCells(FCellData* InFrom, FCellData* InTo);

	// InCell 주변의 셀 중 하나를 랜덤으로 리턴하는 함수
	FCellData* GetRandomNeighborCell(const FCellData& InCell);

	//배열의 순서를 섞는 함수
	void ShuffleArray(TArray<FCellData*>& InOutArray);

	//위치를 인덱스로 변경하는 함수
	inline uint16 LocationToIndex(uint8 InX, uint8 InY)const { return InX + InY * Width; }

	// 위치가 미로 범위 안인지 확인하는 함수
	inline bool IsValidLocation(uint8 InX, uint8 InY)const { return InX < Width && InY < Height; }



private:
	static constexpr int32 RandomSeed = -1;

	//미로 가로 칸 
	uint8 Width = 0;

	//미로 세로칸
	uint8 Height = 0;

	//미로의 모든 칸이 들어있는 배열
	TArray<FCellData> Cells;

	//랜덤한 수를 뽑기 위한 스트림
	FRandomStream RandomStream;

	static constexpr int DirectionCount = 4;
	static const FIntPoint Direction[DirectionCount];
};
