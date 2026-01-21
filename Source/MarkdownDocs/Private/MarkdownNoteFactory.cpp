// Copyright (c) Jared Taylor


#include "MarkdownNoteFactory.h"

#include "MarkdownNote.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MarkdownNoteFactory)

UMarkdownNoteFactory::UMarkdownNoteFactory()
{
	SupportedClass = UMarkdownNote::StaticClass();
	bCreateNew = true;
	bEditorImport = true;
	bEditAfterNew = true;
}

UObject* UMarkdownNoteFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UMarkdownNote>(InParent, InName, Flags);
}
