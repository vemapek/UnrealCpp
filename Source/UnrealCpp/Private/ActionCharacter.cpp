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

// Called when the game starts or when spawned
void AActionCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AActionCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AActionCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(IA_Test, ETriggerEvent::Started, this, &AActionCharacter::OnTestAction);
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AActionCharacter::OnMoveAction);
		EnhancedInputComponent->BindAction(IA_Boost, ETriggerEvent::Started, this, &AActionCharacter::OnBoostOn);
		EnhancedInputComponent->BindAction(IA_Boost, ETriggerEvent::Completed, this, &AActionCharacter::OnBoostOff);
	}
}

void AActionCharacter::OnTestAction(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Log, TEXT("TestAction 실행"));
	//Value.Get<bool>();
	//Value.Get<FVector2D>();
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
	this->GetCharacterMovement()->MaxWalkSpeed = MoveSpeed*3;
}

void AActionCharacter::OnBoostOff(const FInputActionValue& Value)
{
	this->GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
}



