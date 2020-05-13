// Fill out your copyright notice in the Description page of Project Settings.


#include "MyChar.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "kusaGameInstance.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Math/UnrealMathSSE.h"

// Sets default values
AMyChar::AMyChar()
{
 	PrimaryActorTick.bCanEverTick = true;

	CharNumberIndex = 2;
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

	InActionAnimaManager();

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
	if (!bAnim_ActionInMOtion)
		return;
	bActionPressed1 = true;
	actionIndex = GiveMeAction();

	if(bPunchOn)
	{
		PrevActionType_P_K_S = 1;
	}else if (bKickOn)
	{
		PrevActionType_P_K_S = 2;
	}else
	{
		PrevActionType_P_K_S = 3;
	}
	
	PrevAction = actionIndex;
	Anim_InActionMotionIndex = 1;
	bAnim_ActionInMOtion = false;	
}

void AMyChar::SecondAction() {
	if (!bAnim_ActionInMOtion)
		return;
	bActionPressed1 = false;
	actionIndex = GiveMeAction();

	if(bPunchOn)
	{
		PrevActionType_P_K_S = 1;
	}else if (bKickOn)
	{
		PrevActionType_P_K_S = 2;
	}else
	{
		PrevActionType_P_K_S = 3;
	}
	
	PrevAction = actionIndex;
	Anim_InActionMotionIndex = 1;
	bAnim_ActionInMOtion = false;
}

void AMyChar::ActionButtonUp() {
	//actionIndex = 0;
}


int AMyChar::GiveMeAction() {
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
	}else if (CharNumberIndex == 2)
	{
		return TempRet;
	}
	return 0;
}


void AMyChar::TestAction(){
	actionIndex = 22;
}

void AMyChar::MoveLeftRight(float val) {
	if (!bAnim_ActionInMOtion)
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

void AMyChar::InActionAnimaManager()
{
	int dirFactor = 1;
	if (!bEnemyIsOnRight){
		dirFactor = -1;
		//UE_LOG(LogTemp, Warning, TEXT("################################################################################3"));
	}
	
	//Character 1
	if (CharNumberIndex == 1){
		//kick Actions
		if (PrevActionType_P_K_S == 2){
			//Action 2
			if(!bActionPressed1){
				if (PrevAction == 4)
				{
					//UE_LOG(LogTemp, Warning, TEXT("################################################################################3"));
					//Action for action 4 ,kicking, Action_2
					if(Anim_InActionMotionIndex == 1 && bAnimInMotion)
					{
						GetCharacterMovement()->JumpZVelocity = 200;
						Jump();
						GetCharacterMovement()->Velocity.X = 400*dirFactor;
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					} 
				}
				//---------------------------------------------------------------------------------------------------------
				else if (PrevAction == 5)
				{
					//Action for action 5 ,kicking, Action_2	
					if(Anim_InActionMotionIndex == 1 && bAnimInMotion)
					{
						GetCharacterMovement()->JumpZVelocity = 200;
						Jump();
						GetCharacterMovement()->Velocity.X = -400*dirFactor;
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
						GetCharacterMovement()->Velocity.X = -800*dirFactor;
						if(bAnimInMotion)
						{
							GetCharacterMovement()->JumpZVelocity = 300;
							Jump();
							GetCharacterMovement()->Velocity.X = -400*dirFactor;
							Anim_InActionMotionIndex++;
							bAnimInMotion = false;
						}
					}else if (Anim_InActionMotionIndex == 2 && bAnimInMotion)
					{
						GetCharacterMovement()->Velocity.X = 0;
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
				}//---------------------------------------------------------------------------------------------------------
				else if (PrevAction == 7)
				{
					//Action for action 7 ,kicking, Action_2	
					if(Anim_InActionMotionIndex == 1 && bAnimInMotion)
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
                	if(Anim_InActionMotionIndex == 1 && bAnimInMotion)
                	{
                		GetCharacterMovement()->JumpZVelocity = 200;
                		Jump();
                		GetCharacterMovement()->Velocity.X = -400*dirFactor;
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
				if(Anim_InActionMotionIndex == 1 && bAnimInMotion)
				{
					GetCharacterMovement()->JumpZVelocity = 300;
					Jump();
					GetCharacterMovement()->Velocity.X = -400*dirFactor;
					Anim_InActionMotionIndex++;
					bAnimInMotion = false;
				} 
			}
			//----------------------------------------------------------------------------------------------------------------
			else if (PrevAction == 2)
			{
				//Action for action 2 ,Special
				if(Anim_InActionMotionIndex == 1 && bAnimInMotion)
				{
					Anim_InActionMotionIndex++;
					bAnimInMotion = false;
				}
				else if (Anim_InActionMotionIndex == 2)
				{
					GetCharacterMovement()->Velocity.X = -400*dirFactor;
					if (bAnimInMotion)
					{
						GetCharacterMovement()->JumpZVelocity = 300;
						Jump();
						GetCharacterMovement()->Velocity.X = -400*dirFactor;
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
				}
			} 
			//------------------------------------------------------------------------------------------------------------
			else if (PrevAction == 4)
			{
				//Action for action 4 ,Special
				if(Anim_InActionMotionIndex == 1)
				{
					GetCharacterMovement()->Velocity.X = -300*dirFactor;
					if (bAnimInMotion)
					{
						GetCharacterMovement()->Velocity.X = 0;
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
				}else if (Anim_InActionMotionIndex == 2)
				{
					if (bAnimInMotion)
					{
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
				}else if (Anim_InActionMotionIndex == 3)
				{
					GetCharacterMovement()->Velocity.X = 300*dirFactor;
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

	else if (CharNumberIndex == 2)
	{
		if (PrevActionType_P_K_S == 1)
		{
			if (bActionPressed1)
			{
				if (PrevAction == 5)
				{
					//Action for action 5 ,Limb, Action_1	
					if(Anim_InActionMotionIndex == 1 && bAnimInMotion)
					{
						GetCharacterMovement()->JumpZVelocity = 900;
						Jump();
						Anim_InActionMotionIndex++;
						bAnimInMotion = false;
					}
				}
			}
		}
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
 
