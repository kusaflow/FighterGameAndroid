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
	class AMyChar* player;

	UPROPERTY(BlueprintReadOnly, Category = "kusaVar")
	bool bPunching = false;

	UPROPERTY(BlueprintReadOnly, Category = "kusaVar")
	bool bActionPressed1 = false;

	UPROPERTY(BlueprintReadOnly, Category = "kusaVar")
	int actionIndex = 0;

	UPROPERTY(BlueprintReadOnly, Category = "kusaVar")
	int CharNumberIndex = 1;


	UPROPERTY(BlueprintReadOnly, Category = "kusaVar")
	bool bKickOn = false;

	UPROPERTY(BlueprintReadOnly, Category = "kusaVar")
	bool bSpecial = false;

	UPROPERTY(BlueprintReadOnly, Category = "kusaActions")
	bool EnemyIsOnRight = false;

	UPROPERTY(BlueprintReadOnly, Category = "kusaActions")
	float RightLeftMoveVal = 0;


	
};
