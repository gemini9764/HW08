#include "Player/NBGPawn.h"

#include "ChatX.h"

void ANBGPawn::BeginPlay()
{
	Super::BeginPlay();

	FString NetRoleString = ChatXFunctionLibaray::GetNetModeString(this);
	FString CombinedString = FString::Printf(TEXT("HWPawn::BeginPlay() %s [%s]"), *ChatXFunctionLibaray::GetNetModeString(this), *NetRoleString);
	ChatXFunctionLibaray::MyPrintString(this, CombinedString, 10.0f);
}

void ANBGPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	FString NetRoleString = ChatXFunctionLibaray::GetNetModeString(this);
	FString CombinedString = FString::Printf(TEXT("HWPawn::PossessedBy() %s [%s]"), *ChatXFunctionLibaray::GetNetModeString(this), *NetRoleString);
	ChatXFunctionLibaray::MyPrintString(this, CombinedString, 10.0f);
}
