#include "Main.h"
#include "Library.hpp"

AsiPlugin::AsiPlugin() {
	gameloopHook.set_cb([this](auto&&... args) { return AsiPlugin::GameloopHooked(args...); });
	gameloopHook.install();
}

AsiPlugin::~AsiPlugin() {
	wndprocHook.remove();
	gameloopHook.remove();
}

void AsiPlugin::GameloopHooked(const decltype(gameloopHook)& hook) {
	static bool IsInitialized = false;
	if (!IsInitialized && SAMP::IsSAMPInitialized()) {
		switch (SAMP::GetSAMPVersion()) {
		case (SAMP::sampVersion::R1): {
			pInput = reinterpret_cast<void*>(SAMP::GetSAMPHandle() + 0x21A0E8);
			Address = 0x65A63;
			break;
		}
		case (SAMP::sampVersion::R2): {
			pInput = reinterpret_cast<void*>(SAMP::GetSAMPHandle() + 0x21A0F0);
			Address = 0x65B33;
			break;
		}
		case (SAMP::sampVersion::R3): {
			pInput = reinterpret_cast<void*>(SAMP::GetSAMPHandle() + 0x26E8CC);
			Address = 0x68F93;
			break;
		}
		case (SAMP::sampVersion::R4): {
			pInput = reinterpret_cast<void*>(SAMP::GetSAMPHandle() + 0x26E9FC);
			Address = 0x69703;
			break;

		}
		case (SAMP::sampVersion::DL): {
			pInput = reinterpret_cast<void*>(SAMP::GetSAMPHandle() + 0x2ACA14);
			Address = 0x69143;
			break;
		}
		default: {
			break;
		}
		}
		isChatEnabled = 0x14E0;
		if (!pInput)
			return;
		wndprocHook.set_cb([this](auto&&... args) { return AsiPlugin::WndProcHooked(args...); });
		wndprocHook.install();
		IsInitialized = true;
	}
	return hook.get_trampoline()();
}

bool AsiPlugin::IsChatOpened() {
	if (!pInput)
		return false;
	return *(int*)(*(size_t*)pInput + isChatEnabled);
}

template <typename T>
inline void write_memory(std::uintptr_t address, T value, bool protect = true) {
	unsigned long oldProt;
	if (protect) VirtualProtect(reinterpret_cast<void*>(address), sizeof(T), PAGE_READWRITE, &oldProt);
	*reinterpret_cast<T*>(address) = value;
	if (protect) VirtualProtect(reinterpret_cast<void*>(address), sizeof(T), oldProt, &oldProt);
}

LRESULT AsiPlugin::WndProcHooked(const decltype(wndprocHook)& hook, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (IsChatOpened()) {
		switch (uMsg) {
		case (WM_RBUTTONDOWN): {
			write_memory(SAMP::GetSAMPHandle() + Address, char(0), true);
			break;
		}
		case (WM_RBUTTONUP): {
			write_memory(SAMP::GetSAMPHandle() + Address, char(2), true);
			break;
		}
		}
	}
	return hook.get_trampoline()(hWnd, uMsg, wParam, lParam);
}
