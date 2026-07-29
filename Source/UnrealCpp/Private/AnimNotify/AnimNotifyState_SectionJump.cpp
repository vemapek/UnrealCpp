// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/AnimNotifyState_SectionJump.h"
#include "Player/ActionCharacter.h"

void UAnimNotifyState_SectionJump::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	OwnerCharacter = Cast<AActionCharacter>(MeshComp->GetOwner());
	if (OwnerCharacter.IsValid())
	{
		OwnerCharacter->SetSectionJumpNotify(this);
	}
}

void UAnimNotifyState_SectionJump::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (OwnerCharacter.IsValid())
	{
		OwnerCharacter->SetSectionJumpNotify(nullptr); // 구간이 끝났으니 nullptr로 콤보 불가 상태로 전환
		OwnerCharacter = nullptr;
	}
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}
