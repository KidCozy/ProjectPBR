#include "stdafx.h"
#include "VirtualObject.h"


void VirtualObject::Init()
{
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