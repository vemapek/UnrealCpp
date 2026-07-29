// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/InterfaceStamina.h"
#include "Interface/InterfaceHealth.h"
#include "StatActorComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStatEmpty);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStatChange, float, Current, float, Max);

struct FAutoRecoveryData
{
	float CoolTime = 3.0f;			// 자동 회복이 시작되기까지 걸릴 시간
	float TickInterval = 0.1f;		// 한 틱의 시간
	float RecoveryPerTick = 1.0f;	// 틱당 회복량

	FAutoRecoveryData() : CoolTime(1.0f), TickInterval(1.0f), RecoveryPerTick(1.0f)
	{
	};
	FAutoRecoveryData(float InCoolTime, float InTickInterval, float InRecoveryPerTick)
		: CoolTime(InCoolTime), TickInterval(InTickInterval), RecoveryPerTick(InRecoveryPerTick)
	{
	};
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNREALCPP_API UStatActorComponent : public UActorComponent, public IInterfaceStamina, public IInterfaceHealth
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Stat|Stamina")
	FOnStatEmpty OnStaminaEmpty;
	UPROPERTY(BlueprintAssignable, Category = "Stat|Health")
	FOnStatEmpty OnDie;
	UPROPERTY(BlueprintAssignable, Category = "Stat|Stamina")
	FOnStatChange OnStaminaChange;
	UPROPERTY(BlueprintAssignable, Category = "Stat|Health")
	FOnStatChange OnHealthChange;

public:
	// Sets default values for this component's properties
	UStatActorComponent();

	void InitializeStat(FAutoRecoveryData& InData);

	virtual float GetCurrentStamina_Implementation() const override;
	virtual float GetMaxStamina_Implementation() const override;
	virtual bool ConsumeStamina_Implementation(float InAmount) override;
	virtual void RecoveryStamina_Implementation(float InAmount) override;

	virtual float GetCurrentHealth_Implementation() const override;
	virtual float GetMaxHealth_Implementation() const override;
	virtual void DamageHealth_Implementation(float InAmount) override;
	virtual void HealHealth_Implementation(float InAmount) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void StaminaAutoRecoveryPerTick();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentStamina = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxStamina = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentHealth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth = 100.0f;

private:
	// 스태미나 자동 회복 처리를 위한 타이머
	FTimerHandle StaminaAutoRecoveryTimerHandle;

	// 스태미나 자동 회복용 데이터
	FAutoRecoveryData StaminaRecoveryData;

	// "거의 0"으로 취급할 오차 허용 범위
	const float EmptyCheckLimit = 0.01f;
};