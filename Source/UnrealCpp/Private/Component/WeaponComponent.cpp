// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/WeaponComponent.h"
#include "UnrealCpp/UnrealCpp.h"
#include "Interface/InterfaceWeaponUser.h"
#include "Weapon/WeaponActor.h"
#include "Data/WeaponDataAsset.h"
#include "AnimNotify/AnimNotifyState_SectionJump.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	OwnerCharacter = Cast<ACharacter>(GetOwner());

	if (OwnerCharacter.IsValid())
	{
		if (OwnerCharacter->GetMesh())
		{
			OwnerAnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();	// 애님 인스턴스 캐싱
		}

		if (DefaultWeaponData)
		{
			// 기본무기 장착
			IInterfaceWeaponUser::Execute_EqueipWeapon(OwnerCharacter.Get(), DefaultWeaponData);
		}
	}

}

// Called every frame
void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UWeaponComponent::EquipWeapon(UWeaponDataAsset* InWeaponData)
{
	if (InWeaponData == CurrentWeaponData)
	{
		// 같은 종류의 무기를 먹었다.
		CurrentWeapon->ResetUseCount();
	}
	else
	{
		// 다른 종류의 무기를 장비한다.

		// 이전 무기 해제하기
		if (CurrentWeapon.IsValid())
		{
			CurrentWeapon.Get()->DropWeapon();
			CurrentWeapon = nullptr;
		}

		// 새 무기 장비하기
		CurrentWeaponData = InWeaponData;
		if (CurrentWeaponData)	// null일때는 장비 안함
		{
			if (!CurrentWeaponData->IsLoaded())
			{
				// 데이터가 로딩 안되었으면 로딩 요청
				UWeaponDataAsset* RequestedData = CurrentWeaponData;
				CurrentWeaponData->RequestDataLoad(
					FStreamableDelegate::CreateWeakLambda(
						this,
						[this, RequestedData]()
						{
							// 로딩이 완료되면 실행되는 람다 함수
							if (CurrentWeaponData == RequestedData)
							{
								// 중복으로 로딩 요청했을 때를 대비
								SpawnWeaponActorAndEquip();
							}
						})
				);
			}
			else
			{
				// 로딩 된 상황이면 즉시 스폰하고 장비
				SpawnWeaponActorAndEquip();
			}
		}
	}
}

bool UWeaponComponent::Attack()
{
	if (!OwnerCharacter.IsValid() || !OwnerAnimInstance.IsValid()) return false;

	bool bResult = false;
	if (!OwnerAnimInstance->IsAnyMontagePlaying())
	{
		// 첫번째 콤보 공격
		OwnerCharacter->PlayAnimMontage(CurrentWeaponData->AttackMontage);

		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &UWeaponComponent::OnAttackEnded);
		OwnerAnimInstance->Montage_SetEndDelegate(EndDelegate, CurrentWeaponData->AttackMontage);

		OnWeaponAttackState(false);
		bResult = true;
	}
	else if (OwnerAnimInstance->GetCurrentActiveMontage() == CurrentWeaponData->AttackMontage)
	{
		bResult = SectionJumpForCombo();
	}
	return bResult;
}

void UWeaponComponent::AreaAttack()
{
	if (!CurrentWeapon.IsValid() || !CurrentWeaponData || !OwnerCharacter.IsValid() ) return;

	// 디버그 정보 출력
	DrawDebugSphere(
		GetWorld(),
		CurrentWeapon->GetWeaponImpactLocation(),
		CurrentWeaponData->AreaAttackInnerRadius,
		12,
		FColor::Red,
		false,
		5.0f
	);
	DrawDebugSphere(
		GetWorld(),
		CurrentWeapon->GetWeaponImpactLocation(),
		CurrentWeaponData->AreaAttackOutterRadius,
		12,
		FColor::Yellow,
		false,
		5.0f
	);

	TArray<AActor*> IgnoreActors = { CurrentWeapon.Get(), OwnerCharacter.Get() };
	UGameplayStatics::ApplyRadialDamageWithFalloff(
		GetWorld(),
		CurrentWeaponData->AreaAttackPower,
		1,
		CurrentWeapon->GetWeaponImpactLocation(),
		CurrentWeaponData->AreaAttackInnerRadius,
		CurrentWeaponData->AreaAttackOutterRadius,
		1.0f,	// 1일때(거리에 정비례해서 감소), 0에 가까워 질 때(위로 볼록한 그래프), 1보다 커질 때(아래로 오목한 그래프)
		nullptr,
		IgnoreActors,
		CurrentWeapon.Get(),
		OwnerCharacter.Get()->GetController(),
		ECC_Enemy);

}

