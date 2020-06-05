// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyChar.generated.h"

UCLASS()
class FIGHTER_API AMyChar : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyChar();

	/*camera behind the player*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* cameraBoom;

	/* main camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(BlueprintReadOnly, Category = "kusaActions")
	bool bPunchOn = false;
	
	UPROPERTY(BlueprintReadOnly, Category = "kusaActions")
	bool bKickOn = false;
	
	UPROPERTY(BlueprintReadOnly, Category = "kusaActions")
	bool bSpecial = false;

	UPROPERTY(BlueprintReadOnly, Category = "kusaActions")
	bool bActionPressed1 = false;

	UPROPERTY(BlueprintReadOnly, Category = "kusaActions")
	bool bEnemyIsOnRight = true;

	UPROPERTY(BlueprintReadOnly, Category = "kusaActions")
	float RightLeftMoveVal = 0;

	UPROPERTY(BlueprintReadOnly, Category = "kusaActions")
	int actionIndex = 0;

	UPROPERTY(BlueprintReadOnly, Category = "kusaActions")
	int CharNumberIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameInstance")
	class UkusaGameInstance* gameInstance;

	//move flag : it is used so that when an action is pressed then movement will be stopped and start when anim is ended
	UPROPERTY(BlueprintReadOnly, Category = "kusaActions")
	bool bAnim_ActionInMOtion = true;

	UPROPERTY(BlueprintReadWrite, Category = "kusaAction")
	bool bAnimInMotion = false;

	//can do damage
	UPROPERTY(BlueprintReadWrite, Category = "kusaAction")
	bool bCanDoDamage = false;

	
	UPROPERTY(BlueprintReadOnly, Category = "kusaActions")
	int PrevAction = 0, Anim_InActionMotionIndex = 0;

	UPROPERTY(BlueprintReadWrite, Category = "kusaAction")
	int TempRet = 0;


	UPROPERTY(BlueprintReadWrite, Category = "HkusaAction")
	float Health= 100;

	UPROPERTY(BlueprintReadWrite, Category = "HkusaAction")
	bool bisDead = false;

	UPROPERTY(BlueprintReadWrite, Category = "HkusaAction")
	bool bisEnemy = false;

	//reaction handle=======================================================================================
	UPROPERTY(BlueprintReadWrite, Category = "HkusaAction")
	bool bGotHit = false;

	UPROPERTY(BlueprintReadWrite, Category = "HkusaAction")
	int ReactionIndex = 0;




	//androidProperty
	//handled in ui
	UPROPERTY(BlueprintReadWrite, Category = "kusaAndroid")
	int Andro_moveVal = 0;


	UPROPERTY(BlueprintReadWrite, Category = "kusaAndroid")
	float Android_ActionTimingResolver = 0;

	bool bAndroid_Action_iterateTimer = false;

	
	//------------------------------------------------------------------------------------------------------
	//timer to increment health
	UPROPERTY(BlueprintReadWrite, Category = "kusaH")
	float TimerToIncrementHealth = 0;

	UPROPERTY(BlueprintReadWrite, Category = "kusaEnergy")
	float HitEnergy = 0;

	float HitEnergyShouldBe;


	//0 for nothing
	//1 for punch
	//2 for kick
	//3 for special
	short PrevActionType_P_K_S = 0;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UFUNCTION()// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void punchOn(float val);
	
	UFUNCTION()
	void kickOn(float val);
	
	UFUNCTION()
	void specialOn(float val);

	UFUNCTION()
	void FirstAction();

	UFUNCTION()
	void SecondAction();

	UFUNCTION()
	void ActionButtonUp();

	UFUNCTION()
	int GiveMeAction();

	UFUNCTION()
	void TestAction();

	//move left right
	UFUNCTION(BlueprintCallable, category = "kusaFnx")
	void MoveLeftRight(float val);

	//PreJumpMOveBefore Action
	UFUNCTION()
	void PreActionMove();

	UFUNCTION()
	void InActionAnimaManager(float dt);

	UFUNCTION()
	void Testing(float val);

	UFUNCTION()
	void MoveForward();


	UFUNCTION(BlueprintCallable, category = "kusaFnx")
	void Android_Action_Pressed(float val);

	UFUNCTION(BlueprintCallable, category = "kusaFnx")
	void Android_Action_Released(float val);

	
	//UFUNCTION(BlueprintCallable, Category = "kusaFun")
	//void SetCharNumber();


	
};
