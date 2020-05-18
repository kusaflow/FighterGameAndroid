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

	if (attackedChar) {
		attackedChar->Health -= 100;
	}

}

