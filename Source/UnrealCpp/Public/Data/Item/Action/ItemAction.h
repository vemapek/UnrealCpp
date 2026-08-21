// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ItemAction.generated.h"

/**
 * 아이템을 사용했을 때 실제로 벌어질 효과(회복/돈 지급 등)를 나타내는 추상 클래스.
 * UseableItemDataAsset이 이 클래스의 인스턴스를 인라인으로 들고 있다가 사용 시 실행한다.
 */
// Abstract : 추상클래스로 설정(인스턴스 못만듬. 상속받은 자식만 인스턴스 생성 가능)
// EditInlineNew : 디테일 패널 드랍다운에서 즉시 생성 및 편집 가능
// DefaultToInstanced : 이 클래스를 주소로 가지는 모든 UPROPERTY에 Instanced속성을 기본적으로 부여
// Instanced : 이 객체를 소유자의 하위 인스턴스로 취급(=디스크에 저장 가능 + 딥 카피 보장)
UCLASS(Abstract, EditInlineNew, DefaultToInstanced)
class UNREALCPP_API UItemAction : public UObject
{
	GENERATED_BODY()

public:
	// 순수 C++ 가상함수(현재는 블루프린트에서 오버라이드할 필요가 없어서 BlueprintNativeEvent 대신 이걸로 단순화)
	virtual void ExecuteAction(AActor* InInstigator, AActor* InTarget) {}
};
