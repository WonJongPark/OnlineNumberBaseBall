// ONBPlayerController.cpp


#include "ONBPlayerController.h"

#include "UI/ONBChatInput.h"

void AONBPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	FInputModeUIOnly InputModeUIOnly;
	SetInputMode(InputModeUIOnly);
	
	if (IsValid(ChatInputWidgetClass))
	{
		ChatInputWidgetInstance = CreateWidget<UONBChatInput>(this, ChatInputWidgetClass);
		if (IsValid(ChatInputWidgetInstance))
		{
			ChatInputWidgetInstance->AddToViewport();
		}
	}
}

void AONBPlayerController::SetChatMessageString(const FString& IntChatMessageString)
{
	ChatMessageString = IntChatMessageString;
	
	PrintChatMessageString(ChatMessageString);
}

void AONBPlayerController::PrintChatMessageString(const FString& InChatMessageString)
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, *ChatMessageString);
}
