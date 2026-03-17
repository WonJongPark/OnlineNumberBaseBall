// ONBPlayerController.cpp


#include "ONBPlayerController.h"

#include "Game/ONBGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "UI/ONBChatInput.h"

AONBPlayerController::AONBPlayerController()
{
	bReplicates = true;
}

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
	
	if (IsValid(NotificationTextWidgetClass))
	{
		NotificationTextWidgetInstance = CreateWidget<UUserWidget>(this, NotificationTextWidgetClass);
		if (IsValid(NotificationTextWidgetInstance))
		{
			NotificationTextWidgetInstance->AddToViewport();
		}
	}
}

void AONBPlayerController::SetChatMessageString(const FString& InChatMessageString)
{
	ChatMessageString = InChatMessageString;
	
	if (IsLocalController())
	{
		
		ServerRPCPrintChatMessageString(InChatMessageString);
	}
}

void AONBPlayerController::PrintChatMessageString(const FString& InChatMessageString)
{
	//ONBFunctionLibrary::MyPrintString(this, InChatMessageString, 10.0f);
	
	if (IsValid(ChatInputWidgetInstance))
	{
		ChatInputWidgetInstance->AddChatMessage(InChatMessageString);
	}
}

void AONBPlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, NotificationText);
}

void AONBPlayerController::ClientRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	PrintChatMessageString(InChatMessageString);
}

void AONBPlayerController::ServerRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	AGameModeBase* GM = UGameplayStatics::GetGameMode(this);
	if (IsValid(GM))
	{
		AONBGameModeBase* ONBGM = Cast<AONBGameModeBase>(GM);
		if (IsValid(ONBGM))
		{
			ONBGM->PrintChatMessageString(this, InChatMessageString);
		}
	}
}
