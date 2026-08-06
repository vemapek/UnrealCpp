// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNotify/AnimNotify_AreaAttack.h"
#include "Interface/InterfaceWeaponUser.h"
#include "Component/WeaponComponent.h"

void UAnimNotify_AreaAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!CameraManager.IsValid())
	{
		if (UWorld* World = MeshComp->GetWorld())
		{
			if (APlayerController* PC = World->GetFirstPlayerController())
			{
				CameraManager = PC->PlayerCameraManager;
			}
		}
	}

	if (CameraManager.IsValid())
	{
		CameraManager->StartCameraShake(ShakeEffect);
	}

	if (IInterfaceWeaponUser* WeaponUser = Cast<IInterfaceWeaponUser>(MeshComp->GetOwner()))
	{
		if (UWeaponComponent* WeaponComp = WeaponUser->GetWeaponComponent())
		{
			WeaponComp->AreaAttack();
		}
	}

}
