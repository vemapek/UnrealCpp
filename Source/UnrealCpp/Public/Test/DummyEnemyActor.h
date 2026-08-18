// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DummyEnemyActor.generated.h"

class ADamagePopupActor;

/**
 * 데미지 팝업/오브젝트 풀 동작 확인용 테스트 더미. 맞으면 데미지 팝업만 띄운다.
 */
UCLASS()
class UNREALCPP_API ADummyEnemyActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADummyEnemyActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(
		float DamageAmount,
		FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> PopupTransform;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ADamagePopupActor> DamagePopupClass;

};
