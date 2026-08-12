// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))	// 블루프린트에서 비트플래그로 사용
enum class EDirectionType : uint8
{
	None	= 0			UMETA(Hidden),
	North	= 1 << 0	UMETA(DisplayName = "North"),
	East	= 1 << 1	UMETA(DisplayName = "East"),
	South	= 1 << 2	UMETA(DisplayName = "South"),
	West	= 1 << 3	UMETA(DisplayName = "West")
};
ENUM_CLASS_FLAGS(EDirectionType)		// 비트 연산자 오버로딩

/**
 * 미로를 구성하는 셀의 정보를 담을 구조체
 */
struct UNREALCPP_API FCellData
{
public:
	// 셀의 X좌표
	uint8 X = 0;
	// 셀의 Y좌표
	uint8 Y = 0;

	// 이 셀에 열려있는 문의 방향
	EDirectionType Path = EDirectionType::None;

	// 미로 생성 과정에서 이 셀이 미로에 포함되어 있는지 여부
	bool bInMaze = false;

	// 미로 생성 과정에서 다음셀을 기록하기 위한 변수
	FCellData* NextCell = nullptr;

	// 셀의 좌표를 가져오는 함수
	inline FIntPoint GetLocation() const { return FIntPoint(X, Y); }

	// 셀에 길을 추가하는 함수
	inline void AddPath(EDirectionType InAddedPath) { Path |= InAddedPath; }

	// 특정 방향이 길인지 확인하는 함수(길이면 true)
	inline bool IsPath(EDirectionType InCheck) const { return (Path & InCheck) == InCheck; }

	// 특정 방향이 벽인지 확인하는 함수(벽이면 true)
	inline bool IsWall(EDirectionType InCheck) const { return !IsPath(InCheck); }
};
