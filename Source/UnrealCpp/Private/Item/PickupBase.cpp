// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/PickupBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraComponent.h"

// Sets default values
APickupBase::APickupBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("RootCollision"));
	SphereCollision->InitSphereRadius(100.0f);
	SetRootComponent(SphereCollision);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(SphereCollision);
	Mesh->SetCollisionProfileName("NoCollision");

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("VFX"));
	NiagaraComponent->SetupAttachment(SphereCollision);
}

void APickupBase::InitializePickup(UItemDataAsset* InData)
{
	DataAsset = InData;
}

void APickupBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	InitializePickup(DataAsset);
}

// Called when the game starts or when spawned
void APickupBase::BeginPlay()
{
	Super::BeginPlay();
	ElapsedTime = 0.0f;
}

// Called every frame
void APickupBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIdle)
	{
		OnUpdateUpdownSpin(DeltaTime);
	}
}

void APickupBase::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	OnPickup(OtherActor);
}

void APickupBase::OnPickup(AActor* InTarget)
{
	if (GetWorldTimerManager().IsTimerActive(PickupEffectTimerHandle)) return; // 타이머가 이미 작동 중이면 종료(중복실행 방지)

	UE_LOG(LogTemp, Log, TEXT("%s가 %s를 획득했습니다."), *InTarget->GetName(), *this->GetName());
	bIdle = false;

	TargetActor = InTarget;

	if (IsPickupEffectAssetReady()) // 애셋이 준비되어 있으면 연출 시작, 없으면 즉시 획득 처리
	{
		// 더 이상의 오버랩이 발생하지 않게 하기
		SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		PickupStartLocation = Mesh->GetComponentLocation();
		PickupElapsedTime = 0.0f;

		GetWorldTimerManager().SetTimer(
			PickupEffectTimerHandle,
			this,
			&APickupBase::OnUpdatePickupEffect,
			TimerInterval,
			true
		);
	}
	else
	{
		OnFinishPickupEffect();
	}
}

void APickupBase::OnUpdatePickupEffect()
{
	if (!TargetActor.IsValid()) // 타겟이 살아 있을 때만 진행
	{
		OnFinishPickupEffect();
		return;
	}

	PickupElapsedTime += TimerInterval;
	float Progress = PickupElapsedTime / PickupEffectDuration;

	float DistanceAlpha = PickupAlpha->GetFloatValue(Progress);
	FVector Goal = TargetActor.Get()->GetActorLocation();
	FVector NewLocation = FMath::Lerp(PickupStartLocation, Goal, DistanceAlpha);

	float HeightOffset = PickupHeight->GetFloatValue(Progress) * PickupEffecHeight;
	NewLocation.Z += HeightOffset;
	Mesh->SetWorldLocation(NewLocation);

	float Scale = PickupScale->GetFloatValue(Progress);
	Mesh->SetRelativeScale3D(FVector(Scale));

	if (Progress >= 1.0f)
	{
		OnFinishPickupEffect();
	}
}

void APickupBase::OnFinishPickupEffect()
{
	// 기본 구현: 하위 클래스에서 오버라이드해서 장착/인벤토리 추가 등 실제 처리를 하지 않으면 그냥 사라짐
	GetWorldTimerManager().ClearTimer(PickupEffectTimerHandle);
	Destroy();
}

void APickupBase::OnUpdateUpdownSpin(float InDeltaTime)
{
	if (!IsCurveAssetReady()) return;

	ElapsedTime += InDeltaTime;

	float Progress = FMath::Fmod(ElapsedTime / UpDownDuration, 1.0f);
	FVector NewMeshLocation = MeshBaseLocation;
	NewMeshLocation.Z += UpDownCurve->GetFloatValue(Progress) * UpDownHeight;

	Mesh->SetRelativeLocation(NewMeshLocation);

	float NewAngle = SpinCurve->GetFloatValue(Progress) * 360.0f;
	Mesh->SetRelativeRotation(FRotator(0.0f, NewAngle, 0.0f));
}

bool APickupBase::IsCurveAssetReady() const
{
	return UpDownCurve != nullptr && SpinCurve != nullptr;
}

bool APickupBase::IsPickupEffectAssetReady() const
{
	return PickupAlpha != nullptr && PickupHeight != nullptr && PickupScale != nullptr;
}
