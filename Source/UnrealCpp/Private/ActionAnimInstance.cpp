// Fill out your copyright notice in the Description page of Project Settings.

#include "ActionAnimInstance.h"
#include "GameFramework/PawnMovementComponent.h"

void UActionAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (APawn* OwnerPawn = TryGetPawnOwner())
	{
		OwnerMovementComponent = OwnerPawn->GetMovementComponent(); // 찾는 것은 한 번만
	}
}

void UActionAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (OwnerMovementComponent.IsValid())
	{
		MoveSpeed = OwnerMovementComponent->Velocity.Size(); // 이동 속도 업데이트
	}
	else
	{
		// 처음에 못 찾았을 때를 대비한 코드
		if (APawn* OwnerPawn = TryGetPawnOwner())
		{
			OwnerMovementComponent = OwnerPawn->GetMovementComponent();
		}
	}
}