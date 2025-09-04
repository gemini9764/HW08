#include "Game/NBGGameModeBase.h"

#include "EngineUtils.h"
#include "NBGGameStateBase.h"
#include "Player/NBGPlayerController.h"
#include "Player/NBGPlayerState.h"

void ANBGGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	SecretNumberString = GenerateSecretNumber();
	UE_LOG(LogTemp, Error, TEXT("SecretNumberString: %s"), *SecretNumberString);
}

void ANBGGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	ANBGPlayerController* NBGPlayerController = Cast<ANBGPlayerController>(NewPlayer);
	if (IsValid(NBGPlayerController) == true)
	{
		NBGPlayerController->NotificationText = FText::FromString(TEXT("Connected to the game server"));
		
		AllPlayerControllers.Add(NBGPlayerController);

		ANBGPlayerState* NBGPS = NBGPlayerController->GetPlayerState<ANBGPlayerState>();
		if (IsValid(NBGPS) == true)
			NBGPS->PlayerNameString = TEXT("Player") + FString::FromInt(AllPlayerControllers.Num());

		ANBGGameStateBase* NBGGameStateBase = GetGameState<ANBGGameStateBase>();
		if (IsValid(NBGGameStateBase) == true)
			NBGGameStateBase->MulticastRPCBroadcastLoginMessage(NBGPS->PlayerNameString);
	}
}

void ANBGGameModeBase::PrintChatMessageString(ANBGPlayerController* InChattingPlayerController,
	const FString& InChatMessageString)
{
	int Index = InChatMessageString.Len() - 3;
	FString GuessNumberString = InChatMessageString.RightChop(Index);
	if (IsGuessNumberString(GuessNumberString) == true)
	{
		FString JudgeResultString = JudgeResult(SecretNumberString, GuessNumberString);
		IncreaseGuessCount(InChattingPlayerController);
		
		for (TActorIterator<ANBGPlayerController> It(GetWorld()); It; ++It)
		{
			ANBGPlayerController* NBGPlayerController = *It;
			if (IsValid(NBGPlayerController) == true)
			{
				FString CombinedMessageString = InChatMessageString + TEXT(" -> ") + JudgeResultString;
				NBGPlayerController->ClientRPCPrintChatMessageString(CombinedMessageString);

				int32 StrikeCount = FCString::Atoi(*JudgeResultString.Left(1));
				JudgeGame(InChattingPlayerController, StrikeCount);
			}
		}
	}
	else
	{
		for (TActorIterator<ANBGPlayerController> It(GetWorld()); It; ++It)
		{
			ANBGPlayerController* NBGPlayerController = *It;
			if (IsValid(NBGPlayerController) == true)
				NBGPlayerController->ClientRPCPrintChatMessageString(InChatMessageString);
		}
	}
}

void ANBGGameModeBase::IncreaseGuessCount(ANBGPlayerController* InChattingPlayerController)
{
	ANBGPlayerState* NBGPS = InChattingPlayerController->GetPlayerState<ANBGPlayerState>();
	if (IsValid(NBGPS) == true)
		NBGPS->CurrentGuessCount++;
}

FString ANBGGameModeBase::GenerateSecretNumber()
{
	TArray<int32> Numbers;
	for (int32 i = 1; i <= 9; ++i)
		Numbers.Add(i);

	FMath::RandInit(FDateTime::Now().GetTicks());
	Numbers = Numbers.FilterByPredicate([](int32 Num) { return Num > 0; });

	FString Result;
	for (int32 i = 0; i < 3; ++i)
	{
		int32 Index = FMath::RandRange(0, Numbers.Num() - 1);
		Result.Append(FString::FromInt(Numbers[Index]));
		Numbers.RemoveAt(Index);
	}

	return Result;
}

bool ANBGGameModeBase::IsGuessNumberString(const FString& InNumberString)
{
	bool bCanPlay = false;

	do
	{
		if (InNumberString.Len() != 3)
			break;

		bool bIsUnique = true;
		TSet<TCHAR> UniqueDigits;
		for (TCHAR C : InNumberString)
		{
			if (FChar::IsDigit(C) == false || C == '0')
			{
				bIsUnique = false;
				break;
			}

			UniqueDigits.Add(C);
		}

		if (bIsUnique == false)
			break;

		bCanPlay = true;
	}while (false);

	return bCanPlay;
}

FString ANBGGameModeBase::JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString)
{
	int32 StrikeCount = 0, BallCount = 0;

	for (int32 i = 0; i < 3; ++i)
	{
		if (InSecretNumberString[i] == InGuessNumberString[i])
			StrikeCount++;
		else
		{
			FString PlayerGuessChar = FString::Printf(TEXT("%c"), InGuessNumberString[i]);
			if (InSecretNumberString.Contains(PlayerGuessChar))
				BallCount++;
		}
	}

	if (StrikeCount == 0 && BallCount == 0)
		return TEXT("OUT");

	return FString::Printf(TEXT("%dS%dB"), StrikeCount, BallCount);
}

void ANBGGameModeBase::ResetGame()
{
	SecretNumberString = GenerateSecretNumber();

	for (const auto& NBGPlayerController : AllPlayerControllers)
	{
		ANBGPlayerState* NBGPS = NBGPlayerController->GetPlayerState<ANBGPlayerState>();
		if (IsValid(NBGPS) == true)
			NBGPS->CurrentGuessCount = 0;
	}
}

void ANBGGameModeBase::JudgeGame(ANBGPlayerController* InChattingPlayerController, int InStrikeCount)
{
	if (3 == InStrikeCount)
	{
		ANBGPlayerState* NBGPS = InChattingPlayerController->GetPlayerState<ANBGPlayerState>();
		for (const auto& NBGPlayerController : AllPlayerControllers)
		{
			if (IsValid(NBGPS) == true)
			{
				FString CombineMessageString = NBGPS->PlayerNameString + TEXT(" has won the game");
				NBGPlayerController->NotificationText = FText::FromString(CombineMessageString);

				ResetGame();
			}
		}
	}
	else
	{
		bool bIsDraw = true;
		for (const auto& NBGPlayerController : AllPlayerControllers)
		{
			ANBGPlayerState* NBGPS = NBGPlayerController->GetPlayerState<ANBGPlayerState>();
			if (IsValid(NBGPS) == true)
			{
				if (NBGPS->CurrentGuessCount < NBGPS->MaxGuessCount)
				{
					bIsDraw = false;
					break;
				}
			}
		}
		
		if (bIsDraw == true)
		{
			for (const auto& NBGPlayerController : AllPlayerControllers)
			{
				NBGPlayerController->NotificationText = FText::FromString(TEXT("Draw..."));
				ResetGame();
			}
		}
	}
}
