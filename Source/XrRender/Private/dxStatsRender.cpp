#include "stdafx.h"
#include "dxStatsRender.h"
#include "dxRenderDeviceRender.h"

void dxStatsRender::Copy(IStatsRender&_in)
{
	*this = *((dxStatsRender*)&_in);
}

void dxStatsRender::OutData1 (CGameFont &F)
{
}

void dxStatsRender::OutData2 (CGameFont &F)
{
}

void dxStatsRender::OutData3 (CGameFont &F)
{
}

void dxStatsRender::OutData4 (CGameFont &F)
{
}

void dxStatsRender::GuardVerts (CGameFont &F)
{
}

void dxStatsRender::GuardDrawCalls (CGameFont &F)
{
}

void dxStatsRender::SetDrawParams (IRenderDeviceRender *pRender)
{
}