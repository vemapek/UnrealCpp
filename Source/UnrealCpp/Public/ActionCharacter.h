// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "InterfaceStamina.h"
#include "ActionCharacter.generated.h"

class UInputAction;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class UNREALCPP_API AActionCharacter : public ACharacter, public IInterfaceStamina
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AActionCharacter();

	virtual float GetCurrentStamina_Implementation() const override;

	virtual bool ConsumeStamina_Implementation(float InAmount) override;

	virtual void RecoveryStamina_Implementation(float InAmount) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	void OnTestAction(const FInputActionValue& Value);
	void OnMoveAction(const FInputActionValue& Value);
	void OnBoostOn(const FInputActionValue& Value);
	void OnBoostOff(const FInputActionValue& Value);

	

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> IA_Test;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> IA_Move;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> IA_Boost;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USpringArmComponent>CameraSpringArmComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCameraComponent> CameraComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float MoveSpeed = 600;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float CurrentStamina = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float MaxStamina = 2000.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float RecoveryTime = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TWeakObjectPtr<UAnimMontage> RollMontage;

	UPROPERTY()
	TObjectPtr<UAnimInstance> AnimInstance = nullptr;
};