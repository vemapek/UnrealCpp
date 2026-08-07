// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/StreamableManager.h"
#include "ItemDataAsset.generated.h"

class APickupBase;
/**
 *
 */
UCLASS(Abstract, BlueprintType)
class UNREALCPP_API UItemDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	TSharedPtr<FStreamableHandle> RequestDataLoad(FStreamableDelegate InDelegate) const;
	virtual bool IsLoaded() const;

protected:
	virtual void OnAsyncRequest(TArray<FSoftObjectPath>& InOutArray) const;

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base|Data")
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base|Data")
	FText Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base|Data")
	int32 Price = 1;

	// 스폰할 클래스
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base|Spawn")
	TSoftClassPtr<APickupBase> PickupClass;

	// 스폰할 위치의 Offset
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base|Spawn")
	FVector SpawnLocationOffset = FVector::ZeroVector;
};
