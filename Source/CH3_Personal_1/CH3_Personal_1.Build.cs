// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CH3_Personal_1 : ModuleRules
{
	public CH3_Personal_1(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG", "GameplayAbilities", "GameplayTags", "GameplayTasks" });

		PrivateDependencyModuleNames.AddRange(new string[] { "AdvancedWidgets" });
	}
}
