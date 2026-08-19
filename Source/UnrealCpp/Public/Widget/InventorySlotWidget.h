// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlotWidget.generated.h"

class UImage;
class UTextBlock;
class UHorizontalBox;
class UInventoryComponent;
struct FInvenSlot;

/**
 *
 */
UCLASS()
class UNREALCPP_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitializeSlot(UInventoryComponent* InInven, int32 InIndex);
	void RefreshSlot() const;

protected:
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

protected:
	// 아이콘 이미지
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> IconImage;

	// 아이템 개수와 스택 수를 표현하는 텍스트를 묶는 수평 박스
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UHorizontalBox> CountBox;

	// 아이템 개수
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> CountText;

	// 아이템 최대 스택 가능 수
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> MaxStackText;

private:
	// 비정상 인덱스 표현용 상수
	static constexpr int32 InvalidIndex = -1;

	// 바인딩 된 슬롯을 가지고 있을 인벤토리
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<UInventoryComponent> TargetInventory = nullptr;

	// 인벤토리의 슬롯 인덱스
	int32 Index = InvalidIndex;

	// 인벤토리의 슬롯
	const FInvenSlot* Slot = nullptr;	// 구조체는 TWeakObjectPtr로 저장안됨

};
