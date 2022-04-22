// Fill out your copyright notice in the Description page of Project Settings.


#include "PTPlayerController.h"

void APTPlayerController::BeginPlay()
{
	Super::BeginPlay();

	const FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
}
