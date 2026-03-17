// ONBPlayerState.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ONBPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class ONLINENUMBERBASEBALL_API AONBPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	AONBPlayerState();
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	FString GetPlayerInfoString();
	
public:
	UPROPERTY(Replicated)
	FString PlayerNameString;
	
	UPROPERTY(Replicated)
	int32 CurrentGuessCount;
	
	UPROPERTY(Replicated)
	int32 MaxGussCount;
	
};
