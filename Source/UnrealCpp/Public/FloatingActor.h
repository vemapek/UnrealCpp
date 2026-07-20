// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloatingActor.generated.h"

UCLASS()
class UNREALCPP_API AFloatingActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFloatingActor();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// 메시 컴포넌트
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> MainMesh = nullptr;

	// 위아래 이동 폭 (진폭)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floating")
	float Amplitude = 300.0f;

	// 왕복 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floating")
	float Frequency = 30.0f;

	// 시작 위치 저장용
	FVector InitialLocation;

	// 누적 시간
	float RunningTime = 0.0f;

	virtual void TickTock(float DeltaTime);

};