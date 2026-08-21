// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TemporarySlotWidget.generated.h"

class UImage;
class UTextBlock;
class UItemDataAsset;

/**
 * 인벤토리 슬롯을 드래그하는 동안 마우스를 따라다니며 보여지는 드래그 비주얼 위젯
 */
UCLASS()
class UNREALCPP_API UTemporarySlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 드래그 중인 아이템의 아이콘/개수를 표시하도록 설정
	void SetItem(const UItemDataAsset* InItemData, int32 InCount);

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> IconImage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> CountText;
};
