// Fill out your copyright notice in the Description page of Project Settings.
// 새 위치: Source/UnrealCpp/Private/Data/Item/WeaponDataAsset.cpp
// 기존 Source/UnrealCpp/Private/Data/WeaponDataAsset.cpp는 삭제해주세요.

#include "Data/Item/WeaponDataAsset.h"

bool UWeaponDataAsset::IsLoaded() const
{
	return Super::IsLoaded() && Mesh.IsValid();
}

void UWeaponDataAsset::OnAsyncRequest(TArray<FSoftObjectPath>& InOutArray) const
{
	Super::OnAsyncRequest(InOutArray);
	InOutArray.Add(Mesh.ToSoftObjectPath());
}
