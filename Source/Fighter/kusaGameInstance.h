// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "kusaGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class FIGHTER_API UkusaGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	//player variables
	UPROPERTY(BlueprintReadWrite)
	int CharNumberIndex = 3;

	UPROPERTY(BlueprintReadWrite)
	int EnemyNumberIndex = 2;

	UPROPERTY(BlueprintReadWrite)
	float EHealth = 0;

	UPROPERTY(BlueprintReadWrite)
	float PHealth = 0;

	UPROPERTY(BlueprintReadWrite)
	float MaxHealth = 500;


	
};
