#include "Camera.h"
#include "Camera.h"


Camera* Camera::__instance = NULL;
Camera::Camera()
{
}

void Camera::Update(DWORD dt)
{
	if (IsFollowingMario)
	{
		cam_x += cam_vx * dt;
		cam_y += cam_vy * dt;
	}
}
