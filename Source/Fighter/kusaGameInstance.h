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
	int CharNumberIndex = 2;

	UPROPERTY(BlueprintReadWrite)
	int EnemyNumberIndex = 1;

	UPROPERTY(BlueprintReadWrite)
	int EnemyMatIndex = 1;
	
	UPROPERTY(BlueprintReadWrite)
	int CharMatIndex = 1;


	UPROPERTY(BlueprintReadWrite)
	float EHealth = 0;

	UPROPERTY(BlueprintReadWrite)
	float PHealth = 0;

	UPROPERTY(BlueprintReadWrite)
	float MaxHealth = 500;

	UPROPERTY(BlueprintReadWrite, Category = "Energy")
	float MaxHitEnergy = 100;

	UPROPERTY(BlueprintReadWrite, Category = "Energy")
	float HitEnergyPLayer=0;

	UPROPERTY(BlueprintReadWrite, Category = "Energy")
	float HitEnergyEnemy=0;

	UPROPERTY(BlueprintReadWrite, Category = "GameOver")
	bool bisGameOver = false; 

	UPROPERTY(BlueprintReadWrite, Category = "GameOver")
	bool bWeWon = false;

	UPROPERTY(BlueprintReadWrite, Category = "kusacoin")
	int xp = 10000;

	UPROPERTY(BlueprintReadWrite, Category = "kusaWeapon")
	bool bHave_axe= false;

	UPROPERTY(BlueprintReadWrite, Category = "kusaWeapon")
	bool bHave_sword = false;

	UPROPERTY(BlueprintReadWrite, Category = "kusaMaterial")
	bool bmat1 = false;
	
	UPROPERTY(BlueprintReadWrite, Category = "kusaMaterial")
	bool bmat2 = false;
	
	UPROPERTY(BlueprintReadWrite, Category = "kusaMaterial")
	bool bmat3 = false;
	
	UPROPERTY(BlueprintReadWrite, Category = "kusaMaterial")
	bool bmat4 = false;
	
	UPROPERTY(BlueprintReadWrite, Category = "kusaMaterial")
	bool bmat5 = false;



	//weapons Acttual price
	UPROPERTY(BlueprintReadWrite, Category = "kusaWeapon")
	int weaponPrice = 2000;

	UPROPERTY(BlueprintReadWrite, Category = "kusaHealth")
	bool bPlayerHitToPlayReaction = false;

	UPROPERTY(BlueprintReadWrite, Category = "kusaHealth")
	bool bEnemyHitToPlayReaction = false;




};
