// Copyright (c) Jared Taylor


#include "MarkdownNoteEditor.h"

#include "MarkdownDocsStyle.h"
#include "MarkdownNote.h"
#include "MarkdownUserSettings.h"
#include "Components/RichTextBlock.h"

#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"
#include "Framework/Docking/TabManager.h"
#include "Framework/Text/RichTextLayoutMarshaller.h"
#include "Framework/Text/RichTextMarkupProcessing.h"
#include "Widgets/Text/SRichTextBlock.h"

static const FName MarkdownNoteEditorTabId("MarkdownNoteEditor");


static FString ApplyInlineStylePairs(const FString& In, const TCHAR* Token, const TCHAR* StyleName)
{
	FString Out;
	Out.Reserve(In.Len());

	bool bOpen = false;
	const int32 TokenLen = FCString::Strlen(Token);

	for (int32 i = 0; i < In.Len(); )
	{
		if (In[i] == TEXT('\\') && i + 1 < In.Len())
		{
			Out.AppendChar(In[i + 1]);
			i += 2;
		}
		else if (In.Mid(i, TokenLen).Equals(Token))
		{
			Out += bOpen ? TEXT("</>") : FString::Printf(TEXT("<%s>"), StyleName);
			bOpen = !bOpen;
			i += TokenLen;
		}
		else
		{
			Out.AppendChar(In[i]);
			++i;
		}
	}

	if (bOpen)
	{
		Out += TEXT("</>");
	}

	return Out;
}

FText FMarkdownNoteEditor::ConvertMarkdownToRichText(const FText& MarkdownText)
{
	FString S = MarkdownText.ToString();
	S.ReplaceInline(TEXT("\r\n"), TEXT("\n"));

	TArray<FString> Lines;
	S.ParseIntoArray(Lines, TEXT("\n"), false);

	int32 OrderedIndex = 1;
	bool bInOrderedList = false;

	for (FString& Line : Lines)
	{
		if (Line.TrimStart().IsEmpty())
		{
			OrderedIndex = 1;
			bInOrderedList = false;
			continue;
		}

		// Headers: any number of #
		int32 HashCount = 0;
		while (HashCount < Line.Len() && Line[HashCount] == TEXT('#'))
		{
			++HashCount;
		}

		if (HashCount > 0 && Line.Len() > HashCount && Line[HashCount] == TEXT(' '))
		{
			const FString Content = Line.Mid(HashCount + 1);
			Line = FString::Printf(TEXT("<H%d>%s</>"), HashCount, *Content);
			OrderedIndex = 1;
			bInOrderedList = false;
		}
		// Bullet list
		else if (Line.StartsWith(TEXT("- ")) || Line.StartsWith(TEXT("* ")))
		{
			Line = FString::Printf(TEXT("• %s"), *Line.Mid(2));
			OrderedIndex = 1;
			bInOrderedList = false;
		}
		// Numbered list (any N.)
		else
		{
			int32 DotIndex = INDEX_NONE;
			if (Line.FindChar(TEXT('.'), DotIndex))
			{
				const FString Prefix = Line.Left(DotIndex);
				if (Prefix.IsNumeric() && Line.Len() > DotIndex + 1 && Line[DotIndex + 1] == TEXT(' '))
				{
					if (!bInOrderedList)
					{
						OrderedIndex = 1;
						bInOrderedList = true;
					}

					Line = FString::Printf(TEXT("%d. %s"), OrderedIndex++, *Line.Mid(DotIndex + 2));
				}
				else
				{
					OrderedIndex = 1;
					bInOrderedList = false;
				}
			}
			else
			{
				OrderedIndex = 1;
				bInOrderedList = false;
			}
		}

		Line = ApplyInlineStylePairs(Line, TEXT("***"), TEXT("BoldItalic"));
		Line = ApplyInlineStylePairs(Line, TEXT("**"), TEXT("Bold"));
		Line = ApplyInlineStylePairs(Line, TEXT("*"),  TEXT("Italic"));
		Line = ApplyInlineStylePairs(Line, TEXT("`"),  TEXT("Code"));
	}

	return FText::FromString(FString::Join(Lines, TEXT("\n")));
}

