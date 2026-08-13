// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine/StreamableManager.h"
#include "PickupFactorySubsystem.generated.h"

class APickupBase;
class UItemDataAsset;

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnPickupSpawnedDynamic, APickupBase*, SpawnedPickup);
DECLARE_DELEGATE_OneParam(FOnPickupSpawned, APickupBase*);

/**
 *
 */
UCLASS()
class UNREALCPP_API UPickupFactorySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// 픽업 액터 스폰(동기 방식)
	UFUNCTION(BlueprintCallable, Category = "Factory|Pickup")
	APickupBase* SpawnPickup(UItemDataAsset* InItemDataAsset, const FTransform& InTransform);

	// 픽업 액터 스폰(비동기 방식, 로딩 완료 후 OnSpawned 델리게이트 호출)
	void SpawnPickupAsync(UItemDataAsset* InItemDataAsset, const FTransform& InTransform,
		FOnPickupSpawned OnSpawned);

	UFUNCTION(BlueprintCallable, Category = "Factory|Pickup", meta = (DisplayName = "Spawn Pickup Async"))
	void K2_SpawnPickupAsync(UItemDataAsset* InItemDataAsset, const FTransform& InTransform,
		FOnPickupSpawnedDynamic OnSpawned);

	// USubSystem 함수 오버라이드 ------------------------------------------------------------
	// 서브시스템이 생성되었을 때 실행될 함수
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	// 서브시스템이 삭제될 때 실행될 함수
	virtual void Deinitialize() override;
	//---------------------------------------------------------------------------------------

private:
	// 실제 스폰을 처리할 함수
	APickupBase* SpawnProcess(UItemDataAsset* InItemDataAsset, const FTransform& InTransform);

	// 비동기 작업의 핸들 중 완료된 핸들을 정리하는 함수
	void CleanupCompletedHandles();

private:
	// 진행중인 비동기 작업의 핸들 모음
	TArray<TSharedPtr<FStreamableHandle>> ActiveStreamableHandles;
};
