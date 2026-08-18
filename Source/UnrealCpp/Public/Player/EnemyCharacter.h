// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/InterfaceStat.h"
#include "EnemyCharacter.generated.h"

class UStatActorComponent;
class UItemDataAsset;

/**
 * 독립적인 스탯을 가지는 몬스터. AActionCharacter를 상속받지 않고
 * StatComponent를 직접 소유한다 (플레이어 전용 입력/무기 기능과 분리됨).
 */
UCLASS()
class UNREALCPP_API AEnemyCharacter : public ACharacter, public IInterfaceStat
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

	UFUNCTION(BlueprintCallable, Category = "Stat")
	virtual UStatActorComponent* GetStatComponent() const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// StatComponent->OnDie에 바인딩되는 함수
	UFUNCTION()
	virtual void OnDie();

	// 사망 시 드랍 테이블을 굴려서 아이템을 드랍하는 함수
	virtual void OnItemDrop();

private:
	// 데이터 에셋으로부터 픽업 액터를 동기 스폰하는 함수(SpawnPickupAsync를 안 쓰는 대안 경로)
	void SpawnPickup(UItemDataAsset* ItemDataAsset);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStatActorComponent> StatComp;

	// 사망 시 굴릴 아이템 드랍 테이블 (Row Structure: FItemDropTableRow)
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UDataTable> ItemDropTable;
};
