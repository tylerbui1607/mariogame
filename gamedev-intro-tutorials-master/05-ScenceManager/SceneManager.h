#pragma once
#include <Windows.h>
#include "Utils.h"
class SceneManager
{
public:
	int SceneID;
	static SceneManager* __instance;

	static SceneManager* SceneManager::GetInstance()
	{
		if (__instance == NULL) __instance = new SceneManager();
		return __instance;
	}
	SceneManager() {};
};

