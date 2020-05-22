// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAI.h"
#include "kusaGameInstance.h"
#include "DrawDebugHelpers.h"
#include "CollisionQueryParams.h"
#include "Engine/World.h"



AEnemyAI::AEnemyAI() {
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemyAI::myBeginPlay() {
	
}

void AEnemyAI::myTick(float dt) {

	gameInstance = Cast<UkusaGameInstance>(GetGameInstance());
	CharNumberIndex = gameInstance->EnemyNumberIndex;
	gameInstance->EHealth = Health;

	Level_1_AI();
	
}

void AEnemyAI::Level_1_AI() {
	/*
	the basic logic of this is it goes to sleep 
	wakews up assign a move then go to sleep then wake up
	then do a move
	*/
	FHitResult OutHit;
	FVector Start = GetRootComponent()->GetComponentLocation();

	FVector ForwardVector = GetRootComponent()->GetForwardVector();
	FVector End = ((ForwardVector * 1000.f) + Start);
	FCollisionQueryParams CollisionParams;

	DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1, 0, 2);
	if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams))
	{
		if (OutHit.Actor.Get()->GetActorLocation().X - Start.X >= 200) {
			MoveLeftRight(1);
		}
		else {
			MoveLeftRight(0);
		}
		UE_LOG(LogTemp, Warning, TEXT("%f"), (float)(OutHit.Actor.Get()->GetActorLocation().X - Start.X));
		
	}
}