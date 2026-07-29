// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNotify/AnimNotifyState_AttackEnable.h"
#include "Interface/InterfaceWeaponUser.h"

void UAnimNotifyState_AttackEnable::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	WeaponOwner = Cast<IInterfaceWeaponUser>(MeshComp->GetOwner());
	if (WeaponOwner)
	{
		WeaponOwner->OnWeaponAttackState(true);
	}
}

void UAnimNotifyState_AttackEnable::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (WeaponOwner)
	{
		WeaponOwner->OnWeaponAttackState(false);
		WeaponOwner = nullptr;
	}
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}