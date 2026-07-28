// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Interface/InterfaceStat.h"
#include "InterfaceWeaponUser.h"
#include "ActionCharacter.generated.h"

class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UStatActorComponent;
class UAnimNotifyState_SectionJump;

UCLASS()
class UNREALCPP_API AActionCharacter : public ACharacter, public IInterfaceStat, public IInterfaceWeaponUser
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AActionCharacter();

	UFUNCTION(BlueprintCallable, Category = "Stat")
	virtual UStatActorComponent* GetStatComponent() const override;

	virtual void OnWeaponAttackState(bool bEnable) override;

	virtual FOnWeaponAttackStateChanged& GetWeaponAttackStateChangedDelegate() override
	{
		return OnOnWeaponAttackStateChanged;
	};

	void SetSectionJumpNotify(UAnimNotifyState_SectionJump* InSectionJumpNotify);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	void OnTestAction(const FInputActionValue& Value);
	void OnMoveAction(const FInputActionValue& Value);
	void OnBoostOn(const FInputActionValue& Value);
	void OnBoostOff(const FInputActionValue& Value);
	void OnAttackAction(const FInputActionValue& Value);

private:
	void SpendBoostStamina(float DeltaTime);

	void SectionJumpForCombo(); // 콤보용으로 섹션 점프하는 함수

public:
	// 공격 판정 On/Off를 무기에게 전달하는 델리게이트
	FOnWeaponAttackStateChanged OnOnWeaponAttackStateChanged;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> IA_Test;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> IA_Move;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> IA_Attack;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> IA_Boost;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Action Anims")
	TObjectPtr<UAnimMontage> RollMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Action Anims")
	TObjectPtr<UAnimMontage> AttackMontage;

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

private:
	UPROPERTY()
	TObjectPtr<UAnimInstance> AnimInstance = nullptr;

	bool bBoostMode = false;

	// 발생한 콤보 노티파이를 저장해 놓는 변수
	TWeakObjectPtr<UAnimNotifyState_SectionJump> SectionJumpNotify = nullptr;

	// 현재 콤보가 가능한지 확인하기 위한 변수
	bool bComboReady = false;
};