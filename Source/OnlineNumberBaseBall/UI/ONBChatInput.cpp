// ONBChatInput.cpp


#include "ONBChatInput.h"

#include "Components/EditableTextBox.h"
#include "Player/ONBPlayerController.h"

void UONBChatInput::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (!EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted))
	{
		EditableTextBox_ChatInput->OnTextCommitted.AddDynamic(this, &ThisClass::OnChatInputTextCommitted);
	}
}

void UONBChatInput::NativeDestruct()
{
	Super::NativeDestruct();
	
	if (EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted))
	{
		EditableTextBox_ChatInput->OnTextCommitted.RemoveDynamic(this, &ThisClass::OnChatInputTextCommitted);
	}
}

void UONBChatInput::OnChatInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		APlayerController* OwningPlayerController = GetOwningPlayer();
		if (IsValid(OwningPlayerController))
		{
			AONBPlayerController* OwningONBPlayerController = Cast<AONBPlayerController>(OwningPlayerController);
			if (IsValid(OwningONBPlayerController))
			{
				OwningONBPlayerController->SetChatMessageString(Text.ToString());
				
				EditableTextBox_ChatInput->SetText(FText());
			}
		}
	}
}
