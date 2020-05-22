// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyChar.h"
#include "EnemyAI.generated.h"

/**
 * 
 */
UCLASS()
class FIGHTER_API AEnemyAI : public AMyChar
{
	GENERATED_BODY()

	AEnemyAI();

public:
	UPROPERTY(EditAnywhere, category = "meshes")
	TSubclassOf<class AWeaponClass> axe;


	UPROPERTY(EditAnywhere, category = "meshes")
	int L1_AI_indexOfAction;

	
public:

	UFUNCTION(BlueprintCallable, Category = "kusaF")
	void myBeginPlay();

	UFUNCTION(BlueprintCallable, Category = "kusaF")
	void myTick(float dt);

	UFUNCTION()
	void Level_1_AI(float dt);
	
};
