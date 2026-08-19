// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/PickupMisc.h"
#include "Data/Item/MiscItemDataAsset.h"
#include "Components/StaticMeshComponent.h"
#include "Interface/InterfaceInventoryUser.h"
#include "Component/InventoryCommandTypes.h"
#include "Framework/PickupFactorySubsystem.h"

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

	if (IInterfaceInventoryUser* InvenUser = TargetActor.IsValid() ? Cast<IInterfaceInventoryUser>(TargetActor.Get()) : nullptr)
	{
		FInventoryCommand Command = FInventoryCommand::MakeAdd(DataAsset, 1);
		FInventoryCommandResult Result;
		if (InvenUser->ExecuteInventoryCommand(Command, Result))
		{
			// 인벤토리에 잘 들어갔으면 픽업 삭제
			Destroy();
			return;
		}
	}

	// 대상이 인벤토리를 안 갖고 있거나, 인벤토리가 꽉 차서 추가에 실패했으면
	// 이 픽업은 삭제하는 대신 근처에 다시 스폰한다.
	if (UWorld* World = GetWorld())
	{
		if (UPickupFactorySubsystem* Factory = World->GetSubsystem<UPickupFactorySubsystem>())
		{
			FTransform SpawnTransform = GetActorTransform();
			FVector RandomOffset(FMath::RandPointInCircle(300.0f), 0.0f);
			SpawnTransform.AddToTranslation(RandomOffset);

			Factory->SpawnPickupAsync(DataAsset, SpawnTransform,
				FOnPickupSpawned::CreateWeakLambda(
					this,
					[](APickupBase* InSpawned)
					{
						if (InSpawned)
						{
							UE_LOG(LogTemp, Log, TEXT("%s가 다시 스폰되었습니다."), *InSpawned->GetName());
						}
					}
				));
		}
	}
	Destroy();
}
