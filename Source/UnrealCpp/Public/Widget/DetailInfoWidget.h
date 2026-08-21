// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DetailInfoWidget.generated.h"

class UItemDataAsset;
class UImage;
class UTextBlock;
class UCanvasPanelSlot;

/**
 * 인벤토리 슬롯에 마우스를 올렸을 때 마우스를 따라다니며 아이템 상세 정보를 보여주는 위젯
 */
UCLASS()
class UNREALCPP_API UDetailInfoWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Open(const UItemDataAsset* InItemData);
	void Close();

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	// 마우스 위치를 따라 이 위젯의 위치를 갱신하는 함수
	void UpdateLocation();

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> ItemIcon;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemNameText;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemPriceText;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemDescriptionText;

private:
	// 열려있는 동안에만 위치 갱신(Tick)을 하기 위한 플래그
	bool bTickEnable = false;

	// 위치 갱신에 사용할 이 위젯의 캔버스 슬롯
	TWeakObjectPtr<UCanvasPanelSlot> CanvasSlot;
};
