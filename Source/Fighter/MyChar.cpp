// Fill out your copyright notice in the Description page of Project Settings.


#include "MyChar.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "kusaGameInstance.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/CharacterMovementComponent.h"

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
	gameInstance = Cast<UkusaGameInstance>(GetGameInstance());
	CharNumberIndex = gameInstance->CharNumberIndex;

	if (CharNumberIndex == 1) {
		GetCharacterMovement()->MaxWalkSpeed = 145;
	}
	
}

// Called every frame
void AMyChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//gameInstance = Cast<UkusaGameInstance>(GetGameInstance());

	//turing character 
	if (bEnemyIsOnRight) {
		GetRootComponent()->GetChildComponent(1)->SetRelativeRotation(FRotator(0,-90,0));
	}
	else {
		GetRootComponent()->GetChildComponent(1)->SetRelativeRotation(FRotator(0, 90, 0));
	}
	
	PreActionMove();
	actionIndex = 0;
	

}

// Called to bind functionality to input
void AMyChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("punch", this, &AMyChar::punchOn);
	PlayerInputComponent->BindAxis("kick", this, &AMyChar::kickOn);
	PlayerInputComponent->BindAxis("special", this, &AMyChar::specialOn);
	PlayerInputComponent->BindAxis("RightLeftMovement", this, &AMyChar::MoveLeftRight);

	PlayerInputComponent->BindAction("first", IE_Pressed, this, &AMyChar::FirstAction);
	PlayerInputComponent->BindAction("second", IE_Pressed, this, &AMyChar::SecondAction);
	PlayerInputComponent->BindAction("testingAction", IE_Pressed, this, &AMyChar::TestAction);


	PlayerInputComponent->BindAction("first", IE_Released, this, &AMyChar::ActionButtonUp);
	PlayerInputComponent->BindAction("second", IE_Released, this, &AMyChar::ActionButtonUp);

}

void AMyChar::punchOn(float val) {
	if (val == 1)
		bPunchOn = true;
	else
		bPunchOn = false;
}

void AMyChar::kickOn(float val) {
	if (val == 1)
		bKickOn = true; 
	else
		bKickOn= false;
}

void AMyChar::specialOn(float val) {
	if (val == 1)
		bSpecial = true;
	else
		bSpecial = false;
}


void AMyChar::FirstAction() {
	if (!bActionInMOtion)
		return;
	bActionPressed1 = true;
	actionIndex = GiveMeAction();
	
}

void AMyChar::SecondAction() {
	if (!bActionInMOtion)
		return;
	bActionPressed1 = false;
	actionIndex = GiveMeAction();
	
}

void AMyChar::ActionButtonUp() {
	//actionIndex = 0;
}


int AMyChar::GiveMeAction() {
	if (bPunchOn) {
		if (bActionPressed1) {
			return (int)FMath::FRandRange(1,7);
		}
		else {
			return (int)FMath::FRandRange(1,7);
		}
	}
	else if (bKickOn) {
		if (bActionPressed1) {
			return (int)FMath::FRandRange(1, 11);
		}
		else {
			return (int)FMath::FRandRange(1, 11);
		}
	}
	else {
		if (bActionPressed1) {
			return (int)FMath::FRandRange(1,4);
		}
		else {
			return (int)FMath::FRandRange(1,4);
		}
	}
	return 0;
}


void AMyChar::TestAction(){
	actionIndex = 22;
}

void AMyChar::MoveLeftRight(float val) {
	if (!bActionInMOtion)
		return;
	RightLeftMoveVal = val;
	if (Controller != nullptr) {
		//GetRootComponent()->GetChildComponent(1)->SetWorldRotation(FRotator(0, -90, 0));

		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, val);
	}
}


void AMyChar::PreActionMove() {
	int dirFactor = 1;
	if (!bEnemyIsOnRight)
		dirFactor = -1;
	if (CharNumberIndex == 1) {
		if (actionIndex == 9 && bKickOn && bActionPressed1) {
			GetCharacterMovement()->JumpZVelocity = 150 * dirFactor;
			Jump();
			GetCharacterMovement()->Velocity.X = -200 * dirFactor;
		}
		else if (actionIndex == 2 && bKickOn && !bActionPressed1) {
			GetCharacterMovement()->JumpZVelocity = 300 * dirFactor;
			Jump();
			GetCharacterMovement()->Velocity.X = -400 * dirFactor;
		}
	}
}