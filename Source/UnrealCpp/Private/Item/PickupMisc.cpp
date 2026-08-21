// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/PickupMisc.h"
#include "Data/Item/MiscItemDataAsset.h"
#include "Components/StaticMeshComponent.h"

void APickupMisc::InitializePickup(UItemDataAsset* InData)
{
	Super::InitializePickup(InData);

	if (DataAsset)
	{
		MiscData = Cast<UMiscItemDataAsset>(DataAsset);
		if (MiscData.IsValid())
		{
			if (UStaticMesh* MeshData = MiscData->Mesh.LoadSynchronous())
			{
				Mesh->SetStaticMesh(MeshData);
				Mesh->SetRelativeLocation(MeshBaseLocation + MiscData->SpawnLocationOffset);
			}
		}
	}
}

void APickupMisc::OnFinishPickupEffect()
{
	GetWorldTimerManager().ClearTimer(PickupEffectTimerHandle);
	TryAddToInventoryOrThrowBack();
}
