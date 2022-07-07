//#include "offset.h"
//#include "../Utils/memory.h"
//#include "../Utils/vector.h"
//#include <windows.h>

#include "../includes.h"
#include "../Utils/UE4Tools.h"
#include "../Utils/overlay.h"
#include "cfg.h"
#include <vector>
std::vector<EntityList> entityList; //声明人物数组
template<class T>
string to_string(T value)
{
	char str[64];
	int len = ::sprintf(str, "%lld", value);
	return string(str, len);
}
bool initAddress()
{

	if (GameVars.MOUDLE == NULL)
		GameVars.MOUDLE = (ULONG64)GetModuleHandle(NULL);
	GameVars.UWorld = Memory::Read<uint64_t>(GameVars.MOUDLE + GameOffset.OFFSET_UWorld);
	GameVars.GName = GameVars.MOUDLE + GameOffset.OFFSET_GName; // 给个地址 不用读
	GameVars.Matrix = Memory::Read<uint64_t>(Memory::Read<uintptr_t>(GameVars.MOUDLE + GameOffset.OFFSET_Matrix) + 0x20) + 0x270;
	GameVars.Ulever = Memory::Read<uint64_t>(GameVars.UWorld + GameOffset.Offset_Ulever);
	GameVars.Actor = Memory::Read<uint64_t>(GameVars.Ulever + GameOffset.Offset_Actor);
	GameVars.Count = Memory::Read<DWORD>(GameVars.Ulever + GameOffset.Offset_Count);
	GameVars.GameInstance = Memory::Read<uint64_t>(GameVars.UWorld + GameOffset.Offset_GameInstance);
	GameVars.LocalPlayers = Memory::Read<uint64_t>(GameVars.GameInstance + GameOffset.Offset_LocalPlayers); //localplayer
	GameVars.LocalPlayers = Memory::Read<uint64_t>(GameVars.LocalPlayers); //localplayer 读两层
	GameVars.PlayerController = Memory::Read<uint64_t>(GameVars.LocalPlayers + GameOffset.Offset_PlayerController);
	GameVars.ACharacter = Memory::Read<uint64_t>(GameVars.PlayerController + GameOffset.Offset_ACharacter); //or APawn
	GameVars.RootComponent = Memory::Read<uint64_t>(GameVars.ACharacter + GameOffset.Offset_RootComponent);
	GameVars.ViewWorld = Memory::Read<_D3DMATRIX2>(GameVars.Matrix);
	GameVars.CameraManager = Memory::Read<uint64_t>(GameVars.PlayerController + GameOffset.Offset_CameraManager);

	//cout << ViewWorld._11 << endl; //5-31 更改到这
	return true;
}

