// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/InventoryComponent.h"
#include "Framework/PickupFactorySubsystem.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	Slots.SetNum(InventorySize + 1);	// 일반 슬롯 10개 + 임시 슬롯 1개
}

bool UInventoryComponent::ExecuteCommand(const FInventoryCommand& Command, FInventoryCommandResult& OutResult)
{
	switch (Command.Type)
	{
	case EInventoryCommandType::Add:
		HandleAddCommand(Command.ItemData, Command.Count, OutResult);
		break;
	case EInventoryCommandType::Move:
		HandleMoveCommand(Command.SourceIndex, Command.TargetIndex, OutResult);
		break;
	case EInventoryCommandType::Drop:
		HandleDropCommand(Command.SourceIndex, Command.DropPosition, OutResult);
		break;
	case EInventoryCommandType::Use:
		HandleUseCommand(Command.SourceIndex, OutResult);
		break;
	case EInventoryCommandType::Money:
		HandleMoneyCommand(Command.Count, OutResult);
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("알 수 없는 커맨드 입니다."));
		break;
	}

	return OutResult.bSuccess;
}

void UInventoryComponent::AddMoney(int32 InIncome)
{
	Money += InIncome;
	OnMoneyChanged.Broadcast(Money);	// 돈의 변경을 알림
}

int32 UInventoryComponent::AddItem(UItemDataAsset* InItemData, int32 InCount)
{
	if (!InItemData)
	{
		UE_LOG(LogTemp, Warning, TEXT("InItemData가 null입니다."));
		return InCount;
	}
	if (InCount <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("InCount가 0이하 입니다."));
		return InCount;
	}

	int32 RemainingCount = InCount;

	int32 StartIndex = 0;
	// 같은 종류의 아이템이 있는 슬롯을 찾아 최대한 채우기
	while (RemainingCount > 0)	// 남는게 있으면 계속 반복
	{
		// 같은 종류의 아이템이 들어있는 슬롯을 찾아 추가하기
		int32 FoundIndex = FindSlotWithItem(InItemData, StartIndex);
		if (FoundIndex == InventoryFail) break;	// 같은 종류의 아이템이 들어있는 슬롯이 없으면 종료

		// 같은 종류의 아이템이 들어있는 슬롯을 찾았다.
		FInvenSlot& Slot = Slots[FoundIndex];
		int32 AmountToAdd = FMath::Min(Slot.GetRemainingCount(), RemainingCount);
		UpdateSlotCount(FoundIndex, AmountToAdd);	// FoundIndex 슬롯에 채울 수 있는 만큼 채우기

		RemainingCount -= AmountToAdd;	// 남은 개수 갱신
		StartIndex = FoundIndex + 1;	// 새 시작 위치 갱신
	}

	// 빈슬롯을 찾아 최대한 채우기
	while (RemainingCount > 0)
	{
		int32 EmptyIndex = FindEmptySlot();
		if (EmptyIndex == InventoryFail) break;	// 빈슬롯이 없으면 종료

		FInvenSlot& Slot = Slots[EmptyIndex];
		int32 AmountToAdd = FMath::Min(InItemData->MaxStackCount, RemainingCount);
		SetSlot(EmptyIndex, InItemData, AmountToAdd);	// EmptyIndex 슬롯에 아이템 설정

		RemainingCount -= AmountToAdd;	// 남은 개수 갱신
	}

	// RemainingCount가 0이면 인벤토리에 잘 들어감. 0을 초과하면 그만큼은 인벤토리에 못들어갔다는 의미
	return RemainingCount;
}

void UInventoryComponent::UseItem(int32 InIndex)
{
}

FInvenSlot* UInventoryComponent::GetSlot(int InSlotIndex)
{
	check(IsValidIndex(InSlotIndex));

	return &Slots[InSlotIndex];
}

FInvenSlot* UInventoryComponent::GetTempSlot(int InSlotIndex)
{
	return nullptr;
}

void UInventoryComponent::UpdateSlotCount(int32 InSlotIndex, int32 InDeltaCount)
{
	if (!IsValidIndex(InSlotIndex)) return;

	FInvenSlot& Slot = Slots[InSlotIndex];
	if (Slot.IsEmpty()) return;

	int32 NewCount = Slot.GetCount() + InDeltaCount;
	SetSlot(InSlotIndex, Slot.ItemData, NewCount);
}

void UInventoryComponent::SetSlot(int32 InSlotIndex, UItemDataAsset* InItemData, int32 InCount)
{
	if (!IsValidIndex(InSlotIndex)) return;

	FInvenSlot& Slot = Slots[InSlotIndex];
	Slot.ItemData = InItemData;
	Slot.SetCount(InCount);

	// InItemData가 null일 수 있음(슬롯을 비우는 경우) - null이면 로딩 요청하지 않음
	if (InItemData && !InItemData->IsLoaded())
	{
		InItemData->RequestDataLoad(
			FStreamableDelegate::CreateWeakLambda(
				this,
				[this, InSlotIndex]()
				{
					// 리프레시용으로 변경 브로드캐스트 날리기
					OnSlotChanged.ExecuteIfBound(InSlotIndex);
				})
		);
	}

	// 델리게이트 전담 함수(다른 인벤토리 슬롯 변경 함수들은 최종적으로 이 함수를 호출)
	OnSlotChanged.ExecuteIfBound(InSlotIndex);
}

void UInventoryComponent::ClearSlot(int32 InSlotIndex)
{
	SetSlot(InSlotIndex, nullptr, 0);
}

