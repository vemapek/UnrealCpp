// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/WeaponActor.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Interface/InterfaceWeaponUser.h"
#include "Data/WeaponDataAsset.h"
#include "UnrealCpp/UnrealCpp.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
AWeaponActor::AWeaponActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootMesh"));
	SetRootComponent(Mesh);
	Mesh->SetGenerateOverlapEvents(false);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	HitArea = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HitArea"));
	HitArea->SetupAttachment(Mesh);
	HitArea->SetCapsuleHalfHeight(60.0f, false);
	HitArea->SetCapsuleRadius(30.0f, false);
	HitArea->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 처음엔 꺼진 채로 시작
	HitArea->SetCollisionObjectType(ECC_Weapon);
	HitArea->SetCollisionResponseToAllChannels(ECR_Ignore);
	HitArea->SetCollisionResponseToChannel(ECC_Enemy, ECR_Overlap);
	HitArea->SetRelativeLocation(FVector(0.0f, 0.0f, 40.0f));
}

void AWeaponActor::InitializeWeapon(UWeaponDataAsset* InData)
{
	WeaponData = InData;

	// 메시 설정
	Mesh->SetStaticMesh(WeaponData->Mesh.Get());

	// HitArea 크기 조정
	HitArea->SetCapsuleHalfHeight(WeaponData->HitAreaHalfHeight);
	HitArea->SetCapsuleRadius(WeaponData->HitAreaRadius);

	TrailFX = WeaponData->TrailFX;

	// 사용 횟수 초기화 (소모성 무기가 아니면 무한으로 취급)
	RemainingUseCount = WeaponData->bIsConsumable ? WeaponData->MaxUseCount : -1;
	bPendingDiscard = false;
}

void AWeaponActor::EquipToTarget(AActor* Target)
{
	OnEquipped(Target);
}

void AWeaponActor::DropWeapon()
{
	FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, true);
	DetachFromActor(DetachRules);

	Mesh->SetCollisionProfileName(TEXT("PhysicsActor"));
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECC_Player, ECollisionResponse::ECR_Ignore);
	Mesh->SetSimulatePhysics(true);
	Mesh->SetMassOverrideInKg(NAME_None, 0.8f, true);
	HitArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 일정 시간 동안 무기와 플레이어가 충돌 안 하게 설정
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.SetTimer(
		PhysicsDelayTimerHandle,
		FTimerDelegate::CreateLambda(
			[this]()
			{
				Mesh->SetCollisionResponseToChannel(ECC_Player, ECollisionResponse::ECR_Block);
			}
		),
		PhysicsDelay,
		false
	);

	// 뒤로 던지기
	FVector BackwardDirection = -OwnerCharacter->GetActorForwardVector();
	FVector ThrowDirection = BackwardDirection * 300.0f + FVector::UpVector * 200.0f;
	Mesh->AddImpulse(ThrowDirection, NAME_None, true);
	FVector AngularImpulse = FVector(
		FMath::RandRange(-200.0f, 200.0f)
	) + GetActorForwardVector() * 1000.0f;
	//Mesh->AddAngularImpulseInDegrees(AngularImpulse, NAME_None, true);

	// DropLifeSpan초 후에 이 액터 제거하기
	SetLifeSpan(DropLifeSpan);

	OwnerCharacter = nullptr;
}

// Called when the game starts or when spawned
void AWeaponActor::BeginPlay()
{
	Super::BeginPlay();
	HitArea->OnComponentBeginOverlap.AddDynamic(this, &AWeaponActor::OnHitAreaBeginOverlap);
}

void AWeaponActor::OnEquipped(AActor* InOwner)
{
	SetOwner(InOwner);
	OwnerCharacter = Cast<ACharacter>(InOwner);

	FAttachmentTransformRules AttachRules(
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::KeepWorld,
		true);

	if (OwnerCharacter.IsValid())
	{
		AttachToComponent(OwnerCharacter->GetMesh(), AttachRules, WeaponData->AttachSocketName);

		// Offset 적용
		SetActorRelativeLocation(WeaponData->LocationOffset);

		HitArea->IgnoreActorWhenMoving(OwnerCharacter.Get(), true); // 만약을 대비한 것

		if (IInterfaceWeaponUser* WeaponUser = Cast<IInterfaceWeaponUser>(OwnerCharacter))
		{
			WeaponUser->GetWeaponAttackStateChangedDelegate().BindUFunction(this, FName("AttackEnable"));
		}
	}
}

