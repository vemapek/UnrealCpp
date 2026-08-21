// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/Item/MiscItemDataAsset.h"
#include "Data/Item/Action/ItemAction.h"
#include "UseableItemDataAsset.generated.h"

/**
 * 인벤토리에서 사용(우클릭)할 수 있는 소모품 아이템(포션, 돈주머니 등)
 */
UCLASS()
class UNREALCPP_API UUseableItemDataAsset : public UMiscItemDataAsset
{
	GENERATED_BODY()

public:
	// UItemAction의 UPROPERTY 설정으로 인해 에디터 인라인 생성 및 직렬화 자동 보장
	UPROPERTY(EditAnywhere, Category = "Action")
	TObjectPtr<UItemAction> ItemAction = nullptr;
};
