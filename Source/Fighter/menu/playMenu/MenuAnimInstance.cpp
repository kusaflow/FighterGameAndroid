// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuAnimInstance.h"
#include "Math/UnrealMathUtility.h"

void UMenuAnimInstance::NativeInitializeAnimation() {

}


void UMenuAnimInstance::update(float dt) {
	if (timeCounter >= 60.0f) {
		ChangeState = (int)FMath::FRandRange(1, 3);
		timeCounter = 0;
	}
	else {
		timeCounter += 10.0f * dt;
	}
}