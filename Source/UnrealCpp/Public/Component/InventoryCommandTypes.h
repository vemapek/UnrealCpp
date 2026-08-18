// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/Item/ItemDataAsset.h"
#include "InventoryCommandTypes.generated.h"

UENUM(BlueprintType)
enum class EInventoryCommandType : uint8
{
	None,
	Add,		// 아이템 획득
	Move,		// 아이템 슬롯 간 이동
	Drop,		// 아이템 버리기
	Use,		// 아이템 사용하기
	Money		// 돈 변경
};

USTRUCT(BlueprintType)
struct UNREALCPP_API FInventoryCommand
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = "Inventory|Command")
	EInventoryCommandType Type = EInventoryCommandType::None;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory|Command")
	int32 Count = 1;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory|Command")
	TObjectPtr<UItemDataAsset> ItemData = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory|Command")
	int32 SourceIndex = 0;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory|Command")
	int32 TargetIndex = 0;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory|Command")
	FVector DropPosition = FVector::ZeroVector;

public:
	// 커맨드 생성용 static 헬퍼 함수

	// Add 커맨드를 생성해주는 함수
	static FInventoryCommand MakeAdd(UItemDataAsset* InItemData, int32 InCount = 1)
	{
		FInventoryCommand Command;
		Command.Type = EInventoryCommandType::Add;
		Command.ItemData = InItemData;
		Command.Count = InCount;
		return Command;
	}

	// Move 커맨드를 생성해주는 함수
	static FInventoryCommand MakeMove(int32 InSourceIndex, int32 InTargetIndex)
	{
		FInventoryCommand Command;
		Command.Type = EInventoryCommandType::Move;
		Command.SourceIndex = InSourceIndex;
		Command.TargetIndex = InTargetIndex;
		return Command;
	}

	// Drop 커맨드를 생성해주는 함수
	static FInventoryCommand MakeDrop(int32 InSlotIndex, const FVector& InDropLocation)
	{
		FInventoryCommand Command;
		Command.Type = EInventoryCommandType::Drop;
		Command.SourceIndex = InSlotIndex;
		Command.DropPosition = InDropLocation;
		return Command;
	}

	// Use 커맨드를 생성해주는 함수
	static FInventoryCommand MakeUse(int32 InSlotIndex)
	{
		FInventoryCommand Command;
		Command.Type = EInventoryCommandType::Use;
		Command.SourceIndex = InSlotIndex;
		return Command;
	}

	// Money 커맨드를 생성해주는 함수
	static FInventoryCommand MakeMoney(int32 InMoneyDiff)
	{
		FInventoryCommand Command;
		Command.Type = EInventoryCommandType::Money;
		Command.Count = InMoneyDiff;
		return Command;
	}
};

USTRUCT(BlueprintType)
struct UNREALCPP_API FInventoryCommandResult
{
	GENERATED_BODY()

	// 커맨드 성공 여부
	UPROPERTY(BlueprintReadOnly)
	bool bSuccess = false;

	// Add 커맨드용 변수(인벤토리에 들어가지 못한 아이템 개수)
	UPROPERTY(BlueprintReadOnly)
	int32 RemainingCount = 0;
};
