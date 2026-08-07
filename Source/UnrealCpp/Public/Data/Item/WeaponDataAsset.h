
#pragma once

#include "CoreMinimal.h"
#include "Data/Item/ItemDataAsset.h"
#include "Engine/StreamableManager.h"
#include "WeaponDataAsset.generated.h"

class UStaticMesh;
class UNiagaraSystem;

/**
 *
 */
UCLASS(BlueprintType)
class UNREALCPP_API UWeaponDataAsset : public UItemDataAsset
{
	GENERATED_BODY()

public:
	virtual bool IsLoaded() const override;

protected:
	virtual void OnAsyncRequest(TArray<FSoftObjectPath>& InOutArray) const override;

public:
	// 무기의 메시
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Appearance")
	TSoftObjectPtr<UStaticMesh> Mesh;

	// 무기가 Attach될 소켓
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Appearance")
	FName AttachSocketName = TEXT("hand_rSocket");

	// 무기 Attach할 위치의 Offset
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Appearance")
	FVector LocationOffset = FVector::ZeroVector;

	// HitArea 캡슐의 높이 절반
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HitArea")
	float HitAreaHalfHeight = 60.0f;

	// HitArea 캡슐의 반지름
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HitArea")
	float HitAreaRadius = 30.0f;

	// 무기의 공격력
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	float AttackPower = 10.0f;

	// 무기의 범위공격력
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	float AreaAttackPower = 10.0f;

	// 무기의 범위 공격의 반지름(안쪽, 이 안쪽은 100% 데미지)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	float AreaAttackInnerRadius = 100.0f;

	// 무기의 범위 공격의 반지름(바깥, Inner ~ Outter범위는 거리에 따라 감소)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	float AreaAttackOutterRadius = 300.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	TObjectPtr<UNiagaraSystem> HitEffect;

	// 사용 횟수가 제한된 소모성 무기인지 여부 (false면 무한으로 사용 가능한 기본 무기)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	bool bIsConsumable = false;

	// 소모성 무기일 때 사용 가능한 최대 횟수 (공격 1회당 1씩 소모되며, 0이 되면 자동으로 버려짐)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData", meta = (EditCondition = "bIsConsumable", ClampMin = "1"))
	int32 MaxUseCount = 1;

	// 무기의 공격 애니메이션 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponData")
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(EditAnywhere, Category = "VFX")
	class UNiagaraSystem* TrailFX;
};