void AWeaponActor::OnHitAreaBeginOverlap(UPrimitiveComponent* InOverlappedComponent, AActor* InOtherActor, UPrimitiveComponent* InOtherComp, int32 InOtherBodyIndex, bool bFromSweep, const FHitResult& InSweepResult)
{
	float Damage = WeaponData ? WeaponData->AttackPower : 1;

	UE_LOG(LogTemp, Log, TEXT("오버랩 된 대상 : %s"), *InOtherActor->GetName());

	UGameplayStatics::ApplyDamage(InOtherActor, Damage, OwnerCharacter->GetController(), this, nullptr);


	// 맞은 지점에 이펙트 재생
	if (WeaponData && WeaponData->HitEffect)
	{
		FVector EffectLocation = (bFromSweep && InSweepResult.bBlockingHit)
			? FVector(InSweepResult.ImpactPoint)  
			: HitArea->GetComponentLocation();

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			WeaponData->HitEffect,
			EffectLocation,
			GetActorRotation()
		);
	}

}

void AWeaponActor::AttackEnable(bool bEnable)
{
	if (bEnable)
	{
		HitArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		ConsumeUse(); // 공격 1회 사용 처리]
		StartTrail();
	}
	else
	{
		HitArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		StopTrail();
		if (bPendingDiscard) // 방금 공격이 마지막 사용 횟수였다면 이제 버림
		{
			DiscardWeapon();
		}
	}
}

void AWeaponActor::ConsumeUse()
{
	if (RemainingUseCount < 0) return; // 무한 사용 무기는 소모되지 않음

	RemainingUseCount = FMath::Max(RemainingUseCount - 1, 0);
	if (RemainingUseCount == 0)
	{
		bPendingDiscard = true; // 이번 공격의 판정이 끝나면 버려지도록 예약
	}
}

void AWeaponActor::DiscardWeapon()
{
	AActor* PreviousOwner = OwnerCharacter.Get();

	DropWeapon(); // 물리적으로 던져서 버림 (OwnerCharacter는 이 안에서 nullptr로 초기화됨)

	if (PreviousOwner && PreviousOwner->Implements<UInterfaceWeaponUser>())
	{
		IInterfaceWeaponUser::Execute_OnWeaponDepleted(PreviousOwner); // 소유자에게 기본 무기로 교체하도록 알림
	}
}



void AWeaponActor::StartTrail()
{
	if (!TrailFX) return;

	if (!TrailComponent)
	{
		TrailComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
			TrailFX, Mesh, NAME_None,
			FVector::ZeroVector, FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset, false
		);
	}
	else
	{
		TrailComponent->Activate(true);
	}

	bIsTrailActive = true;

	// 디버그용 상태 체크
	FTimerHandle CheckTimer;
	GetWorld()->GetTimerManager().SetTimer(CheckTimer, [this]()
		{
			if (TrailComponent)
			{
				UE_LOG(LogTemp, Warning, TEXT("=== Trail 상태 체크 ==="));
				UE_LOG(LogTemp, Warning, TEXT("IsActive=%d"), TrailComponent->IsActive());
				UE_LOG(LogTemp, Warning, TEXT("IsVisible=%d"), TrailComponent->IsVisible());
				UE_LOG(LogTemp, Warning, TEXT("IsRegistered=%d"), TrailComponent->IsRegistered());
				UE_LOG(LogTemp, Warning, TEXT("HiddenInGame=%d"), TrailComponent->bHiddenInGame);
				UE_LOG(LogTemp, Warning, TEXT("월드위치=%s"), *TrailComponent->GetComponentLocation().ToString());

				if (UNiagaraSystem* Asset = TrailComponent->GetAsset())
				{
					UE_LOG(LogTemp, Warning, TEXT("에셋=%s"), *Asset->GetName());
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("에셋 = NULL!"));
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("TrailComponent 자체가 NULL!"));
			}
		}, 0.5f, false);
}

void AWeaponActor::StopTrail()
{
	bIsTrailActive = false;
	if (TrailComponent)
	{
		TrailComponent->Deactivate();
	}
}

void AWeaponActor::UpdateTrail()
{
	if (!TrailComponent || !Mesh) return;

	FVector Start = Mesh->GetSocketLocation(TEXT("TrailStart"));
	FVector End = Mesh->GetSocketLocation(TEXT("TrailEnd"));


	TrailComponent->SetVectorParameter(TEXT("User.TrailStart"), Start);
	TrailComponent->SetVectorParameter(TEXT("User.TrailEnd"), End);
}

void AWeaponActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsTrailActive)
	{
		UpdateTrail();
	}
}
