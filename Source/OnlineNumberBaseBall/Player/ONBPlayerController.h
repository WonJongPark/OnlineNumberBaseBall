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
	
	void SetChatMessageString(const FString& IntChatMessageString);
	
	void PrintChatMessageString(const FString& InChatMessageString);
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UONBChatInput> ChatInputWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UONBChatInput> ChatInputWidgetInstance;
	
	FString ChatMessageString;
	
};
