#include "stdafx.h"
#include "VirtualObject.h"


void VirtualObject::Init()
{
	PostInitialize();
	OnInit();
}

void VirtualObject::Update()
{
	OnUpdate();
}

void VirtualObject::Render()
{
	OnRender();
}

void VirtualObject::Release()
{
	OnRelease();
}

VirtualObject::VirtualObject()
{
}


VirtualObject::~VirtualObject()
{
}