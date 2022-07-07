#pragma once

#ifndef BLOODHUNT_H

#include <windows.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <iostream>
#include <dwmapi.h>
#include  <d3d9.h>
#include  <d3dx9.h>

#include "singleton.h"
#include "../Utils/vector.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "dwmapi.lib")
inline namespace Mycsgo
{
	class Variables : public Singleton<Variables>
	{
	public:
		uint64_t MOUDLE = NULL;

		uint64_t UWorld = NULL;
		uint64_t Matrix = NULL;
		uint64_t GName = NULL;
		uint64_t Ulever = NULL;
		uint64_t Count = NULL;
		uint64_t Actor = NULL;
		uint64_t GameInstance = NULL;
		uint64_t LocalPlayers = NULL;
		uint64_t PlayerController = NULL;
		uint64_t ACharacter = NULL;
		uint64_t Vector3d = NULL;
		uint64_t RootComponent = NULL;
		uint64_t CameraManager = NULL; //相机
		_D3DMATRIX2 ViewWorld;
		Vector3 CameraPos;
		float health;
		float WindowWidth;
		float WindowHeight;
		float ScreenWidth;
		float ScreenHeight;
		//void initAddress()
		//{
		//	if (!this->MOUDLE)
		//		this->MOUDLE = (ULONG64)GetModuleHandle(NULL);
		//	this->UWorld = Memory::Read<uint64_t>(this->MOUDLE + OFFSETS.OFFSET_UWorld);
		//	this->GName = Memory::Read<uint64_t>(this->MOUDLE + OFFSETS.OFFSET_GName);
		//	this->Matrix = Memory::Read<uintptr_t>(Memory::Read<uintptr_t>(this->MOUDLE + OFFSETS.OFFSET_Matrix) + 0x20) + 0x280;
		//	this->Ulever = Memory::Read<uint64_t>(this->UWorld + OFFSETS.Offset_Ulever);
		//	this->Actor = Memory::Read<uint64_t>(Ulever + OFFSETS.Offset_Actor);
		//	this->Count = Memory::Read<DWORD>(this->Ulever + OFFSETS.Offset_Count);
		//	this->GameInstance = Memory::Read<uint64_t>(this->UWorld + OFFSETS.Offset_GameInstance);
		//	this->LocalPlayers = Memory::Read<uint64_t>(this->GameInstance + OFFSETS.Offset_LocalPlayers); //localplayer
		//	this->LocalPlayers = Memory::Read<uint64_t>(this->LocalPlayers); //localplayer 读两层
		//	this->PlayerController = Memory::Read<uint64_t>(this->LocalPlayers + OFFSETS.Offset_PlayerController);
		//	this->ACharacter = Memory::Read<uint64_t>(this->PlayerController + OFFSETS.Offset_ACharacter); //or APawn
		//	this->RootComponent = Memory::Read<uint64_t>(this->ACharacter + OFFSETS.Offset_RootComponent);
		//}
	};
#define GameVars Mycsgo::Variables::Get()
	class Offsets : public Singleton<Offsets>
	{
	public:
		uint64_t OFFSET_UWorld = 0x81A24B8;
		uint64_t OFFSET_Matrix = 0x07CBA590;
		uint64_t OFFSET_GName = 0x8011500;
		uintptr_t Offset_Ulever = 0x30;
		uintptr_t Offset_Count = 0xA0;
		uintptr_t Offset_Actor = 0x98;
		uintptr_t Offset_GameInstance = 0x188;
		uintptr_t Offset_LocalPlayers = 0x38;
		uintptr_t Offset_PlayerController = 0x30;
		uintptr_t Offset_ACharacter = 0x260;
		uintptr_t Offset_Vector3d = 0x1D0;
		uintptr_t Offset_RootComponent = 0x130;
		/*骨骼*/
		uintptr_t Offset_CharacterMesh = 0x280; //ACharacter->CharacterMesh0
		uintptr_t Offset_CompnentToWorld = 0x1C0; //骨骼旋转 1C0固定
		uintptr_t Offset_Mesh1 = 0x450;
		uintptr_t Offset_Mesh2 = 0x460;
		/*相机*/
		uintptr_t Offset_CameraManager = 0x2B8;
		uintptr_t Offset_CamerCache = 0x2040;
		uintptr_t Offset_CameraPOS = 0xE60;
		uintptr_t Offset_health = 0x6F4;
	};
#define GameOffset Mycsgo::Offsets::Get()
}
#endif  !BLOODHUNT_H