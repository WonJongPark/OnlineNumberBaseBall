// ONBPawn.cpp


#include "ONBPawn.h"

#include "OnlineNumberBaseBall.h"


// Called when the game starts or when spawned
void AONBPawn::BeginPlay()
{
	Super::BeginPlay();
	
	FString NetRoleString = ONBFunctionLibrary::GetRoleString(this);
	FString CombineString = FString::Printf(TEXT("ONBPawn::BeginPlay() %s [%s]"), *ONBFunctionLibrary::GetNetModeString(this), *NetRoleString);
	ONBFunctionLibrary::MyPrintString(this, CombineString, 10.0f);
}

void AONBPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	FString NetRoleString = ONBFunctionLibrary::GetRoleString(this);
	FString CombinedString = FString::Printf(TEXT("ONBPawn::PossessedBy() %s [%s]"), *ONBFunctionLibrary::GetNetModeString(this), *NetRoleString);
	ONBFunctionLibrary::MyPrintString(this, CombinedString, 10.0f);
}
