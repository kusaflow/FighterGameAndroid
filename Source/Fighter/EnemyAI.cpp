// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAI.h"
#include "kusaGameInstance.h"
#include "DrawDebugHelpers.h"
#include "CollisionQueryParams.h"
#include "Engine/World.h"
#include "Math/UnrealMathUtility.h"



AEnemyAI::AEnemyAI() {
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemyAI::myBeginPlay() {
	
}

void AEnemyAI::myTick(float dt) {

	bEnemyIsOnRight = false;
	
	gameInstance = Cast<UkusaGameInstance>(GetGameInstance());
	CharNumberIndex = gameInstance->EnemyNumberIndex;
	gameInstance->EHealth = Health;

	GetRootComponent()->GetChildComponent(1)->SetRelativeRotation(FRotator(0, 90, 0));

	Level_1_AI(dt);
	
}

void AEnemyAI::Level_1_AI(float dt) {
	/*
	the basic logic of this is it goes to sleep 
	wakews up assign a move then go to sleep then wake up
	then do a move
	*/

	//pre req
	FHitResult OutHit;
	FVector Start = GetRootComponent()->GetComponentLocation();

	FVector ForwardVector = GetRootComponent()->GetForwardVector();
	FVector End = ((ForwardVector * 1000.f) - Start);
	FCollisionQueryParams CollisionParams;
	//======================

	if (!bAnim_ActionInMOtion || bGotHit) {
		return;
	}

	//if no action assigned then assign
	
	if (L1_AI_indexOfAction == 0) {
		/*
		choose 3 digits
		1 for move
		2 for action
		*/
		L1_AI_indexOfAction = (int)FMath::FRandRange(1, 30);
		int t = 0;
		if (L1_AI_indexOfAction%7 == 0) {
			t = (int)FMath::FRandRange(1, 30);
			if (t%2 == 0) {
				MovingTowardsPLayer = true;
			}
			else {
				MovingTowardsPLayer = false;
			}
			L1_AI_indexOfAction = 1;
		}
		else if (L1_AI_indexOfAction%4 == 0) {
			sleepTime = 0;
			sleepTotime = FMath::FRandRange(1, 50);
			L1_AI_indexOfAction = 2;
		}
		else {
			L1_AI_indexOfAction = 5;
		}
		
	}




	//validate and then perform-------------------------------------------------------------
	if (L1_AI_indexOfAction == 1) {
		//move 
		if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams))
		{
			int t = 0;
			t = (int)FMath::FRandRange(1, 30);
			//UE_LOG(LogTemp, Warning, TEXT("%f"), (float)(OutHit.Actor.Get()->GetActorLocation().X - Start.X));
			//1 is going towards
			if (MovingTowardsPLayer == true) {
				
				if (OutHit.Actor.Get()->GetActorLocation().X - Start.X <= 100 || t%9 == 0) {
					L1_AI_indexOfAction = 0;
				}
				else {
					MoveLeftRight(1);
				}
				
			}
			else {
				if (OutHit.Actor.Get()->GetActorLocation().X - Start.X >= 700 || t % 5 == 0) {
					if (t % 9 != 0) {
						L1_AI_indexOfAction = 1;
						MovingTowardsPLayer = true;
					}
				}
				else {
					MoveLeftRight(-1);
				}
			}
		}
	}
	else {
		MoveLeftRight(0);
	}
	

	if (L1_AI_indexOfAction == 2) {
		//UE_LOG(LogTemp, Warning, TEXT("%f======================%f"), (float)sleepTime, (float)sleepTotime);
		if (sleepTime >= sleepTotime) {
			L1_AI_indexOfAction = 0;
		}
		else {
			if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams)) {
				if (OutHit.Actor.Get()->GetActorLocation().X - Start.X <= 300) {
					++sleepTime;
				}
			}
			sleepTime += 20.0f * GetWorld()->GetDeltaSeconds();
		}
	}
	else if (L1_AI_indexOfAction == 5) {
		//action
		L1_AI_indexOfAction = 0;
		if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams)) {
			if (OutHit.Actor.Get()->GetActorLocation().X - Start.X <= 200) {

				UE_LOG(LogTemp, Warning, TEXT("====================Action======================"));
				bPunchOn = false;
				bKickOn = true;
				bSpecial = false;
				FirstAction();

				//sleep time
				sleepTime = 0;
				sleepTotime = FMath::FRandRange(20, 50);
				L1_AI_indexOfAction = 2;

			}
			else {
				L1_AI_indexOfAction = 1;
				MovingTowardsPLayer = true;
			}
		}

		//UE_LOG(LogTemp, Warning, TEXT("====================kusa======================"));
	}


	/*
	FHitResult OutHit;
	FVector Start = GetRootComponent()->GetComponentLocation();

	FVector ForwardVector = GetRootComponent()->GetForwardVector();
	FVector End = ((ForwardVector * 1000.f) + Start);
	FCollisionQueryParams CollisionParams;

	DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1, 0, 2);
	if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams))
	{
		if (OutHit.Actor.Get()->GetActorLocation().X - Start.X >= 250) {
			MoveLeftRight(1);
		}
		else if (OutHit.Actor.Get()->GetActorLocation().X - Start.X <= 160) {
			MoveLeftRight(-1);
		}
		else {
			MoveLeftRight(0);
		}
		UE_LOG(LogTemp, Warning, TEXT("%f"), (float)(OutHit.Actor.Get()->GetActorLocation().X - Start.X));
		
	}
	*/
}