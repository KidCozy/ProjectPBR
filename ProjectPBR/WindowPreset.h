#pragma once

struct WinEvent
{
	bool LButton;
	bool RButton;
	bool WButton;

};

struct WinMessage
{
	UINT Message;
	WPARAM wParam;
	LPARAM lParam;
	bool Keys[256];
};