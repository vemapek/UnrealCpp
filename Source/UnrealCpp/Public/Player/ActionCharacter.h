// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Interface/InterfaceStat.h"
#include "Interface/InterfaceWeaponUser.h"
#include "Interface/InterfaceInventoryUser.h"
#include "ActionCharacter.generated.h"

class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UStatActorComponent;
class UWeaponComponent;
class UWeaponDataAsset;

UCLASS()
class UNREALCPP_API AActionCharacter : public ACharacter, public IInterfaceStat, public IInterfaceWeaponUser, public IInterfaceInventoryUser
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AActionCharacter();

	// 무기 장비 함수 (WeaponComponent로 전달)
	virtual void EqueipWeapon_Implementation(UWeaponDataAsset* InWeaponData) override;

	UFUNCTION(BlueprintCallable, Category = "Stat")
	virtual UStatActorComponent* GetStatComponent() const override;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual UWeaponComponent* GetWeaponComponent() const override;

	// InventoryComponent로 전달할 함수들 -------------------------------------------------------------
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual UInventoryComponent* GetInventoryComponent() const override;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual bool ExecuteInventoryCommand(const FInventoryCommand& Command, FInventoryCommandResult& OutResult) override;
	// -----------------------------------------------------------------------------------------------

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 언리얼 표준 데미지 시스템(ApplyDamage)이 호출하는 함수 - 받은 데미지를 StatComponent로 연결
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	void OnTestAction(const FInputActionValue& Value);
	void OnMoveAction(const FInputActionValue& Value);
	void OnBoostOn(const FInputActionValue& Value);
	void OnBoostOff(const FInputActionValue& Value);
	void OnAttackAction(const FInputActionValue& Value);
	void OnDropWeaponAction(const FInputActionValue& Value);
	// 인벤토리 여닫기 토글 입력 처리 함수(HUD의 MainHudWidget을 찾아 토글을 위임)
	void OnToggleInventoryAction(const FInputActionValue& Value);

private:
	void SpendBoostStamina(float DeltaTime);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> IA_Test;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> IA_Move;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> IA_Attack;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> IA_DropWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> IA_Boost;

	// 인벤토리 여닫기 입력 액션(에디터에서 E키로 매핑)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> IA_ToggleInventory;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Action Anims")
	TObjectPtr<UAnimMontage> RollMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	float BoostSpeed = 1200;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	float MoveSpeed = 600;

	// 구르기에 필요한 스태미나 코스트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat|Stamina")
	float RollStaminaCost = 20.0f;

	// 부스트에 필요한 초당 스태미나 코스트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat|Stamina")
	float BoostStaminaCostPerSec = 5.0f;

	// 스태미나 사용 후 자동 회복에 걸리는 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat|Stamina")
	float StaminaAutoRecoveryCoolTime = 3.0f;

	// 스태미나가 자동 회복될 때 타이머 틱당 회복량
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat|Stamina")
	float StaminaAutoRecoveryPerTick = 1.0f;

	// 스태미나가 자동 회복될 때 타이머 한 틱의 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat|Stamina")
	float StaminaAutoRecoveryInterval = 0.1f;

	// 공격 시 소비되는 스태미나 양
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float AttackStamina = 5.0f;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USpringArmComponent> CameraSpringArmComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCameraComponent> CameraComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStatActorComponent> StatComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWeaponComponent> WeaponComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UInventoryComponent> InvenComponent = nullptr;

private:
	UPROPERTY()
	TObjectPtr<UAnimInstance> AnimInstance = nullptr;

	bool bBoostMode = false;
};
