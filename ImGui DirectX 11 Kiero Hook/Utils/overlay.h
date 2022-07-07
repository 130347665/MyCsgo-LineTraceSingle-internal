#include <windows.h>
#include <tchar.h>
#include <memory>
#include <thread>
#include <functional>
#include <array>
#include <dwmapi.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <string>
#include <sstream>

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx11.h"
#include "../imgui/imgui_impl_win32.h"

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"dwmapi.lib")
#pragma comment(lib,"d3dcompiler") 

using namespace std;
float pi = 3.14159265358979323846;

ImFont* Verdana, * DefaultFont;

typedef struct
{
	DWORD R;
	DWORD G;
	DWORD B;
	DWORD A;
}RGBA;

std::string string_To_UTF8(const std::string& str)
{
	int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
	wchar_t* pwBuf = new wchar_t[nwLen + 1];
	ZeroMemory(pwBuf, nwLen * 2 + 2);
	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);
	int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);
	char* pBuf = new char[nLen + 1];
	ZeroMemory(pBuf, nLen + 1);
	::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);
	std::string retStr(pBuf);
	delete[]pwBuf;
	delete[]pBuf;
	pwBuf = NULL;
	pBuf = NULL;
	return retStr;
}

auto DrawCrossHair(const FLOAT aSize, ImU32 aColor)-> VOID
{
	auto vList = ImGui::GetOverlayDrawList();

	vList->AddLine({ GameVars.ScreenWidth ,GameVars.ScreenHeight - (aSize + 1) }, { GameVars.ScreenWidth ,GameVars.ScreenHeight + (aSize + 1) }, aColor, 2);
	vList->AddLine({ GameVars.ScreenWidth - (aSize + 1),GameVars.ScreenHeight }, { GameVars.ScreenWidth + (aSize + 1)  ,GameVars.ScreenHeight }, aColor, 2);
}

auto DrawLine(const ImVec2& aPoint1, const ImVec2 aPoint2, ImU32 aColor, const FLOAT aLineWidth) -> VOID
{
	auto vList = ImGui::GetOverlayDrawList();
	vList->AddLine(aPoint1, aPoint2, aColor, aLineWidth);
}
void DrawLine2(int x, int y, int x1, int y1, ImU32 color, float thickness)
{

	ImGui::GetOverlayDrawList()->AddLine(ImVec2(x, y), ImVec2(x1, y1), color, thickness);
}

auto DrawBox(float x, float y, float w, float h, ImColor color)-> VOID
{
	DrawLine(ImVec2(x, y), ImVec2(x + w, y), color, 1.3f); // top 
	DrawLine(ImVec2(x, y - 1.3f), ImVec2(x, y + h + 1.4f), color, 1.3f); // left
	DrawLine(ImVec2(x + w, y - 1.3f), ImVec2(x + w, y + h + 1.4f), color, 1.3f);  // right
	DrawLine(ImVec2(x, y + h), ImVec2(x + w, y + h), color, 1.3f);   // bottom 
}
auto RectFilled(float x0, float y0, float x1, float y1, ImColor color, float rounding, int rounding_corners_flags)-> VOID
{
	auto vList = ImGui::GetOverlayDrawList();
	vList->AddRectFilled(ImVec2(x0, y0), ImVec2(x1, y1), color, rounding, rounding_corners_flags);
}

#define max(a,b)            (((a) > (b)) ? (a) : (b))
#define min(a,b)            (((a) < (b)) ? (a) : (b))

auto HealthBar(float x, float y, float w, float h, int phealth, bool Outlined)-> VOID
{
	auto vList = ImGui::GetOverlayDrawList();

	int healthValue = max(0, min(phealth, 100));

	ImColor barColor = ImColor
	(
		min(510 * (100 - healthValue) / 100, 255), min(510 * healthValue / 100, 255),
		25,
		255
	);
	if (Outlined)
		vList->AddRect(ImVec2(x - 1, y - 1), ImVec2(x + w + 1, y + h + 1), ImColor(0.f, 0.f, 0.f), 0.0f, 0, 1.0f);

	RectFilled(x, y, x + w, y + (int)(((float)h / 100.0f) * (float)phealth), barColor, 0.0f, 0);
}

