#include "stdafx.h"
#pragma hdrstop

/*
#include "../../xrCore/xr_resource.h"

// res
class	test_resource	: public xr_resource	{
public:
	void				_release_	(test_resource * ptr)	{	xr_delete	(ptr);	}
};

// ptr
typedef	xr_resorce_ptr<test_resource>	test_ptr;

// the test itself
BOOL	AAA_test		()
{
	test_ptr	A		(xr_new<test_resource>());
	test_ptr	B		= xr_new<test_resource>	();
	A					= B;
	return		A!=B;
}
*/

void	CConstant::Calculate()
{
	if (dwFrame==Device->dwFrame)	return;
	dwFrame		= Device->dwFrame;
	if (modeProgrammable==dwMode)	return;

	float	t	= Device->fTimeGlobal;
}

void	CConstant::Load	(IReader* fs)
{
	dwMode		= modeWaveForm;
}

void	CConstant::Save	(IWriter* fs)
{
}

