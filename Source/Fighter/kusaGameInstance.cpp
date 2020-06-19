// Fill out your copyright notice in the Description page of Project Settings.


#include "kusaGameInstance.h"
#include "saveGame/kusaSaveGame.h"
#include "Kismet/GameplayStatics.h"

UkusaGameInstance::UkusaGameInstance() {
	//UkusaSaveGame* load = Cast<UkusaSaveGame>(UGameplayStatics::LoadGameFromSlot("kusa", 0));

	//if (load) {

	//	//load = Cast<UkusaSaveGame>(UGameplayStatics::LoadGameFromSlot("kusa", 0));

	//	xp = load->xp;
	//	bHave_axe = load->bHave_axe;
	//	bHave_sword = load->bHave_sword;
	//	bmat1 = load->bmat1;
	//	bmat2 = load->bmat2;
	//	bmat3 = load->bmat3;
	//	bmat4 = load->bmat4;
	//	bmat5 = load->bmat5;
	//}
}

void UkusaGameInstance::saveGame() {
	UkusaSaveGame* save = Cast<UkusaSaveGame>(UGameplayStatics::CreateSaveGameObject(UkusaGameInstance::StaticClass()));

	if (save) {
		save->xp = xp;
		save->bHave_axe = bHave_axe;
		save->bHave_sword = bHave_sword;
		save->bmat1 = bmat1;
		save->bmat2 = bmat2;
		save->bmat3 = bmat3;
		save->bmat4 = bmat4;
		save->bmat5 = bmat5;

		UGameplayStatics::SaveGameToSlot(save, "kusa", 0);
	}


}

