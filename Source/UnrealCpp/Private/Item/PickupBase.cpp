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

	// 스폰 직후 유예 시간 동안은 오버랩을 무시한다(스폰과 동시에 겹친 대상으로
	// 인한 즉시 재귀 호출/스택 오버플로우 방지). 유예 시간이 끝나면
	// ActivatePickupReadiness에서 픽업을 정상적으로 받아준다.
	GetWorldTimerManager().SetTimer(
		SpawnGraceTimerHandle,
		this,
		&APickupBase::ActivatePickupReadiness,
		SpawnGraceTime,
		false
	);
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

	if (!bReadyForPickup) return; // 스폰 유예 시간 중이면 무시
	if (OtherActor && OtherActor->IsA<APickupBase>()) return; // 픽업끼리는 서로 주울 수 없음(안 그러면 근처에 재스폰될 때마다 서로 겹쳐서 계속 분열함)

	OnPickup(OtherActor);
}

void APickupBase::ActivatePickupReadiness()
{
	bReadyForPickup = true;

	// 유예 시간 동안 계속 겹쳐있던 대상이 있었다면(예: 인벤토리가 꽉 차서
	// 대상 바로 옆에 재스폰된 경우) 그 대상에 대해 픽업을 한 번 재시도한다.
	// (여기서 실패해도 다음에 스폰되는 대체 픽업은 각자 새로운 유예 시간을
	// 가지므로 무한 재귀로 이어지지 않는다)
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors);
	for (AActor* OverlappingActor : OverlappingActors)
	{
		if (OverlappingActor && !OverlappingActor->IsA<APickupBase>()) // 픽업끼리는 제외
		{
			OnPickup(OverlappingActor);
			break;
		}
	}
}

void APickupBase::OnPickup(AActor* InTarget)
{
	// 같은 프레임에 여러 컴포넌트가 동시에 겹치는 등의 이유로 NotifyActorBeginOverlap이
	// 중복으로 들어올 수 있어서, 타이머 상태가 아니라 1회용 잠금으로 확실히 막는다.
	// (커브 애셋이 없어서 OnFinishPickupEffect가 타이머 없이 즉시 실행되는 경우엔
	// 타이머 상태만으로는 중복 실행을 막을 수 없었음)
	if (bIsPickupHandled) return;
	bIsPickupHandled = true;

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
