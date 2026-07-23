// Fill out your copyright notice in the Description page of Project Settings.

#include "StatActorComponent.h"

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

		bResult = true;
	}

	UE_LOG(LogTemp, Log, TEXT("Stamina : %.1f / %.1f"), CurrentStamina, MaxStamina);
	return bResult;
}

void UStatActorComponent::StaminaAutoRecoveryPerTick()
{
	IInterfaceStamina::Execute_RecoveryStamina(this, StaminaRecoveryData.RecoveryPerTick);
}

void UStatActorComponent::RecoveryStamina_Implementation(float InAmount)
{
	CurrentStamina = FMath::Clamp(CurrentStamina + InAmount, 0.0f, MaxStamina);
	UE_LOG(LogTemp, Log, TEXT("Stamina : %.1f / %.1f"), CurrentStamina, MaxStamina);

	if (CurrentStamina >= MaxStamina)
	{
		FTimerManager& TimerManager = GetWorld()->GetTimerManager();
		TimerManager.ClearTimer(StaminaAutoRecoveryTimerHandle);
	}
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