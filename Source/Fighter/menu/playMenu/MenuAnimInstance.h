// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MenuAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class FIGHTER_API UMenuAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable, Category = "AnimProperty")
	void update(float dt);
	

	float timeCounter = 0;

	UPROPERTY(BlueprintReadWrite, Category = "kusa")
	int ChangeState = 0;

};
