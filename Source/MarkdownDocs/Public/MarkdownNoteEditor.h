// Copyright (c) Jared Taylor

#pragma once

#include "CoreMinimal.h"
#include "Toolkits/AssetEditorToolkit.h"

class SRichTextBlock;
class FRichTextLayoutMarshaller;
class SMultiLineEditableTextBox;
class UMarkdownNote;

class FMarkdownNoteEditor : public FAssetEditorToolkit
{
public:
	static FText ConvertMarkdownToRichText(const FText& MarkdownText);
	
	void Init(UMarkdownNote* InNote);
	
	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& TabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& TabManager) override;

	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	
	void AddToolbarToggle();
	void FillToolbar(FToolBarBuilder& ToolbarBuilder);
	FText GetModeLabel() const;
	FText GetModeTooltip() const;
	FSlateIcon GetModeIcon() const;
	void ToggleEditMode();
	bool IsEditMode() const;

private:
	TSharedRef<SDockTab> SpawnEditorTab(const FSpawnTabArgs& Args);
	
	void RebuildMarkdownStyleFromSettings();

	UMarkdownNote* Note = nullptr;
	TSharedPtr<SMultiLineEditableTextBox> EditBox;
	TSharedPtr<SRichTextBlock> ReadBox;
	
	bool bEditMode = false;
	
	TSharedPtr<FRichTextLayoutMarshaller> MarkdownMarshaller;
	TSharedPtr<FSlateStyleSet> MarkdownStyleSet;
	FTextBlockStyle DefaultMarkdownTextStyle;
};