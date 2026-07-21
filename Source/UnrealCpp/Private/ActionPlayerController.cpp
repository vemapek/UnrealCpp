// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"

void AActionPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UEnhancedInputLocalPlayerSubsystem* SubSystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if (SubSystem && DefaultMappingContext)
	{
		SubSystem->AddMappingContext(DefaultMappingContext, GameInputPriority);
	}

	PlayerCameraManager->ViewPitchMax = ViewPitchMax;
	PlayerCameraManager->ViewPitchMin = ViewPitchMin;

}

void AActionPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	if (UEnhancedInputComponent* Enhanced = Cast<UEnhancedInputComponent>(InputComponent))
	{
		Enhanced->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AActionPlayerController::OnLookInput);
		
	}
}

void AActionPlayerController::OnLookInput(const FInputActionValue& InValue)
{
	FVector2D LookAxis = InValue.Get<FVector2D>();
	AddYawInput(LookAxis.X);
	AddPitchInput(LookAxis.Y);


}
