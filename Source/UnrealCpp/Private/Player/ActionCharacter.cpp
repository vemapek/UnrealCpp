// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/ActionCharacter.h"
#include "Component/StatActorComponent.h"
#include "AnimNotify/AnimNotifyState_SectionJump.h"
#include "Data/WeaponDataAsset.h"
#include "Weapon/WeaponActor.h"

#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

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

void AActionCharacter::EqueipWeapon_Implementation(UWeaponDataAsset* InWeaponData)
{
	// 이전 무기 해제하기
	if (CurrentWeapon.IsValid())
	{
		CurrentWeapon.Get()->DropWeapon();
		CurrentWeapon = nullptr;
	}

	// 새 무기 장비하기
	CurrentWeaponData = InWeaponData;
	if (!InWeaponData->IsLoaded())
	{
		UWeaponDataAsset* RequestedData = InWeaponData;
		InWeaponData->RequestDataLoad(
			FStreamableDelegate::CreateWeakLambda(
				this,
				[this, RequestedData]()
				{
					// 로딩이 완료되면 실행되는 람다 함수
					if (CurrentWeaponData == RequestedData)
					{
						// 중복으로 로딩 요청했을 때를 대비
						SpawnWeaponActor();
					}
				})
		);
	}
	else
	{
		SpawnWeaponActor();
	}
}

void AActionCharacter::OnWeaponDepleted_Implementation()
{
	// 소모성 무기의 사용 횟수가 다 되어 무기가 스스로 버려진 뒤 호출됨
	CurrentWeapon = nullptr;

	if (DefaultWeaponData)
	{
		EqueipWeapon_Implementation(DefaultWeaponData); // 기본 무기로 복귀
	}
}

UStatActorComponent* AActionCharacter::GetStatComponent() const
{
	return StatComponent;
}

void AActionCharacter::OnWeaponAttackState(bool bEnable)
{
	OnOnWeaponAttackStateChanged.ExecuteIfBound(bEnable);
}

void AActionCharacter::SetSectionJumpNotify(UAnimNotifyState_SectionJump* InSectionJumpNotify)
{
	SectionJumpNotify = InSectionJumpNotify;
	bComboReady = SectionJumpNotify.IsValid();
}

float AActionCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (UStatActorComponent* StatComp = GetStatComponent())
	{
		IInterfaceHealth::Execute_DamageHealth(StatComp, Damage);

		UE_LOG(LogTemp, Log, TEXT("%.1f 데미지를 입었습니다. (공격자:%s)"), Damage, *EventInstigator->GetName());
	}

	return Damage;
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

	if (!CurrentWeapon.IsValid() && DefaultWeaponData) // 시작할 때 기본 무기를 장비한다
	{
		EqueipWeapon_Implementation(DefaultWeaponData);
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

void AActionCharacter::SectionJumpForCombo()
{
	if (SectionJumpNotify.IsValid() && bComboReady)
	{
		UAnimMontage* Current = AnimInstance->GetCurrentActiveMontage();
		AnimInstance->Montage_SetNextSection( // 섹션을 변경한다
			AnimInstance->Montage_GetCurrentSection(Current), // 이 섹션에서(from)
			SectionJumpNotify->GetNextSectionName(),          // 이 섹션으로 변경(to)
			Current // 적용할 몽타주
		);

		IInterfaceStamina::Execute_ConsumeStamina(GetStatComponent(), AttackStamina);
		bComboReady = false; // 중복실행 방지
	}
}

void AActionCharacter::SpawnWeaponActor()
{
	if (!CurrentWeaponData)
	{
		return; // 로딩 요청이 끝나기 전에 해제되었을 때를 대비
	}

	CurrentWeapon = GetWorld()->SpawnActorDeferred<AWeaponActor>(
		AWeaponActor::StaticClass(), FTransform::Identity, this, this); // 스폰 시작
	if (CurrentWeapon.IsValid())
	{
		CurrentWeapon->InitializeWeapon(CurrentWeaponData);
		UGameplayStatics::FinishSpawningActor(CurrentWeapon.Get(), FTransform::Identity); // 스폰 완료(=BeginPlay까지 실행)
		CurrentWeapon->EquipToTarget(this);
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
		EnhancedInputComponent->BindAction(IA_Attack, ETriggerEvent::Started, this, &AActionCharacter::OnAttackAction);
		EnhancedInputComponent->BindAction(IA_DropWeapon, ETriggerEvent::Started, this, &AActionCharacter::OnDropWeaponAction);
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

	// 몽타주 재생 중이 아닐 때만 구르기 시도 (재생 중엔 스태미나 소모 자체를 안 함)
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

void AActionCharacter::OnAttackAction(const FInputActionValue& Value)
{
	if (AnimInstance && IInterfaceStamina::Execute_GetCurrentStamina(GetStatComponent()) > AttackStamina)
	{
		if (!AnimInstance->IsAnyMontagePlaying())
		{
			// 첫 번째 콤보 공격
			PlayAnimMontage(AttackMontage);
			IInterfaceStamina::Execute_ConsumeStamina(GetStatComponent(), AttackStamina);
		}
		else if (AnimInstance->GetCurrentActiveMontage() == AttackMontage)
		{
			SectionJumpForCombo();
		}
	}
}

void AActionCharacter::OnDropWeaponAction(const FInputActionValue& Value)
{
	// 이미 기본 무기를 들고 있으면 버릴 게 없으니 아무 것도 안 함
	if (DefaultWeaponData && CurrentWeaponData != DefaultWeaponData)
	{
		EqueipWeapon_Implementation(DefaultWeaponData);
	}
}
