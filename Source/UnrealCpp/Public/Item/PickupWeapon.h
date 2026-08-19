// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/PickupBase.h"
#include "PickupWeapon.generated.h"

class UWeaponDataAsset;

/**
 * 무기 픽업. 줍는 연출(날아오는 연출)이 끝나면 인벤토리를 거치지 않고
 * 대상에게 바로 장착(EqueipWeapon)시킨다.
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
