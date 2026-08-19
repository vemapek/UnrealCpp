// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UButton;
class UMoneyPanelWidget;
class UUniformGridPanel;
class UInventoryComponent;
class UInventorySlotWidget;

/**
 *
 */
UCLASS()
class UNREALCPP_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 인벤토리 설정용(인벤토리 컴포넌트와 인벤토리 위젯을 연결)
	void InitializeInventoryWidget(UInventoryComponent* InInven);
	// 인벤토리 초기화용(인벤토리 위젯과 인벤토리 컴포넌트 사이의 연결을 제거)
	void ClearInventoryWidget();

	// 인벤토리 열기
	void OpenInventoryWidget();
	// 인벤토리 닫기
	void CloseInventoryWidget();

#if WITH_EDITOR
	void TestRefresh();
#endif

protected:
	// 인벤토리 위젯 전체 갱신
	void RefreshInventoryWidget() const;
	// 인벤토리 내의 특정 슬롯 위젯 갱신
	void RefreshSlotWidget(int32 InSlotIndex) const;
	// 입력 받은 돈으로 머니 패널 위젯 갱신
	void RefreshMoneyPanel(int32 InCurrentMoney) const;

	virtual void NativeConstruct() override;

private:
	// CloseButton이 눌려졌을 때 실행될 함수
	UFUNCTION()
	void OnClickedCloseButton();

	// 확인할 인덱스가 유효한 범위인지 확인
	inline bool IsValidIndex(int32 InIndex) const { return InIndex >= 0 && InIndex < SlotSize; }

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> CloseButton;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UMoneyPanelWidget> MoneyPanel;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> SlotGridPanel;

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<UInventoryComponent> TargetInventory = nullptr;

	UPROPERTY()
	TArray<TObjectPtr<UInventorySlotWidget>> SlotWidgets;

	// 슬롯의 개수(위젯의 슬롯 개수와 컴포넌트의 슬롯 개수 중 작은 수)
	int32 SlotSize = 0;

};
