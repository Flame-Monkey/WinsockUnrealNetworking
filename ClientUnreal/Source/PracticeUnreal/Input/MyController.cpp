


#include "MyController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Slate/MySlate.h"

#include "Network/ChattingClient.h"

AMyController::AMyController() :
	Client()
{

}

void AMyController::BeginPlay()
{
	bShowMouseCursor = true;

	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	SetInputMode(InputMode);
	Client.Init();
	Client.Connect("127.0.0.1", 5000);
	Client.Login("Jin", 20);
}

void AMyController::Tick(float DeltaTime)
{
	if (Client.IsMessageAvailable())
	{
		Message::StructMessage message = Client.GetStructMessage();
		if (message.Type == Message::EPayloadType::Chatting)
		{
			std::string chat = std::string(message.Payload.chatting.Sender) + ": " + std::string(message.Payload.chatting.Message);
			FString m = ANSI_TO_TCHAR(chat.c_str());
			FText text = FText::FromString(m);
			MyUI->AddChat(text);
		}
	}
}

void AMyController::SendChat(const FText& Chat)
{
	UE_LOG(LogTemp, Warning, TEXT("Chat Text Committed: %s"), *Chat.ToString());
	std::string StdChatString(TCHAR_TO_ANSI(*Chat.ToString()));
	Client.SendChat(StdChatString);
}

void AMyController::OpenChat(const FInputActionValue& Value)
{
	MyUI->OpenChatting();
}

void AMyController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(UIMapping, 0);
		Subsystem->AddMappingContext(CharacterMapping, 0);
	}

	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(InputComponent))
	{
		Input->BindAction(ChatAction, ETriggerEvent::Triggered, this, &AMyController::OpenChat);
	}

	MyUI = SNew(SMySlate);
	MyUI->SetController(this);
	GEngine->GameViewport->AddViewportWidgetContent(MyUI.ToSharedRef(), 10);
}

void AMyController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	Client.Disconnect();
}