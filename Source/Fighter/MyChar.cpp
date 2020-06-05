// Fill out your copyright notice in the Description page of Project Settings.


#include "MyChar.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "kusaGameInstance.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "weapon/WeaponClass.h"
#include "Engine/World.h"
#include "Engine/EngineTypes.h"


// Sets default values
AMyChar::AMyChar()
{
 	PrimaryActorTick.bCanEverTick = true;

	//CharNumberIndex = 2;
	//create camera bomm
	//pull towards player when coloosion happens
	cameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	cameraBoom->SetupAttachment(GetRootComponent());
	cameraBoom->bDoCollisionTest = false;
	cameraBoom->TargetArmLength = 800;
	cameraBoom->SocketOffset = FVector(0.f, 0.f, 0.f);
	cameraBoom->SetRelativeRotation(FRotator(-20, -90, 0.f));
	//cameraBoom->RelativeRotation = FRotator(0.f, 0.f, 0.f);
	cameraBoom->bUsePawnControlRotation = false;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(cameraBoom, USpringArmComponent::SocketName);

	//gameInstance = Cast<UkusaGameInstance>(GetGameInstance());
	//CharNumberIndex = gameInstance->CharNumberIndex;


}

// Called when the game starts or when spawned
void AMyChar::BeginPlay()
{
	Super::BeginPlay();
	gameInstance = Cast<UkusaGameInstance>(GetGameInstance());
	CharNumberIndex = gameInstance->CharNumberIndex;

	if (CharNumberIndex == 1) {
		GetCharacterMovement()->MaxWalkSpeed = 218;
	}
	else if (CharNumberIndex == 2) {
		GetCharacterMovement()->MaxWalkSpeed = 218;
	}
	else if (CharNumberIndex == 3) {
		GetCharacterMovement()->MaxWalkSpeed = 218;
	}

	//setting health
	Health = 500;
	bisDead = false;

	bisEnemy = false;

	//turing character 
	GetRootComponent()->GetChildComponent(1)->SetRelativeRotation(FRotator(0, -90, 0));

	HitEnergy = gameInstance->MaxHitEnergy;
	HitEnergyShouldBe = HitEnergy;
	
	//SetCharNumber();
}


// Called every frame
void AMyChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	gameInstance = Cast<UkusaGameInstance>(GetGameInstance());

	//GetRootComponent()->SetRelativeRotation(FRotator(0, -180, 0));
	
	
	
	PreActionMove();

	InActionAnimaManager(DeltaTime);

	//damage check
	if (Health <= 0) {
		bisDead = true;
	}

	if (bCanDoDamage) {
		if (bAnim_ActionInMOtion) {
			bCanDoDamage = false;
		}
	}
	//health

	if (!bisEnemy) {
		gameInstance->PHealth = Health;
		gameInstance->HitEnergyPLayer = HitEnergy;

		if (gameInstance->EHealth <= 0 || gameInstance->PHealth <= 0) {
			gameInstance->bisGameOver = true;
			gameInstance->bWeWon = true;
			if (gameInstance->PHealth <= 0) {
				gameInstance->bWeWon = false;
			}
		}

	}

	//timer to increament health
	if (TimerToIncrementHealth >= 40) {
		TimerToIncrementHealth = 0;
		Health += 10;
		if (Health >= 500)
			Health = 500;
	}
	else {
		//TimerToIncrementHealth += 10 * DeltaTime;
	}


	 //timer to increment energy
	 HitEnergyShouldBe += 4 * DeltaTime;
		
	

	if (HitEnergyShouldBe < 0) {
		HitEnergyShouldBe = 0;
	}

	if (HitEnergyShouldBe >= 100)
		HitEnergyShouldBe = 100;

	if (HitEnergyShouldBe > HitEnergy) {

		HitEnergy += 4 * DeltaTime;
	}
	else if (HitEnergyShouldBe < HitEnergy) {
		HitEnergy -= 30 * DeltaTime;
	}
	else {

	}

	//stop motion when in animation
	if (bGotHit) 
		GetCharacterMovement()->MaxWalkSpeed = 0;


	//android Action Mapping
	if (bAndroid_Action_iterateTimer)
		Android_ActionTimingResolver += (25.0f * DeltaTime);


}

