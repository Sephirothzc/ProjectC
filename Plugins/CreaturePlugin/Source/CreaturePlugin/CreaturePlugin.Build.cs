using System.IO;

namespace UnrealBuildTool.Rules
{
    public class CreaturePlugin : ModuleRules
    {
        private string ModulePath
        {
            get { return Path.GetDirectoryName(RulesCompiler.GetModuleFilename(this.GetType().Name)); }
        }

        private string ThirdPartyPath
        {
            get { return Path.GetFullPath(Path.Combine(ModulePath, "../../../../ThirdParty/")); }
            //get { return "c:\\Work\\NewPluginTest\\MyProject\\ThirdParty"; }
        }

        public bool LoadCreatureLib(TargetInfo Target)
        {
            bool isLibrarySupported = false;

            if ((Target.Platform == UnrealTargetPlatform.Win64) || (Target.Platform == UnrealTargetPlatform.Win32))
            {
                isLibrarySupported = true;

                string PlatformString = (Target.Platform == UnrealTargetPlatform.Win64) ? "x64" : "x86";
                string LibrariesPath = Path.Combine(ThirdPartyPath, "CreatureLib", "Libraries");

                PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "CreatureLib." + PlatformString + ".lib"));
            }

            if (isLibrarySupported)
            {
                // Include path
                PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "CreatureLib", "Includes"));
            }

            Definitions.Add(string.Format("WITH_CREATURE_LIB_BINDING={0}", isLibrarySupported ? 1 : 0));

            return isLibrarySupported;
        }

        public CreaturePlugin(TargetInfo Target)
        {
            PublicIncludePaths.AddRange(new string[] { "CreaturePlugin/Public", });
            PrivateIncludePaths.AddRange(new string[] { "CreaturePlugin/Private", });

            PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

            PrivateDependencyModuleNames.AddRange(new string[] { "RHI", "RenderCore", "ShaderCore" });

            LoadCreatureLib(Target);
        }
    }
}