// Fill out your copyright notice in the Description page of Project Settings.

#include "Framework/PickupFactorySubsystem.h"
#include "Data/Item/ItemDataAsset.h"
#include "Item/PickupBase.h"

void UPickupFactorySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogTemp, Log, TEXT("[PickupFactorySubsystem] : Initialized"));
}

void UPickupFactorySubsystem::Deinitialize()
{
	// 진행중인 비동기 로딩 핸들 정리(게임 인스턴스가 종료될 경우에는 콜백을 호출하지 않음)
	for (TSharedPtr<FStreamableHandle>& Handle : ActiveStreamableHandles)
	{
		if (Handle.IsValid() && Handle->IsActive())
		{
			Handle->CancelHandle();
		}
	}
	ActiveStreamableHandles.Empty();
	UE_LOG(LogTemp, Log, TEXT("[PickupFactorySubsystem] : Deinitialized"));

	Super::Deinitialize();
}

APickupBase* UPickupFactorySubsystem::SpawnPickup(UItemDataAsset* InItemDataAsset, const FTransform& InTransform)
{
	if (!InItemDataAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PickupFactorySubsystem] : InItemDataAsset가 null입니다."));
		return nullptr;
	}
	if (!InItemDataAsset->IsLoaded())
	{
		UE_LOG(LogTemp, Warning, TEXT("[PickupFactorySubsystem] : %s의 SoftPtr이 모두 로드되지 않았습니다."),
			*InItemDataAsset->DisplayName.ToString());
		return nullptr;
	}

	return SpawnProcess(InItemDataAsset, InTransform);
}

void UPickupFactorySubsystem::SpawnPickupAsync(UItemDataAsset* InItemDataAsset, const FTransform& InTransform, FOnPickupSpawned OnSpawned)
{
	if (!InItemDataAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PickupFactorySubsystem] : InItemDataAsset가 null입니다."));
		OnSpawned.ExecuteIfBound(nullptr);
		return;
	}
	// 로드 되었으면 즉시 스폰
	if (InItemDataAsset->IsLoaded())
	{
		APickupBase* Spawned = SpawnProcess(InItemDataAsset, InTransform);
		OnSpawned.ExecuteIfBound(Spawned);
		return;
	}

	// 로딩이 안되었으니 비동기 로딩 요청
	TWeakObjectPtr<UItemDataAsset> WeakDataAsset(InItemDataAsset);
	TSharedPtr<FStreamableHandle> Handle = InItemDataAsset->RequestDataLoad(
		FStreamableDelegate::CreateWeakLambda(
			this,
			[this, WeakDataAsset, InTransform, OnSpawned]()
			{
				if (!WeakDataAsset.IsValid())
				{
					UE_LOG(LogTemp, Warning, TEXT("[PickupFactorySubsystem] : 비동기 로딩 요청 중 InItemDataAsset이 Invalid합니다."));
					OnSpawned.ExecuteIfBound(nullptr);
					CleanupCompletedHandles();
					return;
				}

				UItemDataAsset* LoadedItemDataAsset = WeakDataAsset.Get();
				APickupBase* Spawned = SpawnProcess(LoadedItemDataAsset, InTransform);
				OnSpawned.ExecuteIfBound(Spawned);
				UE_LOG(LogTemp, Log, TEXT("[PickupFactorySubsystem] : 비동기 스폰 완료(%s)"),
					*LoadedItemDataAsset->DisplayName.ToString());
				CleanupCompletedHandles();
			}
		)
	);

	if (Handle.IsValid())
	{
		ActiveStreamableHandles.Add(Handle);
	}
	else
	{
		OnSpawned.ExecuteIfBound(nullptr);
		UE_LOG(LogTemp, Warning, TEXT("[PickupFactorySubsystem] : 비동기 로딩 요청에 실패했습니다."));
	}
}

void UPickupFactorySubsystem::K2_SpawnPickupAsync(UItemDataAsset* InItemDataAsset, const FTransform& InTransform, FOnPickupSpawnedDynamic OnSpawned)
{
	SpawnPickupAsync(InItemDataAsset, InTransform,
		FOnPickupSpawned::CreateLambda(
			[OnSpawned](APickupBase* SpawnedPickup)
			{
				OnSpawned.ExecuteIfBound(SpawnedPickup);
			}
		)
	);
}

APickupBase* UPickupFactorySubsystem::SpawnProcess(UItemDataAsset* InItemDataAsset, const FTransform& InTransform)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PickupFactorySubsystem] : World가 없습니다."));
		return nullptr;
	}
	UClass* PickupClass = InItemDataAsset->PickupClass.Get();
	if (!PickupClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PickupFactorySubsystem] : %s의 PickupClass가 없습니다."),
			*InItemDataAsset->DisplayName.ToString());
		return nullptr;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	APickupBase* Spawned = World->SpawnActor<APickupBase>(
		PickupClass, InTransform, SpawnParams);
	if (Spawned)
	{
		Spawned->InitializePickup(InItemDataAsset);
		UE_LOG(LogTemp, Log, TEXT("[PickupFactorySubsystem] : %s를 %s 위치에 스폰했습니다."),
			*InItemDataAsset->DisplayName.ToString(), *InTransform.GetLocation().ToString());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[PickupFactorySubsystem] : %s를 스폰하는데 실패했습니다."),
			*InItemDataAsset->DisplayName.ToString());
	}

	return Spawned;
}

void UPickupFactorySubsystem::CleanupCompletedHandles()
{
	ActiveStreamableHandles.RemoveAll(
		[](const TSharedPtr<FStreamableHandle>& Handle)
		{
			return !Handle.IsValid() || Handle->HasLoadCompleted();
		}
	);
}
