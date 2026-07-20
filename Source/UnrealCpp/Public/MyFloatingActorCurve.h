// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FloatingActor.h"
#include "MyFloatingActorCurve.generated.h"


UCLASS()
class UNREALCPP_API AMyFloatingActorCurve : public AFloatingActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	UCurveFloat* Curve = nullptr;
	
	virtual void TickTock(float DeltaTime)	override;
};
