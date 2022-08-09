// Fill out your copyright notice in the Description page of Project Settings.


#include "PTPlayerCharacter.h"
#include "Components/PTTargetingSystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "PTDummyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

APTPlayerCharacter::APTPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// CapsuleComponent
	GetCapsuleComponent()->InitCapsuleSize(36.0f, 94.0f);

	// SkeletalMesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -94.0f), FRotator(0.0f, -90.0f, 0.0f));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Mannequin(TEXT("/Game/Mannequin/Character/Mesh/SK_Mannequin"));
	if(SK_Mannequin.Succeeded() == true)
	{
		GetMesh()->SetSkeletalMesh(SK_Mannequin.Object);
	}

	// Controller
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// CharacterMovement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;

	// SpringArm
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	BaseSpringArmLength = 400.0f;
	SpringArm->TargetArmLength = BaseSpringArmLength;
	SpringArm->SocketOffset = FVector(0.0f, 0.0f, 200.0f);
	SpringArm->bUsePawnControlRotation = true;										// 컨트롤러를 기준으로 SpringArm을 회전합니다.
	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraLagSpeed = 10.0f;
	SpringArm->CameraRotationLagSpeed = 20.0f;
	

	// FollowCamera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;									// 카메라가 SpringArm을 기준으로 회전하지 않습니다.
	FollowCamera->SetRelativeRotation(FRotator(-20.0f, 0.0f, 0.0f));

	// Camera
	bTurnMoveCamera = false;
	bLookUpMoveCamera = false;
	bTurnRateMoveCamera = false;
	bLookUpRateMoveCamera = false;
	BaseTurnRate = 45.0f;
	BaseLookUpRate = 45.0f;
	
	// TargetingSystem
	TargetingSystem = CreateDefaultSubobject<UPTTargetingSystemComponent>(TEXT("TargetingSystem"));
	bReadyChangeTarget = false;
}

void APTPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void APTPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APTPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Turn", this, &APTPlayerCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &APTPlayerCharacter::LookUp);

	PlayerInputComponent->BindAxis("TurnRate", this, &APTPlayerCharacter::TurnRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APTPlayerCharacter::LookUpRate);

	PlayerInputComponent->BindAxis("MoveForward", this, &APTPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APTPlayerCharacter::MoveRight);
	
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APTPlayerCharacter::Jump);
	
	PlayerInputComponent->BindAction("TargetLockOn", IE_Pressed, this, &APTPlayerCharacter::ExecuteLockOnTarget);

	PlayerInputComponent->BindAction("KillTarget", IE_Pressed, this, &APTPlayerCharacter::KillTarget);

	PlayerInputComponent->BindAction("ToggleDynamicLockOnTarget", IE_Pressed, this, &APTPlayerCharacter::ToggleDynamicLockOnTarget);

	PlayerInputComponent->BindAction<TDelegate<void(bool)>>("ReadyChangeTarget", IE_Pressed, this, &APTPlayerCharacter::SetReadyChangeTarget, true);
	PlayerInputComponent->BindAction<TDelegate<void(bool)>>("ReadyChangeTarget", IE_Released, this, &APTPlayerCharacter::SetReadyChangeTarget, false);

	PlayerInputComponent->BindAction("ToggleDrawDebug", IE_Pressed, this, &APTPlayerCharacter::ToggleDrawDebug);
}

#pragma region Camera
bool APTPlayerCharacter::IsMoveCamera() const
{
	return bTurnMoveCamera || bLookUpMoveCamera || bTurnRateMoveCamera || bLookUpRateMoveCamera;
}

void APTPlayerCharacter::Turn(float Value)
{
	if(Value != 0.0f)
	{
		bTurnMoveCamera = true;
	}
	else
	{
		bTurnMoveCamera = false;
	}
	
	// LockOnTarget 상태일 때는 Target을 입력방향에 존재하는 Target으로 변경하는 함수를 싱행합니다.
	if(TargetingSystem->IsLockOnTarget() == true && (TargetingSystem->IsDynamicLockOnTarget() == false || (TargetingSystem->IsDynamicLockOnTarget() == true && bReadyChangeTarget)) && Value != 0.0f)
	{
		TargetingSystem->ChangeLockOnTargetForTurnValue(EPTInputMode::InputMode_Mouse, Value);
	}
	else
	{
		AddControllerYawInput(Value);
	}
}

void APTPlayerCharacter::LookUp(float Value)
{
	if(Value != 0.0f)
	{
		bLookUpMoveCamera = true;
	}
	else
	{
		bLookUpMoveCamera = false;
	}
	
	if(TargetingSystem->IsLockOnTarget() == false || (TargetingSystem->IsDynamicLockOnTarget() == true && !bReadyChangeTarget))
	{
		AddControllerPitchInput(Value);
	}
}

void APTPlayerCharacter::TurnRate(float Rate)
{
	if(Rate != 0.0f)
	{
		bTurnRateMoveCamera = true;
	}
	else
	{
		bTurnRateMoveCamera = false;
	}
	
	// LockOnTarget 상태일 때는 Target을 입력방향에 존재하는 Target으로 변경하는 함수를 싱행합니다.
	if(TargetingSystem->IsLockOnTarget() == true && (TargetingSystem->IsDynamicLockOnTarget() == false || (TargetingSystem->IsDynamicLockOnTarget() == true && bReadyChangeTarget)) && Rate != 0.0f)
	{
		TargetingSystem->ChangeLockOnTargetForTurnValue(EPTInputMode::InputMode_Gamepad, Rate);		
	}
	else
	{
		AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	}
}

void APTPlayerCharacter::LookUpRate(float Rate)
{
	if(Rate != 0.0f)
	{
		bLookUpRateMoveCamera = true;
	}
	else
	{
		bLookUpRateMoveCamera = false;
	}
	
	if(TargetingSystem->IsLockOnTarget() == false || (TargetingSystem->IsDynamicLockOnTarget() == true && !bReadyChangeTarget))
	{
		AddControllerPitchInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	}
}
#pragma endregion

#pragma region CharacterMovement
void APTPlayerCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void APTPlayerCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}
#pragma endregion 

#pragma region TargetingSystem
void APTPlayerCharacter::ExecuteLockOnTarget()
{
	if(TargetingSystem->IsLockOnTarget() == false)
	{
		TargetingSystem->ExecuteLockOnTarget();
	}
	else
	{
		TargetingSystem->CancelLockOnTarget();
	}
}

void APTPlayerCharacter::ToggleDynamicLockOnTarget()
{
	if(TargetingSystem->IsDynamicLockOnTarget() == true)
	{
		TargetingSystem->SetDynamicLockOnTarget(false);
	}
	else
	{
		TargetingSystem->SetDynamicLockOnTarget(true);
	}
}

void APTPlayerCharacter::SetReadyChangeTarget(bool bFlag)
{
	bReadyChangeTarget = bFlag;
}

void APTPlayerCharacter::ToggleDrawDebug()
{
	if(TargetingSystem->IsDrawDebug() == true)
	{
		TargetingSystem->SetDrawDebug(false);
	}
	else
	{
		TargetingSystem->SetDrawDebug(true);
	}
}
#pragma endregion 

#pragma region Kill
void APTPlayerCharacter::KillTarget()
{
	if(TargetingSystem->IsLockOnTarget() == true)
	{
		APTDummyCharacter* DummyCharacter = Cast<APTDummyCharacter>(TargetingSystem->GetTarget());
		if(IsValid(DummyCharacter) == true)
		{
			DummyCharacter->Dead();
		}
	}
}
#pragma endregion 

