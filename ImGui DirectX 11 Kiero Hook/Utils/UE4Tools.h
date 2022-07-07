#pragma once


struct FMinimalViewInfo
{
	class Vector3                                            Location;
	class Vector3                                            Rotation;
	float                                              FOV;
	float                                              OrthoWidth;
	float                                              OrthoNearClipPlane;
	float                                              OrthoFarClipPlane;
	float                                              AspectRatio;
};

struct FCameraCacheEntry
{
	float                                              TimeStamp;
	unsigned char                                      UnknownData00[0xC];
	struct FMinimalViewInfo                            POV;
};
typedef struct _EntityList {
	uint64_t Obj;
	int AimLocation;
}EntityList;

const char* GetFromFName(const int key)
{
	DWORD chunkOffset = ((int)(key) >> 16); // Block
	WORD nameOffset = (WORD)key;
	auto namePoolChunk = Memory::Read<uint64_t>(GameVars.GName + ((chunkOffset + 2) * 8));
	auto entryOffset = namePoolChunk + (DWORD)(2 * nameOffset);
	INT16 nameLength = Memory::Read<INT16>(entryOffset) >> 6;
	if (nameLength > 256)nameLength = 255;
	char cBuf[256] = {};
	RtlCopyMemory(&cBuf, (void*)(entryOffset + 2), nameLength);
	cBuf[nameLength] = 0;
	return cBuf;
}

bool WorldToScreen(const class Vector3& WorldLocation, Vector4* Rect,Vector2* vec2)
{
	Vector3 ViewW = Vector3(0.0f, 0.0f, 0.0f);
	float ViewY2 = 0.0f, ItemY = 0.0f;
	ViewW.z = GameVars.ViewWorld._14 * WorldLocation.x + GameVars.ViewWorld._24 * WorldLocation.y + GameVars.ViewWorld._34 * WorldLocation.z + GameVars.ViewWorld._44;
	//printf("ViewWorld._14:%f\n", GameVars.ViewWorld._14);
	//printf("WorldLocation.X:%f\n", WorldLocation.x);
	if (ViewW.z <= 0.01f) { return false; }
	ViewW.z = 1 / ViewW.z;
	ViewW.x = GameVars.ScreenWidth + (GameVars.ViewWorld._11 * WorldLocation.x + GameVars.ViewWorld._21 * WorldLocation.y + GameVars.ViewWorld._31 * WorldLocation.z + GameVars.ViewWorld._41) * ViewW.z * GameVars.ScreenWidth;
	ViewW.y = GameVars.ScreenHeight - (GameVars.ViewWorld._12 * WorldLocation.x + GameVars.ViewWorld._22 * WorldLocation.y + GameVars.ViewWorld._32 * (WorldLocation.z - 90.0f) + GameVars.ViewWorld._42) * ViewW.z * GameVars.ScreenHeight;
	ViewY2 = GameVars.ScreenHeight - (GameVars.ViewWorld._12 * WorldLocation.x + GameVars.ViewWorld._22 * WorldLocation.y + GameVars.ViewWorld._32 * (WorldLocation.z + 90.0f) + GameVars.ViewWorld._42) * ViewW.z * GameVars.ScreenHeight;
	ItemY = GameVars.ScreenHeight - (GameVars.ViewWorld._12 * WorldLocation.x + GameVars.ViewWorld._22 * WorldLocation.y + GameVars.ViewWorld._32 * WorldLocation.z + GameVars.ViewWorld._42) * ViewW.z * GameVars.ScreenHeight;
	Rect->x = ViewW.x - (ViewY2 - ViewW.y) / 4;
	Rect->y = ViewW.y;
	Rect->w = (ViewY2 - ViewW.y) / 2;
	Rect->h = ViewY2 - ViewW.y;
	vec2->x = ViewW.x - GameVars.ScreenWidth;
	vec2->y = ViewY2 + (ViewW.x - ViewY2) / 3 - GameVars.ScreenHeight;
	return true;
}

