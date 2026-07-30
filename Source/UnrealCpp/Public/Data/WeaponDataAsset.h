// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/StreamableManager.h"
#include "WeaponDataAsset.generated.h"

class UStaticMesh;
class UNiagaraSystem;

/**
 *
 */
UCLASS(BlueprintType)
class UNREALCPP_API UWeaponDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	TSharedPtr<FStreamableHandle> RequestDataLoad(FStreamableDelegate InDelegate) const;
	bool IsLoaded() const;

public:
	// 무기의 메시
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Appearance")
	TSoftObjectPtr<UStaticMesh> Mesh;

	// 무기가 Attach될 소켓
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Appearance")
	FName AttachSocketName = TEXT("hand_rSocket");

	// 무기 Attach할 위치의 Offset
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Appearance")
	FVector LocationOffset = FVector::ZeroVector;

	// HitArea 캡슐의 높이 절반
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HitArea")
	float HitAreaHalfHeight = 60.0f;

	// HitArea 캡슐의 반지름
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HitArea")
	float HitAreaRadius = 30.0f;

	// 무기의 공격력
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	float AttackPower = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	TObjectPtr<UNiagaraSystem> HitEffect;

	// 사용 횟수가 제한된 소모성 무기인지 여부 (false면 무한으로 사용 가능한 기본 무기)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	bool bIsConsumable = false;

	// 소모성 무기일 때 사용 가능한 최대 횟수 (공격 1회당 1씩 소모되며, 0이 되면 자동으로 버려짐)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData", meta = (EditCondition = "bIsConsumable", ClampMin = "1"))
	int32 MaxUseCount = 1;
};
