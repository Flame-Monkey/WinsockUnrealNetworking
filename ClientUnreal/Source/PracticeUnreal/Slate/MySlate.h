

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */

class SButton;
class AMyController;

class PRACTICEUNREAL_API SMySlate : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMySlate)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	void SetController(TWeakObjectPtr<AMyController> Controller);

	void OpenChatting();
	void AddChat(const FText& Text);

protected:
	FReply OnMyButtonClicked();
	FReply SettingButtonClicked();

	TWeakObjectPtr<AMyController> OwnerController;

	TSharedPtr<SButton> SettingButton;
	TSharedPtr<SButton> PlayerInfoButton;

	TSharedPtr<SBorder> SettingPanel;

	TSharedPtr<SEditableTextBox> ChatInput;
	TSharedPtr<SScrollBox> ChattingBox;
	TSharedPtr<SScrollBar> ChattingScroll;

	void ChatCommitted(const FText& Text, ETextCommit::Type CommitType);


	virtual FReply OnKeyDown(const FGeometry& MeGeometry, const FKeyEvent& InKeyEvent) override;
};
