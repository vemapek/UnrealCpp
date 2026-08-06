// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"

class AWeaponActor;
class UWeaponDataAsset;
class UAnimNotifyState_SectionJump;

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnWeaponAttackStateChanged, bool, bEnable);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALCPP_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UWeaponComponent();

	// 무기 장비 처리
	void EquipWeapon(UWeaponDataAsset* InWeaponData);

	// 공격 처리 함수(리턴값은 공격을 했는지 여부)
	bool Attack();

	// 범위 공격 처리 함수
	void AreaAttack();


	// 이벤트 함수들 ---------------------------------------------------------------------------------

	// 무기를 다 사용하여 드랍되었을 때 실행될 함수(웨폰 액터 쪽에서 바인딩)
	void OnWeaponDrop(UWeaponDataAsset* InDropWeaponData);

	// 무기 공격 활성화/비활성화 때 실행되는 함수
	void OnWeaponAttackState(bool bEnable);
	// ----------------------------------------------------------------------------------------------

	// Getter/Setter --------------------------------------------------------------------------------
	// 무기를 사용할 수 있는지 확인하는 Getter
	bool CanWeaponUse();

	// 섹션 점프를 위한 노티파이를 설정하는 함수
	void SetSectionJumpNotify(UAnimNotifyState_SectionJump* InSectionJunpNotify);

	// 현재 장비 중인 무기 데이터 Getter
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	UWeaponDataAsset* GetCurrentWeaponData() const { return CurrentWeaponData; }

	// 기본 무기 데이터 Getter
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	UWeaponDataAsset* GetDefaultWeaponData() const { return DefaultWeaponData; }

	// -----------------------------------------------------------------------------------------------

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// 무기를 스폰하고 장비하는 함수
	void SpawnWeaponActorAndEquip();

	// 콤보용으로 섹션 점프하는 함수(리턴값은 공격을 했는지 여부)
	bool SectionJumpForCombo();

	// 공격 몽타주가 끝났을 때 실행될 함수
	void OnAttackEnded(UAnimMontage* InMontage, bool bInterrupted);

public:
	// 무기 공격 활성화/비활성화 노티파이를 받는 델리게이트
	FOnWeaponAttackStateChanged OnWeaponAttackStateChanged;

protected:
	// 현재 장비 중인 무기
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	TWeakObjectPtr<AWeaponActor> CurrentWeapon = nullptr;

	// 현재 장비할 무기의 데이터 에셋(임시 : 무기 관리자로 넘길 예정)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<UWeaponDataAsset> CurrentWeaponData = nullptr;

	// 기본 무기의 데이터 에셋
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<UWeaponDataAsset> DefaultWeaponData = nullptr;

private:
	// 발생한 콤보 노티파이를 저장해 놓는 변수
	TWeakObjectPtr<UAnimNotifyState_SectionJump> SectionJumpNotify = nullptr;

	// 현재 콤보가 가능한지 확인하기 위한 변수
	bool bComboReady = false;

	// 이 컴포넌트가 붙어있는 캐릭터
	TWeakObjectPtr<ACharacter> OwnerCharacter = nullptr;

	// OwnerCharacter의 AnimInstance
	TWeakObjectPtr<UAnimInstance> OwnerAnimInstance = nullptr;

};
