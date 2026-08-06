// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InterfacePoolable.h"
#include "DamagePopupActor.generated.h"

class UWidgetComponent;
class UDamagePopupWidget;

UCLASS()
class UNREALCPP_API ADamagePopupActor : public AActor, public IInterfacePoolable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADamagePopupActor();

	UFUNCTION(BlueprintCallable)
	void OnPopupStart(float InDamage);

	UFUNCTION(BlueprintCallable)
	virtual void UseFinish() override;

	virtual void OnSpawn_Implementation() override;

	virtual void OnReturn_Implementation() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> DamagePopupWidgetComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float LifeTime = 1.5f;

private:
	TObjectPtr<UDamagePopupWidget> DamagePopupWidget = nullptr;

};