void FMarkdownNoteEditor::Init(UMarkdownNote* InNote)
{
	Note = InNote;

	const TSharedRef<FTabManager::FLayout> Layout =
		FTabManager::NewLayout("MarkdownNoteEditor_TabLayout")
		->AddArea
		(
			FTabManager::NewPrimaryArea()
			->SetOrientation(Orient_Vertical)
			->Split
			(
				FTabManager::NewStack()
				->AddTab(MarkdownNoteEditorTabId, ETabState::OpenedTab)
			)
		);

	InitAssetEditor(
		EToolkitMode::Standalone,
		TSharedPtr<IToolkitHost>(),
		FName("MarkdownNoteEditor"),
		Layout,
		true,
		true,
		InNote
	);
	
	AddToolbarToggle();
	RegenerateMenusAndToolbars();
}

void FMarkdownNoteEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	InTabManager->RegisterTabSpawner(
		MarkdownNoteEditorTabId,
		FOnSpawnTab::CreateRaw(this, &FMarkdownNoteEditor::SpawnEditorTab)
	)
	.SetDisplayName(FText::FromString("Markdown"))
	.SetIcon(FSlateIcon("EditorStyle", "Icons.Details"));
}

void FMarkdownNoteEditor::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	InTabManager->UnregisterTabSpawner(MarkdownNoteEditorTabId);
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);
}

FName FMarkdownNoteEditor::GetToolkitFName() const
{
	return FName("MarkdownNoteEditor");
}

FText FMarkdownNoteEditor::GetBaseToolkitName() const
{
	return FText::FromString("Markdown Note");
}

FString FMarkdownNoteEditor::GetWorldCentricTabPrefix() const
{
	return TEXT("Markdown Note");
}

FLinearColor FMarkdownNoteEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor::White;
}

void FMarkdownNoteEditor::AddToolbarToggle()
{
	TSharedPtr<FExtender> Extender = MakeShared<FExtender>();

	Extender->AddToolBarExtension(
		"Asset",
		EExtensionHook::After,
		GetToolkitCommands(),
		FToolBarExtensionDelegate::CreateRaw(this, &FMarkdownNoteEditor::FillToolbar)
	);

	AddToolbarExtender(Extender);
}

void FMarkdownNoteEditor::FillToolbar(FToolBarBuilder& ToolbarBuilder)
{
	ToolbarBuilder.AddToolBarButton(
		FUIAction(
			FExecuteAction::CreateRaw(this, &FMarkdownNoteEditor::ToggleEditMode),
			FCanExecuteAction(),
			FIsActionChecked::CreateRaw(this, &FMarkdownNoteEditor::IsEditMode)
		),
			NAME_None,
			TAttribute<FText>::CreateRaw(this, &FMarkdownNoteEditor::GetModeLabel),
			TAttribute<FText>::CreateRaw(this, &FMarkdownNoteEditor::GetModeTooltip),
			TAttribute<FSlateIcon>::CreateRaw(this, &FMarkdownNoteEditor::GetModeIcon),
			EUserInterfaceActionType::ToggleButton
	);
}

FText FMarkdownNoteEditor::GetModeLabel() const
{
	return bEditMode ? FText::FromString("Edit") : FText::FromString("Read");
}

FText FMarkdownNoteEditor::GetModeTooltip() const
{
	return bEditMode
		? FText::FromString("Switch to read-only mode")
		: FText::FromString("Switch to edit mode");
}

FSlateIcon FMarkdownNoteEditor::GetModeIcon() const
{
	return bEditMode
		? FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Edit")
		: FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Documentation");
}

