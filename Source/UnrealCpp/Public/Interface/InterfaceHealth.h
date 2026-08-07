// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InterfaceHealth.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInterfaceHealth : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class UNREALCPP_API IInterfaceHealth
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Health")
	float GetCurrentHealth() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Health")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Health")
	void DamageHealth(float InAmount);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Health")
	void HealHealth(float InAmount);

	// 죽었는지 여부 (0 밑으로 데미지가 여러 번 들어와도 OnDie가 한 번만 브로드캐스트되도록 하는 가드용)
	virtual bool IsAlive() const = 0;
};
