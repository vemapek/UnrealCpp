// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Maze/MazeData.h"
#include "MazeActor.generated.h"

class ACellActor;

UCLASS()
class UNREALCPP_API AMazeActor : public AActor
{
	GENERATED_BODY()

public:
	AMazeActor();

	// 미로를 생성하고 CellActor들을 배치하는 함수, 디테일 창에서도 바로 실행 가능
	UFUNCTION(CallInEditor, Category = "Maze")
	void GenerateMaze();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// MazeData로 생성된 셀들을 기반으로 CellActor를 스폰하는 함수
	void SpawnCells();

	// 스폰해둔 CellActor들을 전부 제거하는 함수
	void ClearCells();

protected:
	//미로 가로 칸 수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maze")
	uint8 Width = 10;

	//미로 세로 칸 수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maze")
	uint8 Height = 10;

	//미로 생성 시드값(-1이면 매번 랜덤)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maze")
	int32 Seed = -1;

	//미로를 구성할 셀 액터 클래스
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maze")
	TSubclassOf<ACellActor> CellActorClass;

private:
	FMazeData MazeData;

	UPROPERTY()
	TArray<TObjectPtr<ACellActor>> CellActors;
};
