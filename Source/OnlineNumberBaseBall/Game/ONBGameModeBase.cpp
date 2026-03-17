// ONBGameModeBase.cpp


#include "ONBGameModeBase.h"

#include "EngineUtils.h"
#include "ONBGameStateBase.h"
#include "Player/ONBPlayerController.h"
#include "Player/ONBPlayerState.h"

void AONBGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);
	
	AONBPlayerController* ONBPlayerController = Cast<AONBPlayerController>(NewPlayer);
	if (IsValid(ONBPlayerController))
	{
		ONBPlayerController->NotificationText = FText::FromString(TEXT("Connected to the game server."));
		
		AllPlayerControllers.Add(ONBPlayerController);
		
		AONBPlayerState* ONBPS = ONBPlayerController->GetPlayerState<AONBPlayerState>();
		if (IsValid(ONBPS))
		{
			ONBPS->PlayerNameString = TEXT("Player") + FString::FromInt(AllPlayerControllers.Num());
		}
		
		AONBGameStateBase* ONBGameStateBase = GetGameState<AONBGameStateBase>();
		if (IsValid(ONBGameStateBase))
		{
			ONBGameStateBase->MulticastRPCBroadcastLoginMessage(ONBPS->PlayerNameString);
		}
	}
}

void AONBGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	SecretNumberString = GenerateSecretNumber();
}

FString AONBGameModeBase::GenerateSecretNumber()
{
	TArray<int32> Numbers;
	for (int32 i = 1; i <= 9; ++i)
	{
		Numbers.Add(i);
	}
	
	FMath::RandInit(FDateTime::Now().GetTicks());
	// Numbers = Numbers.FilterByPredicate([](int32 Num) { return Num > 0; });
	
	FString Result;
	for (int32 i = 0; i < 3; ++i)
	{
		int32 Index = FMath::RandRange(0, Numbers.Num() - 1);
		Result.Append(FString::FromInt(Numbers[Index]));
		Numbers.RemoveAt(Index);
	}
	
	return Result;
}

bool AONBGameModeBase::IsGuessNumberString(const FString& InNumberString)
{
	bool bCanPlay = false;
	
	do
	{
		if (InNumberString.Len() != 3)
		{
			break;
		}
		
		bool bIsUnique = true;
		TSet<TCHAR> UniqueDigits;
		for (TCHAR C : InNumberString)
		{
			if (!FChar::IsDigit(C) || C == '0')
			{
				bIsUnique = false;
				break;
			}
			
			UniqueDigits.Add(C);
		}
		
		if (!bIsUnique)
		{
			break;
		}
		
		bCanPlay = true;
		
	} while (false);
	
	return bCanPlay;
}

FString AONBGameModeBase::JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString)
{
	int32 StrikeCount = 0, BallCount = 0;
	
	for (int32 i = 0; i < 3; ++i)
	{
		if (InSecretNumberString[i] == InGuessNumberString[i])
		{
			StrikeCount++;
		}
		else
		{
			FString PlayerGuessChar = FString::Printf(TEXT("%c"), InGuessNumberString[i]);
			if (InSecretNumberString.Contains(PlayerGuessChar))
			{
				BallCount++;
			}
		}
	}
	
	if (StrikeCount == 0 && BallCount == 0)
	{
		return TEXT("OUT");
	}
	
	return FString::Printf(TEXT("%dS%dB"), StrikeCount, BallCount);
}

void AONBGameModeBase::PrintChatMessageString(AONBPlayerController* InChattingPlayerController, const FString& InChatMessageString)
{
	FString ChatMessageString = InChatMessageString;
	int Index = InChatMessageString.Len() - 3;
	FString GuessNumberString = InChatMessageString.RightChop(Index);
	
	if (IsGuessNumberString(GuessNumberString))
	{
		FString JudgeResultString = JudgeResult(SecretNumberString, GuessNumberString);
		
		IncreaseGuessCount(InChattingPlayerController);
		
		for (TActorIterator<AONBPlayerController> It(GetWorld()); It; ++It)
		{
			AONBPlayerController* ONBPlayerController = *It;
			if (IsValid(ONBPlayerController))
			{
				FString CombinedMessageString = InChatMessageString + TEXT(" -> ") + JudgeResultString;
				ONBPlayerController->ClientRPCPrintChatMessageString(CombinedMessageString);
			}
		}
	}
	else
	{
		for (TActorIterator<AONBPlayerController> It(GetWorld()); It; ++It)
		{
			AONBPlayerController* ONBPlayerController = *It;
			if (IsValid(ONBPlayerController))
			{
				ONBPlayerController->ClientRPCPrintChatMessageString(InChatMessageString);
			}
		}
	}
}

void AONBGameModeBase::IncreaseGuessCount(AONBPlayerController* InChattingPlayerController)
{
	AONBPlayerState* ONBPS = InChattingPlayerController->GetPlayerState<AONBPlayerState>();
	if (IsValid(ONBPS))
	{
		ONBPS->CurrentGuessCount++;
	}
}
