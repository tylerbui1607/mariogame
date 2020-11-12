#pragma once
#include <Windows.h>
#define SCREEN_WIDTH 700
#define SCREEN_HEIGHT 250
class Camera
{
public:
	float cam_x, cam_y, cam_vx, cam_vy;
	bool IsFollowingMario;
public:
	static Camera* __instance;

	static Camera* Camera::GetInstance()
	{
		if (__instance == NULL) __instance = new Camera();
		return __instance;
	}
	Camera();
	void SetCamSpeed(float vx, float vy)
	{
		cam_vx = vx;
		cam_vy = vy;
	}
	bool IsInCam(float objX, float objY)
	{
		if (objX < cam_x || objX - cam_x>SCREEN_WIDTH || objY < cam_y)
			return false;
		else
			return true;
	}
	void SetCamPos(float x, float y);
	void Update( DWORD dt);
};

