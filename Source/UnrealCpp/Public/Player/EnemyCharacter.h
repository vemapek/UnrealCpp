// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/ActionCharacter.h"
#include "EnemyCharacter.generated.h"

/**
 * 테스트용 몬스터. ActionCharacter를 상속받아 StatComponent/TakeDamage 등
 * 공통 스탯 처리를 그대로 재사용한다. (WASD/공격 등 입력 관련 기능은
 * 컨트롤러가 이 폰을 Possess하지 않는 이상 아무 영향도 주지 않는다)
 */
UCLASS()
class UNREALCPP_API AEnemyCharacter : public AActionCharacter
{
	GENERATED_BODY()

public:
	AEnemyCharacter();

protected:
	virtual void BeginPlay() override;

	// StatComponent->OnDie에 바인딩되는 함수
	UFUNCTION()
	virtual void OnDie();

	virtual void OnItemDrop();

protected:
	// 사망 시 굴릴 아이템 드랍 테이블 (Row Structure: FItemDropTableRow)
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UDataTable> ItemDropTable;
};
