// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InterfaceStamina.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInterfaceStamina : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class UNREALCPP_API IInterfaceStamina
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Stamina")
	float GetCurrentStamina() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Stamina")
	float GetMaxStamina() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Stamina")
	bool ConsumeStamina(float InAmount);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Stamina")
	void RecoveryStamina(float InAmount);

	// C++ 전용
	//virtual bool RecoveryStamina(float InAmount) = 0;
};