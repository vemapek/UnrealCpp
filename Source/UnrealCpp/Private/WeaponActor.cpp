// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponActor.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "InterfaceWeaponUser.h"
#include "UnrealCpp/UnrealCpp.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWeaponActor::AWeaponActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

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
	HitArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	HitArea->SetCollisionObjectType(ECC_Weapon);
	HitArea->SetCollisionResponseToAllChannels(ECR_Ignore);
	HitArea->SetCollisionResponseToChannel(ECC_Enemy, ECR_Overlap);
	HitArea->SetRelativeLocation(FVector(0.0f, 0.0f, 40.0f));
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
		EAttachmentRule::SnapToTarget,
		true);

	if (OwnerCharacter.IsValid())
	{
		AttachToComponent(OwnerCharacter->GetMesh(), AttachRules, AttachSocketName);
		HitArea->IgnoreActorWhenMoving(OwnerCharacter.Get(), true); // 만약을 대비한 것

		if (IInterfaceWeaponUser* WeaponUser = Cast<IInterfaceWeaponUser>(OwnerCharacter))
		{
			WeaponUser->GetWeaponAttackStateChangedDelegate().BindUFunction(this, FName("AttackEnable"));
		}
	}
}

void AWeaponActor::OnHitAreaBeginOverlap(UPrimitiveComponent* InOverlappedComponent, AActor* InOtherActor, UPrimitiveComponent* InOtherComp, int32 InOtherBodyIndex, bool bFromSweep, const FHitResult& InSweepResult)
{
	if (!InOtherActor) return;

	UE_LOG(LogTemp, Warning, TEXT("ApplyDamage 호출: 대상=%s, 데미지=%.1f"), *InOtherActor->GetName(), Damage);

	AController* InstigatorController = OwnerCharacter.IsValid() ? OwnerCharacter->GetController() : nullptr;

	UGameplayStatics::ApplyDamage(
		InOtherActor,          // 맞은 대상
		Damage,                // 데미지량
		InstigatorController,  // 누가 때렸는지 (무기 주인의 컨트롤러)
		this,                  // 데미지를 유발한 물체 (이 무기)
		nullptr                // 데미지 타입 (기본값)
	);
}

void AWeaponActor::AttackEnable(bool bEnable)
{
	if (bEnable)
	{
		HitArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		HitArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}