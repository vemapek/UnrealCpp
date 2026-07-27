// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestActor.generated.h"

UCLASS()
class UNREALCPP_API ATestActor : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ATestActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;	// 틱을 protected로 변경

	void Test_NormalFunction();

	UFUNCTION(BlueprintCallable)	// C++로 작성한 함수를 블루프린트에서 호출할 수 있다.
		void Test_UFunction();

	UFUNCTION(BlueprintImplementableEvent)	// C++에서 선언만하고 구현은 블루프린트에서 하겠다.
		void Test_ImplementableFunction();		// (블루프린트 코드를 써야만 하는 상황일 떄 사용. CPP에 구현하면 안됨)

	UFUNCTION(BlueprintNativeEvent)			// C++에서 선언과 구현을 했는데 블루프린트에서 재정의 할 수도 있다.
		void Test_NativeEventFunction();		// (구현은 _Implementation붙여서 구현해야 한다.) (호출은 원래 이름 그대로)

protected:
	int32 Data1 = 10;	// C++ 전용 변수.

	// UPROPERTY : 클래스나 구조체의 맴버 변수를 언리얼 엔진에 등록하는 것(리플렉션 지원, 가비지 콜랙팅 지원)
	UPROPERTY(VisibleAnywhere, Category = "TestActor변수|Visible")		// 클래스 디폴트 창과, 맵에 배치된 액터의 디테일 창에서 확인 가능
		int32 Data2_1 = 21;

	UPROPERTY(VisibleDefaultsOnly, Category = "TestActor변수|Visible")	// 클래스 디폴트 창에서만 보임
		int32 Data2_2 = 22;

	UPROPERTY(VisibleInstanceOnly, Category = "TestActor변수|Visible")	// 맵에 배치된 액터의 디테일 창에서만 보임
		int32 Data2_3 = 23;

	UPROPERTY(EditAnywhere, Category = "TestActor변수|Editable")			// 클래스 디폴트 창과, 맵에 배치된 액터의 디테일 창에서 수정 가능
		int32 Data3_1 = 31;

	UPROPERTY(EditDefaultsOnly, Category = "TestActor변수|Editable")		// 클래스 디폴트 창에서만 수정 가능
		int32 Data3_2 = 32;

	UPROPERTY(EditInstanceOnly, Category = "TestActor변수|Editable")		// 맵에 배치된 액터의 디테일 창에서만 수정 가능
		int32 Data3_3 = 33;

	UPROPERTY(BlueprintReadOnly, Category = "TestActor변수|Read")	// 블루프린트 이벤트 그래프에서 읽을 수 있다.
		int32 Data4_1 = 41;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "TestActor변수|Read")	// 블루프린트 이벤트 그래프에서 읽을 수 있고 디테일 창에서 편집도 가능.
		int32 Data4_2 = 42;

	UPROPERTY(BlueprintReadWrite, Category = "TestActor변수|ReadWrite")	// 블루프린트 이벤트 그래프에서 읽고 쓸 수 있다.
		int32 Data4_3 = 43;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "TestActor변수|ReadWrite")	// 블루프린트 이벤트 그래프에서 읽고 쓸 수 있다. 그리고 디테일 창에서 편집도 가능
		int32 Data4_4 = 44;

	UPROPERTY(VisibleAnywhere ,BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> MainMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveSpeed = 10.0f;
};

