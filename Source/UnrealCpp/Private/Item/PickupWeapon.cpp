// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/PickupWeapon.h"
#include "Interface/InterfaceWeaponUser.h"
#include "Data/Item/WeaponDataAsset.h"
#include "Components/SphereComponent.h"
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

void APickupWeapon::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	UE_LOG(LogTemp, Warning, TEXT("[Pickup] OnConstruction called on %s, DataAsset=%s"),
		*GetName(), DataAsset ? *DataAsset->GetName() : TEXT("NULL"));

	InitializePickup(DataAsset);
}

void APickupWeapon::OnPickup(AActor* InTarget)
{
	if (GetWorldTimerManager().IsTimerActive(PickupEffectTimerHandle)) return; // 타이머가 이미 작동 중이면 종료(중복실행 방지)

	Super::OnPickup(InTarget);

	TargetActor = InTarget;

	if (IsPickupEffectAssetReady()) // 애셋이 준비되어 있으면 연출 시작, 없으면 즉시 획득 처리
	{
		// 더 이상의 오버랩이 발생하지 않게 하기
		SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		PickupStartLocation = Mesh->GetComponentLocation();
		PickupElapsedTime = 0.0f;

		GetWorldTimerManager().SetTimer(
			PickupEffectTimerHandle,
			this,
			&APickupWeapon::OnUpdatePickupEffect,
			TimerInterval,
			true
		);
	}
	else
	{
		OnFinishPickupEffect();
	}
}

void APickupWeapon::OnUpdatePickupEffect()
{
	if (!TargetActor.IsValid()) // 타겟이 살아 있을 때만 진행
	{
		OnFinishPickupEffect();
		return;
	}

	PickupElapsedTime += TimerInterval;
	float Progress = PickupElapsedTime / PickupEffectDuration;

	float DistanceAlpha = PickupAlpha->GetFloatValue(Progress);
	FVector Goal = TargetActor.Get()->GetActorLocation();
	FVector NewLocation = FMath::Lerp(PickupStartLocation, Goal, DistanceAlpha);

	float HeightOffset = PickupHeight->GetFloatValue(Progress) * PickupEffecHeight;
	NewLocation.Z += HeightOffset;
	Mesh->SetWorldLocation(NewLocation);

	float Scale = PickupScale->GetFloatValue(Progress);
	Mesh->SetRelativeScale3D(FVector(Scale));

	if (Progress >= 1.0f)
	{
		OnFinishPickupEffect();
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

bool APickupWeapon::IsPickupEffectAssetReady() const
{
	return PickupAlpha != nullptr && PickupHeight != nullptr && PickupScale != nullptr;
}
