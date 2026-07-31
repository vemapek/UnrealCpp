// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "WeaponActor.generated.h"

class ACharacter;
class UCapsuleComponent;
class UWeaponDataAsset;

UCLASS()
class UNREALCPP_API AWeaponActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeaponActor();

	UFUNCTION(BlueprintCallable)
	void InitializeWeapon(UWeaponDataAsset* InData);

	UFUNCTION(BlueprintCallable)
	void EquipToTarget(AActor* Target);

	UFUNCTION(BlueprintCallable)
	void DropWeapon();

	// 남은 사용 횟수를 반환 (무한 사용 무기면 -1)
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetRemainingUseCount() const { return RemainingUseCount; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void OnEquipped(AActor* InOwner);

	UFUNCTION()
	void OnHitAreaBeginOverlap(
		UPrimitiveComponent* InOverlappedComponent,
		AActor* InOtherActor,
		UPrimitiveComponent* InOtherComp,
		int32 InOtherBodyIndex,
		bool bFromSweep,
		const FHitResult& InSweepResult);

	UFUNCTION(BlueprintCallable)
	void AttackEnable(bool bEnable);

private:
	// 사용 횟수를 1 소모시키고, 소모성 무기의 사용 횟수가 다 되면 버려지도록 예약함
	void ConsumeUse();

	// 무기를 던져서 버리고, 소유자에게 기본 무기로 교체하도록 알림
	void DiscardWeapon();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> Mesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCapsuleComponent> HitArea = nullptr;

	// 무기 데이터 애셋
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UWeaponDataAsset> WeaponData;

	// 무기가 드랍된 후 사라질 때까지의 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DropLifeSpan = 10.0f;

	// 드랍 직후에 플레이어와 물리 상호작용이 안 되는 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PhysicsDelay = 0.8f;

private:
	// 무기를 장비하고 있는 대상
	TWeakObjectPtr<ACharacter> OwnerCharacter = nullptr;

	// PhysicsDelay용 타이머 핸들
	FTimerHandle PhysicsDelayTimerHandle;

	// 소모성 무기의 남은 사용 횟수 (무한 사용 무기면 -1)
	int32 RemainingUseCount = -1;

	// 이번 공격이 끝나면 사용 횟수 소진으로 버려질지 여부
	bool bPendingDiscard = false;

protected:
	UPROPERTY(EditAnywhere, Category = "VFX")
	class UNiagaraSystem* TrailFX;

	UPROPERTY()
	class UNiagaraComponent* TrailComponent;

	void StartTrail();
	void StopTrail();
	void UpdateTrail();

	bool bIsTrailActive = false;

public:
	virtual void Tick(float DeltaTime) override;
};
