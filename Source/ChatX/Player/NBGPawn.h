#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "NBGPawn.generated.h"

UCLASS()
class CHATX_API ANBGPawn : public APawn
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
};
