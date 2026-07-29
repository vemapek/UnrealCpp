// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/PickupBase.h"
#include "Data/WeaponDataAsset.h"
#include "PickupWeapon.generated.h"

/**
 *
 */
UCLASS()
class UNREALCPP_API APickupWeapon : public APickupBase
{
	GENERATED_BODY()

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void OnPickup(AActor* InTarget) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UWeaponDataAsset> WeaponData = nullptr;
};