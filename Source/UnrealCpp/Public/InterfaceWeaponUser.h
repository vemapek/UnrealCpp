// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InterfaceWeaponUser.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnWeaponAttackStateChanged, bool, bEnable);

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInterfaceWeaponUser : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class UNREALCPP_API IInterfaceWeaponUser
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void OnWeaponAttackState(bool bEnable) = 0;
	virtual FOnWeaponAttackStateChanged& GetWeaponAttackStateChangedDelegate() = 0;
};