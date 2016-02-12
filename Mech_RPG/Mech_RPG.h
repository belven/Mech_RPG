// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#ifndef __MECH_RPG_H__
#define __MECH_RPG_H__

#pragma once
#include "Engine.h"

#define MIN(a,b) (a < b) ? (a) : (b)
#define MAX(a,b) (a > b) ? (a) : (b)

#include "Engine.h"
#include "Taunt.h"
#include "Heal.h"
#include "Snipe.h"
#include "Grenade.h"
#include "Disable.h"
#include "Stun.h"
#include "DamageBoost.h"
#include "OrbitalStrike.h"
#include "Immobilise.h"
#include "ChannelledAbility.h"
#include "CritBoost.h"
#include "TimedHealthChange.h"
#include "AoEHeal.h"
#include "DefenceBoost.h"
#include "ParticleBomb.h"
#include "SummonDamageDrone.h"

#include "AOEHealthChange.h"

#include "Cover.h"

//#include "Mech_RPGCharacter.h"
//#include "Mech_RPGPlayerController.h"
//#include "BaseAIController.h"
//#include "AllyAIController.h"

#include "MagazineWeapon.h"
#include "OverHeatWeapon.h"
#include "MiscLibrary.h"
#include "Armour.h"

#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/UMGStyle.h"
#include "Runtime/UMG/Public/Slate/SObjectWidget.h"
#include "Runtime/UMG/Public/IUMGModule.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/Components/WidgetComponent.h"

DECLARE_LOG_CATEGORY_EXTERN(LogMech_RPG, Log, All);


#endif
