// ONBPlayerController.cpp


#include "ONBPlayerController.h"

#include "OnlineNumberBaseBall.h"
#include "UI/ONBChatInput.h"

void AONBPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (!IsLocalController())
	{
		return;
	}
	
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
	FString NetModeString = ONBFunctionLibrary::GetNetModeString(this);
	FString CombineMessageString = FString::Printf(TEXT("%s: %s"), *NetModeString, *InChatMessageString);
	ONBFunctionLibrary::MyPrintString(this, CombineMessageString, 10.0f);
}
