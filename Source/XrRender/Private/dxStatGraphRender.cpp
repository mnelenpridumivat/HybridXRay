#include "stdafx.h"
#include "dxStatGraphRender.h"

void dxStatGraphRender::Copy(IStatGraphRender&_in)
{
	*this = *((dxStatGraphRender*)&_in);
}

void dxStatGraphRender::OnDeviceCreate()
{
	hGeomLine.create(FVF::F_TL0uv,RCache.Vertex.Buffer(),RCache.Index.Buffer());
	hGeomTri.create(FVF::F_TL0uv,RCache.Vertex.Buffer(),RCache.QuadIB);
}

void dxStatGraphRender::OnDeviceDestroy()
{
	hGeomLine.destroy();
	hGeomTri.destroy();
}

void dxStatGraphRender::OnRender(CStatGraph &owner)
{
}

void dxStatGraphRender::RenderBack(CStatGraph &owner)
{

}

void dxStatGraphRender::RenderBars(CStatGraph &owner, FVF::TL0uv** ppv, CStatGraph::ElementsDeq* pelements)
{
	float elem_offs		= float(owner.rb.x-owner.lt.x)/owner.max_item_count;
	float elem_factor	= float(owner.rb.y-owner.lt.y)/float(owner.mx-owner.mn);
	float base_y		= float(owner.rb.y)+(owner.mn*elem_factor);

	float column_width = elem_offs;
	if (column_width > 1) column_width--;
	for (CStatGraph::ElementsDeqIt it=pelements->begin(); it!=pelements->end(); it++)
	{
		float X		= float(it-pelements->begin())*elem_offs+owner.lt.x;
		float Y0	= base_y;
		float Y1	= base_y - it->data*elem_factor;

		if (Y1 > Y0)
		{
			(*ppv)->set		(X,Y1,it->color); (*ppv)++;
			(*ppv)->set		(X,Y0,it->color); (*ppv)++;
			(*ppv)->set		(X+column_width,Y1,it->color); (*ppv)++;
			(*ppv)->set		(X+column_width,Y0,it->color); (*ppv)++;
		}
		else
		{
			(*ppv)->set		(X,Y0,it->color); (*ppv)++;
			(*ppv)->set		(X,Y1,it->color); (*ppv)++;
			(*ppv)->set		(X+column_width,Y0,it->color); (*ppv)++;
			(*ppv)->set		(X+column_width,Y1,it->color); (*ppv)++;
		};
	};
}

void dxStatGraphRender::RenderLines(CStatGraph &owner, FVF::TL0uv** ppv, CStatGraph::ElementsDeq* pelements )
{
	float elem_offs		= float(owner.rb.x-owner.lt.x)/owner.max_item_count;
	float elem_factor	= float(owner.rb.y-owner.lt.y)/float(owner.mx-owner.mn);
	float base_y		= float(owner.rb.y)+(owner.mn*elem_factor);

	for (CStatGraph::ElementsDeqIt it=pelements->begin()+1;  it!=pelements->end() && it!=pelements->end()+1; it++)
	{
		CStatGraph::ElementsDeqIt it_prev = it-1;
		float X0	= float(it_prev-pelements->begin())*elem_offs+owner.lt.x;
		float Y0	= base_y-it_prev->data*elem_factor;
		(*ppv)->set		(X0,Y0,it->color); (*ppv)++;
		float X1	= float(it-pelements->begin())*elem_offs+owner.lt.x;
		float Y1	= base_y-it->data*elem_factor;
		(*ppv)->set		(X1,Y1,it->color); (*ppv)++;
	}
};

void dxStatGraphRender::RenderBarLines(CStatGraph &owner, FVF::TL0uv** ppv, CStatGraph::ElementsDeq* pelements )
{
	float elem_offs		= float(owner.rb.x-owner.lt.x)/owner.max_item_count;
	float elem_factor	= float(owner.rb.y-owner.lt.y)/float(owner.mx-owner.mn);
	float base_y		= float(owner.rb.y)+(owner.mn*elem_factor);

	for (CStatGraph::ElementsDeqIt it=pelements->begin()+1; it!=pelements->end() && it!=pelements->end()+1; it++)
	{
		CStatGraph::ElementsDeqIt it_prev = it-1;
		float X0	= float(it_prev-pelements->begin())*elem_offs+owner.lt.x+elem_offs;
		float Y0	= base_y-it_prev->data*elem_factor;
		(*ppv)->set		(X0,Y0,it->color); (*ppv)++;
		float X1	= float(it-pelements->begin())*elem_offs+owner.lt.x;
		float Y1	= base_y-it->data*elem_factor;
		(*ppv)->set		(X1,Y1,it->color); (*ppv)++;
		(*ppv)->set		(X1,Y1,it->color); (*ppv)++;
		X1 += elem_offs;
		(*ppv)->set		(X1,Y1,it->color); (*ppv)++;
	}
};

void dxStatGraphRender::RenderMarkers(CStatGraph &owner, FVF::TL0uv** ppv, CStatGraph::MarkersDeq* pmarkers )
{
	float elem_offs		= float(owner.rb.x-owner.lt.x)/owner.max_item_count;
	float elem_factor	= float(owner.rb.y-owner.lt.y)/float(owner.mx-owner.mn);
	float base_y		= float(owner.rb.y)+(owner.mn*elem_factor);

	for (CStatGraph::MarkersDeqIt it=pmarkers->begin();  it!=pmarkers->end() && it!=pmarkers->end()+1; it++)
	{
		CStatGraph::SMarker &CurMarker = *it;
		float X0 = 0, Y0 = 0, X1 = 0, Y1 = 0;
		switch (CurMarker.m_eStyle)
		{
		case CStatGraph::stVert:
			{
				X0 = CurMarker.m_fPos*elem_offs+owner.lt.x;
				clamp(X0, float(owner.lt.x), float(owner.rb.x));
				X1 = X0;
				Y0 = float(owner.lt.y);
				Y1 = float(owner.rb.y);
			}break;
		case CStatGraph::stHor:
			{
				X0 = float(owner.lt.x);
				X1 = float(owner.rb.x);
				Y0 = base_y - CurMarker.m_fPos*elem_factor;
				clamp(Y0, float(owner.lt.y), float(owner.rb.y));
				Y1 = Y0;
			}break;
		}
		(*ppv)->set		(X0,Y0,CurMarker.m_dwColor); (*ppv)++;
		(*ppv)->set		(X1,Y1,CurMarker.m_dwColor); (*ppv)++;
	}
}