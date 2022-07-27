// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PTPlayerController.generated.h"

/**
 * 플레이어가 사용하는 PlayerController 클래스입니다.
 */
UCLASS()
class PROJECTT_API APTPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

protected:
	/** 인게임에서 사용하는 InGameHUD입니다. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "InGameHUD", meta = (AllowPrivateAccess = "true"))
	class UUserWidget* InGameHUD;
};
