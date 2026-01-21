// Copyright Epic Games, Inc. All Rights Reserved.

#include "MarkdownDocs.h"

#include "MarkdownDocsStyle.h"

#define LOCTEXT_NAMESPACE "FMarkdownDocsModule"

void FMarkdownDocsModule::StartupModule()
{
	FMarkdownDocsStyle::Register();
}

void FMarkdownDocsModule::ShutdownModule()
{
	FMarkdownDocsStyle::Unregister();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMarkdownDocsModule, MarkdownDocs)