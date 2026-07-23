// Fill out your copyright notice in the Description page of Project Settings.

#include "ActionCharacter.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "StatActorComponent.h"

// Sets default values
AActionCharacter::AActionCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraSpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	CameraSpringArmComponent->SetupAttachment(RootComponent);
	CameraSpringArmComponent->bUsePawnControlRotation = true; // 스프링암은 컨트롤러 입력에 맞게 회전되기

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	CameraComponent->SetupAttachment(CameraSpringArmComponent);

	StatComponent = CreateDefaultSubobject<UStatActorComponent>(TEXT("Stat"));

	bUseControllerRotationYaw = false; // 컨트롤러 움직일 때 폰이 같이 회전되는 것 방지
	GetCharacterMovement()->bOrientRotationToMovement = true; // 캐릭터 이동방향으로 바라보게 만들기
}

// Called when the game starts or when spawned
void AActionCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
	}
	if (GetMesh())
	{
		AnimInstance = GetMesh()->GetAnimInstance();
	}
	if (StatComponent)
	{
		FAutoRecoveryData Data = FAutoRecoveryData(
			StaminaAutoRecoveryCoolTime,
			StaminaAutoRecoveryInterval,
			StaminaAutoRecoveryPerTick);
		StatComponent->InitializeStat(Data);
	}
}

// Called every frame
void AActionCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SpendBoostStamina(DeltaTime);
}

void AActionCharacter::SpendBoostStamina(float DeltaTime)
{
	// 부스트 모드이고, 이동하고 있고, 몽타주 재생 중이 아니면
	if (bBoostMode && !GetVelocity().IsNearlyZero() &&
		(AnimInstance && !AnimInstance->IsAnyMontagePlaying()))
	{
		// 스태미나 지속적으로 감소
		if (!IInterfaceStamina::Execute_ConsumeStamina(StatComponent, BoostStaminaCostPerSec * DeltaTime))
		{
			OnBoostOff(FInputActionValue()); // 스태미나가 다 떨어지면 부스트 모드 정지
		}
	}
}

// Called to bind functionality to input
void AActionCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(IA_Test, ETriggerEvent::Started, this, &AActionCharacter::OnTestAction);
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AActionCharacter::OnMoveAction);
		EnhancedInputComponent->BindActionValueLambda(IA_Boost, ETriggerEvent::Started,
			[this](const FInputActionValue& _) {
				OnBoostOn(_);
			});
		EnhancedInputComponent->BindActionValueLambda(IA_Boost, ETriggerEvent::Completed,
			[this](const FInputActionValue& _) {
				OnBoostOff(_);
			});
	}
}

void AActionCharacter::OnTestAction(const FInputActionValue& Value)
{
	if (!RollMontage) return;

	if (!AnimInstance)
	{
		AnimInstance = GetMesh()->GetAnimInstance();
	}

	// 몽타주 재생 중이 아닐 때만 구르기 시도
	if (AnimInstance && !AnimInstance->IsAnyMontagePlaying())
	{
		if (IInterfaceStamina::Execute_ConsumeStamina(StatComponent, RollStaminaCost)) // 스태미나 소비 시도 후 소비되면 구르기 실행
		{
			if (!GetLastMovementInputVector().IsNearlyZero()) // 이동 입력 중이면
			{
				SetActorRotation(GetLastMovementInputVector().Rotation()); // 입력방향으로 즉시 회전해서 구르기
			}

			PlayAnimMontage(RollMontage);
		}
	}
}

void AActionCharacter::OnMoveAction(const FInputActionValue& Value)
{
	FVector2D Input = Value.Get<FVector2D>();
	FVector WorldDirection = FVector(Input.Y, Input.X, 0).GetSafeNormal();

	// 카메라의 Yaw 회전각(Degree)을 Radian으로 변경
	float YawRadian = FMath::DegreesToRadians(GetControlRotation().Yaw);

	// 좌우 회전만 할거라 UpVector를 기준축으로 Yaw회전각 만큼 돌리는 회전 만들기
	FQuat ControlYawRotation(FVector::UpVector, YawRadian);

	// 입력된 방향에 회전 적용(=카메라 Yaw회전 만큼 입력방향을 회전시키기)
	WorldDirection = ControlYawRotation.RotateVector(WorldDirection);

	AddMovementInput(WorldDirection);
}

void AActionCharacter::OnBoostOn(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = BoostSpeed;
	bBoostMode = true;
}

void AActionCharacter::OnBoostOff(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
	bBoostMode = false;
}