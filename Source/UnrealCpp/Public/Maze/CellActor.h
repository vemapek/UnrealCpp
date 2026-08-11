// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Maze/CellData.h"
#include "CellActor.generated.h"

UCLASS()
class UNREALCPP_API ACellActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACellActor();

	// CellData 기반으로 경로 설정하고 문 열기
	void InitializeCell(FCellData* InCellData);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Path 설정에 따라 문이 제대로 열리는지 테스트 하기 위한 함수
	UFUNCTION(CallInEditor, Category = "Cell")
	void TestPath();

	// Path 설정에 따라 문을 여는 함수
	void OpenGate();


	bool IsPath(EDirectionType InDirection);


protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cell")
	float CellHalfSize = 1000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cell")
	float WallHalfThickness = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cell", meta = (Bitflags, BitmaskEnum = "/Script/Maze/CellData.EDirectionType"));
	int32 Path = 0;
	// EDirectionType Path = EDirectionType::None;


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> FloorMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<UStaticMeshComponent>> WallMeshes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<UStaticMeshComponent>> GateMeshes;


};
