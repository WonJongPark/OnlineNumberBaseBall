// ONBPlayerController.cpp


#include "ONBPlayerController.h"

#include "ONBPlayerState.h"
#include "OnlineNumberBaseBall.h"
#include "Game/ONBGameModeBase.h"
#include "Kismet/GameplayStatics.h"
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

void AONBPlayerController::SetChatMessageString(const FString& InChatMessageString)
{
	ChatMessageString = InChatMessageString;
	
	if (IsLocalController())
	{
		AONBPlayerState* ONBPS = GetPlayerState<AONBPlayerState>();
		if (IsValid(ONBPS))
		{
			FString CombinedMessageString = ONBPS->PlayerNameString + TEXT(": ") + InChatMessageString;
			
			ServerRPCPrintChatMessageString(CombinedMessageString);
		}
	}
}

void AONBPlayerController::PrintChatMessageString(const FString& InChatMessageString)
{
	FString NetModeString = ONBFunctionLibrary::GetNetModeString(this);
	FString CombineMessageString = FString::Printf(TEXT("%s: %s"), *NetModeString, *InChatMessageString);
	ONBFunctionLibrary::MyPrintString(this, CombineMessageString, 10.0f);
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
