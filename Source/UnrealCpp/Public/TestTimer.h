// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestTimer.generated.h"

UCLASS()
class UNREALCPP_API ATestTimer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestTimer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void Test_TimerOnce();

	UFUNCTION(BlueprintCallable)
	void Test_TimerRepeat();

	UFUNCTION(BlueprintCallable)
	void Test_TimerDelayRepeat();

	UFUNCTION(BlueprintCallable)
	void Test_TimerAllClear();



	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	void Test_MessagePrint1();
	void Test_MessagePrint2();
	void Test_MessagePrint3();
private:
	FTimerHandle TestOnce;
	FTimerHandle TestRepeat;
	FTimerHandle TestDelay;


};
