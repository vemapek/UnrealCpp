// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/MyFloatingActorCurve.h"

void AMyFloatingActorCurve::TickTock(float DeltaTime)
{
	float CurveValue = Curve->GetFloatValue(RunningTime);
	MainMesh->SetRelativeLocation(FVector(0.0f, 0.0f, CurveValue * Amplitude));

	RunningTime += DeltaTime;
	if (RunningTime > 2.0f)
	{
		RunningTime = 0.0f;
	}
}
