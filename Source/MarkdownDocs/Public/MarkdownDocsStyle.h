// Copyright (c) Jared Taylor

#pragma once

#include "Styling/SlateStyle.h"

class FMarkdownDocsStyle
{
public:
	static void Register();
	static void Unregister();
	static const ISlateStyle& Get();

private:
	static TSharedPtr<FSlateStyleSet> StyleSet;
};
