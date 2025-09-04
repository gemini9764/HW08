// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/NBGGameStateBase.h"

#include "Kismet/GameplayStatics.h"
#include "Player/NBGPlayerController.h"

void ANBGGameStateBase::MulticastRPCBroadcastLoginMessage_Implementation(const FString& InNameString)
{
	if (HasAuthority() == false)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (IsValid(PC) == true)
		{
			ANBGPlayerController* NBGPC = Cast<ANBGPlayerController>(PC);
			if (IsValid(NBGPC) == true)
			{
				FString NotificationString = InNameString + TEXT(" has joined the game.");
				NBGPC->PrintChatMessageString(NotificationString);
			}
		}
	}
}
