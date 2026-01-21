// Copyright (c) Jared Taylor


#include "MarkdownDocsStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Styling/AppStyle.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateTypes.h"

TSharedPtr<FSlateStyleSet> FMarkdownDocsStyle::StyleSet;

void FMarkdownDocsStyle::Register()
{
	if (StyleSet.IsValid())
	{
		return;
	}

	StyleSet = MakeShared<FSlateStyleSet>("MarkdownDocsStyle");

	// Starship SVG directory
	StyleSet->SetContentRoot(FPaths::EngineContentDir() / TEXT("Editor/Slate"));

	// Re-register Details.svg as an ICON (16x16 + 20x20)
	StyleSet->Set(
		"MarkdownDocs.Documentation",
		new FSlateVectorImageBrush(
			StyleSet->RootToContentDir(TEXT("Starship/Common/Details.svg")),
			FVector2D(16.0f, 16.0f)
		));

	StyleSet->Set(
		"MarkdownDocs.Documentation.Small",
		new FSlateVectorImageBrush(
			StyleSet->RootToContentDir(TEXT("Starship/Common/Details.svg")),
			FVector2D(20.0f, 20.0f)
		));
	
	StyleSet->Set(
		"ClassIcon.MarkdownNote",
		new FSlateVectorImageBrush(
			StyleSet->RootToContentDir(TEXT("Starship/Common/Details.svg")),
			FVector2D(16.0f, 16.0f)
	));

	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet);
}

void FMarkdownDocsStyle::Unregister()
{
	if (StyleSet.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet);
		StyleSet.Reset();
	}
}

const ISlateStyle& FMarkdownDocsStyle::Get()
{
	return *StyleSet;
}
