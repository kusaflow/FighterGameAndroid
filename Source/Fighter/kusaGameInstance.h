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
	UPROPERTY()
	int CharNumberIndex = 2;

	
};
