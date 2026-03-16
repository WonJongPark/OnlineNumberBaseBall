// ONBGameStateBase.cpp


#include "ONBGameStateBase.h"

#include "Kismet/GameplayStatics.h"
#include "Player/ONBPlayerController.h"

void AONBGameStateBase::MulticastRPCBroadcastLoginMessage_Implementation(const FString& InNameString)
{
	if (!HasAuthority())
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (IsValid(PC))
		{
			AONBPlayerController* ONBPC = Cast<AONBPlayerController>(PC);
			if (IsValid(ONBPC))
			{
				FString NotificationString = InNameString + TEXT(" has joined the game.");
				ONBPC->PrintChatMessageString(NotificationString);
			}
		}
	}
}
