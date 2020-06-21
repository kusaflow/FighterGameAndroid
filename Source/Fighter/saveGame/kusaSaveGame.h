// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "kusaSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class FIGHTER_API UkusaSaveGame : public USaveGame
{
	GENERATED_BODY()

public :

	UkusaSaveGame();

	UPROPERTY(BlueprintReadWrite, Category = "kusacoin")
	int xp;

	UPROPERTY(BlueprintReadWrite, Category = "kusaWeapon")
	bool bHave_axe;

	UPROPERTY(BlueprintReadWrite, Category = "kusaWeapon")
	bool bHave_sword;

	UPROPERTY(BlueprintReadWrite, Category = "kusaMaterial")
	bool bmat1;

	UPROPERTY(BlueprintReadWrite, Category = "kusaMaterial")
	bool bmat2;

	UPROPERTY(BlueprintReadWrite, Category = "kusaMaterial")
	bool bmat3;

	UPROPERTY(BlueprintReadWrite, Category = "kusaMaterial")
	bool bmat4;

	UPROPERTY(BlueprintReadWrite, Category = "kusaMaterial")
	bool bmat5;

	UPROPERTY(BlueprintReadWrite, Category = "FiveWin")
	int winCount = 0;
	
};
