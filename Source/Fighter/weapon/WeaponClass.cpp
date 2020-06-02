// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponClass.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "../MyChar.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
AWeaponClass::AWeaponClass()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	RootComponent = mesh;

	hitArea = CreateDefaultSubobject<UBoxComponent>(TEXT("hitArea"));
	hitArea->SetupAttachment(GetRootComponent());

	hitArea->OnComponentBeginOverlap.AddDynamic(this, &AWeaponClass ::BeginOverlap);


}

// Called when the game starts or when spawned
void AWeaponClass::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeaponClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponClass::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{
	AMyChar* attackedChar = Cast<AMyChar>(OtherActor);
	AMyChar* parent = Cast<AMyChar>(GetAttachParentActor());
	if (parent->bCanDoDamage == true) {
		if (attackedChar && parent) {
			if (attackedChar->bisEnemy != parent->bisEnemy) {
				//attackedChar->Health -= 10;

				attackedChar->bGotHit = true;


				//reaction System
				//if Parent is Axe Man
				if (attackedChar->CharNumberIndex == 1) {
					attackedChar->ReactionIndex = (int)FMath::FRandRange(1, 5);
				}
				else if (attackedChar->CharNumberIndex == 2) {
					attackedChar->ReactionIndex = (int)FMath::FRandRange(1, 4);
				}
				else if (attackedChar->CharNumberIndex == 3) {
					attackedChar->ReactionIndex = (int)FMath::FRandRange(1, 4);
				}

				//-==-====------------------------==================================================================================


				//Damage System----------------------------------------------------------------------------------------------------
				
				//damage amount resolver
				if (parent->CharNumberIndex == 1 || parent->CharNumberIndex == 2 || parent->CharNumberIndex == 3) {
					if (parent->PrevActionType_P_K_S == 1) {
						if (parent->bActionPressed1) {
							attackedChar->Health -= 12;
						}
						else {
							attackedChar->Health -= 22;
						}
					}
					else if (parent->PrevActionType_P_K_S == 2) {
						if (parent->bActionPressed1) {
							attackedChar->Health -= 20;
						}
						else {
							attackedChar->Health -= 35;
						}
					}
					else {
						attackedChar->Health -= 8;
					}

				}
				
				
				
				
				//Damage Count Resolver
				if (parent->bCanDoDamage) {
					if (parent->CharNumberIndex == 1) {
						if (parent->PrevActionType_P_K_S == 1 || parent->PrevActionType_P_K_S == 2) {
							parent->bCanDoDamage = false;
						}
					}
				}
				//====================================================================================================================


				if (attackedChar->Health <= 0) {
					attackedChar->bisDead = true;
				}




			}
		}
	}

}