bool BoneToScreen(const class Vector3& WorldLocation, Vector2* Rect)
{
	Vector3 ViewW = Vector3(0.0f, 0.0f, 0.0f);
	float ViewY2 = 0.0f, ItemY = 0.0f, Box = 0.0f;
	ViewW.z = GameVars.ViewWorld._14 * WorldLocation.x + GameVars.ViewWorld._24 * WorldLocation.y + GameVars.ViewWorld._34 * WorldLocation.z + GameVars.ViewWorld._44;
	if (ViewW.z <= 0.01f) { return false; }
	ViewW.z = 1 / ViewW.z;
	ViewW.x = GameVars.ScreenWidth + (GameVars.ViewWorld._11 * WorldLocation.x + GameVars.ViewWorld._21 * WorldLocation.y + GameVars.ViewWorld._31 * WorldLocation.z + GameVars.ViewWorld._41) * ViewW.z * GameVars.ScreenWidth;
	ViewW.y = GameVars.ScreenHeight - (GameVars.ViewWorld._12 * WorldLocation.x + GameVars.ViewWorld._22 * WorldLocation.y + GameVars.ViewWorld._32 * WorldLocation.z + GameVars.ViewWorld._42) * ViewW.z * GameVars.ScreenHeight;
	Rect->x = ViewW.x;
	Rect->y = ViewW.y;
	return true;
}

Vector3 GetBoneFTransform(ULONG64 Mesh, const int Id)
{
	ULONG64 BoneActor = Memory::Read<ULONG64>(Mesh + GameOffset.Offset_Mesh1);
	if (BoneActor == 0x0)
	{
		BoneActor = Memory::Read<ULONG64>(Mesh + GameOffset.Offset_Mesh2);
	}
	FTransform lpFTransform = Memory::Read<FTransform>(BoneActor + Id * 0x30);
	FTransform ComponentToWorld = Memory::Read<FTransform>(Mesh + GameOffset.Offset_CompnentToWorld);
	D3DMATRIX2 Matrix = FTransform::MatrixMultiplication(lpFTransform.ToMatrixWithScale(), ComponentToWorld.ToMatrixWithScale());

	return Vector3(Matrix._41, Matrix._42, Matrix._43);
}


bool LineOfSightTo(ULONG64 PlayerController, ULONG64 Other, Vector3 ViewPoint, bool bAlternateChecks)
{
	//SCUM.exe+34AB110 - 40 55                 - push rbp
	auto function = reinterpret_cast<bool(__fastcall*)(ULONG64, ULONG64, Vector3, bool)>(GameVars.MOUDLE + 0x4504360);

	return function(PlayerController, Other, ViewPoint, bAlternateChecks);

}
//static bool LineTraceSingle(const UObject* WorldContextObject, const FVector Start, const FVector End, ETraceTypeQuery TraceChannel, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType, FHitResult& OutHit, bool bIgnoreSelf, FLinearColor TraceColor = FLinearColor::Red, FLinearColor TraceHitColor = FLinearColor::Green, float DrawTime = 5.0f);

bool LineTraceSingle(Vector3 Start, Vector3 End)
{

	struct TArray
	{
		ULONG64 Actor;
		int Num;
		int MaxNum;

	};

	auto function = reinterpret_cast<bool(__fastcall*)(
		ULONG64 WorldContextObject,
		Vector3 Start,
		Vector3 End,
		ETraceTypeQuery TraceChannel,
		bool bTraceComplex,
		TArray*,
		EDrawDebugTrace DrawDebugType,
		FHitResult * OutHit,
		bool bIgnoreSelf,
		FLinearColor TraceColor,
		FLinearColor TraceHitColor,
		float DrawTime
		)>(GameVars.MOUDLE + 0x47759F0);

	FHitResult lpFHitResult = {};

	return !function(GameVars.UWorld, Start, End, ETraceTypeQuery::TraceTypeQuery1, true, (TArray*)(GameVars.Ulever + GameOffset.Offset_Actor), EDrawDebugTrace::None, &lpFHitResult, true, FLinearColor(), FLinearColor(), 0.f);


}
int getMouseDistance(Vector2 target) {
	float x = GameVars.ScreenWidth - target.x;
	float y = GameVars.ScreenHeight - target.y;
	int distance = abs(sqrt(x * x + y * y));
	return distance;
}