bool UInventoryComponent::HandleAddCommand(UItemDataAsset* InItemData, int32 InCount, FInventoryCommandResult& OutResult)
{
	int32 RemainingCount = AddItem(InItemData, InCount);

	// RemainingCount가 0이면 인벤토리에 잘 들어갔음. 0을 초과하면 그만큼은 인벤토리에 못들어갔다는 의미
	if (RemainingCount > 0)
	{
		OutResult.bSuccess = false;
		OutResult.RemainingCount = RemainingCount;
	}
	else
	{
		OutResult.bSuccess = true;
		OutResult.RemainingCount = 0;
	}

	if (OutResult.bSuccess)
	{
		UE_LOG(LogTemp, Log, TEXT("[%s] 추가가 성공적으로 완료되었습니다."), *(InItemData->DisplayName.ToString()));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("%d개의 아이템이 남았습니다."), OutResult.RemainingCount);
	}

	return OutResult.bSuccess;
}

bool UInventoryComponent::HandleMoveCommand(int32 InSourceIndex, int32 InTargetIndex, FInventoryCommandResult& OutResult)
{
	if (!IsValidIndex(InSourceIndex) || !IsValidIndex(InTargetIndex))
	{
		OutResult.bSuccess = false;
		return false;
	}

	if (InSourceIndex == InTargetIndex)
	{
		OutResult.bSuccess = true;
		return true;
	}

	FInvenSlot& SourceSlot = Slots[InSourceIndex];
	FInvenSlot& TargetSlot = Slots[InTargetIndex];

	// 소스가 비어있으면 실패(처리안함)
	if (SourceSlot.IsEmpty())
	{
		OutResult.bSuccess = false;
		return false;
	}

	if (TargetSlot.IsEmpty())
	{
		// 대상 슬롯이 비어있다 => 그대로 이동처리
		SetSlot(InTargetIndex, SourceSlot.ItemData, SourceSlot.GetCount());
		ClearSlot(InSourceIndex);
		OutResult.bSuccess = true;
	}
	else if (TargetSlot.ItemData == SourceSlot.ItemData)
	{
		// 이동을 시키는데 같은 아이템이 들어있다 => 병합 처리
		int32 AmountToAdd = FMath::Min(TargetSlot.GetRemainingCount(), SourceSlot.GetCount());
		if (AmountToAdd > 0)
		{
			UpdateSlotCount(InTargetIndex, AmountToAdd);
			UpdateSlotCount(InSourceIndex, -AmountToAdd);
			OutResult.bSuccess = true;
		}
		else
		{
			OutResult.bSuccess = false;
		}
	}
	else
	{
		// 소스와 타겟이 서로 다른 아이템이다 => 슬롯 스왑
		UItemDataAsset* SourceItem = SourceSlot.ItemData;
		int32 SourceCount = SourceSlot.GetCount();
		UItemDataAsset* TargetItem = TargetSlot.ItemData;
		int32 TargetCount = TargetSlot.GetCount();

		SetSlot(InSourceIndex, TargetItem, TargetCount);
		SetSlot(InTargetIndex, SourceItem, SourceCount);
		OutResult.bSuccess = true;
	}

	return OutResult.bSuccess;
}

bool UInventoryComponent::HandleDropCommand(int32 InSlotIndex, const FVector& InDropLocation, FInventoryCommandResult& OutResult)
{
	OutResult.bSuccess = false;
	if (!IsValidIndex(InSlotIndex))
	{
		return OutResult.bSuccess;
	}

	FInvenSlot& Slot = Slots[InSlotIndex];

	// 슬롯이 비어있으면 실패(처리안함)
	if (Slot.IsEmpty())
	{
		return OutResult.bSuccess;
	}

	UWorld* World = GetWorld();
	UItemDataAsset* ItemData = Slot.ItemData;
	if (World && ItemData)
	{
		if (UPickupFactorySubsystem* Factory = World->GetSubsystem<UPickupFactorySubsystem>())
		{
			for (int32 i = 0; i < Slot.GetCount(); i++)
			{
				FVector SpawnLocation(FMath::RandPointInCircle(100.0f), 0);
				SpawnLocation += InDropLocation;

				FTransform SpawnTransform(FRotator::ZeroRotator, SpawnLocation);
				Factory->SpawnPickupAsync(ItemData, SpawnTransform, FOnPickupSpawned());
			}
			Slot.Clear();
			OutResult.bSuccess = true;
		}
	}

	return OutResult.bSuccess;
}

bool UInventoryComponent::HandleUseCommand(int32 InSlotIndex, FInventoryCommandResult& OutResult)
{
	OutResult.bSuccess = false;
	if (!IsValidIndex(InSlotIndex))
	{
		return OutResult.bSuccess;
	}

	UseItem(InSlotIndex);
	OutResult.bSuccess = true;

	return OutResult.bSuccess;
}

bool UInventoryComponent::HandleMoneyCommand(int32 InMoneyDiff, FInventoryCommandResult& OutResult)
{
	OutResult.bSuccess = false;

	AddMoney(InMoneyDiff);
	OutResult.bSuccess = true;

	return OutResult.bSuccess;
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

int32 UInventoryComponent::FindSlotWithItem(const UItemDataAsset* InItemData, int32 InStartIndex)
{
	int32 Result = InventoryFail;

	for (int32 i = InStartIndex; i < InventorySize; i++)
	{
		if (Slots[i].ItemData == InItemData && !Slots[i].IsFull())	// 같은 종류의 아이템인데 스택이 남아있는 경우
		{
			Result = i;
			break;
		}
	}

	return Result;
}

int32 UInventoryComponent::FindEmptySlot()
{
	int32 Result = InventoryFail;
	for (int32 i = 0; i < InventorySize; i++)
	{
		if (Slots[i].IsEmpty())
		{
			Result = i;
			break;
		}
	}

	return Result;
}
