#pragma once
#include "singleton.h"


inline namespace Configuration
{
	class Settings : public Singleton<Settings>
	{
	public:
		/*͸��ѡ��*/
		bool P_Menu				=				true;//�˵�����
		bool P_DrawBox			=				false;//����͸��
		ImVec4 C_DrawBoxColor	=				ImColor(0.0f, 0.0f, 0.0f); //������ɫ R,G,B
		bool P_Skeleton			=				false;//��������
		ImVec4 C_SkeletonColor	=				ImColor(0.0f, 0.0f, 0.0f);//������ɫ R,G,B
		bool P_MagicBullet		=				false;//׷�ٿ���
		ImVec4 C_VisibleColor	=				ImColor(0.0f, 0.0f, 0.0f); //���ӹ�����ɫ R,G,B
		ImVec4 C_UnVisibleColor =				ImColor(0.0f, 0.0f, 0.0f); //�����ӹ�����ɫ R,G,B
		bool P_DrawDist			=				false; //���ƾ���
		
		bool P_DrawRay			=				false;//��������
		ImVec4 C_DrawRayCcolor	=				ImColor(0.0f, 0.0f, 0.0f); //������ɫ
		/*����ѡ��*/
		bool P_Aimbot			=				false;//���翪��
		bool P_SHOW_FOV			=				false ;//FOV����
		float AimbotFOV			=				0.f;//FOV��С
		/*ħ�������*/
		float pointX1 = 1.f;
		float pointY1 = 1.f;
		float pointX2 = 1.f;
		float pointY2 = 1.f;
		float pointX3 = 1.f;
		float pointY3 = 1.f;

	};
#define CFG Configuration::Settings::Get()
}