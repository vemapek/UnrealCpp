// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "UnrealCpp/UnrealCpp.h"

AEnemyCharacter::AEnemyCharacter()
{

	GetCapsuleComponent()->SetCollisionObjectType(ECC_Enemy);
}