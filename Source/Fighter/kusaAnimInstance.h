// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "kusaAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class FIGHTER_API UkusaAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable, Category = "AnimProperty")
	void update();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	class APawn* Pawn;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameInstance")
	class UkusaGameInstance* gameInstance;

	UPROPERTY(BlueprintReadOnly, category = "kusaVar")
	bool punching = false;

	UPROPERTY(BlueprintReadOnly, category = "kusaVar")
	int actionIndex = 0;
	
};
