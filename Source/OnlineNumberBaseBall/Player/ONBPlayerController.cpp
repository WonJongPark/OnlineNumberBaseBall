// ONBPlayerController.cpp


#include "ONBPlayerController.h"

#include "ONBPlayerState.h"
#include "OnlineNumberBaseBall.h"
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
		AONBPlayerState* ONBPS = GetPlayerState<AONBPlayerState>();
		if (IsValid(ONBPS))
		{
			FString CombinedMessageString = ONBPS->GetPlayerInfoString() + TEXT(": ") + InChatMessageString;
			
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
