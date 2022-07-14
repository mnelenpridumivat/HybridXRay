#ifndef SH_CONSTANT_H
#define SH_CONSTANT_H
#pragma once

class		IReader;
class		IWriter;

class	ECORE_API	CConstant	: public xr_resource_named									{
public:
	enum			{ modeProgrammable=0, modeWaveForm	};
public:
	Fcolor			const_float;
	u32				const_dword;

	u32				dwFrame;
	u32				dwMode;

	CConstant		()
	{
		Memory.mem_fill	(this,0,sizeof(CConstant));
	}
	IC void			set_float	(float r, float g, float b, float a)
	{
		const_float.set	(r,g,b,a);
		const_dword		= const_float.get();
	}
	IC void			set_float	(Fcolor& c)
	{
		const_float.set	(c);
		const_dword		= const_float.get();
	}
	IC void			set_dword	(u32 c)
	{
		const_float.set(c);
		const_dword		= c;
	}
	void			Calculate	();
	IC	BOOL		Similar		(CConstant& C)		// comare by modes and params
	{
		return TRUE;
	}
	void			Load		(IReader* fs);
	void			Save		(IWriter* fs);
};

typedef	resptr_core<CConstant,resptr_base<CConstant> >	
	ref_constant_obsolette;

#endif
