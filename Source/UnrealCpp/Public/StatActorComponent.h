// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InterfaceStamina.h"
#include "StatActorComponent.generated.h"

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
class UNREALCPP_API UStatActorComponent : public UActorComponent, public IInterfaceStamina
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UStatActorComponent();

	void InitializeStat(FAutoRecoveryData& InData);

	virtual float GetCurrentStamina_Implementation() const override;
	virtual bool ConsumeStamina_Implementation(float InAmount) override;
	virtual void RecoveryStamina_Implementation(float InAmount) override;

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

private:
	// 스태미나 자동 회복 처리를 위한 타이머
	FTimerHandle StaminaAutoRecoveryTimerHandle;

	// 스태미나 자동 회복용 데이터
	FAutoRecoveryData StaminaRecoveryData;
};