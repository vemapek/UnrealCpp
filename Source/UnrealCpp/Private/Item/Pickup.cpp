// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/Pickup.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Component/StatActorComponent.h"
#include "Interface/InterfaceStamina.h"
#include "Interface/InterfaceHealth.h"
#include "Interface/InterfaceStat.h"

// Sets default values
APickup::APickup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("RootCollision"));
	SphereCollision->InitSphereRadius(100.0f);
	SetRootComponent(SphereCollision);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(SphereCollision);
}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APickup::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	ApplyEffects(OtherActor);
}

void APickup::ApplyEffects(AActor* InTarget)
{
	// Cast<IInterfaceStat>가 성공하면 = 이 액터가 스탯 컴포넌트를 가지고 있다는 뜻
	if (IInterfaceStat* Stat = Cast<IInterfaceStat>(InTarget))
	{
		UStatActorComponent* StatComp = Stat->GetStatComponent();

		if (Stamina > 0)
		{
			IInterfaceStamina::Execute_RecoveryStamina(StatComp, Stamina);
		}
		else if (Stamina < 0)
		{
			IInterfaceStamina::Execute_ConsumeStamina(StatComp, -Stamina);
		}

		if (Health > 0)
		{
			IInterfaceHealth::Execute_HealHealth(StatComp, Health);
		}
		else if (Health < 0)
		{
			IInterfaceHealth::Execute_DamageHealth(StatComp, -Health);
		}
	}
}