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
	gameInstance = Cast<UkusaGameInstance>(GetGameInstance());
	CharMaterialIndex = gameInstance->EnemyMatIndex;

	decideMateral();
}

void AEnemyAI::myTick(float dt) {

	bEnemyIsOnRight = false;
	
	gameInstance = Cast<UkusaGameInstance>(GetGameInstance());
	CharNumberIndex = gameInstance->EnemyNumberIndex;
	gameInstance->EHealth = Health;
	gameInstance->HitEnergyEnemy = HitEnergy;

	//GetRootComponent()->GetChildComponent(1)->SetRelativeRotation(FRotator(0, 90, 0));

	//Level_1_AI(dt);
	
	if (gameInstance->StartFight && !gameInstance->bisGameOver) {
		if (gameInstance->aiIndex == 1) {
			l1AI(dt);
		}
		else if (gameInstance->aiIndex == 2) {
			l2AI(dt);
		}
		else {
			l1AI(dt);
		}
	}

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
	FVector End = ((ForwardVector * 1000.f) + Start);
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
			int t = 60;
			t = (int)FMath::FRandRange(1, 30);
			UE_LOG(LogTemp, Warning, TEXT("%f"), (float)(OutHit.Actor.Get()->GetActorLocation().X - Start.X));
			//1 is going towards
			if (MovingTowardsPLayer) {
				
				if (OutHit.Actor.Get()->GetActorLocation().X - Start.X <= 100 || t%9 == 0) {
					L1_AI_indexOfAction = 0;
					//UE_LOG(LogTemp, Warning, TEXT("%f"), (float)(OutHit.Actor.Get()->GetActorLocation().X - Start.X));
				}
				else {
					//towards player
					MoveLeftRight(-1);
				}
				
			}
			else {
				if (OutHit.Actor.Get()->GetActorLocation().X - Start.X >= 1000 || t % 50 == 0) {
					if (t % 9 != 0) {
						L1_AI_indexOfAction = 1;
						MovingTowardsPLayer = true;
					}
				}
				else {
					//away from playerb
					MoveLeftRight(1);
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
				if ((int)(FMath::FRandRange(0, 100)) % 2 == 0) {
					MovingTowardsPLayer = true;
				}
				else {
					//MovingTowardsPLayer = false;
				}
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

void AEnemyAI::l1AI(float dt) {
	if (!bAnim_ActionInMOtion || bGotHit) {
		return;
	}


	FHitResult OutHit;
	FVector Start = GetRootComponent()->GetComponentLocation();

	FVector ForwardVector = GetRootComponent()->GetForwardVector();
	FVector End = ((ForwardVector * 1100.f) + Start);
	FCollisionQueryParams CollisionParams;

	//DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1, 0, 2);
	//-1 is forward movement

	//decide the role
	if (L1_AI_indexOfAction == 0) {
		/*
		choose 3 digits
		1 for move
		2 for action
		*/
		L1_AI_indexOfAction = (int)FMath::FRandRange(1, 30);
		int t = 0;
		// do movement
		if (L1_AI_indexOfAction % 7 == 0) {
			t = (int)FMath::FRandRange(1, 30);
			if (t % 2 == 0) {
				MovingTowardsPLayer = true;
			}
			else {
				MovingTowardsPLayer = false;
			}
			L1_AI_indexOfAction = 1;
		}
		//go to sleep
		else if (L1_AI_indexOfAction % 8 == 0) {
			sleepTime = 0;
			sleepTotime = FMath::FRandRange(1, 50);
			L1_AI_indexOfAction = 2;
		}
		//hit bro hit
		else {
			L1_AI_indexOfAction = 5;
		}

	}


	// do the role
	if (L1_AI_indexOfAction == 1) {
		if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams)) {
			int t = 60;
			t = (int)FMath::FRandRange(1, 30);

			if (MovingTowardsPLayer) {
				if (OutHit.Actor.Get()->GetActorLocation().X - Start.X >= 100) {
					if (t%22 != 0)
						MoveLeftRight(-1);
					else {
						if ((int)(FMath::FRandRange(1, 10)) % 8 == 0) {
							L1_AI_indexOfAction = 0;
						}
						else {
							sleepTime = 0;
							sleepTotime = FMath::FRandRange(1, 50);
							L1_AI_indexOfAction = 2;
						}
					}

				}
				else {
					L1_AI_indexOfAction = 0;
				}
			}
			else {
				if (OutHit.Actor.Get()->GetActorLocation().X - Start.X <= 900) {
					if (t % 10 != 0)
						MoveLeftRight(1);
					else {
						if ((int)(FMath::FRandRange(1, 10)) % 8 == 0) {
							L1_AI_indexOfAction = 0;
						}
						else {
							sleepTime = 0;
							sleepTotime = FMath::FRandRange(1, 50);
							L1_AI_indexOfAction = 2;
						}
					}
				}
				else {
					L1_AI_indexOfAction = 0;
				}
			}




		}
		else {
			MoveLeftRight(-1);
		}
	}
	else {
		MoveLeftRight(0);
	}
	if (L1_AI_indexOfAction == 2) {
		if (sleepTime >= sleepTotime) {
			L1_AI_indexOfAction = 0;
		}
		else {
			if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams)) {
				if (OutHit.Actor.Get()->GetActorLocation().X - Start.X <= 300) {
					//++sleepTime;
				}
			}
			sleepTime += 20.0f * GetWorld()->GetDeltaSeconds();
		}
	}
	else if (L1_AI_indexOfAction == 5){
		L1_AI_indexOfAction = 0;
		if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams)) {
			if (OutHit.Actor.Get()->GetActorLocation().X - Start.X <= 200) {
				//--------------action Set=====================================
				int typeOfAttack = (int)FMath::FRandRange(1, 4);
				bPunchOn = false;
				bKickOn = false;
				bSpecial = false;

				if (typeOfAttack == 1) {
					bPunchOn = true;
				}
				else if (typeOfAttack == 2) {
					bKickOn = true;
				}
				else {
					bSpecial = true;
				}


				typeOfAttack = (int)FMath::FRandRange(1, 3);
				if (typeOfAttack == 1) {
					FirstAction();
				}
				else if (typeOfAttack == 2) {
					SecondAction();
				}


				//sleep time
				sleepTime = 0;
				sleepTotime = FMath::FRandRange(10, 30);
				L1_AI_indexOfAction = 2;

			}
			else {
				L1_AI_indexOfAction = 1;
				if ((int)(FMath::FRandRange(0, 100)) % 30 == 0) {
					sleepTime = 0;
					sleepTotime = FMath::FRandRange(1, 25);
					L1_AI_indexOfAction = 2;
				}
				else {
					if ((int)(FMath::FRandRange(0, 100)) % 40 == 0){
						MovingTowardsPLayer = false;
					}
					else {
						MovingTowardsPLayer = false;
					}
				}
			}
		}
		else {
			L1_AI_indexOfAction = 1;
			MovingTowardsPLayer = false;
		}
	}

}


