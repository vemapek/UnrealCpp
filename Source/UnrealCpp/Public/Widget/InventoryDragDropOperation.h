// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "InventoryDragDropOperation.generated.h"

class UItemDataAsset;

/**
 * 인벤토리 슬롯 드래그 앤 드롭에 사용하는 데이터. 드래그가 시작된 슬롯 인덱스와
 * 드래그 비주얼(TemporarySlotWidget) 표시에 필요한 아이템 정보를 담는다.
 */
UCLASS()
class UNREALCPP_API UInventoryDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:
	// 드래그가 시작된 슬롯의 인덱스(드롭 시 Move 커맨드의 SourceIndex로 사용)
	UPROPERTY(BlueprintReadOnly)
	int32 SourceIndex = INDEX_NONE;

	// 드래그 중인 아이템 데이터(드래그 비주얼 표시용)
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UItemDataAsset> ItemData = nullptr;

	// 드래그 중인 아이템 개수(드래그 비주얼 표시용)
	UPROPERTY(BlueprintReadOnly)
	int32 Count = 0;
};
