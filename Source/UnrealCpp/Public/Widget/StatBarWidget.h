// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatBarWidget.generated.h"

class UProgressBar;
class UTextBlock;

/**
 * 체력/스태미나 등 "현재값 / 최댓값" 형태의 스탯을 게이지 바로 보여주는 재사용 위젯
 */
UCLASS()
class UNREALCPP_API UStatBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 현재값/최댓값을 받아서 게이지 채움 비율과 텍스트를 갱신
	UFUNCTION(BlueprintCallable)
	void UpdateStat(float InCurrent, float InMax);

protected:
	virtual void NativePreConstruct() override;

#if WITH_EDITOR
	// #if ~ #endif 사이의 코드는 에디터 상에서만 존재한다.
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:
	// 채워지는 게이지 바 (에디터에서 이름을 "Bar"로 맞춰야 함)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UProgressBar> Bar;

	// 현재값 텍스트 (에디터에서 이름을 "CurrentText"로 맞춰야 함)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> CurrentText;

	// 최댓값 텍스트 (에디터에서 이름을 "MaxText"로 맞춰야 함)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> MaxText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor FillColor = FLinearColor(0.0f, 1.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor BackgroundColor = FLinearColor(0.0f, 1.0f, 0.0f, 0.2f);
};