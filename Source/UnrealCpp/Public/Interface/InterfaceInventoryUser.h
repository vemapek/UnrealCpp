// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InterfaceInventoryUser.generated.h"

class UInventoryComponent;
struct FInventoryCommand;
struct FInventoryCommandResult;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInterfaceInventoryUser : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class UNREALCPP_API IInterfaceInventoryUser
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual UInventoryComponent* GetInventoryComponent() const = 0;
	virtual bool ExecuteInventoryCommand(const FInventoryCommand& Command, FInventoryCommandResult& OutResult) = 0;
};
