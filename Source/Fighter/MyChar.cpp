// Fill out your copyright notice in the Description page of Project Settings.


#include "MyChar.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "kusaGameInstance.h"

// Sets default values
AMyChar::AMyChar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//create camera bomm
	//pull towards player when coloosion happens
	cameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	cameraBoom->SetupAttachment(GetRootComponent());
	cameraBoom->bDoCollisionTest = false;
	cameraBoom->TargetArmLength = 800;
	cameraBoom->SocketOffset = FVector(0.f, 0.f, 0.f);
	cameraBoom->RelativeRotation = (FRotator(-20, -90, 0.f));
	//cameraBoom->RelativeRotation = FRotator(0.f, 0.f, 0.f);
	cameraBoom->bUsePawnControlRotation = false;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(cameraBoom, USpringArmComponent::SocketName);

}

// Called when the game starts or when spawned
void AMyChar::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	gameInstance = Cast<UkusaGameInstance>(GetGameInstance());



}

// Called to bind functionality to input
void AMyChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("punch", this, &AMyChar::punchOn);
	PlayerInputComponent->BindAxis("kick", this, &AMyChar::kickOn);
	PlayerInputComponent->BindAxis("special", this, &AMyChar::specialOn);

	PlayerInputComponent->BindAction("first", IE_Pressed, this, &AMyChar::FirstAction);
	PlayerInputComponent->BindAction("second", IE_Pressed, this, &AMyChar::SecondAction);
	PlayerInputComponent->BindAction("third", IE_Pressed, this, &AMyChar::ThirdAction);


}

void AMyChar::punchOn(float val) {
	if (val == 1)
		bPunchOn = true;
	else
		bPunchOn = false;
}

void AMyChar::kickOn(float val) {
	bKickOn = true;
}

void AMyChar::specialOn(float val) {
	bSpecial = true;
}


void AMyChar::FirstAction() {


}

void AMyChar::SecondAction() {


}

void AMyChar::ThirdAction() {


}