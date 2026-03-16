// ONBGameModeBase.cpp


#include "ONBGameModeBase.h"

#include "ONBGameStateBase.h"

void AONBGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);
	
	AONBGameStateBase* ONBGameSateBase = GetGameState<AONBGameStateBase>();
	if (IsValid(ONBGameSateBase))
	{
		ONBGameSateBase->MulticastRPCBroadcastLoginMessage(TEXT("XXXXXXX"));
	}
}
