// Fill out your copyright notice in the Description page of Project Settings.


#include "kusaAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "kusaGameInstance.h"
#include "MyChar.h"

void UkusaAnimInstance::NativeInitializeAnimation() {
	
}


void UkusaAnimInstance::update() {

	player = Cast<AMyChar>(TryGetPawnOwner());

	if (player) {
		bPunching = player->bPunchOn;
		bActionPressed1 = player->bActionPressed1;
		actionIndex = player->actionIndex;
		CharNumberIndex = player->CharNumberIndex;
		bKickOn = player->bKickOn;
		bSpecial = player->bSpecial;
		EnemyIsOnRight = player->bEnemyIsOnRight;
		RightLeftMoveVal = player->RightLeftMoveVal;
		//isdead Check
		bisDead = player->bisDead;
		if (actionIndex != 0){
			bActionInMOtion = false;
			player->actionIndex = 0;
			player->bPunchOn = false;
			player->bKickOn = false;
			player->bSpecial = false;
		}
		if (bInAnimMotion)
		{
			player->bAnimInMotion = true;
			bInAnimMotion = false;
		}

		player->bAnim_ActionInMOtion = bActionInMOtion;

		//reaction
		ReactionIndex = player->ReactionIndex;

		if (ReactionIndex != 0) {
			bGotHit = true;
			bReaction_CanSetValueToMainPlayer = true;
			player->ReactionIndex = 0;
		}

		if (bReaction_CanSetValueToMainPlayer && !bGotHit) {
			player->bGotHit = false;
			bReaction_CanSetValueToMainPlayer = false;
		}
		
	}
	
	if (!EnemyIsOnRight) {
		RightLeftMoveVal *= -1;
	}
	RightLeftMoveVal *= 100;

}

