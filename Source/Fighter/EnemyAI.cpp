// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAI.h"
#include "kusaGameInstance.h"



AEnemyAI::AEnemyAI() {
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemyAI::myBeginPlay() {

}

void AEnemyAI::myTick(float dt) {

	gameInstance = Cast<UkusaGameInstance>(GetGameInstance());

	gameInstance->EHealth = Health;

	UE_LOG(LogTemp, Warning, TEXT("-------------------------------------------------------------------"));
}