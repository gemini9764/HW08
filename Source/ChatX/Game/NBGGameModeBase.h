#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NBGGameModeBase.generated.h"

class ANBGPlayerController;

UCLASS()
class CHATX_API ANBGGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void OnPostLogin(AController* NewPlayer) override;
	
	FString GenerateSecretNumber();
	bool IsGuessNumberString(const FString& InNumberString);
	FString JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString);
	void PrintChatMessageString(ANBGPlayerController* InChattingPlayerController, const FString& InChatMessageString);
	void IncreaseGuessCount(ANBGPlayerController* InChattingPlayerController);
	void ResetGame();
	void JudgeGame(ANBGPlayerController* InChattingPlayerController, int InStrikeCount);
	
protected:
	FString SecretNumberString;
	TArray<TObjectPtr<ANBGPlayerController>> AllPlayerControllers;
};
