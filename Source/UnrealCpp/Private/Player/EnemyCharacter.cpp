// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/EnemyCharacter.h"
#include "Component/StatActorComponent.h"
#include "Components/CapsuleComponent.h"
#include "UnrealCpp/UnrealCpp.h"
#include "Item/ItemDropTable.h"
#include "Item/PickupBase.h"
#include "Data/Item/ItemDataAsset.h"
#include "Framework/PickupFactorySubsystem.h"

AEnemyCharacter::AEnemyCharacter()
{
	GetCapsuleComponent()->SetCollisionObjectType(ECC_Enemy);
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (UStatActorComponent* StatComp = GetStatComponent())
	{
		StatComp->OnDie.AddDynamic(this, &AEnemyCharacter::OnDie);
	}
}

void AEnemyCharacter::OnDie()
{
	UE_LOG(LogTemp, Log, TEXT("%s가 죽었습니다."), *this->GetName());

	// 아이템 드랍 처리
	OnItemDrop();
}

void AEnemyCharacter::OnItemDrop()
{
	if (!GetWorld()) return;
	if (ItemDropTable)
	{
		TArray<FItemDropTableRow*> AllRows;
		ItemDropTable->GetAllRows(TEXT("AEnemyCharacter::OnItemDrop"), AllRows);
		for (FItemDropTableRow* Row : AllRows)
		{
			// 필수 데이터 확인
			if (!Row || !Row->PickupData) continue;

			// 드랍 확률 체크
			if (FMath::FRand() > Row->DropRate) continue;

			if (UPickupFactorySubsystem* PickupFactory = GetGameInstance()->GetSubsystem<UPickupFactorySubsystem>())
			{
				PickupFactory->SpawnPickupAsync(Row->PickupData, GetActorTransform(),
					FOnPickupSpawned::CreateWeakLambda(
						this,
						[](APickupBase* InSpawned)
						{
							if (InSpawned)
							{
								UE_LOG(LogTemp, Log, TEXT("%s가 드랍되었습니다."), *InSpawned->GetName());
							}
						}
					)
				);
			}
		}
	}
}