void FMarkdownNoteEditor::ToggleEditMode()
{
	bEditMode = !bEditMode;

	if (EditBox.IsValid())
	{
		EditBox->SetIsReadOnly(!bEditMode);
		EditBox->SetVisibility(bEditMode ? EVisibility::Visible : EVisibility::Collapsed);
	}

	if (ReadBox.IsValid())
	{
		ReadBox->SetVisibility(bEditMode ? EVisibility::Collapsed : EVisibility::Visible);

		if (!bEditMode && Note)
		{
			ReadBox->SetText(ConvertMarkdownToRichText(Note->GetText()));
		}
	}

	RegenerateMenusAndToolbars();
}

bool FMarkdownNoteEditor::IsEditMode() const
{
	return bEditMode;
}

TSharedRef<SDockTab> FMarkdownNoteEditor::SpawnEditorTab(const FSpawnTabArgs& Args)
{
	RebuildMarkdownStyleFromSettings();

	return SNew(SDockTab)
	[
		SNew(SOverlay)

		+ SOverlay::Slot()
		[
			SAssignNew(EditBox, SMultiLineEditableTextBox)
			.Text_Lambda([this]() { return Note ? Note->GetText() : FText::GetEmpty(); })
			.OnTextCommitted_Lambda([this](const FText& NewText, ETextCommit::Type)
			{
				if (!Note || !bEditMode)
				{
					return;
				}

				if (!NewText.EqualTo(Note->GetText()))
				{
					Note->Modify();
					Note->SetText(NewText);
				}
			})
			.IsReadOnly_Lambda([this]() { return !bEditMode; })
			.Visibility_Lambda([this]() { return bEditMode ? EVisibility::Visible : EVisibility::Collapsed; })
		]

		+ SOverlay::Slot()
		[
			SAssignNew(ReadBox, SRichTextBlock)
			.AutoWrapText(true)
			.Text_Lambda([this]() { return Note ? ConvertMarkdownToRichText(Note->GetText()) : FText::GetEmpty(); })
			.TextStyle(&DefaultMarkdownTextStyle)
			.DecoratorStyleSet(MarkdownStyleSet.Get())
			.Marshaller(MarkdownMarshaller.ToSharedRef())
			.Visibility_Lambda([this]() { return bEditMode ? EVisibility::Collapsed : EVisibility::Visible; })
		]
	];
}

void FMarkdownNoteEditor::RebuildMarkdownStyleFromSettings()
{
	MarkdownStyleSet.Reset();
	MarkdownMarshaller.Reset();

	const UDataTable* Table = UMarkdownUserSettings::GetMarkdownTextStyleSet();
	if (!Table)
	{
		return;
	}

	// Build style set
	TSharedPtr<FSlateStyleSet> NewSet = MakeShared<FSlateStyleSet>(FName("MarkdownDocsRuntimeStyle"));
	MarkdownStyleSet = NewSet;
	
	if (Table && Table->GetRowStruct()->IsChildOf(FRichTextStyleRow::StaticStruct()))
	{
		for (const auto& Entry : Table->GetRowMap())
		{
			FName SubStyleName = Entry.Key;
			FRichTextStyleRow* RichTextStyle = reinterpret_cast<FRichTextStyleRow*>(Entry.Value);

			if (SubStyleName == FName("Default"))
			{
				DefaultMarkdownTextStyle = RichTextStyle->TextStyle;
			}

			MarkdownStyleSet->Set(SubStyleName, RichTextStyle->TextStyle);
		}
	}
	
	// Build marshaller and assign style set
	TArray<TSharedRef<ITextDecorator>> Decorators;
	MarkdownMarshaller = FRichTextLayoutMarshaller::Create(
		FDefaultRichTextMarkupParser::Create(),
		FDefaultRichTextMarkupWriter::Create(),
		Decorators,
		MarkdownStyleSet.Get()
	);
}
