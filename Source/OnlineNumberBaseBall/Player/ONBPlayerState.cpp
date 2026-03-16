// ONBPlayerState.cpp


#include "ONBPlayerState.h"

#include "Net/UnrealNetwork.h"

AONBPlayerState::AONBPlayerState()
	: PlayerNameString(TEXT("None"))
{
	bReplicates = true;
}

void AONBPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, PlayerNameString);
}