void AEnemyAI::l2AI(float dt) {
	if (!bAnim_ActionInMOtion || bGotHit) {
		return;
	}


	FHitResult OutHit;
	FVector Start = GetRootComponent()->GetComponentLocation();

	FVector ForwardVector = GetRootComponent()->GetForwardVector();
	FVector End = ((ForwardVector * 1100.f) + Start);
	FCollisionQueryParams CollisionParams;

	//DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1, 0, 2);
	//-1 is forward movement

	//decide the role
	if (L1_AI_indexOfAction == 0) {
		/*
		choose 3 digits
		1 for move
		2 for action
		*/
		L1_AI_indexOfAction = (int)FMath::FRandRange(1, 30);
		int t = 0;
		// do movement
		if (L1_AI_indexOfAction % 7 == 0) {
			t = (int)FMath::FRandRange(1, 30);
			if (t % 4 == 0) {
				MovingTowardsPLayer = false;
			}
			else {
				MovingTowardsPLayer = true;
			}
			L1_AI_indexOfAction = 1;
		}
		//go to sleep
		else if (L1_AI_indexOfAction % 8 == 0) {
			sleepTime = 0;
			sleepTotime = FMath::FRandRange(1, 50);
			L1_AI_indexOfAction = 2;
		}
		//hit bro hit
		else {
			L1_AI_indexOfAction = 5;
		}

	}


	// do the role
	if (L1_AI_indexOfAction == 1) {
		if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams)) {
			int t = 60;
			t = (int)FMath::FRandRange(1, 30);

			if (MovingTowardsPLayer) {
				if (OutHit.Actor.Get()->GetActorLocation().X - Start.X >= 100) {
					if (t % 22 != 0)
						MoveLeftRight(-1);
					else {
						if ((int)(FMath::FRandRange(1, 10)) % 8 == 0) {
							L1_AI_indexOfAction = 0;
						}
						else {
							sleepTime = 0;
							sleepTotime = FMath::FRandRange(1, 50);
							L1_AI_indexOfAction = 2;
						}
					}

				}
				else {
					L1_AI_indexOfAction = 0;
				}
			}
			else {
				if (OutHit.Actor.Get()->GetActorLocation().X - Start.X <= 900) {
					if (t % 10 != 0)
						MoveLeftRight(1);
					else {
						if ((int)(FMath::FRandRange(1, 10)) % 8 == 0) {
							L1_AI_indexOfAction = 0;
						}
						else {
							sleepTime = 0;
							sleepTotime = FMath::FRandRange(1, 50);
							L1_AI_indexOfAction = 2;
						}
					}
				}
				else {
					L1_AI_indexOfAction = 0;
				}
			}




		}
		else {
			MoveLeftRight(-1);
		}
	}
	else {
		MoveLeftRight(0);
	}
	if (L1_AI_indexOfAction == 2) {
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
		L1_AI_indexOfAction = 0;
		if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams)) {
			if (OutHit.Actor.Get()->GetActorLocation().X - Start.X <= 200) {
				//--------------action Set=====================================
				int typeOfAttack = (int)FMath::FRandRange(1, 4);
				bPunchOn = false;
				bKickOn = false;
				bSpecial = false;

				if (typeOfAttack == 1) {
					bPunchOn = true;
				}
				else if (typeOfAttack == 2) {
					bKickOn = true;
				}
				else {
					bSpecial = true;
				}


				typeOfAttack = (int)FMath::FRandRange(1, 3);
				if (typeOfAttack == 1) {
					FirstAction();
				}
				else if (typeOfAttack == 2) {
					SecondAction();
				}


				//sleep time
				sleepTime = 0;
				sleepTotime = FMath::FRandRange(10, 40);
				L1_AI_indexOfAction = 2;

			}
			else {
				L1_AI_indexOfAction = 1;
				if ((int)(FMath::FRandRange(0, 100)) % 30 == 0) {
					sleepTime = 0;
					sleepTotime = FMath::FRandRange(1, 25);
					L1_AI_indexOfAction = 2;
				}
				else {
					if ((int)(FMath::FRandRange(0, 100)) % 20 == 0) {
						MovingTowardsPLayer = false;
					}
					else {
						MovingTowardsPLayer = true;
					}
				}
			}
		}
		else {
			L1_AI_indexOfAction = 1;
			MovingTowardsPLayer = false;
		}
	}

}