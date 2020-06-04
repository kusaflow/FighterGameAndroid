// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class FighterTarget : TargetRules
{
	public FighterTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "Fighter" } );

        if (Target.Platform == UnrealTargetPlatform.Android)
        {
            ExtraModuleNames.Add("OnlineSubsystemGooglePlay");
            ExtraModuleNames.Add("OnlineSubsystem");
            ExtraModuleNames.Add("AndroidAdvertising");
        }

    }
}
