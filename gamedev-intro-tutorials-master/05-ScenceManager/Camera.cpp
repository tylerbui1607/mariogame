#include "Camera.h"
#include "Camera.h"


Camera* Camera::__instance = NULL;
Camera::Camera()
{
	cam_y = 240;
}

void Camera::Update(DWORD dt)
{
	if (IsFollowingMario)
	{
		SetCamSpeed();
	}
	else
		cam_vy = 0;
	cam_y += cam_vy * dt;
	floor(cam_y);
}
