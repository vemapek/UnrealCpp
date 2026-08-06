// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/AnimNotifyState_SectionJump.h"
#include "Interface/InterfaceWeaponUser.h"
#include "Component/WeaponComponent.h"

void UAnimNotifyState_SectionJump::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (IInterfaceWeaponUser* WeaponOwner = Cast<IInterfaceWeaponUser>(MeshComp->GetOwner()))
	{
		if (UWeaponComponent* WeaponComp = WeaponOwner->GetWeaponComponent())
		{
			WeaponComp->SetSectionJumpNotify(this);
		}
	}
}

void UAnimNotifyState_SectionJump::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (IInterfaceWeaponUser* WeaponOwner = Cast<IInterfaceWeaponUser>(MeshComp->GetOwner()))
	{
		if (UWeaponComponent* WeaponComp = WeaponOwner->GetWeaponComponent())
		{
			WeaponComp->SetSectionJumpNotify(nullptr); // 구간이 끝났으니 nullptr로 콤보 불가 상태로 전환
		}
	}

	Super::NotifyEnd(MeshComp, Animation, EventReference);
}