void UWeaponComponent::OnWeaponDrop(UWeaponDataAsset* InDropWeaponData)
{
	if (DefaultWeaponData && (DefaultWeaponData != InDropWeaponData))
	{
		EquipWeapon(DefaultWeaponData);
	}
}

void UWeaponComponent::OnWeaponAttackState(bool bEnable)
{
	OnWeaponAttackStateChanged.ExecuteIfBound(bEnable);
}

bool UWeaponComponent::CanWeaponUse()
{
	return CurrentWeapon.IsValid() && CurrentWeapon->CanUse();
}

void UWeaponComponent::SetSectionJumpNotify(UAnimNotifyState_SectionJump* InSectionJunpNotify)
{
	SectionJumpNotify = InSectionJunpNotify;
	bComboReady = SectionJumpNotify.IsValid();
}

void UWeaponComponent::SpawnWeaponActorAndEquip()
{
	if (!CurrentWeaponData || !OwnerCharacter.IsValid())
	{
		return;	// 로딩 요청이 끝나기 전에 해제되었을 때를 대비
	}

	CurrentWeapon = GetWorld()->SpawnActorDeferred<AWeaponActor>(
		AWeaponActor::StaticClass(), FTransform::Identity, OwnerCharacter.Get(), OwnerCharacter.Get());	// 스폰 시작

	if (CurrentWeapon.IsValid())
	{
		CurrentWeapon->InitializeWeapon(CurrentWeaponData);
		CurrentWeapon->OnWeaponDrop.BindUObject(this, &UWeaponComponent::OnWeaponDrop);
		UGameplayStatics::FinishSpawningActor(CurrentWeapon.Get(), FTransform::Identity);	// 스폰 완료(=BeginPlay까지 실행)
		CurrentWeapon->EquipToTarget(OwnerCharacter.Get());
	}
}

bool UWeaponComponent::SectionJumpForCombo()
{
	if (!OwnerAnimInstance.IsValid()) return false;

	bool bResult = false;
	if (SectionJumpNotify.IsValid() && bComboReady)
	{
		OnAttackEnded(nullptr, true);	// 콤보로 몽타주가 시작되었다 => 이전 애니메이션이 끝났다

		UAnimMontage* Current = OwnerAnimInstance->GetCurrentActiveMontage();
		OwnerAnimInstance->Montage_SetNextSection(	// 섹션을 변경한다.
			OwnerAnimInstance->Montage_GetCurrentSection(Current),	// 이 섹션에서(from)
			SectionJumpNotify->GetNextSectionName(),				// 이 섹션으로 변경(to)
			Current	// 적용할 몽타주
		);
		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &UWeaponComponent::OnAttackEnded);
		OwnerAnimInstance->Montage_SetEndDelegate(EndDelegate, CurrentWeaponData->AttackMontage);	// 애니메이션이 끝나는 타이밍에 실행

		OnWeaponAttackState(false);
		bComboReady = false;	// 중복실행 방지

		bResult = true;	// 공격 성공
	}
	return bResult;
}

void UWeaponComponent::OnAttackEnded(UAnimMontage* InMontage, bool bInterrupted)
{
	UE_LOG(LogTemp, Log, TEXT("OnAttackEnded"));

	// 참고: unrealcpp의 AWeaponActor는 10th와 달리 AttackEnable(true) 시점(HitArea가 켜질 때)에
	// ConsumeUse()로 사용 횟수를 자체 소모한다. 그래서 10th의 WeaponComponent::OnAttackEnded()가
	// 여기서 호출하던 CurrentWeapon->Use()는 unrealcpp에서는 호출하지 않는다(중복 소모 방지).
}