// Called to bind functionality to input
void AMyChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("punch", this, &AMyChar::punchOn);
	PlayerInputComponent->BindAxis("kick", this, &AMyChar::kickOn);
	PlayerInputComponent->BindAxis("special", this, &AMyChar::specialOn);
	PlayerInputComponent->BindAxis("RightLeftMovement", this, &AMyChar::MoveLeftRight);

	//testing
	PlayerInputComponent->BindAxis("Testing", this, &AMyChar::Testing);


	PlayerInputComponent->BindAction("first", IE_Pressed, this, &AMyChar::FirstAction);
	PlayerInputComponent->BindAction("second", IE_Pressed, this, &AMyChar::SecondAction);
	PlayerInputComponent->BindAction("testingAction", IE_Pressed, this, &AMyChar::TestAction);


	PlayerInputComponent->BindAction("first", IE_Released, this, &AMyChar::ActionButtonUp);
	PlayerInputComponent->BindAction("second", IE_Released, this, &AMyChar::ActionButtonUp);

}

void AMyChar::punchOn(float val) {
	if (val == 1) {
		bPunchOn = true;
		bKickOn = false;
		bSpecial = false;
	}
}

void AMyChar::kickOn(float val) {
	if (val == 1) {
		bKickOn = true;
		bSpecial = false;
		bPunchOn = false;
	}
}

void AMyChar::specialOn(float val) {
	if (val == 1) {
		bSpecial = true;
		bKickOn = false;
		bPunchOn = false;
	}
}

void AMyChar::MoveLeftRight(float val) {
	if (!bAnim_ActionInMOtion || bGotHit)
		return;

	//interopolation
	if (val == -1 || Andro_moveVal == -1) {
		if(RightLeftMoveVal >= -1){
			RightLeftMoveVal -= GetWorld()->GetDeltaSeconds() * 3;
		}
	}
	else if (val == 1 || Andro_moveVal == 1) {
		if (RightLeftMoveVal <= 1) {
			RightLeftMoveVal += GetWorld()->GetDeltaSeconds() * 3;
		}
	}
	else if (val == 0 || Andro_moveVal == 0) {
		if (RightLeftMoveVal > 0) {
			RightLeftMoveVal -= GetWorld()->GetDeltaSeconds() * 3;
		}
		else {
			RightLeftMoveVal += GetWorld()->GetDeltaSeconds() * 3;
		}
	}


	

	
	if (Controller != nullptr) {
		//GetRootComponent()->GetChildComponent(1)->SetWorldRotation(FRotator(0, -90, 0));


		 const FRotator Rotation = Controller->GetControlRotation();
		 const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

		 const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		 if (bEnemyIsOnRight) {
			 AddMovementInput(Direction, RightLeftMoveVal);
		 }
		 else {
			 AddMovementInput(Direction, RightLeftMoveVal*-1);
		 }
		
	}
}


void AMyChar::FirstAction() {
	if (!bAnim_ActionInMOtion || bGotHit)
		return;
	
	bActionPressed1 = true;

	float priceOfAttack = 0;

	if(bPunchOn)
	{
		PrevActionType_P_K_S = 1;
		priceOfAttack = 10;
		
	}else if (bKickOn)
	{
		PrevActionType_P_K_S = 2;
		priceOfAttack = 13;
	}else if (bSpecial)
	{
		PrevActionType_P_K_S = 3;
		priceOfAttack = 40;
	}
	else {
		return;
	}

	if (HitEnergyShouldBe - priceOfAttack < 0) {
		return;
	}
	else {
		HitEnergyShouldBe -= priceOfAttack;
	}

	actionIndex = GiveMeAction();

	UE_LOG(LogTemp, Warning, TEXT("0000000000000000000000000000000000000000000000000000000000000000000000000"));


	PrevAction = actionIndex;
	Anim_InActionMotionIndex = 1;
	bAnim_ActionInMOtion = false;
	bAnimInMotion = false;
}

