// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponActor.generated.h"

class ACharacter;
class UCapsuleComponent;
class UWeaponDataAsset;

UCLASS()
class UNREALCPP_API AWeaponActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeaponActor();

	UFUNCTION(BlueprintCallable)
	void InitializeWeapon(UWeaponDataAsset* InData);

	UFUNCTION(BlueprintCallable)
	void EquipToTarget(AActor* Target);

	UFUNCTION(BlueprintCallable)
	void DropWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void OnEquipped(AActor* InOwner);

	UFUNCTION()
	void OnHitAreaBeginOverlap(
		UPrimitiveComponent* InOverlappedComponent,
		AActor* InOtherActor,
		UPrimitiveComponent* InOtherComp,
		int32 InOtherBodyIndex,
		bool bFromSweep,
		const FHitResult& InSweepResult);

	UFUNCTION(BlueprintCallable)
	void AttackEnable(bool bEnable);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> Mesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCapsuleComponent> HitArea = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName AttachSocketName = TEXT("hand_rSocket");

	// 이 무기가 입히는 데미지량
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Damage = 10.0f;

	// 이 무기를 생성한 데이터 애셋 (장착 시 InitializeWeapon으로 전달됨)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UWeaponDataAsset> WeaponData;

private:
	// 무기를 장비하고 있는 대상
	TWeakObjectPtr<ACharacter> OwnerCharacter = nullptr;
};