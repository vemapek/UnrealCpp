// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "ture"))
enum class EDirectionType : uint8
{
	None = 0 UMETA(Hidden),
	North = 1 << 0 UMETA(DisplayName = "North"),
	East = 1 << 1 UMETA(DisplayName = "East"),
	South = 1 << 2 UMETA(DisplayName = "South"),
	West = 1 << 3 UMETA(DisplayName = "West"),

};
ENUM_CLASS_FLAGS(EDirectionType)

struct UNREALCPP_API FCellData
{

public:
	//셀 x 자표
	uint8 X = 0;
	//셀 y 좌표
	uint8 Y = 0;

	//이 셀에 열려있는 문의 방향
	EDirectionType Path = EDirectionType::None;

	//미로 생성 과정에서 이 셀이 미로에 포함되어 있는지 여부
	bool bInMaze = false;

	//미로 생성과정에서 다음셀을 기록하기 위한 변수
	FCellData* NextCell = nullptr;

	//셀의 좌표를 가져오는 함수
	inline FIntPoint GetLocation() const { return FIntPoint(X, Y); }

	//셀에 길을 추가하는 함수
	inline void AddPath(EDirectionType InAddPath) { Path |= InAddPath; }

	//특정 방향이 길인지 확인 
	inline bool IsPath(EDirectionType InCheck) const { return (Path & InCheck) == InCheck; }

	inline bool IsWall(EDirectionType InCheck) const { return !IsPath(InCheck); }
};