void AMyChar::SecondAction() {
	if (!bAnim_ActionInMOtion || bGotHit)
		return;
	bActionPressed1 = false;
	
	float priceOfAttack = 0;


	if(bPunchOn)
	{
		PrevActionType_P_K_S = 1;
		priceOfAttack = 18;
	}else if (bKickOn)
	{
		PrevActionType_P_K_S = 2;
		priceOfAttack = 25;
	}else if (bSpecial)
	{
		PrevActionType_P_K_S = 3;
		priceOfAttack = 40;
	}
	else {
		return;
	}

	if (HitEnergyShouldBe - priceOfAttack < 0) {
		return;
	}
	else {
		HitEnergyShouldBe -= priceOfAttack;
	}

	actionIndex = GiveMeAction();

	PrevAction = actionIndex;
	Anim_InActionMotionIndex = 1;
	bAnim_ActionInMOtion = false;
	bAnimInMotion = false;
}

void AMyChar::ActionButtonUp() {
	//actionIndex = 0;
}


int AMyChar::GiveMeAction() {

	
	//to start damage the player
	bCanDoDamage = true;


	if(CharNumberIndex == 1)
	{
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
		else if (bSpecial){	
			return (int)FMath::FRandRange(1,4);
		
		}
		
	}
	//=====================================================================================================
	else if (CharNumberIndex == 2)
	{
		if (bPunchOn) {
			if (bActionPressed1) {
				return (int)FMath::FRandRange(1, 6);
			}
			else {
				return (int)FMath::FRandRange(1, 6);
			}
		}
		else if (bKickOn) {
			if (bActionPressed1) {
				return (int)FMath::FRandRange(1, 6);
			}
			else {
				return (int)FMath::FRandRange(1, 6);
			}
		}
		else if (bSpecial) {
			return (int)FMath::FRandRange(1, 6);

		}
	}
	//=====================================================================================================
	else if (CharNumberIndex == 3) {
		if (bPunchOn) {
			if (bActionPressed1) {
				return (int)FMath::FRandRange(1, 6);
			}
			else {
				return (int)FMath::FRandRange(1, 6);
			}
		}
		else if (bKickOn) {
			if (bActionPressed1) {
				return (int)FMath::FRandRange(1, 8);
			}
			else {
				return (int)FMath::FRandRange(1, 8);
			}
		}
		else if (bSpecial) {
			return (int)FMath::FRandRange(1, 8);

		}
		return TempRet;
	}
	return 0;
}


