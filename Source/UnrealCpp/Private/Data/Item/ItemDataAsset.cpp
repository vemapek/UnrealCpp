// Fill out your copyright notice in the Description page of Project Settings.

#include "Data/Item/ItemDataAsset.h"
#include "Engine/AssetManager.h"
#include "Item/PickupBase.h"

TSharedPtr<FStreamableHandle> UItemDataAsset::RequestDataLoad(FStreamableDelegate InDelegate) const
{
	TArray<FSoftObjectPath> TargetsToLoad;
	OnAsyncRequest(TargetsToLoad);

	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	return Streamable.RequestAsyncLoad(TargetsToLoad, MoveTemp(InDelegate));
}

bool UItemDataAsset::IsLoaded() const
{
	return PickupClass.IsValid();
}

void UItemDataAsset::OnAsyncRequest(TArray<FSoftObjectPath>& InOutArray) const
{
	InOutArray.Add(PickupClass.ToSoftObjectPath());

	// 상속받은 클래스에서 추가 요소들 추가 등록
}