auto DrawOutlinedText(ImFont* pFont, const std::string& text, const ImVec2& pos, float size, ImU32 color, bool center) -> VOID
{
	ImGui::PushFont(Verdana);
	std::stringstream stream(text);
	std::string line;

	float y = 0.0f;
	int index = 0;

	while (std::getline(stream, line))
	{
		ImVec2 textSize = pFont->CalcTextSizeA(size, FLT_MAX, 0.0f, line.c_str());

		if (center)
		{
			ImGui::GetOverlayDrawList()->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) + 1, (pos.y + textSize.y * index) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
			ImGui::GetOverlayDrawList()->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) - 1, (pos.y + textSize.y * index) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
			ImGui::GetOverlayDrawList()->AddText(pFont, size, ImVec2(pos.x - textSize.x / 2.0f, pos.y + textSize.y * index), ImGui::GetColorU32(color), line.c_str());
		}
		else
		{//
			ImGui::GetOverlayDrawList()->AddText(pFont, size, ImVec2((pos.x) + 1, (pos.y + textSize.y * index) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
			ImGui::GetOverlayDrawList()->AddText(pFont, size, ImVec2((pos.x) - 1, (pos.y + textSize.y * index) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
			ImGui::GetOverlayDrawList()->AddText(pFont, size, ImVec2(pos.x, pos.y + textSize.y * index), ImGui::GetColorU32(color), line.c_str());
		}

		y = pos.y + textSize.y * (index + 1);
		index++;
	}
	ImGui::PopFont();
}

auto DrawCorneredBox(float X, float Y, float W, float H, const ImU32& color, float thickness) -> VOID
{
	auto vList = ImGui::GetOverlayDrawList();

	float lineW = (W / 3);
	float lineH = (H / 3);
	//black outlines
	auto col = ImGui::GetColorU32(color);

	//corners
	vList->AddLine(ImVec2(X, Y - thickness / 2), ImVec2(X, Y + lineH), col, thickness);//top left
	vList->AddLine(ImVec2(X - thickness / 2, Y), ImVec2(X + lineW, Y), col, thickness);

	vList->AddLine(ImVec2(X + W - lineW, Y), ImVec2(X + W + thickness / 2, Y), col, thickness);//top right horizontal
	vList->AddLine(ImVec2(X + W, Y - thickness / 2), ImVec2(X + W, Y + lineH), col, thickness);

	vList->AddLine(ImVec2(X, Y + H - lineH), ImVec2(X, Y + H + (thickness / 2)), col, thickness);//bot left
	vList->AddLine(ImVec2(X - thickness / 2, Y + H), ImVec2(X + lineW, Y + H), col, thickness);

	vList->AddLine(ImVec2(X + W - lineW, Y + H), ImVec2(X + W + thickness / 2, Y + H), col, thickness);//bot right
	vList->AddLine(ImVec2(X + W, Y + H - lineH), ImVec2(X + W, Y + H + (thickness / 2)), col, thickness);

}

auto DrawCircle(const ImVec2& aPoint, const FLOAT aR, ImU32 aColor, const FLOAT aLineWidth) -> VOID
{
	auto vList = ImGui::GetOverlayDrawList();
	vList->AddCircle(aPoint, aR, aColor, 120, aLineWidth);
}
auto DrawCircle(float x, float y, float radius, ImVec4 color, int segments)-> VOID
{
	auto vList = ImGui::GetOverlayDrawList();
	vList->AddCircle(ImVec2(x, y), radius, ImGui::ColorConvertFloat4ToU32(color), segments);
}
auto DrawString(const ImVec2& aPos, const std::string& aString, ImU32 aColor) -> VOID
{
	auto vList = ImGui::GetOverlayDrawList();
	vList->AddText(aPos, aColor, aString.data());
}
auto TabButton(const char* label, int* index, int val, bool sameline) -> VOID
{
	if (*index == val)
	{
		if (ImGui::Button(label, ImVec2(80, 25)))
			*index = val;
		if (sameline)
			ImGui::SameLine();
	}
	else
	{
		if (ImGui::Button(label, ImVec2(80, 25)))
			*index = val;
		if (sameline)
			ImGui::SameLine();
	}
}
void Draw3D4(Vector3 PlayerPos, float Angle, float range ,ImU32 col) //绘制魔法阵
{
	const int count = 6; //变边数
	Vector3 tmpxyz;
	Vector2 tmp_bottom;
	Vector2 draw[count]{};//数组初始化清空

	float rad = ((float)0 + 1.0f) * 360.0f / count;
	tmpxyz.x = PlayerPos.x + cos((Angle + rad) * pi / 180) * range;
	tmpxyz.x = PlayerPos.x + sin((Angle + rad) * pi / 180) * range;
	tmpxyz.z = PlayerPos.z;

	draw[0] = tmp_bottom;
	printf("%f\n",draw[0].x);
	rad = ((float)1 + 1.0f) * 360.f / count;
	tmpxyz.x = PlayerPos.x + cos((Angle + rad) * pi / 180) * range;
	tmpxyz.x = PlayerPos.x + sin((Angle + rad) * pi / 180) * range;
	tmpxyz.z = PlayerPos.z;

	draw[1] = tmp_bottom;
	rad = ((float)2 + 1.0f) * 360.f / count;
	tmpxyz.x = PlayerPos.x + cos((Angle + rad) * pi / 180) * range;
	tmpxyz.x = PlayerPos.x + sin((Angle + rad) * pi / 180) * range;
	tmpxyz.z = PlayerPos.z;

	draw[2] = tmp_bottom;
	rad = ((float)3 + 1.0f) * 360.f / count;
	tmpxyz.x = PlayerPos.x + cos((Angle + rad) * pi / 180) * range;
	tmpxyz.x = PlayerPos.x + sin((Angle + rad) * pi / 180) * range;
	tmpxyz.z = PlayerPos.z;

	draw[3] = tmp_bottom;
	rad = ((float)4 + 1.0f) * 360.f / count;
	tmpxyz.x = PlayerPos.x + cos((Angle + rad) * pi / 180) * range;
	tmpxyz.x = PlayerPos.x + sin((Angle + rad) * pi / 180) * range;
	tmpxyz.z = PlayerPos.z;

	draw[4] = tmp_bottom;
	rad = ((float)5 + 1.0f) * 360.f / count;
	tmpxyz.x = PlayerPos.x + cos((Angle + rad) * pi / 180) * range;
	tmpxyz.x = PlayerPos.x + sin((Angle + rad) * pi / 180) * range;
	tmpxyz.z = PlayerPos.z;

	draw[5] = tmp_bottom;
	//六芒星
	if (draw[0].x > 0 && draw[0].y>0 && draw[2].x>0 && draw[2].x>0)
	{
		DrawLine(ImVec2(draw[0].x, draw[0].y), ImVec2(draw[2].x, draw[2].y), col, 1.3f);
	}
	if (draw[2].x > 0 && draw[2].y > 0 && draw[4].x > 0 && draw[4].x > 0)
	{
		DrawLine(ImVec2(draw[2].x, draw[2].y), ImVec2(draw[4].x, draw[4].y), col, 1.3f);
	}
	if (draw[4].x > 0 && draw[4].y > 0 && draw[0].x > 0 && draw[0].x > 0)
	{
		DrawLine(ImVec2(draw[4].x, draw[4].y), ImVec2(draw[0].x, draw[0].y), col, 1.3f);
	}
	if (draw[1].x > 0 && draw[1].y > 0 && draw[3].x > 0 && draw[3].x > 0)
	{
		DrawLine(ImVec2(draw[1].x, draw[1].y), ImVec2(draw[3].x, draw[3].y), col, 1.3f);
	}
	if (draw[3].x > 0 && draw[3].y > 0 && draw[5].x > 0 && draw[5].x > 0)
	{
		DrawLine(ImVec2(draw[3].x, draw[3].y), ImVec2(draw[5].x, draw[5].y), col, 1.3f);
	}
	if (draw[5].x > 0 && draw[5].y > 0 && draw[1].x > 0 && draw[1].x > 0)
	{
		DrawLine(ImVec2(draw[5].x, draw[5].y), ImVec2(draw[1].x, draw[1].y), col, 1.3f);
	}
}