// ONBPawn.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ONBPawn.generated.h"

UCLASS()
class ONLINENUMBERBASEBALL_API AONBPawn : public APawn
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;
	
};
