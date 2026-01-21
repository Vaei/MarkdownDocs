// Copyright (c) Jared Taylor

#pragma once

#include "CoreMinimal.h"
#include "AssetDefinitionDefault.h"
#include "MarkdownNote.h"
#include "AssetDefinition_MarkdownNote.generated.h"

/**
 * Asset definition for UMarkdownNote
 * This applies FMarkdownNoteEditor as the editor for the asset, and sets the icon and color
 */
UCLASS()
class UAssetDefinition_MarkdownNote : public UAssetDefinitionDefault
{
	GENERATED_BODY()
	
public:
	// UAssetDefinition Begin
	virtual FText GetAssetDisplayName() const override { return NSLOCTEXT("AssetTypeActions", "FAssetTypeActions_MarkdownNote", "Markdown Note"); }
	virtual FLinearColor GetAssetColor() const override { return FLinearColor(FColor(175, 75, 0)); }
	virtual TSoftClassPtr<UObject> GetAssetClass() const override { return UMarkdownNote::StaticClass(); }
	virtual const FSlateBrush* GetIconBrush(const FAssetData& InAssetData, const FName InClassName) const override;
	virtual const FSlateBrush* GetThumbnailBrush(const FAssetData& InAssetData, const FName InClassName) const override;
	virtual EAssetCommandResult OpenAssets(const FAssetOpenArgs& OpenArgs) const override;
	// UAssetDefinition End
};
