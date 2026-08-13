// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/InventoryComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UInventoryComponent::AddMoney(int32 InIncome)
{
}

void UInventoryComponent::AddItem(UItemDataAsset* InItemData, int32 InCount)
{
}

void UInventoryComponent::UseItem(int32 InIndex)
{
}

FInvenSlot* UInventoryComponent::GetSlot(int InSlotIndex)
{
	return nullptr;
}

FInvenSlot* UInventoryComponent::GetTempSlot(int InSlotIndex)
{
	return nullptr;
}

void UInventoryComponent::UpdateSlotCount(int32 InSlotIndex, int32 InDeltaCount)
{
}

void UInventoryComponent::SetSlot(int32 InSlotIndex, UItemDataAsset* InItemData, int32 InCount)
{
}

void UInventoryComponent::ClearSlot(int32 InSlotIndex)
{
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
	return int32();
}

int32 UInventoryComponent::FindEmptySlot()
{
	return int32();
}
