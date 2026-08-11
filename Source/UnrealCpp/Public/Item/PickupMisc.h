// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/PickupBase.h"
#include "PickupMisc.generated.h"

class UMiscItemDataAsset;

/**
 * 기타 아이템(소모품 등) 픽업. unrealcpp의 픽업들은 전부 StaticMesh를 쓰기 때문에
 * 10th처럼 별도의 Mesh 컴포넌트를 새로 만들지 않고, PickupBase가 이미 갖고 있는
 * 공용 Mesh(StaticMeshComponent)를 그대로 사용한다.
 */
UCLASS()
class UNREALCPP_API APickupMisc : public APickupBase
{
	GENERATED_BODY()

public:
	virtual void InitializePickup(UItemDataAsset* InData) override;

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

private:
	UPROPERTY(Transient)
	TWeakObjectPtr<UMiscItemDataAsset> MiscData;
};
