


#include "MySlate.h"
#include "Input/MyController.h"

#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SMySlate::Construct(const FArguments& InArgs)
{
	ChildSlot
		[
			SNew(SOverlay)
				// Menu UI
				+ SOverlay::Slot()
				.HAlign(HAlign_Right)
				.VAlign(VAlign_Bottom)
				.Padding(FMargin(10.f))
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					[
						SNew(SBox)
							.WidthOverride(200.f)
							.HeightOverride(100.f)
						[
							SAssignNew(PlayerInfoButton,SButton)
								.HAlign(HAlign_Center)
								.VAlign(VAlign_Center)
								.OnClicked(this, &SMySlate::OnMyButtonClicked)
								.IsFocusable(false)
							[
								SNew(STextBlock)
									.Text(FText::FromString(TEXT("Player Info")))
									.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 24))
									.Justification(ETextJustify::Center)
							]
						]
					]
					+ SHorizontalBox::Slot()
					[
						SNew(SBox)
							.WidthOverride(200.f)
							.HeightOverride(100.f)
						[
							SAssignNew(SettingButton ,SButton)
								.HAlign(HAlign_Center)
								.VAlign(VAlign_Center)
								.OnClicked(this, &SMySlate::SettingButtonClicked)
								.IsFocusable(false)
								[
									SNew(STextBlock)
										.Text(FText::FromString(TEXT("Setting")))
										.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 24))
										.Justification(ETextJustify::Center)
								]
						]
					]
				]
			// Chatting UI
			+ SOverlay::Slot()
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Bottom)
				.Padding(20.f, 0.f)
			[
				SNew(SBox)
					.WidthOverride(500.f)
					.HeightOverride(500.f)
				[
					SNew(SVerticalBox)

					+ SVerticalBox::Slot()
						.FillHeight(0.9f)
					[
						SNew(SHorizontalBox)

						+ SHorizontalBox::Slot()
							.HAlign(HAlign_Left)
							.AutoWidth()
						[
							SAssignNew(ChattingScroll, SScrollBar)
								.Orientation(EOrientation::Orient_Vertical)
								.Thickness(FVector2D(10.f, 0.f)) 
								.AlwaysShowScrollbar(true)
						]

						+ SHorizontalBox::Slot()
							.FillWidth(1.f)
						[
							SNew(SBorder)
								.BorderBackgroundColor(FLinearColor(0.f, 0.f, 0.f, 0.5f))
							[
								SAssignNew(ChattingBox, SScrollBox)
									.ExternalScrollbar(ChattingScroll)
									.ScrollBarAlwaysVisible(true)
								+ SScrollBox::Slot()
								[
									SNew(STextBlock)
										.Text(FText::FromString(TEXT("hello, world!!")))
										.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 24))
								]
							]
						]
					]

					+ SVerticalBox::Slot()
						.FillHeight(0.1f)
						.Padding(0, 5)
					[
						SAssignNew(ChatInput, SEditableTextBox)
							.HintText(FText::FromString(TEXT("Type message...")))
							.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 24))
							.OnTextCommitted(this, &SMySlate::ChatCommitted)
							.ClearKeyboardFocusOnCommit(false)
					]
				]
			]
		];
}
FReply SMySlate::OnMyButtonClicked()
{
	UE_LOG(LogTemp, Error, TEXT("hello, world!!"));

	return FReply::Handled();
}
FReply SMySlate::SettingButtonClicked()
{
	UE_LOG(LogTemp, Error, TEXT("bye, world!!"));

	return FReply::Handled();
}

FReply SMySlate::OnKeyDown(const FGeometry& MeGeometry, const FKeyEvent& InKeyEvent)
{
	FKey PressedKey = InKeyEvent.GetKey();
	UE_LOG(LogTemp, Warning, TEXT("Key Down: %s"), *PressedKey.ToString());
	return FReply::Unhandled();
}

void SMySlate::SetController(TWeakObjectPtr<AMyController> Controller)
{
	OwnerController = Controller;
}

void SMySlate::OpenChatting()
{
	if (ChatInput.IsValid())
	{
		FSlateApplication::Get().SetKeyboardFocus(ChatInput, EFocusCause::SetDirectly);
	}
}

void SMySlate::AddChat(const FText& Text)
{
	if (!ChattingBox.IsValid())
		return;

	ChattingBox->AddSlot()
		[
			SNew(STextBlock)
				.Text(Text)
				.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 24))
		];

	ChattingBox->ScrollToEnd();
}

void SMySlate::ChatCommitted(const FText& Text, ETextCommit::Type CommitType)
{
	if (CommitType == ETextCommit::OnEnter)
	{
		if (OwnerController.IsValid())
		{
			OwnerController->SendChat(Text);
		}
		UE_LOG(LogTemp, Warning, TEXT("Chat Text Committed: %s"), *Text.ToString());
		ChatInput->SetText(FText::GetEmpty());
	}
	FSlateApplication::Get().SetAllUserFocusToGameViewport(EFocusCause::SetDirectly);
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
