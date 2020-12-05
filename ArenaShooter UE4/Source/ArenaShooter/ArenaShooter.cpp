// Fill out your copyright notice in the Description page of Project Settings.

#include "ArenaShooter.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, ArenaShooter, "ArenaShooter" );

#if GAME_LOG_DEFINED
DEFINE_LOG_CATEGORY(GameLog);
#endif