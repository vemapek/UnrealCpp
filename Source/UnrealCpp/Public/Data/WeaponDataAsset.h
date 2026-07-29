// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeaponDataAsset.generated.h"



class UStaticMesh;
/**
 * 
 */
UCLASS()
class UNREALCPP_API UWeaponDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	TSharedPtr<FStreamableHandle> RequestDataLoad(FStreamableDelegate InDelegate) const
	
public:
	// 소캣
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Appearance")
	FName AttachSocketName = TEXT("hand_rSocket");

	//무기의 attch할 위치의 offset
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Appearance")
	FVector LocationOffset = FVector::ZeroVector;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="WeaponData")
	float Damage = 10.0f;

	//무기의 매시
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Appearance")
	TSoftObjectPtr <UStaticMesh> Mesh;

	//무기 크기
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HitArea")
	float HitAreaHalfHeight = 60.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HitArea")
	float HitAreaRadius = 30.0f;

};
