// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class OverTale : ModuleRules
{
	public OverTale(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new[] {
				"OverTale"
			}
		);
		
	
		PublicDependencyModuleNames.AddRange(new[] { "Core", 
			"Core",
			"CoreOnline",
			"CoreUObject",
			"ApplicationCore",
			"Engine",
			"PhysicsCore",
			"GameplayTags",
			"GameplayTasks",
			"GameplayAbilities",
			"AIModule",
			"ModularGameplay",
			"ModularGameplayActors",
			"DataRegistry",
			//"ReplicationGraph",
			"GameFeatures",
			//"SignificanceManager",
			//"Hotfix",
			"Niagara",
			"ControlFlows",
			"PropertyPath", 
		});
		
		PrivateDependencyModuleNames.AddRange(new []
		{
			"InputCore",
			"Slate",
			"SlateCore",
			"RenderCore",
			"DeveloperSettings",
			"EnhancedInput",
			//"NetCore",
			"RHI",
			"Projects",
			"Gauntlet",
			"UMG",
			"CommonUI",
			"CommonInput",
			"CommonGame",
			"AudioMixer",
			//"NetworkReplayStreaming",
			//"ClientPilot",
			"AudioModulation",
			"EngineSettings",
			//"DTLSHandlerComponent",
		});
	}
}
