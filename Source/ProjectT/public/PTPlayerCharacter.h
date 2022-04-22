// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PTPlayerCharacter.generated.h"

UCLASS()
class PROJECTT_API APTPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APTPlayerCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

#pragma region Camera
protected:
	/**
	 * 마우스 사용할 때 사용하는 카메라 좌우 회전 함수입니다.
	 * @Value: 마우스를 사용한 좌우 회전값입니다.
	 */
	void Turn(float Value);

	/**
	 * 마우스를 사용할 때 사용하는 카메라 상하 회전 함수입니다.
	 * @Value: 마우스를 사용한 상하 회전값입니다.
	 */
	void LookUp(float Value);

	/**
	 * 게임패드를 사용할 때 사용하는 카메라 좌우 회전 함수입니다.
	 * @Rate: 게임패드를 사용한 좌우 회전값입니다.
	 */
	void TurnRate(float Rate);
	
	/**
	 * 게임패드를 사용할 때 사용하는 카메라 상하 회전 함수입니다.
	 * @Rate: 게임패드를 사용한 상하 회전값입니다.
	 */
	void LookUpRate(float Rate);

protected:
	/** 캐릭터 뒤에 카메라를 배치하는 SpringArm입니다. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class USpringArmComponent* SpringArm;

	/** 캐릭터 뒤에서 따라오는 카메라입니다. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* FollowCamera;

	/** 카메라가 캐릭터 뒤에서 떨어져서 따라오는 거리입니다. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	float BaseSpringArmLength;

	/** 카메라의 좌우 기본 회전 값입니다. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	float BaseTurnRate;

	/** 카메라의 상하 기본 회전 값입니다. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	float BaseLookUpRate;
#pragma endregion

#pragma region CharacterMovement

#pragma endregion 
protected:
	/**
	 * MoveForward 키를 입력할 경우 호출하는 함수입니다.
	 * @param Value MoveForward의 입력값입니다.
	 */
	void MoveForward(float Value);

	/**
	 * MoveRight 키를 입력할 경우 호출하는 함수입니다.
	 * @param Value MoveRight의 입력값입니다.
	 */
	void MoveRight(float Value);
#pragma region TargetingSystem
protected:
	/** Target을 LockOn하는 함수입니다. */
	void ExecuteLockOnTarget();

protected:
	/** 일정범위 안이 LockOn이 가능한 Actor들을 탐지하여 Target으로 지정하고 플레이어의 카메라를 Target에게 고정하는 ActorComponent 클래스입니다. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TargetingSystem")
	class UPTTargetingSystemComponent* TargetingSystem;

public:
	/** TargetingSystem을 반환하는 함수입니다. */
	FORCEINLINE class UPTTargetingSystemComponent* GetTargetingSystem() const
	{
		return TargetingSystem;
	}
#pragma endregion
};
