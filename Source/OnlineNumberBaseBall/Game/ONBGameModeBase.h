// ONBGameModeBase.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ONBGameModeBase.generated.h"

class AONBPlayerController;

/**
 * 
 */
UCLASS()
class ONLINENUMBERBASEBALL_API AONBGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void OnPostLogin(AController* NewPlayer) override;
	
	virtual void BeginPlay() override;
	
	FString GenerateSecretNumber();
	
	bool IsGuessNumberString(const FString& InNumberString, FString& OutWarningString);
	
	FString JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString);
	
	void PrintChatMessageString(AONBPlayerController* InChattingPlayerController, const FString& InChatMessageString);
	
	void IncreaseGuessCount(AONBPlayerController* InChattingPlayerController);
	
	void RestGame();
	
	void JudgeGame(AONBPlayerController* InChattingPlayerController, int32 InStrikeCount);
	
protected:
	FString SecretNumberString;
	
	TArray<TObjectPtr<AONBPlayerController>> AllPlayerControllers;
	
};
