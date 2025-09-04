#include "UI/NBGChatInput.h"

#include "Components/EditableTextBox.h"
#include "Player/NBGPlayerController.h"

void UNBGChatInput::NativeConstruct()
{
	Super::NativeConstruct();

	if (EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted) == false)
	{
		EditableTextBox_ChatInput->OnTextCommitted.AddDynamic(this, &ThisClass::OnChatInputTextCommitted);
	}
}

void UNBGChatInput::NativeDestruct()
{
	Super::NativeDestruct();

	if (EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted) == true)
	{
		EditableTextBox_ChatInput->OnTextCommitted.RemoveDynamic(this, &ThisClass::OnChatInputTextCommitted);
	}
}

void UNBGChatInput::OnChatInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		APlayerController* OwningPlayerController = GetOwningPlayer();
		if (IsValid(OwningPlayerController) == true)
		{
			ANBGPlayerController* OwningCXPlayerController = Cast<ANBGPlayerController>(OwningPlayerController);
			if (IsValid(OwningPlayerController) == true)
			{
				OwningCXPlayerController->SetChatMessgeString(Text.ToString());

				EditableTextBox_ChatInput->SetText(FText());
			}
		}
	}
}
