// Copyright (c) Jared Taylor


#include "AssetDefinition_MarkdownNote.h"

#include "MarkdownDocsStyle.h"
#include "MarkdownNote.h"
#include "MarkdownNoteEditor.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AssetDefinition_MarkdownNote)


const FSlateBrush* UAssetDefinition_MarkdownNote::GetIconBrush(const FAssetData& InAssetData, const FName InClassName) const
{
	return FMarkdownDocsStyle::Get().GetBrush("MarkdownDocs.Documentation");
}

const FSlateBrush* UAssetDefinition_MarkdownNote::GetThumbnailBrush(const FAssetData& InAssetData, const FName InClassName) const
{
	return FMarkdownDocsStyle::Get().GetBrush("MarkdownDocs.Documentation");
}

EAssetCommandResult UAssetDefinition_MarkdownNote::OpenAssets(const FAssetOpenArgs& OpenArgs) const
{
	for (UObject* Object : OpenArgs.LoadObjects<UMarkdownNote>())
	{
		TSharedRef<FMarkdownNoteEditor> Editor = MakeShared<FMarkdownNoteEditor>();
		Editor->Init(CastChecked<UMarkdownNote>(Object));
	}

	return EAssetCommandResult::Handled;
}
