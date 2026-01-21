// Copyright (c) Jared Taylor


#include "MarkdownUserSettings.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MarkdownUserSettings)

UDataTable* UMarkdownUserSettings::GetMarkdownTextStyleSet()
{
	if (const UMarkdownUserSettings* Settings = GetDefault<UMarkdownUserSettings>())
	{
		const TSoftObjectPtr<UDataTable>& DataTablePtr = Settings->MarkdownTextStyleSet;
		if (!DataTablePtr.IsNull())
		{
			return DataTablePtr.LoadSynchronous();
		}
	}
	return nullptr;
}
