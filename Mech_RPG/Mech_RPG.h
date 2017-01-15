// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#ifndef __MECH_RPG_H__
#define __MECH_RPG_H__

#pragma once
#include "Engine.h"

#define MIN(a,b) (a < b) ? (a) : (b)
#define MAX(a,b) (a > b) ? (a) : (b)

#include "MiscLibrary.h"
#include "CollisionChannels.h"

#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/UMGStyle.h"
#include "Runtime/UMG/Public/Slate/SObjectWidget.h"
#include "Runtime/UMG/Public/IUMGModule.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/Components/WidgetComponent.h"

DECLARE_LOG_CATEGORY_EXTERN(AbilitiesLog, Log, All);
#endif