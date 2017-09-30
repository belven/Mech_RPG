// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class Mech_RPGTarget : TargetRules
{
	public Mech_RPGTarget(TargetInfo Target) : base(Target)
    {
		Type = TargetType.Game;
        ExtraModuleNames.Add("Mech_RPG");
    }
}
