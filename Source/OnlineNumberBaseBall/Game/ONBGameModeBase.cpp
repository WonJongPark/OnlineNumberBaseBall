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
	UE_LOG(LogTemp, Error, TEXT("%s"), *SecretNumberString);
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

bool AONBGameModeBase::IsGuessNumberString(const FString& InNumberString, FString& OutWarningString)
{
	OutWarningString = TEXT("");
	
	bool bIsOnlyDigits = true;
	for (TCHAR C : InNumberString)
	{
		if (!FChar::IsDigit(C))
		{
			bIsOnlyDigits = false;
			break;
		}
	}
	
	if (!bIsOnlyDigits)
	{
		return false;
	}
	
	if (InNumberString.Len() != 3)
	{
		OutWarningString = TEXT("Please enter a 3-digit number.");
		return false;
	}
	
	TSet<TCHAR> UniqueDigits;
	for (TCHAR C : InNumberString)
	{
		if (C == '0')
		{
			OutWarningString = TEXT("Please enter a number between 1 and 9.");
			return false;
		}
		
		if (UniqueDigits.Contains(C))
		{
			OutWarningString = TEXT("Please enter unique numbers.");
			return false;
		}
		
		UniqueDigits.Add(C);
	}
	
	return true;
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
	AONBPlayerState* ONBPS = InChattingPlayerController->GetPlayerState<AONBPlayerState>();
	if (!IsValid(ONBPS)) return;

	FString FinalMessageString;
	FString WarningMessage;
	
	bool bIsGuess = IsGuessNumberString(InChatMessageString, WarningMessage);
	
	if (bIsGuess)
	{
		if (ONBPS->CurrentGuessCount >= ONBPS->MaxGussCount)
		{
			WarningMessage = ONBPS->GetPlayerInfoString() + TEXT(" has exhausted all opportunities.");
			
			InChattingPlayerController->ClientRPCPrintChatMessageString(WarningMessage);
			
			return;
		}
		
		IncreaseGuessCount(InChattingPlayerController);
		FString JudgeResultString = JudgeResult(SecretNumberString, InChatMessageString);
		FinalMessageString = ONBPS->GetPlayerInfoString() + TEXT(": ") + InChatMessageString + TEXT(" -> ") + JudgeResultString;
		int32 StrikeCount = FCString::Atoi(*JudgeResultString.Left(1));
		JudgeGame(InChattingPlayerController, StrikeCount);
	}
	else
	{
		if (!WarningMessage.IsEmpty())
		{
			InChattingPlayerController->ClientRPCPrintChatMessageString(WarningMessage);
			return;
		}
		else
		{
			FinalMessageString = ONBPS->GetPlayerInfoString() + TEXT(": ") + InChatMessageString;
		}
	}
	
	if (!FinalMessageString.IsEmpty())
	{
		for (TActorIterator<AONBPlayerController> It(GetWorld()); It; ++It)
		{
			AONBPlayerController* ONBPlayerController = *It;
			if (IsValid(ONBPlayerController))
			{
				ONBPlayerController->ClientRPCPrintChatMessageString(FinalMessageString);
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

void AONBGameModeBase::RestGame()
{
	SecretNumberString = GenerateSecretNumber();
	
	for (AONBPlayerController* ONBPlayerController : AllPlayerControllers)
	{
		AONBPlayerState* ONBPS = ONBPlayerController->GetPlayerState<AONBPlayerState>();
		if (IsValid(ONBPS))
		{
			ONBPS->CurrentGuessCount = 0;
		}
	}
	
	UE_LOG(LogTemp, Error, TEXT("%s"), *SecretNumberString);
}

void AONBGameModeBase::JudgeGame(AONBPlayerController* InChattingPlayerController, int32 InStrikeCount)
{
	if (InStrikeCount == 3)
	{
		AONBPlayerState* ONBPS = InChattingPlayerController->GetPlayerState<AONBPlayerState>();
		if (IsValid(ONBPS))
		{
			FString CombinedMessageString = ONBPS->PlayerNameString + TEXT(" has won the game.");
			
			for (AONBPlayerController* ONBPlayerController : AllPlayerControllers)
			{
				if (IsValid(ONBPlayerController))
				{
					ONBPlayerController->NotificationText = FText::FromString(CombinedMessageString);
				}
			}
			
			RestGame();
		}
	}
	else
	{
		bool bIsDraw = true;
		for (AONBPlayerController* ONBPlayerController : AllPlayerControllers)
		{
			AONBPlayerState* ONBPS = ONBPlayerController->GetPlayerState<AONBPlayerState>();
			if (IsValid(ONBPS))
			{
				if (ONBPS->CurrentGuessCount < ONBPS->MaxGussCount)
				{
					bIsDraw = false;
					break;
				}
			}
		}
		
		if (bIsDraw)
		{
			for (AONBPlayerController* ONBPlayerController : AllPlayerControllers)
			{
				ONBPlayerController->NotificationText = FText::FromString(TEXT("Draw..."));
			}
			
			RestGame();
		}
	}
}
