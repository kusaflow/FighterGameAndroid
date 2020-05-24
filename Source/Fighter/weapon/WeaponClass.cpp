// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponClass.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "../MyChar.h"

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
				attackedChar->Health -= 10;

				attackedChar->bGotHit = true;


				//if Parent is Axe Man
				if (parent->CharNumberIndex == 2) {
					if (parent->PrevActionType_P_K_S == 1) {
						if (parent->bActionPressed1) {
							if (parent->PrevAction == 1) {
								attackedChar->ReactionIndex = 1;
							}
							else if (parent->PrevAction == 2) {
								attackedChar->ReactionIndex = 1;
							}
							else if (parent->PrevAction == 3) {
								attackedChar->ReactionIndex = 2;
							}
							else if (parent->PrevAction == 4) {
								attackedChar->ReactionIndex = 4;
							}
							else if (parent->PrevAction == 5) {
								attackedChar->ReactionIndex = 3;
							}
						}
						else {
							if (parent->PrevAction == 1) {
								attackedChar->ReactionIndex = 4;
							}
							else if (parent->PrevAction == 2) {
								attackedChar->ReactionIndex = 1;
							}
							else if (parent->PrevAction == 3) {
								attackedChar->ReactionIndex = 3;
							}
							else if (parent->PrevAction == 4) {
								attackedChar->ReactionIndex = 3;
							}
							else if (parent->PrevAction == 5) {
								attackedChar->ReactionIndex = 1;
							}
						}
					}
					//===================================================================================================
					else if (parent->PrevActionType_P_K_S == 2) {
						if (parent->bActionPressed1) {
							if (parent->PrevAction == 1) {
								attackedChar->ReactionIndex = 1;
							}
							else if (parent->PrevAction == 2) {
								attackedChar->ReactionIndex = 1;
							}
							else if (parent->PrevAction == 3) {
								attackedChar->ReactionIndex = 2;
							}
							else if (parent->PrevAction == 4) {
								attackedChar->ReactionIndex = 1;
							}
							else if (parent->PrevAction == 5) {
								attackedChar->ReactionIndex = 1;
							}
						}
						else {
							if (parent->PrevAction == 1) {
								attackedChar->ReactionIndex = 3;
							}
							else if (parent->PrevAction == 2) {
								attackedChar->ReactionIndex = 2;
							}
							else if (parent->PrevAction == 3) {
								attackedChar->ReactionIndex = 1;
							}
							else if (parent->PrevAction == 4) {
								attackedChar->ReactionIndex = 1;
							}
							else if (parent->PrevAction == 5) {
								attackedChar->ReactionIndex = 3;
							}
						}
					}

					//===========================================================================





				}



				if (attackedChar->Health <= 0) {
					attackedChar->bisDead = true;
				}




			}
		}
	}

}

