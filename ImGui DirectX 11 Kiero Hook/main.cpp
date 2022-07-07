#include "includes.h"
#include "Hack/cfg.h"
#include "Hack/HACK.h"
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Present oPresent;
HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;



void InitImGui()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(pDevice, pContext);
}

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

bool init = false;
HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (!init)
	{
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)& pDevice)))
		{
			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			window = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)& pBackBuffer);
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
			pBackBuffer->Release();
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
			InitImGui();
			init = true;
		}

		else
			return oPresent(pSwapChain, SyncInterval, Flags);
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	/*��ʼ��ӫĻ���*/
	ImGuiIO& io = ImGui::GetIO();
	GameVars.ScreenWidth = io.DisplaySize.x / 2; //��ʼ��ӫĻ��
	GameVars.ScreenHeight = io.DisplaySize.y / 2;  //��ʼ��ӫĻ��
	/*�趨ImgGui�˵���С*/
	ImGui::SetNextWindowSize(ImVec2(400, 300));
	/*����Home ʹ P_Menu ���ó�True ����괩͸*/
	if (GetAsyncKeyState(VK_HOME) & 1)CFG.P_Menu = !CFG.P_Menu;
	{
		ImGui::GetIO().MouseDrawCursor = CFG.P_Menu; // ����괩͸
	}
	if (CFG.P_Menu) //����˵�����
	{
		ImGui::Begin("ImGui Window", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
		if (ImGui::CollapsingHeader("ESP_MENU"))
		{
			ImGui::Checkbox("DrawBox", &CFG.P_DrawBox); ImGui::SameLine();
			ImGui::ColorEdit3("Color", (float*)&CFG.C_DrawBoxColor); //��ɫת�� �����ɫ ColorEdit��ɫʹ��float����Ҫ����ת
			ImGui::Checkbox("Skeleton", &CFG.P_Skeleton); ImGui::SameLine();
			ImGui::ColorEdit3("VisibleColor", (float*)&CFG.C_VisibleColor); //��ɫת�� �����ɫ ColorEdit��ɫʹ��float����Ҫ����ת
			ImGui::ColorEdit3("UnVisibleColor", (float*)&CFG.C_UnVisibleColor); //��ɫת�� �����ɫ ColorEdit��ɫʹ��float����Ҫ����ת

			ImGui::Checkbox("DawDist", &CFG.P_DrawDist); //��ʾ����
			/*����*/
			ImGui::Checkbox("Ray", &CFG.P_DrawRay); ImGui::SameLine();//��ʾ����
			ImGui::ColorEdit3("RayColor", (float*)&CFG.C_DrawRayCcolor); //������ɫ


			ImGui::SliderFloat("X1", &CFG.pointX1, 0.f, 1080.f);
			ImGui::SliderFloat("Y1", &CFG.pointY1, 0.f, 1080.f);
			ImGui::SliderFloat("X2", &CFG.pointX2, 0.f, 1080.f);
			ImGui::SliderFloat("Y2", &CFG.pointY2, 0.f, 1080.f);
			ImGui::SliderFloat("X3", &CFG.pointX3, 0.f, 1080.f);
			ImGui::SliderFloat("Y3", &CFG.pointY3, 0.f, 1080.f);

		}
		if (ImGui::CollapsingHeader("Aimbot_MENU"))
		{
			ImGui::Checkbox("Aimbot", &CFG.P_Aimbot); 
			ImGui::Checkbox("SHOW_FOV", &CFG.P_SHOW_FOV); ImGui::SameLine();
			ImGui::SliderFloat("FOV", &CFG.AimbotFOV, 0.f, 360.f);

		}
		ImGui::End();
	}
		
	ESP(); //���ܿ���
	ImGui::Render();

	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	return oPresent(pSwapChain, SyncInterval, Flags);
}

DWORD WINAPI MainThread(LPVOID lpReserved)
{
	bool init_hook = false;
	
	do
	{
		if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
		{
			kiero::bind(8, (void**)& oPresent, hkPresent);
			init_hook = true;
		}
	} while (!init_hook);
	return TRUE;
}

BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		AllocConsole();
		printf("666");
		SetConsoleTitle("Debug");
		freopen("CONIN$", "r", stdin);
		freopen("CONOUT$", "w", stdout);
		freopen("CONOUT$", "w", stderr);
		DisableThreadLibraryCalls(hMod);
		CreateThread(nullptr, 0, MainThread, hMod, 0, nullptr);
		break;
	case DLL_PROCESS_DETACH:
		kiero::shutdown();
		break;
	}
	return TRUE;
}