static int bonelist[][2]
{
	{6,1},//脊柱

	{6,10},//左手
	{10,11},
	{11,12},

	{6,75},//右手
	{75,45},
	{45,46},

	{1,79},//左腿
	{79,80},
	{80,81},

	{1,86}, //右腿
	{86,87},
	{87,88}

};
void ESP()
{
	Vector2 Vec2 = Vector2();
	Vector4 Rect = Vector4();
	Vector2 Rect_0 = Vector2();
	int aimLoc;//自瞄部位
	uint64_t target_entity=NULL;
	if (!initAddress()) return; //如果地址没初始化成功 就不执行
	for (int i = 0; i < GameVars.Count; i++)
	{
		//printf("Count:%d\n", GameVars.Count);
		ULONG64 Object = Memory::Read<ULONG64>(GameVars.Actor + i * 8); //遍历Object 偏移8
		DWORD dwObjectId = Memory::Read<ULONG64>(Object + 0x18); //拿到ObjectID 
		const char* ClassName = GetFromFName(dwObjectId);//拿到类名
		float target_dist = FLT_MAX;
		//printf("类名:%s\n", ClassName);
		if (Object == GameVars.ACharacter) continue;
		if (strcmp(ClassName,"BP_AI_Bot_CT_C") == 0 || strcmp(ClassName, "BP_AI_Bot_T_C") == 0 || strcmp(ClassName, "BP_AI_Bot_C") == 0) //如果类名 等于 Players 继续向下执行
		{
			GameVars.health = Memory::Read<float>(Object + GameOffset.Offset_health);
			
			if (GameVars.health <= 0) continue;
				//printf("Health:%f\n", GameVars.health); 
				//printf("对象类名:%s \n", ClassName);
				ULONG64 ObjectRootComp = Memory::Read<ULONG64>(Object + GameOffset.Offset_RootComponent); //对象的RootComponent
				Vector3 ObjectVector = Memory::Read<Vector3>(ObjectRootComp + GameOffset.Offset_Vector3d); //对象的坐标 X,Y,Z
				//printf("%f",ObjectVector.x); //打印坐标
				Vector3 LocalVector = Memory::Read<Vector3>(GameVars.RootComponent + GameOffset.Offset_Vector3d); //自身的坐标 X,Y,Z
				float Dist = LocalVector.Distance(ObjectVector) / 100.f;
				bool b_Rect = WorldToScreen(ObjectVector, &Rect,&Vec2); //三维转屏幕坐标 返回 框Rect - b_Rect是否成功 成功True 错误 False
			
				if (b_Rect && CFG.P_DrawBox) //如果把方框透视打开 并且 b_Rect 返回True
				{

					char buffer[128] = {};
					sprintf_s(buffer, sizeof(buffer) / sizeof(char), "%llX  %s[%.fm]", Object, ClassName, Dist);
					DrawString(ImVec2(Rect.x, Rect.y), ClassName, ImColor(255, 255, 255));
					DrawBox(Rect.x, Rect.y, Rect.w, Rect.h, CFG.C_DrawBoxColor);
					//Draw3D4(ObjectVector, 100.f, 30, ImColor(255,255,255,255));

				}

				//if (WorldToScreen(ObjectVector, &Rect))
				//{
				//	
				//	//DrawOutlinedText(Verdana, buffer, ImVec2(Rect.x, Rect.y), 14.0f, ImColor(255, 255, 255), true);
				//}

				if (b_Rect && CFG.P_Skeleton) //绘制骨骼
				{
					Vector2 rect_0 = Vector2(); Vector2 rect_1 = Vector2();
					for (int i = 0; i < 14; ++i)
					{

						auto Start = GetBoneFTransform(Memory::Read<ULONG64>(Object + GameOffset.Offset_CharacterMesh), bonelist[i][0]);
						auto End = GetBoneFTransform(Memory::Read<ULONG64>(Object + GameOffset.Offset_CharacterMesh), bonelist[i][1]);
						if (BoneToScreen(Start, &rect_0) &&
							BoneToScreen(End, &rect_1))
						{
							GameVars.CameraPos = Memory::Read<Vector3>(GameVars.CameraManager + GameOffset.Offset_CameraPOS);
							auto CamerCache = Memory::Read<FCameraCacheEntry>(GameVars.CameraManager + GameOffset.Offset_CamerCache);
							if (LineTraceSingle(CamerCache.POV.Location, Start) || LineTraceSingle(CamerCache.POV.Location, End))
							{
								//DrawString(ImVec2(rect_0.x, rect_0.y), to_string(i).c_str(), ImGui::GetColorU32(CFG.C_VisibleColor)); //绘制骨骼点
								DrawLine(ImVec2(rect_0.x, rect_0.y), ImVec2(rect_1.x, rect_1.y), ImGui::GetColorU32(CFG.C_VisibleColor), 1.3f); //绘制骨骼线
							}
							else
							{
								//DrawString(ImVec2(rect_0.x, rect_0.y), to_string(i).c_str(), ImGui::GetColorU32(CFG.C_UnVisibleColor)); //绘制骨骼点
								DrawLine(ImVec2(rect_0.x, rect_0.y), ImVec2(rect_1.x, rect_1.y), ImGui::GetColorU32(CFG.C_UnVisibleColor), 1.3f);
							}
						}
					}
				}
				if (b_Rect && CFG.P_DrawDist) //绘制距离
				{
					DrawString(ImVec2(Rect.x, Rect.y), to_string((int)Dist).c_str(), ImColor(255, 255, 255));
				}
				if (b_Rect && CFG.P_DrawRay) //绘制射线
				{
					DrawLine(ImVec2(GameVars.ScreenWidth, 0), ImVec2(Rect.x + (Rect.w / 2), Rect.y + Rect.h), ImGui::GetColorU32(CFG.C_DrawRayCcolor), 1.3f);
				}
				if (CFG.P_SHOW_FOV)
				{
					DrawCircle(GameVars.ScreenWidth, GameVars.ScreenHeight, CFG.AimbotFOV, ImColor(245, 181, 209, 255), 0); //绘制FOV
				}
				//DrawMagicArray(ImVec2(CFG.pointX1, CFG.pointY1), ImVec2(CFG.pointX2, CFG.pointY2), ImVec2(CFG.pointX3, CFG.pointY3), ImGui::GetColorU32(CFG.C_DrawRayCcolor), 1.3f);
				if (Rect.x + GameVars.ScreenWidth > GameVars.ScreenWidth - CFG.AimbotFOV / 2.0f && GameVars.ScreenWidth < GameVars.ScreenWidth + CFG.AimbotFOV / 2.0f) //计算敌人使否在园内
				{
					if (Vec2.y + GameVars.ScreenHeight > GameVars.ScreenHeight - CFG.AimbotFOV / 2.0f && GameVars.ScreenHeight < GameVars.ScreenHeight + CFG.AimbotFOV / 2.0f)
					{
						Vector2 BoneRect;
						BoneToScreen(GetBoneFTransform(Memory::Read<ULONG64>(Object + GameOffset.Offset_CharacterMesh), 7), &BoneRect);
						float x = BoneRect.x - GameVars.ScreenWidth;
						float y = BoneRect.y - GameVars.ScreenHeight;
						float crosshair_dist = sqrtf((x * x) + (y * y));
						if (crosshair_dist <= FLT_MAX && crosshair_dist <= target_dist)
						{
							if (crosshair_dist > CFG.AimbotFOV) // FOV
								continue;
							target_dist = crosshair_dist;
							for (int j = 7; j >= 1; j--) {
								Vector2 BoneRect;
								uint64_t Mesh = Memory::Read<ULONG64>(Object + GameOffset.Offset_CharacterMesh);
								//printf("Mesh:%x\n", Mesh);
								auto CamerCache = Memory::Read<FCameraCacheEntry>(GameVars.CameraManager + GameOffset.Offset_CamerCache);
								auto BoneCheck = GetBoneFTransform(Mesh, j);
								if (LineTraceSingle(CamerCache.POV.Location, BoneCheck)) {
									EntityList tmp{};
									tmp.Obj = Object;
									tmp.AimLocation = j;
									entityList.push_back(tmp);
									break;
								}
							}
						}
					}
				}
		}
	}
	int bigMouseDistance = 999999;
	if (CFG.P_Aimbot)
	{

		if (entityList.size() > 0 && GetAsyncKeyState(2) != 0 )
			for (int i = 0; i < entityList.size(); i++) {
				uint64_t obj = entityList[i].Obj;
				Vector2 BoneRect;
				uint64_t Mesh = Memory::Read<ULONG64>(obj + GameOffset.Offset_CharacterMesh);
				auto BoneCheck = GetBoneFTransform(Mesh, entityList[i].AimLocation);
				BoneToScreen(BoneCheck, &BoneRect);
				int smallMouseDistance = getMouseDistance(BoneRect);
				if (bigMouseDistance > smallMouseDistance) {
					target_entity = obj;
					aimLoc = entityList[i].AimLocation;
					bigMouseDistance = smallMouseDistance;
				}
			}
	}
	if (CFG.P_Aimbot && target_entity != 0 && GetAsyncKeyState(2) != 0) 
	{
		if (aimLoc != 0)
		{
			//if (CFG.b_MagicBullet) {
			//	uint64_t Mesh = Read<ULONG64>(target_entity + OFFSET_MeshComponent);
			//	Vector3 AimLocation = GetBoneFTransform(Mesh, aimLoc);
			//	Write<Vector3>(MagicBulletAddr, AimLocation);
			//}
			Vector2 BoneRect;
			uint64_t Mesh = Memory::Read<ULONG64>(target_entity + GameOffset.Offset_CharacterMesh);
			BoneToScreen(GetBoneFTransform(Mesh, aimLoc), &BoneRect);
			mouse_event(1, (BoneRect.x - GameVars.ScreenWidth) / 2, (BoneRect.y - GameVars.ScreenHeight) / 2, 0, 0);

		}
	}
	entityList.clear();
	aimLoc = 0;
	target_entity = 0;
}

