// ONBChatInput.h

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ONBChatInput.generated.h"

class UScrollBox;
class UEditableTextBox;

/**
 * 
 */
UCLASS()
class ONLINENUMBERBASEBALL_API UONBChatInput : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void AddChatMessage(const FString& InMessage);
	
	virtual void NativeConstruct() override;
	
	virtual void NativeDestruct() override;
	
protected:
	UFUNCTION()
	void OnChatInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> ChatScrollBox;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> EditableTextBox_ChatInput;
	
};
