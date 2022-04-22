// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PTDummyCharacter.generated.h"

UCLASS()
class PROJECTT_API APTDummyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APTDummyCharacter();

#pragma region CharacterState

protected:
	/** 캐릭터가 사망했는지 나타내는 변수입니다. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterState", Meta = (AllowPrivateAccess = "true"))
	bool bIsDead;

public:
	/** 캐릭터가 사망했는지 판별하는 함수입니다. */
	bool IsDead() const;

protected:
	/** 캐릭터가 사망했을 때 호출하는 함수입니다. */
	virtual void Dead();
#pragma endregion 

#pragma region TargetAimSystem
public:
	/**
	 * Target으로 지정되었을 때 실행하는 함수입니다.
	 * 입력된 인자에 따라 TargetAimSystem의 LockWidget을 활성화를 설정합니다.
	 * @param bFlag true일 경우 TargetAimSystem을 활성화합니다.
	 *				false일 경우 TargetAimSystem을 비활성화 합니다.
	 */
	void SelectedTarget(bool bFlag);
	
protected:
	/** Targeting TraceChannel에 블록되는 Collision과 Target으로 지정되어 LockOn 되었을 때 활성화하는 위젯을 가진 SphereComponent 클래스입니다. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TargetAimSystem")
	class UPTTargetAimSystemComponent* TargetAimSystem;

public:
	/** TargetAimSystem을 반환하는 함수입니다. */
	FORCEINLINE class UPTTargetAimSystemComponent* GetTargetAimSystem() const
	{
		return TargetAimSystem;
	}

	/** TargetAimSystem의 위치를 반환하는 함수입니다. */
	FVector GetTargetAimLocation() const;
#pragma endregion 
};
