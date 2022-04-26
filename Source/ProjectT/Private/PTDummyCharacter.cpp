// Fill out your copyright notice in the Description page of Project Settings.


#include "PTDummyCharacter.h"
#include "Components/PTTargetAimSystemComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

APTDummyCharacter::APTDummyCharacter()
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

	// CharacterState
	bIsDead = false;
	
	// TargetAimSystem
	TargetAimSystem = CreateDefaultSubobject<UPTTargetAimSystemComponent>(TEXT("TargetAimSystem"));
	TargetAimSystem->SetupAttachment(RootComponent);
}

bool APTDummyCharacter::IsDead() const
{
	return bIsDead;
}

void APTDummyCharacter::Dead()
{
	bIsDead = true;
	GetMesh()->SetSimulatePhysics(true);
	GetCharacterMovement()->DisableMovement();
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
}

void APTDummyCharacter::SelectedTarget(bool bFlag)
{
	TargetAimSystem->GetLockOnWidget()->SetHiddenInGame(!bFlag);
}

FVector APTDummyCharacter::GetTargetAimLocation() const
{
	return GetTargetAimSystem()->GetComponentLocation();
}

