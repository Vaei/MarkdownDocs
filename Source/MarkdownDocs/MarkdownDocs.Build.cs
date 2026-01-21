// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MarkdownDocs : ModuleRules
{
	public MarkdownDocs(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core", 
				"UnrealEd",
				"AssetDefinition",
				"DeveloperSettings",
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"UMG",
			}
			);
	}
}
