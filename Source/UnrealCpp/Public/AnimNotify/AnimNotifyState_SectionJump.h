// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_SectionJump.generated.h"

/**
 * 
 */
class AActionCharacter;

UCLASS()
class UNREALCPP_API UAnimNotifyState_SectionJump : public UAnimNotifyState
{
	GENERATED_BODY()


public:
	inline FName GetNextSectionName() const { return NextSectionName; }
	
protected:
	virtual void NotifyBegin(
		USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation,
		float TotalDuration,
		const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(
		USkeletalMeshComponent* MeshComp, 
		UAnimSequenceBase* Animation, 
		const FAnimNotifyEventReference& EventReference)override;


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName NextSectionName;

private:
	TWeakObjectPtr<AActionCharacter> OwnerCharacter = nullptr;
};
