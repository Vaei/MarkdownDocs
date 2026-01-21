// Copyright (c) Jared Taylor

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/DeveloperSettings.h"
#include "UObject/SoftObjectPtr.h"
#include "MarkdownUserSettings.generated.h"

class UDataTable;
/**
 * User settings for the MarkdownDocs plugin
 * Allows specifying a default Rich Text Style Set for markdown notes - providing theme support for markdown rendering
 */
UCLASS(MinimalAPI, config=EditorPerProjectUserSettings)
class UMarkdownUserSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Config, EditAnywhere, Category = Markdown, meta = (RequiredAssetDataTags = "RowStructure=/Script/UMG.RichTextStyleRow"))
	TSoftObjectPtr<UDataTable> MarkdownTextStyleSet = TSoftObjectPtr<UDataTable>(FSoftObjectPath("/MarkdownDocs/MarkdownRichTextStyle.MarkdownRichTextStyle"));
	
	static MARKDOWNDOCS_API UDataTable* GetMarkdownTextStyleSet();
};
