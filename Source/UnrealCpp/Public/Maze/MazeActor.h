// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Maze/MazeData.h"
#include "MazeActor.generated.h"

struct FCellData;
class ACellActor;
class UHierarchicalInstancedStaticMeshComponent;

UCLASS()
class UNREALCPP_API AMazeActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMazeActor();

	// 미로를 생성하고 배치하는 함수, 디테일 창에서도 바로 실행 가능
	UFUNCTION(CallInEditor, Category = "Maze")
	void GenerateMaze();

	// 생성된 미로를 제거하는 함수, 디테일 창에서도 바로 실행 가능
	UFUNCTION(CallInEditor, Category = "Maze")
	void ClearMaze();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnPreMazeGenerate();
	virtual void OnPostMazeGenerate();

private:
	TUniquePtr<FMazeData> MakeMazeData();

	// CellActor 스폰 방식 ------------------------------------------------------------------------
	void SpawnCells(FMazeData* InMaze);
	void ClearSpawnedCells();
	//---------------------------------------------------------------------------------------------

	// HISM 사용 방식 ------------------------------------------------------------------------------
	void BuildMazeHISM(FMazeData* InMaze);
	void ClearHISMInstances();
	void MakeCellHISM(const FCellData* InCell, const FVector& InLocation);
	//---------------------------------------------------------------------------------------------

protected:
	//미로 가로 칸 수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maze", meta = (ClampMin = "3", ClampMax = "100"))
	int32 Width = 3;

	//미로 세로 칸 수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maze", meta = (ClampMin = "3", ClampMax = "100"))
	int32 Height = 3;

	//미로 생성 시드값(-1이면 매번 랜덤)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maze")
	int32 RandomSeed = FMazeData::RandomSeed;

	// 인스턴싱(HISM)으로 미로를 그릴지, 셀마다 액터를 스폰할지 선택
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maze|Performance")
	bool bUseHISM = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maze|HISM")
	float CellHalfSize = 1000.0f;

	// CellActor 스폰 방식 ------------------------------------------------------------------------
	//미로를 구성할 셀 액터 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Maze|Spawn")
	TSubclassOf<ACellActor> CellActorClass = nullptr;

	UPROPERTY(VisibleInstanceOnly, Category = "Maze|Spawn")
	TArray<TObjectPtr<ACellActor>> SpawnedCells;
	//---------------------------------------------------------------------------------------------

	// HISM 사용 방식 ------------------------------------------------------------------------------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Maze|Components")
	TObjectPtr<UHierarchicalInstancedStaticMeshComponent> FloorHISM = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Maze|Components")
	TObjectPtr<UHierarchicalInstancedStaticMeshComponent> WallHISM = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Maze|Components")
	TObjectPtr<UHierarchicalInstancedStaticMeshComponent> GateHISM = nullptr;
	//---------------------------------------------------------------------------------------------

};
