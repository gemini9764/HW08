#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NBGPlayerController.generated.h"

class UNBGChatInput;
class UUserWidget;

UCLASS()
class CHATX_API ANBGPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ANBGPlayerController();
	
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	void SetChatMessgeString(const FString& InChatMessageString);
	void PrintChatMessageString(const FString& InChatMessageString);
	
	UFUNCTION(Client, Reliable)
	void ClientRPCPrintChatMessageString(const FString& InChatMessageString);
	UFUNCTION(Server, Reliable)
	void ServerRPCPrintChatMessageString(const FString& InChatMessageString);

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UNBGChatInput> ChatInputWidgetClass;
	UPROPERTY()
	TObjectPtr<UNBGChatInput> ChatInputWidgetInstance;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> NotificationTextWidgetClass;
	UPROPERTY()
	TObjectPtr<UUserWidget> NotificationTextWidgetInstance;
	
	FString ChatMessageString;

public:
	UPROPERTY(Replicated, BlueprintReadOnly)
	FText NotificationText;
};
