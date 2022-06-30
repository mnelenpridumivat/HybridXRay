// DXT.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

int DXTCompressImage	(LPCSTR out_name, u8* raw_data, u32 w, u32 h, u32 pitch, STextureParams* fmt, u32 depth)
{
	return 0;
}

extern int DXTCompressBump(LPCSTR out_name, u8* raw_data, u8* normal_map, u32 w, u32 h, u32 pitch, STextureParams* fmt, u32 depth);

extern "C" __declspec(dllexport) 
int   DXTCompress	(LPCSTR out_name, u8* raw_data, u8* normal_map, u32 w, u32 h, u32 pitch, 
					STextureParams* fmt, u32 depth)
{
	switch (fmt->type){
	case STextureParams::ttImage:	
	case STextureParams::ttCubeMap: 
	case STextureParams::ttNormalMap:
	case STextureParams::ttTerrain:
		return DXTCompressImage	(out_name, raw_data, w, h, pitch, fmt, depth);
	break;
	case STextureParams::ttBumpMap: 
		return DXTCompressBump	(out_name, raw_data, normal_map, w, h, pitch, fmt, depth);
	break;
	default: NODEFAULT;
	}
	return -1;
}
