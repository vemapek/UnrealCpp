// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "InterfaceStat.h"
#include "ActionCharacter.generated.h"

class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UStatActorComponent;

UCLASS()
class UNREALCPP_API AActionCharacter : public ACharacter, public IInterfaceStat
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AActionCharacter();

	UFUNCTION(BlueprintCallable, Category = "Stat")
	virtual UStatActorComponent* GetStatComponent() const override;

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

private:
	void SpendBoostStamina(float DeltaTime);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> IA_Test;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> IA_Move;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> IA_Boost;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> RollMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BoostSpeed = 1200;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
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
};