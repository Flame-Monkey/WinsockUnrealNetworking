

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Network/ChattingClient.h"
#include "MyController.generated.h"


class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class SMySlate;

class ChattingClient;
/**
 * 
 */
UCLASS()
class PRACTICEUNREAL_API AMyController : public APlayerController
{
	GENERATED_BODY()
	
	
protected:
	virtual void SetupInputComponent() override;

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void Tick(float DeltaTime) override;

	void OpenChat(const FInputActionValue& Value);
	
	UPROPERTY(EditAnywhere, Category = "Input|Input Mapping")
	UInputMappingContext* CharacterMapping;

	UPROPERTY(EditAnywhere, Category = "Input|Input Mapping")
	UInputMappingContext* UIMapping;

	UPROPERTY(EditAnywhere, Category = "Input|Input Action")
	UInputAction* ChatAction;

	TSharedPtr<SMySlate> MyUI;

	ChattingClient Client;

public:
	AMyController();
	void SendChat(const FText& Chat);
};
