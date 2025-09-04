#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NBGChatInput.generated.h"

class UEditableTextBox;

UCLASS()
class CHATX_API UNBGChatInput : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> EditableTextBox_ChatInput;

protected:
	UFUNCTION()
	void OnChatInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);
};
