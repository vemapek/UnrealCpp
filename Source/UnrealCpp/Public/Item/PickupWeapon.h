// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/PickupBase.h"
#include "PickupWeapon.generated.h"

class UWeaponDataAsset;

/**
 * 무기 픽업. 줍는 연출(날아오는 연출)이 끝나면 다른 아이템처럼 인벤토리에 추가된다.
 * 장착은 인벤토리에서 우클릭(사용)했을 때 이루어진다.
 */
UCLASS()
class UNREALCPP_API APickupWeapon : public APickupBase
{
	GENERATED_BODY()

public:
	virtual void InitializePickup(UItemDataAsset* InData) override;

protected:
	virtual void OnFinishPickupEffect() override;
};
