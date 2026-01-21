// Copyright (c) Jared Taylor

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MarkdownNote.generated.h"

/**
 * Note object for holding Markdown text
 */
UCLASS(MinimalAPI, NotBlueprintable, BlueprintType)
class UMarkdownNote : public UObject
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Markdown)
	FText Text;
	
public:
	MARKDOWNDOCS_API FText GetText() const { return Text; }
	MARKDOWNDOCS_API void SetText(const FText& NewText) { Text = NewText; }
	
	virtual bool IsEditorOnly() const override { return true; }
};
