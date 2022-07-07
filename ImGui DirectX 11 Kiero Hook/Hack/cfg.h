#pragma once
#include "singleton.h"


inline namespace Configuration
{
	class Settings : public Singleton<Settings>
	{
	public:
		/*透视选项*/
		bool P_Menu				=				true;//菜单开关
		bool P_DrawBox			=				false;//方框透视
		ImVec4 C_DrawBoxColor	=				ImColor(0.0f, 0.0f, 0.0f); //方框颜色 R,G,B
		bool P_Skeleton			=				false;//骨骼开关
		ImVec4 C_SkeletonColor	=				ImColor(0.0f, 0.0f, 0.0f);//骨骼颜色 R,G,B
		bool P_MagicBullet		=				false;//追踪开关
		ImVec4 C_VisibleColor	=				ImColor(0.0f, 0.0f, 0.0f); //可视骨骼颜色 R,G,B
		ImVec4 C_UnVisibleColor =				ImColor(0.0f, 0.0f, 0.0f); //不可视骨骼颜色 R,G,B
		bool P_DrawDist			=				false; //绘制距离
		
		bool P_DrawRay			=				false;//绘制射线
		ImVec4 C_DrawRayCcolor	=				ImColor(0.0f, 0.0f, 0.0f); //射线颜色
		/*自苗选项*/
		bool P_Aimbot			=				false;//自苗开关
		bool P_SHOW_FOV			=				false ;//FOV开关
		float AimbotFOV			=				0.f;//FOV大小
		/*魔法阵绘制*/
		float pointX1 = 1.f;
		float pointY1 = 1.f;
		float pointX2 = 1.f;
		float pointY2 = 1.f;
		float pointX3 = 1.f;
		float pointY3 = 1.f;

	};
#define CFG Configuration::Settings::Get()
}