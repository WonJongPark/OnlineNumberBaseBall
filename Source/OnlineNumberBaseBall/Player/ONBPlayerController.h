// ONBPlayerController.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ONBPlayerController.generated.h"

class UONBChatInput;

/**
 * 
 */
UCLASS()
class ONLINENUMBERBASEBALL_API AONBPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	
	void SetChatMessageString(const FString& InChatMessageString);
	
	void PrintChatMessageString(const FString& InChatMessageString);
	
	UFUNCTION(Client, Reliable)
	void ClientRPCPrintChatMessageString(const FString& InChatMessageString);
	
	UFUNCTION(Server, Reliable)
	void ServerRPCPrintChatMessageString(const FString& InChatMessageString);
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UONBChatInput> ChatInputWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UONBChatInput> ChatInputWidgetInstance;
	
	FString ChatMessageString;
	
};
