// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/StatActorComponent.h"

// Sets default values for this component's properties
UStatActorComponent::UStatActorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UStatActorComponent::InitializeStat(FAutoRecoveryData& InData)
{
	CurrentStamina = MaxStamina;
	StaminaRecoveryData = InData;
}

float UStatActorComponent::GetCurrentStamina_Implementation() const
{
	return CurrentStamina;
}

float UStatActorComponent::GetMaxStamina_Implementation() const
{
	return MaxStamina;
}

bool UStatActorComponent::ConsumeStamina_Implementation(float InAmount)
{
	bool bResult = false;
	if (CurrentStamina >= InAmount)
	{
		CurrentStamina -= InAmount;

		FTimerManager& TimerManager = GetWorld()->GetTimerManager();
		TimerManager.SetTimer(
			StaminaAutoRecoveryTimerHandle,
			this,
			&UStatActorComponent::StaminaAutoRecoveryPerTick,
			StaminaRecoveryData.TickInterval,
			true,
			StaminaRecoveryData.CoolTime
		);

		OnStaminaChange.Broadcast(CurrentStamina, MaxStamina); // 블루프린트 디스패처 call과 같다

		if (CurrentStamina < EmptyCheckLimit)
		{
			OnStaminaEmpty.Broadcast();
		}

		bResult = true;
		UE_LOG(LogTemp, Log, TEXT("Stamina : %.1f / %.1f"), CurrentStamina, MaxStamina);
	}

	return bResult;
}

void UStatActorComponent::StaminaAutoRecoveryPerTick()
{
	IInterfaceStamina::Execute_RecoveryStamina(this, StaminaRecoveryData.RecoveryPerTick);
}

void UStatActorComponent::RecoveryStamina_Implementation(float InAmount)
{
	CurrentStamina = FMath::Clamp(CurrentStamina + InAmount, 0.0f, MaxStamina);
	OnStaminaChange.Broadcast(CurrentStamina, MaxStamina);

	if (CurrentStamina >= MaxStamina)
	{
		FTimerManager& TimerManager = GetWorld()->GetTimerManager();
		TimerManager.ClearTimer(StaminaAutoRecoveryTimerHandle);
	}
}

float UStatActorComponent::GetCurrentHealth_Implementation() const
{
	return CurrentHealth;
}

float UStatActorComponent::GetMaxHealth_Implementation() const
{
	return MaxHealth;
}

void UStatActorComponent::DamageHealth_Implementation(float InAmount)
{
	CurrentHealth -= InAmount;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red,
			FString::Printf(TEXT("%s Health: %.1f / %.1f"), *GetOwner()->GetName(), CurrentHealth, MaxHealth));
	}

	if (CurrentHealth < 0.0f)
	{
		CurrentHealth = 0;
		OnHealthChange.Broadcast(CurrentHealth, MaxHealth);
		OnDie.Broadcast();
	}
	else
	{
		OnHealthChange.Broadcast(CurrentHealth, MaxHealth);
	}
}

void UStatActorComponent::HealHealth_Implementation(float InAmount)
{
	CurrentHealth = FMath::Clamp(CurrentHealth + InAmount, 0.0f, MaxHealth);
	OnHealthChange.Broadcast(CurrentHealth, MaxHealth);
}

// Called when the game starts
void UStatActorComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UStatActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}