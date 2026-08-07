// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupBase.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UNiagaraComponent;
class UCurveFloat;
class UItemDataAsset;

UCLASS()
class UNREALCPP_API APickupBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APickupBase();

	// 픽업이 어떤 아이템 데이터를 나타내는지 셋업하는 공통 진입점 (하위 클래스에서 확장)
	virtual void InitializePickup(UItemDataAsset* InData);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	// 오버랩 됐을 때 대상에게 실제 작업을 처리하는 함수
	virtual void OnPickup(AActor* InTarget);

	virtual void OnUpdateUpdownSpin(float InDeltaTime);

private:
	bool IsCurveAssetReady() const;

protected:
	// 픽업 시 획득할 데이터 애셋
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base Data")
	TObjectPtr<UItemDataAsset> DataAsset;

	// 메시의 기본 위치
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base Data")
	FVector MeshBaseLocation = FVector(0, 0, 0.0f);

	// 맵에 있을 때 위아래로 왕복하는 모습용 커브
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect|Default")
	TObjectPtr<UCurveFloat> UpDownCurve;

	// 맵에 있을 때 회전하는 모습용 커브
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect|Default")
	TObjectPtr<UCurveFloat> SpinCurve;

	// 위아래로 왕복하는 데 걸리는 시간
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect|Default")
	float UpDownDuration = 2.0f;

	// 위아래로 움직이는 거리
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect|Default")
	float UpDownHeight = 100.0f;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> SphereCollision = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> Mesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UNiagaraComponent> NiagaraComponent;

private:
	float ElapsedTime = 0.0f;
	bool bIdle = true;
};
