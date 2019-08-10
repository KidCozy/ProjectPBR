#pragma once
#include"WindowPreset.h"

class EventHandler
{
	
public:
	virtual void RDragNotify(WinMessage* Event, WinMessage* NewEvent) = 0;
	virtual void LDragNotify(WinMessage* Event, WinMessage* NewEvent) = 0;
	
	virtual void KeyEnterNotify(WinMessage* Event, WinMessage* NewEvent) = 0;
	virtual void KeyPressNotify(WinMessage* Event, WinMessage* NewEvent) = 0;
	virtual void KeyReleaseNotify(WinMessage* Event, WinMessage* NewEvent) = 0;
	
};