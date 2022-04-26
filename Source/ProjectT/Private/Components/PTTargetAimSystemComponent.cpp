// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PTTargetAimSystemComponent.h"
#include "Components/WidgetComponent.h"

UPTTargetAimSystemComponent::UPTTargetAimSystemComponent()
{
	// TargetAimSystem
	UPrimitiveComponent::SetCollisionProfileName(TEXT("Target"));
	
	// LockOnWidget
	LockOnWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("LockOnWidget"));
	LockOnWidget->SetupAttachment(this);
	LockOnWidget->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> BP_LockOnWidget(TEXT("/Game/Blueprints/BP_PTLockOnWidget.BP_PTLockOnWidget_C"));
	if(BP_LockOnWidget.Succeeded())
	{
		LockOnWidget->SetWidgetClass(BP_LockOnWidget.Class);
	}
	LockOnWidget->SetDrawSize(FVector2D(25.0f, 25.0f));
	LockOnWidget->SetHiddenInGame(true);
}