void AMyChar::TestAction(){
	actionIndex = 22;
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

void AMyChar::InActionAnimaManager(float dt)
{
	int dirFactor = 1;
	if (!bEnemyIsOnRight) {
		dirFactor = -1;
		//UE_LOG(LogTemp, Warning, TEXT("################################################################################3"));
	}

	//Character 1
	if (CharNumberIndex == 1) {
		//UE_LOG(LogTemp, Warning, TEXT("################################################################################3"));
		if (PrevActionType_P_K_S == 1) {
			if (bActionPressed1) {
				if (PrevAction == 1 || PrevAction == 2 || PrevAction == 3
					|| PrevAction == 4 || PrevAction == 5 || PrevAction == 6) {
					if (Anim_InActionMotionIndex == 1 && bAnimInMotion)
					{
						GetCharacterMovement()->Velocity.X = -2000 * dirFactor;
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
					else if (Anim_InActionMotionIndex == 2 && bAnimInMotion)
					{
						GetCharacterMovement()->Velocity.X = 0 * dirFactor;
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
					else if (Anim_InActionMotionIndex == 3 && bAnimInMotion)
					{
						GetCharacterMovement()->Velocity.X = 1500 * dirFactor;
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
					else if (Anim_InActionMotionIndex == 4 && bAnimInMotion)
					{
						GetCharacterMovement()->Velocity.X = 0 * dirFactor;
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
				}

			}
			else {
				if (PrevAction == 1 || PrevAction == 2 || PrevAction == 3
					|| PrevAction == 4 || PrevAction == 5 || PrevAction == 6) {
					if (Anim_InActionMotionIndex == 1 && bAnimInMotion)
					{
						GetCharacterMovement()->Velocity.X = -2000 * dirFactor;
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
					else if (Anim_InActionMotionIndex == 2 && bAnimInMotion)
					{
						GetCharacterMovement()->Velocity.X = 0 * dirFactor;
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
					else if (Anim_InActionMotionIndex == 3 && bAnimInMotion)
					{
						GetCharacterMovement()->Velocity.X = 1500 * dirFactor;
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
					else if (Anim_InActionMotionIndex == 4 && bAnimInMotion)
					{
						GetCharacterMovement()->Velocity.X = 0 * dirFactor;
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
				}
			}
		}
		//kick Actions
		else if (PrevActionType_P_K_S == 2) {
			//Action 2
			if (!bActionPressed1) {
				if (PrevAction == 1) {
					if (Anim_InActionMotionIndex == 1 && bAnimInMotion)
					{
						GetCharacterMovement()->JumpZVelocity = 100;
						Jump();
						GetCharacterMovement()->Velocity.X = -500 * dirFactor;
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
					else if (Anim_InActionMotionIndex == 2 && bAnimInMotion)
					{
						GetCharacterMovement()->Velocity.X = 0 * dirFactor;
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}

				}
				else if (PrevAction == 2) {
					if (Anim_InActionMotionIndex == 1 && bAnimInMotion)
					{
						GetCharacterMovement()->JumpZVelocity = 300;
						Jump();
						GetCharacterMovement()->Velocity.X = -500 * dirFactor;
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
					else if (Anim_InActionMotionIndex == 2 && bAnimInMotion)
					{
						GetCharacterMovement()->Velocity.X = 0 * dirFactor;
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}

				}
				else if (PrevAction == 4)
				{
					//UE_LOG(LogTemp, Warning, TEXT("################################################################################3"));
					//Action for action 4 ,kicking, Action_2
					if (Anim_InActionMotionIndex == 1 && bAnimInMotion)
					{
						GetCharacterMovement()->JumpZVelocity = 200;
						Jump();
						GetCharacterMovement()->Velocity.X = 400 * dirFactor;
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
				}
				//---------------------------------------------------------------------------------------------------------
				else if (PrevAction == 5)
				{
					//Action for action 5 ,kicking, Action_2	
					if (Anim_InActionMotionIndex == 1 && bAnimInMotion)
					{
						GetCharacterMovement()->JumpZVelocity = 200;
						Jump();
						GetCharacterMovement()->Velocity.X = -400 * dirFactor;
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
				}
				//-------------------------------------------------------------------------------------------------------------------
				else if (PrevAction == 6)
				{
					//Action for action 6 ,kicking, Action_2	
					if (Anim_InActionMotionIndex == 1)
					{
						GetCharacterMovement()->Velocity.X = -800 * dirFactor;
						if (bAnimInMotion)
						{
							GetCharacterMovement()->JumpZVelocity = 300;
							Jump();
							GetCharacterMovement()->Velocity.X = -400 * dirFactor;
							Anim_InActionMotionIndex++;
							bAnimInMotion = false;
						}
					}
					else if (Anim_InActionMotionIndex == 2 && bAnimInMotion)
					{
						GetCharacterMovement()->Velocity.X = 0;
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
				}//---------------------------------------------------------------------------------------------------------
				else if (PrevAction == 7)
				{
					//Action for action 7 ,kicking, Action_2	
					if (Anim_InActionMotionIndex == 1 && bAnimInMotion)
					{
						GetCharacterMovement()->JumpZVelocity = 200;
						Jump();
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
				}
				//---------------------------------------------------------------------------------------------------------
				else if (PrevAction == 8)
				{
					//Action for action 8 ,kicking, Action_2	
					if (Anim_InActionMotionIndex == 1 && bAnimInMotion)
					{
						GetCharacterMovement()->JumpZVelocity = 200;
						Jump();
						GetCharacterMovement()->Velocity.X = -400 * dirFactor;
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
				}
				else if (PrevAction == 9 || PrevAction == 10)
				{
					//Action for action 8 ,kicking, Action_2	
					if (Anim_InActionMotionIndex == 1 && bAnimInMotion)
					{
						GetCharacterMovement()->Velocity.X = -1400 * dirFactor;
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
					else if (Anim_InActionMotionIndex == 2 && bAnimInMotion) {
						GetCharacterMovement()->Velocity.X = 0 * dirFactor;
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
				}

			}
			else {
				if (PrevAction == 1 || PrevAction == 3 || PrevAction == 4 || PrevAction == 5
					|| PrevAction == 6 || PrevAction == 7 || PrevAction == 8 || PrevAction == 9) {
					if (Anim_InActionMotionIndex == 1 && bAnimInMotion)
					{
						GetCharacterMovement()->Velocity.X = -1500 * dirFactor;
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
					else if (Anim_InActionMotionIndex == 2 && bAnimInMotion)
					{
						GetCharacterMovement()->Velocity.X = 0 * dirFactor;
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
					else if (Anim_InActionMotionIndex == 3 && bAnimInMotion)
					{
						GetCharacterMovement()->Velocity.X = 1000 * dirFactor;
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
					else if (Anim_InActionMotionIndex == 4 && bAnimInMotion)
					{
						GetCharacterMovement()->Velocity.X = 0 * dirFactor;
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
				}
				else if (PrevAction == 2) {
					if (Anim_InActionMotionIndex == 1 && bAnimInMotion)
					{
						GetCharacterMovement()->Velocity.X = 1000 * dirFactor;
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
					else if (Anim_InActionMotionIndex == 2 && bAnimInMotion)
					{
						GetCharacterMovement()->Velocity.X = 0 * dirFactor;
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
					else if (Anim_InActionMotionIndex == 3 && bAnimInMotion)
					{
						GetCharacterMovement()->Velocity.X = -2000 * dirFactor;
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
					else if (Anim_InActionMotionIndex == 4 && bAnimInMotion)
					{
						GetCharacterMovement()->Velocity.X = 0 * dirFactor;
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
				}

			}

		}
		else if (PrevActionType_P_K_S == 3)
		{
			//------------------------------------------------------------------------------------------------------------
			if (PrevAction == 1)
			{
				//Action for action 1 ,Special
				if (Anim_InActionMotionIndex == 1 && bAnimInMotion)
				{
					GetCharacterMovement()->JumpZVelocity = 300;
					Jump();
					GetCharacterMovement()->Velocity.X = -400 * dirFactor;
					Anim_InActionMotionIndex++;
					bAnimInMotion = false;
				}
			}
			//----------------------------------------------------------------------------------------------------------------
			else if (PrevAction == 2)
			{
				//Action for action 2 ,Special
				if (Anim_InActionMotionIndex == 1 && bAnimInMotion)
				{
					Anim_InActionMotionIndex++;
					bAnimInMotion = false;
				}
				else if (Anim_InActionMotionIndex == 2)
				{
					GetCharacterMovement()->Velocity.X = -400 * dirFactor;
					if (bAnimInMotion)
					{
						GetCharacterMovement()->JumpZVelocity = 300;
						Jump();
						GetCharacterMovement()->Velocity.X = -400 * dirFactor;
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
				}
			}
			//------------------------------------------------------------------------------------------------------------
			else if (PrevAction == 4)
			{
				//Action for action 4 ,Special
				if (Anim_InActionMotionIndex == 1)
				{
					GetCharacterMovement()->Velocity.X = -300 * dirFactor;
					if (bAnimInMotion)
					{
						GetCharacterMovement()->Velocity.X = 0;
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
				}
				else if (Anim_InActionMotionIndex == 2)
				{
					if (bAnimInMotion)
					{
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
				}
				else if (Anim_InActionMotionIndex == 3)
				{
					GetCharacterMovement()->Velocity.X = 300 * dirFactor;
					if (bAnimInMotion)
					{
						GetCharacterMovement()->Velocity.X = 0;
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
				}

			}
		}
	}
	//=====================================================================2====================================================
	else if (CharNumberIndex == 2)
	{
		if (PrevActionType_P_K_S == 1)
		{
			if (bActionPressed1)
			{
				if (PrevAction == 1 || PrevAction == 2 || PrevAction == 4)
				{
					//Action for action 5 ,Limb, Action_1	
					if (Anim_InActionMotionIndex == 1 && bAnimInMotion)
					{
						GetCharacterMovement()->Velocity.X = -1300 * dirFactor;
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
					else if (Anim_InActionMotionIndex == 2 && bAnimInMotion)
					{
						GetCharacterMovement()->Velocity.X = 0 * dirFactor;
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
				}
				else if (PrevAction == 3) {
					if (Anim_InActionMotionIndex == 1 && bAnimInMotion)
					{
						GetCharacterMovement()->JumpZVelocity = 50;
						Jump();
						GetCharacterMovement()->Velocity.X = -500 * dirFactor;
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
					else if (Anim_InActionMotionIndex == 2 && bAnimInMotion)
					{
						GetCharacterMovement()->Velocity.X = 0 * dirFactor;
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
				}
			}
			else {
				if (PrevAction == 1 || PrevAction == 2 || PrevAction == 3
					|| PrevAction == 4 || PrevAction == 5) {
					if (Anim_InActionMotionIndex == 1 && bAnimInMotion)
					{
						GetCharacterMovement()->Velocity.X = -2000 * dirFactor;
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
					else if (Anim_InActionMotionIndex == 2 && bAnimInMotion)
					{
						GetCharacterMovement()->Velocity.X = 0 * dirFactor;
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
				}
			}
		}
		else if (PrevActionType_P_K_S == 2) {
			if (!bActionPressed1) {
				if (PrevAction == 3) {
					if (Anim_InActionMotionIndex == 1)
					{
						GetCharacterMovement()->MaxWalkSpeed = 218;
						MoveForward();
						if (bAnimInMotion) {
							GetCharacterMovement()->MaxWalkSpeed = 218;
							Anim_InActionMotionIndex++;
							bAnimInMotion = false;
						}
					}
				}
				else if (PrevAction == 4)
				{
					//Action for action 4 ,Limb, !Action_1	
					if (Anim_InActionMotionIndex == 1 && bAnimInMotion)
					{
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
					else if (Anim_InActionMotionIndex == 2) {
						GetCharacterMovement()->Velocity.X = -300 * dirFactor;

						if (bAnimInMotion) {
							GetCharacterMovement()->Velocity.X = 0;
							Anim_InActionMotionIndex++;
							bAnimInMotion = false;
						}
					}
				}
				else if (PrevAction == 5)
				{
					//Action for action 5 ,Limb, !Action_1	
					if (Anim_InActionMotionIndex == 1 && bAnimInMotion)
					{
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
					else if (Anim_InActionMotionIndex == 2) {
						GetCharacterMovement()->Velocity.X = -100 * dirFactor;

						if (bAnimInMotion) {
							GetCharacterMovement()->Velocity.X = 0;
							Anim_InActionMotionIndex++;
							bAnimInMotion = false;
						}
					}
				}
			}
		}
		else if (PrevActionType_P_K_S == 3) {

			if (PrevAction == 1)
			{
				//Action for action 1 ,Limb, !Action_1	
				if (Anim_InActionMotionIndex == 1 && bAnimInMotion)
				{
					Anim_InActionMotionIndex++;
					bAnimInMotion = false;
				}
				else if (Anim_InActionMotionIndex == 2) {
					GetCharacterMovement()->Velocity.X = -100 * dirFactor;

					if (bAnimInMotion) {
						GetCharacterMovement()->Velocity.X = 0;
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
				}
			}
			else if (PrevAction == 2)
			{
				//Action for action 2 ,Limb, !Action_1	
				if (Anim_InActionMotionIndex == 1 && bAnimInMotion)
				{
					Anim_InActionMotionIndex++;
					bAnimInMotion = false;
				}
				else if (Anim_InActionMotionIndex == 2) {
					GetCharacterMovement()->Velocity.X = -150 * dirFactor;

					if (bAnimInMotion) {
						GetCharacterMovement()->Velocity.X = 0;
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
				}
				else if (Anim_InActionMotionIndex == 3 && bAnimInMotion)
				{
					Anim_InActionMotionIndex++;
					bAnimInMotion = false;
				}
				else if (Anim_InActionMotionIndex == 4) {
					GetCharacterMovement()->Velocity.X = -150 * dirFactor;

					if (bAnimInMotion) {
						GetCharacterMovement()->Velocity.X = 0;
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
				}
			}
			else if (PrevAction == 4)
			{
				//Action for action 1 ,Limb, !Action_1	

				if (Anim_InActionMotionIndex == 1) {
					GetCharacterMovement()->Velocity.X = -300 * dirFactor;

					if (bAnimInMotion) {

						GetCharacterMovement()->JumpZVelocity = 500;
						Jump();
						GetCharacterMovement()->Velocity.X = -300 * dirFactor;
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
				}
				else if (Anim_InActionMotionIndex == 2 && bAnimInMotion) {
					GetCharacterMovement()->Velocity.X = 0;
					Anim_InActionMotionIndex++;
					bAnimInMotion = false;
				}
			}
			else if (PrevAction == 5)
			{
				//Action for action 1 ,Limb, !Action_1	

				if (Anim_InActionMotionIndex == 1) {
					GetCharacterMovement()->Velocity.X = -200 * dirFactor;

					if (bAnimInMotion) {

						GetCharacterMovement()->Velocity.X = 0;
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
				}

			}



		}
	}
	//==========================================================3================================================================
	else if (CharNumberIndex == 3) {
		if(PrevActionType_P_K_S == 1){
			if (bActionPressed1) {
				if (PrevAction == 1 || PrevAction == 2 || PrevAction == 3 
						|| PrevAction == 4 || PrevAction == 5) {
					if (Anim_InActionMotionIndex == 1 && bAnimInMotion)
					{
						GetCharacterMovement()->Velocity.X = -1500 * dirFactor;
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
					else if (Anim_InActionMotionIndex == 2 && bAnimInMotion)
					{
						GetCharacterMovement()->Velocity.X = 0 * dirFactor;
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
				}
			}
			else {
				if (PrevAction == 1 || PrevAction == 2 || PrevAction == 3
					|| PrevAction == 4 || PrevAction == 5 || PrevAction == 6) {
					if (Anim_InActionMotionIndex == 1 && bAnimInMotion)
					{
						GetCharacterMovement()->Velocity.X = -1500 * dirFactor;
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
					else if (Anim_InActionMotionIndex == 2 && bAnimInMotion)
					{
						GetCharacterMovement()->Velocity.X = 0 * dirFactor;
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
				}
			}
		}
		else if (PrevActionType_P_K_S == 2) {
			if (bActionPressed1) {
				if (PrevAction == 1) {
					//UE_LOG(LogTemp, Warning, TEXT("################################################################################3"));

					if (Anim_InActionMotionIndex == 1 && bAnimInMotion)
					{
						GetCharacterMovement()->JumpZVelocity = 100;
						Jump();
						GetCharacterMovement()->Velocity.X = -200 * dirFactor;
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
					else if (Anim_InActionMotionIndex == 2 && bAnimInMotion)
					{
						GetCharacterMovement()->JumpZVelocity = 100;
						Jump();
						GetCharacterMovement()->Velocity.X = -600 * dirFactor;
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
					else if (Anim_InActionMotionIndex == 3 && bAnimInMotion)
					{
						GetCharacterMovement()->Velocity.X = 0 * dirFactor;
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}

				}
				else if (PrevAction == 2) {
					if (Anim_InActionMotionIndex == 1 && bAnimInMotion)
					{
						GetCharacterMovement()->Velocity.X = -1400 * dirFactor;
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
					else if (Anim_InActionMotionIndex == 2 && bAnimInMotion)
					{
						GetCharacterMovement()->Velocity.X = 0 * dirFactor;
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
				}
			}
			else {
				if (PrevAction == 6) {
					if (Anim_InActionMotionIndex == 1 )
					{
						GetCharacterMovement()->MaxWalkSpeed = 218;
						MoveForward();
						if (bAnimInMotion) {
							GetCharacterMovement()->MaxWalkSpeed = 218;
							Anim_InActionMotionIndex++;
							bAnimInMotion = false;
						}
					}
				}
				else if (PrevAction == 6) {
					if (Anim_InActionMotionIndex == 1 && bAnimInMotion) {
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
					else if (Anim_InActionMotionIndex == 2)
					{
						GetCharacterMovement()->MaxWalkSpeed = 218;
						MoveForward();
						if (bAnimInMotion) {
							GetCharacterMovement()->MaxWalkSpeed = 218;
							Anim_InActionMotionIndex++;
							bAnimInMotion = false;
						}
					}
				}
			}
		}
		else if (PrevActionType_P_K_S == 3) {

			if (PrevAction == 1)
			{
				//Action for action 1 ,Limb, !Action_1	
				if (Anim_InActionMotionIndex == 1 && bAnimInMotion)
				{
					Anim_InActionMotionIndex++;
					bAnimInMotion = false;
				}
				else if (Anim_InActionMotionIndex == 2) {
					GetCharacterMovement()->Velocity.X = -100 * dirFactor;

					if (bAnimInMotion) {
						GetCharacterMovement()->Velocity.X = 0;
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
				}
			}
			else if (PrevAction == 2)
			{
				//Action for action 2 ,Limb, !Action_1	
				if (Anim_InActionMotionIndex == 1 && bAnimInMotion)
				{
					Anim_InActionMotionIndex++;
					bAnimInMotion = false;
				}
				else if (Anim_InActionMotionIndex == 2) {
					GetCharacterMovement()->Velocity.X = -150 * dirFactor;

					if (bAnimInMotion) {
						GetCharacterMovement()->Velocity.X = 0;
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
				}
				else if (Anim_InActionMotionIndex == 3 && bAnimInMotion)
				{
					Anim_InActionMotionIndex++;
					bAnimInMotion = false;
				}
				else if (Anim_InActionMotionIndex == 4) {
					GetCharacterMovement()->Velocity.X = -150 * dirFactor;

					if (bAnimInMotion) {
						GetCharacterMovement()->Velocity.X = 0;
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
				}
			}
			else if (PrevAction == 4)
			{
				//Action for action 1 ,Limb, !Action_1	

				if (Anim_InActionMotionIndex == 1) {
					GetCharacterMovement()->Velocity.X = -300 * dirFactor;

					if (bAnimInMotion) {

						GetCharacterMovement()->JumpZVelocity = 500;
						Jump();
						GetCharacterMovement()->Velocity.X = -300 * dirFactor;
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
				}
				else if (Anim_InActionMotionIndex == 2 && bAnimInMotion) {
					GetCharacterMovement()->Velocity.X = 0;
					Anim_InActionMotionIndex++;
					bAnimInMotion = false;
				}
			}
			else if (PrevAction == 5)
			{
				//Action for action 1 ,Limb, !Action_1	

				if (Anim_InActionMotionIndex == 1) {
					GetCharacterMovement()->Velocity.X = -200 * dirFactor;

					if (bAnimInMotion) {

						GetCharacterMovement()->Velocity.X = 0;
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
				}

			}
			else if (PrevAction == 6) {
				if (Anim_InActionMotionIndex == 1)
				{
					GetCharacterMovement()->MaxWalkSpeed = 218;
					MoveForward();
					if (bAnimInMotion) {
						GetCharacterMovement()->MaxWalkSpeed = 218;
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
				}
			}
			else if (PrevAction == 7) {
				if (Anim_InActionMotionIndex == 1)
				{
					GetCharacterMovement()->MaxWalkSpeed = 100;
					MoveForward();
					if (bAnimInMotion) {
						GetCharacterMovement()->MaxWalkSpeed = 218;
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
				}
			}
			else if (PrevAction == 8) {
				if (Anim_InActionMotionIndex == 1)
				{
					GetCharacterMovement()->MaxWalkSpeed = 100;
					MoveForward();
					if (bAnimInMotion) {
						GetCharacterMovement()->MaxWalkSpeed = 218;
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
				}
			}



		}
		//
	}
}

void AMyChar::MoveForward() {
	if (Controller != nullptr) {
		//GetRootComponent()->GetChildComponent(1)->SetWorldRotation(FRotator(0, -90, 0));


		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, 1);
	}
}

void AMyChar::Testing(float val)
{
	if (val == 1)
	{
		bEnemyIsOnRight = true;
	}else if (val == -1)
	{
		bEnemyIsOnRight = false;
	}
	
}


///android actions
void AMyChar::Android_Action_Pressed(float val) {
	bAndroid_Action_iterateTimer = true;
	Android_ActionTimingResolver = 0;
	bPunchOn = false;
	bKickOn = false;
	bSpecial = false;	
}


void AMyChar::Android_Action_Released(float val) {
	
	if (val == 1) {
		bPunchOn = true;
	}
	else if (val == 2) {
		bKickOn = true;
	}
	else if (val == 3) {
		bSpecial = true;
	}
	else {
		return;
	}

	if (Android_ActionTimingResolver >= 19.0f) {
		SecondAction();
	}
	else {
		
		FirstAction();
	}
	bAndroid_Action_iterateTimer = false;
	Android_ActionTimingResolver = 0;
	
}
