// Fill out your copyright notice in the Description page of Project Settings.

#include "Test/DummyEnemyActor.h"
#include "Framework/ObjectPoolSubsystem.h"
#include "DamagePopupActor.h"

// Sets default values
ADummyEnemyActor::ADummyEnemyActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	PopupTransform = CreateDefaultSubobject<USceneComponent>(TEXT("PopupTransform"));
	PopupTransform->SetupAttachment(Mesh);
	PopupTransform->SetRelativeLocation(FVector(0, 0, 70));
}

// Called when the game starts or when spawned
void ADummyEnemyActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ADummyEnemyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float ADummyEnemyActor::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (UObjectPoolSubsystem* SubSystem = GameInstance->GetSubsystem<UObjectPoolSubsystem>())
		{
			if (ADamagePopupActor* DamagePopup = SubSystem->Spawn(
				DamagePopupClass, PopupTransform->GetComponentTransform()))
			{
				DamagePopup->OnPopupStart(Damage);
			}
		}
	}

	return Damage;
}
