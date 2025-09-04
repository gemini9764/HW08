#include "Player/NBGPlayerController.h"
#include "ChatX.h"
#include "UI/NBGChatInput.h"
#include "EngineUtils.h"
#include "NBGPlayerState.h"
#include "Game/NBGGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

ANBGPlayerController::ANBGPlayerController()
{
	bReplicates = true;
}

void ANBGPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController() == false)
		return;
	
	FInputModeUIOnly InputModeUIOnly;
	SetInputMode(InputModeUIOnly);

	if (IsValid(ChatInputWidgetClass) == true)
	{
		ChatInputWidgetInstance = CreateWidget<UNBGChatInput>(this, ChatInputWidgetClass);
		if (IsValid(ChatInputWidgetInstance) == true)
		{
			ChatInputWidgetInstance->AddToViewport();
		}
	}

	if (IsValid(NotificationTextWidgetClass) == true)
	{
		NotificationTextWidgetInstance = CreateWidget<UUserWidget>(this, NotificationTextWidgetClass);
		if (IsValid(NotificationTextWidgetInstance) == true)
			NotificationTextWidgetInstance->AddToViewport();
	}
}

void ANBGPlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, NotificationText);
}

void ANBGPlayerController::SetChatMessgeString(const FString& InChatMessageString)
{
	ChatMessageString = InChatMessageString;

	// PrintChatMessageString(ChatMessageString);
	if (IsLocalController() == true)
	{
		// ServerRPCPrintChatMessageString(InChatMessageString);

		ANBGPlayerState* NBGPS = GetPlayerState<ANBGPlayerState>();
		if (IsValid(NBGPS) == true)
		{
			FString CombineMessageString = NBGPS->GetPlayerInfoString() + TEXT(": ") + InChatMessageString;
			ServerRPCPrintChatMessageString(CombineMessageString);
		}
	}
}

void ANBGPlayerController::ServerRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	// for (TActorIterator<ANBGPlayerController> It(GetWorld()); It; ++It)
	// {
	// 	ANBGPlayerController* CXPlayerController = *It;
	// 	if (IsValid(CXPlayerController) == true)
	// 		CXPlayerController->ClientRPCPrintChatMessageString(InChatMessageString);
	// }

	AGameModeBase* GM = UGameplayStatics::GetGameMode(this);
	if (IsValid(GM) == true)
	{
		ANBGGameModeBase* NBGGM = Cast<ANBGGameModeBase>(GM);
		if (IsValid(NBGGM) == true)
			NBGGM->PrintChatMessageString(this, InChatMessageString);
	}
}

void ANBGPlayerController::ClientRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	PrintChatMessageString(InChatMessageString);
}

void ANBGPlayerController::PrintChatMessageString(const FString& InChatMessageString)
{
	// UKismetSystemLibrary::PrintString(this, ChatMessageString, true, true, FLinearColor::Green, 5.0f);

	// FString NetModeString = ChatXFunctionLibaray::GetNetModeString(this);
	// FString CombineMessageString = FString::Printf(TEXT("%s : %s"), *NetModeString, *InChatMessageString);
	// ChatXFunctionLibaray::MyPrintString(this, CombineMessageString, 10.0f);

	ChatXFunctionLibaray::MyPrintString(this, InChatMessageString, 10.f);
}