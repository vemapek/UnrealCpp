// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ActionAnimInstance.generated.h"

class UPawnMovementComponent;

/**
 *
 */
UCLASS()
class UNREALCPP_API UActionAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float MoveSpeed = 0;

private:
	TWeakObjectPtr<UPawnMovementComponent> OwnerMovementComponent = nullptr;
};