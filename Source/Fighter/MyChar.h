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
	int CharNumberIndex = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameInstance")
	class UkusaGameInstance* gameInstance;

	//move flag : it is used so that when an action is pressed then movement will be stopped and start when anim is ended
	UPROPERTY(BlueprintReadOnly, Category = "kusaActions")
	bool bActionInMOtion = true;


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
	UFUNCTION()
	void MoveLeftRight(float val);

	//PreJumpMOveBefore Action
	UFUNCTION()
	void PreActionMove();

};
