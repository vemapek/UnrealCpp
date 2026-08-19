// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/PickupWeapon.h"
#include "Interface/InterfaceWeaponUser.h"
#include "Data/Item/WeaponDataAsset.h"
#include "Components/StaticMeshComponent.h"

void APickupWeapon::InitializePickup(UItemDataAsset* InData)
{
	Super::InitializePickup(InData);

	if (UWeaponDataAsset* CastedWeaponData = Cast<UWeaponDataAsset>(DataAsset))
	{
		if (UStaticMesh* StaticMeshData = CastedWeaponData->Mesh.LoadSynchronous())
		{
			Mesh->SetStaticMesh(StaticMeshData);
			Mesh->SetRelativeLocation(MeshBaseLocation + CastedWeaponData->SpawnLocationOffset);
		}
	}
}

void APickupWeapon::OnFinishPickupEffect()
{
	GetWorldTimerManager().ClearTimer(PickupEffectTimerHandle);

	if (UWeaponDataAsset* CurrentWeaponData = Cast<UWeaponDataAsset>(DataAsset))
	{
		if (TargetActor.IsValid())
		{
			IInterfaceWeaponUser::Execute_EqueipWeapon(TargetActor.Get(), CurrentWeaponData);
		}
	}
	Destroy();
}
