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
	virtual void OnConstruction(const FTransform& Transform) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	// 오버랩 됐을 때 대상에게 실제 작업을 처리하는 함수(줍는 연출 시작)
	virtual void OnPickup(AActor* InTarget);

	// 줍는 연출(플레이어 쪽으로 날아가는 연출) 진행 함수
	virtual void OnUpdatePickupEffect();

	// 줍는 연출이 끝났을 때 실제 처리를 담당하는 함수. 하위 클래스에서 오버라이드해서
	// 장착(무기)/인벤토리 추가(잡화) 등 각자의 처리를 구현한다. 기본 구현은 그냥 사라짐.
	virtual void OnFinishPickupEffect();

	virtual void OnUpdateUpdownSpin(float InDeltaTime);

private:
	bool IsCurveAssetReady() const;
	bool IsPickupEffectAssetReady() const;

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
	// 아이템을 줍는 연출의 진행 상황용 커브
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect|Pickup")
	TObjectPtr<UCurveFloat> PickupAlpha;

	// 아이템을 줍는 연출 중 위아래 움직임을 위한 커브
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect|Pickup")
	TObjectPtr<UCurveFloat> PickupHeight;

	// 아이템을 줍는 연출 중 크기 변경을 위한 커브
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect|Pickup")
	TObjectPtr<UCurveFloat> PickupScale;

	// 아이템을 줍는 연출의 전체 진행 시간
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect|Pickup")
	float PickupEffectDuration = 0.5f;

	// PickupHeight로 인해 올라가는 높이
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect|Pickup")
	float PickupEffecHeight = 50.0f;

	// 아이템을 줍는 대상(하위 클래스의 OnFinishPickupEffect에서 사용)
	TWeakObjectPtr<AActor> TargetActor = nullptr;

	// 아이템을 줍는 연출용 타이머 핸들(하위 클래스의 OnFinishPickupEffect에서 정리해야 함)
	FTimerHandle PickupEffectTimerHandle;

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

	// 아이템을 줍는 연출이 진행된 시간
	float PickupElapsedTime = 0.0f;

	// 아이템을 줍는 연출용 타이머의 실행 간격
	const float TimerInterval = 0.02f;

	// 아이템을 줍는 연출용 시작 위치
	FVector PickupStartLocation;
};
