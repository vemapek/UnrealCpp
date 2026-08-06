// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_AreaAttack.generated.h"

/**
 *
 */
UCLASS()
class UNREALCPP_API UAnimNotify_AreaAttack : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(
		USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UCameraShakeBase> ShakeEffect = nullptr;

private:
	TWeakObjectPtr<APlayerCameraManager> CameraManager = nullptr;
};
