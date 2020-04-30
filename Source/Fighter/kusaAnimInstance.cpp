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
	}
}

