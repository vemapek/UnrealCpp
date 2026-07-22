// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionCharacter.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
AActionCharacter::AActionCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraSpringArmComponent = CreateDefaultSubobject<USpringArmComponent> ( TEXT("CameraSpringArm"));
	CameraSpringArmComponent->SetupAttachment(RootComponent);
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	CameraComponent->SetupAttachment(CameraSpringArmComponent);

	bUseControllerRotationYaw = false; // 컨트롤러 움직일 때 같이 회전되는 것 방지
	GetCharacterMovement()->bOrientRotationToMovement = true; //캐릭터 이동방향으로 바라보게 만들기
	CameraSpringArmComponent->bUsePawnControlRotation = true; //스프링암은 컨트롤러 입력에 맞게 회전되기

	this->GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;

}

float AActionCharacter::GetCurrentStamina_Implementation() const
{
	return CurrentStamina;
}

bool AActionCharacter::ConsumeStamina_Implementation(float InAmount)
{
	bool bResult = false;
	if (CurrentStamina >= InAmount)
	{
		CurrentStamina -= InAmount;
		bResult = true;
	}
	UE_LOG(LogTemp, Log, TEXT("현재 Stamina : %.1f"), CurrentStamina);
	return bResult;
}

void AActionCharacter::RecoveryStamina_Implementation(float InAmount)
{
	CurrentStamina = FMath::Clamp(CurrentStamina + InAmount, 0.0f, MaxStamina);
	UE_LOG(LogTemp, Log, TEXT("현재 Stamina : %.1f"), CurrentStamina);
}


// Called when the game starts or when spawned
void AActionCharacter::BeginPlay()
{
	Super::BeginPlay();
	AnimInstance = GetMesh()->GetAnimInstance();
	CurrentStamina = MaxStamina;
	//GetCurrentStamina(); // 실행했을 때 C++에 구현된 내용만 호출한다.
	//IInterfaceStamina::Execute_GetCurrentStamina(this); // 실행했을 때 블루프린트 구현으로 호출한다
	
}

// Called every frame
void AActionCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (RecoveryTime > 0)
	{
		RecoveryTime -= DeltaTime;
	}
	else
	{
		RecoveryTime = 0.0f;
		IInterfaceStamina::Execute_RecoveryStamina(this,2.0f);
	}

}

void AActionCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(IA_Test, ETriggerEvent::Started, this, &AActionCharacter::OnTestAction);
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AActionCharacter::OnMoveAction);
		EnhancedInputComponent->BindAction(IA_Boost, ETriggerEvent::Triggered, this, &AActionCharacter::OnBoostOn);
		EnhancedInputComponent->BindAction(IA_Boost, ETriggerEvent::Completed, this, &AActionCharacter::OnBoostOff);
	}
}

void AActionCharacter::OnTestAction(const FInputActionValue& Value)
{
	if (!RollMontage.IsValid()) return;

	if (AnimInstance = GetMesh()->GetAnimInstance())
	{
		if (!AnimInstance->IsAnyMontagePlaying())
		{
			if (!GetLastMovementInputVector().IsNearlyZero()) // 이동 입력중이면
			{
				SetActorRotation(GetLastMovementInputVector().Rotation()); // 입력방향으로 즉시 회전해서 구르기
			}

			if (IInterfaceStamina::Execute_ConsumeStamina(this, 20))
			{
				PlayAnimMontage(RollMontage.Get());
				RecoveryTime = 3.0f;
				
			}

			
		}
	}

}

void AActionCharacter::OnMoveAction(const FInputActionValue& Value)
{
	const FVector2D MoveInput = Value.Get<FVector2D>();

	// 컨트롤러(카메라)가 보는 방향 기준으로 앞/오른쪽 벡터 구하기
	const FRotator ControlRotation = GetControlRotation();
	const FRotator YawRotation(0, ControlRotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MoveInput.Y);
	AddMovementInput(RightDirection, MoveInput.X);
}

void AActionCharacter::OnBoostOn(const FInputActionValue& Value)
{
	if (ConsumeStamina_Implementation(5))
	{
		this->GetCharacterMovement()->MaxWalkSpeed = MoveSpeed * 3;
		RecoveryTime = 3.0f;
	}
	else
	{
		this->GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
		
	}
}

void AActionCharacter::OnBoostOff(const FInputActionValue& Value)
{
	this->GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
	
}



