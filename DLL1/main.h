#ifndef _MAIN_H_
#define _MAIN_H_

#include <kthook/kthook.hpp> // kthook::kthook_simple

using GameloopPrototype = void(__cdecl*)();
using WndProcPrototype = LRESULT(__stdcall*)(HWND, UINT, WPARAM, LPARAM);

class AsiPlugin
{
	kthook::kthook_simple<GameloopPrototype> gameloopHook{ 0x748DA3 };
	kthook::kthook_simple<WndProcPrototype> wndprocHook{ 0x747EB0 };

	void* pInput = nullptr;
	void* pOpenChat = nullptr;
	int isChatEnabled = 0;
	int Address = 0;
public:
	explicit AsiPlugin();
	virtual ~AsiPlugin();
private:
	void GameloopHooked(const decltype(gameloopHook)& hook);
	LRESULT WndProcHooked(const decltype(wndprocHook)& hook, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	bool IsChatOpened();
} AsiPlugin;

#endif // !_MAIN_H_