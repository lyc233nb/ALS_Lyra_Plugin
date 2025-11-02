using UnrealBuildTool;

public class Als : ModuleRules
{
	public Als(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_6;

	
		
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"LyraGame", "AIModule", "GameplayTags", "GameplayAbilities", "ModularGameplay", "ModularGameplayActors"
		});

		PrivateDependencyModuleNames.AddRange(new[]
		{
			"Core", "CoreUObject", "Engine", "NetCore", "PhysicsCore", "GameplayTags", "AnimGraphRuntime", "RigVM", "ControlRig", "Niagara", "LyraGame","ModularGameplay","Chooser"
		});

		if (Target.Type == TargetRules.TargetType.Editor)
		{
			PrivateDependencyModuleNames.AddRange(new[] {"MessageLog"});
		}
	}
}