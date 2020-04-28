// Fill out your copyright notice in the Description page of Project Settings.


#include "kusaAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "kusaGameInstance.h"

void UkusaAnimInstance::NativeInitializeAnimation() {
	if (Pawn == nullptr) {
		Pawn = TryGetPawnOwner();
	}
}


void UkusaAnimInstance::update() {
	if (Pawn == nullptr) {
		Pawn = TryGetPawnOwner();
	}
	if (Pawn) {
		gameInstance = Cast<UkusaGameInstance>(Pawn->GetGameInstance());
		punching = gameInstance->punching;
		actionIndex = gameInstance->actionIndex;
	}
}

