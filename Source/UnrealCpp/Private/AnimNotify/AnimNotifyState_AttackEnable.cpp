// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNotify/AnimNotifyState_AttackEnable.h"
#include "Interface/InterfaceWeaponUser.h"
#include "Component/WeaponComponent.h"

void UAnimNotifyState_AttackEnable::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (IInterfaceWeaponUser* WeaponOwner = Cast<IInterfaceWeaponUser>(MeshComp->GetOwner()))
	{
		if (UWeaponComponent* WeaponComp = WeaponOwner->GetWeaponComponent())
		{
			WeaponComp->OnWeaponAttackState(true);
		}
	}
}

void UAnimNotifyState_AttackEnable::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (IInterfaceWeaponUser* WeaponOwner = Cast<IInterfaceWeaponUser>(MeshComp->GetOwner()))
	{
		if (UWeaponComponent* WeaponComp = WeaponOwner->GetWeaponComponent())
		{
			WeaponComp->OnWeaponAttackState(false);
		}
	}
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}
