// DXT.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

int DXTCompressImage	(LPCSTR out_name, u8* raw_data, u32 w, u32 h, u32 pitch, STextureParams* fmt, u32 depth)
{
	R_ASSERT(false);
	return 0;
	/*CTimer T; T.Start();

	Msg("DXT: Compressing Image: %s %uX%u", out_name, w, h);

	R_ASSERT(0 != w && 0 != h);
	BearImage Image;
	BearTexturePixelFormat Format = BearTexturePixelFormat::R8G8B8A8;
	switch (fmt->fmt)
	{
		case STextureParams::tfDXT1: 	Format = BearTexturePixelFormat::BC1; 	  break;
		case STextureParams::tfADXT1:	Format = BearTexturePixelFormat::BC1a; 	  break;
		case STextureParams::tfDXT3: 	Format = BearTexturePixelFormat::BC2; 	  break;
		case STextureParams::tfDXT5: 	Format = BearTexturePixelFormat::BC3;	  break;
		case STextureParams::tfBC4: 	Format = BearTexturePixelFormat::BC4;	  break;
		case STextureParams::tfBC5: 	Format = BearTexturePixelFormat::BC5;	  break;
		case STextureParams::tfBC6: 	Format = BearTexturePixelFormat::BC6;	  break;
		case STextureParams::tfBC7: 	Format = BearTexturePixelFormat::BC7;	  break;
		case STextureParams::tfRGB: 	Format = BearTexturePixelFormat::R8G8B8;  break;
		case STextureParams::tfRGBA: 	Format = BearTexturePixelFormat::R8G8B8A8;break;
	}

	Image.Create(w, h, 1, 1, BearTexturePixelFormat::R8G8B8A8);
	bear_copy(*Image, raw_data, w * h * 4);
	Image.SwapRB();
	BearResizeFilter ResizeFilter = BearResizeFilter::Default;
	switch (fmt->mip_filter)
	{
	case STextureParams::kMIPFilterBox:       ResizeFilter = BearResizeFilter::Box;     break;
	case STextureParams::kMIPFilterTriangle:    ResizeFilter = BearResizeFilter::Triangle; break;
	case STextureParams::kMIPFilterKaiser:     ResizeFilter = BearResizeFilter::Catmullrom;   break;
	}
	Image.GenerateMipmap(ResizeFilter);
	Image.Convert(Format);
	Msg("DXT: Compressing Image: 2 [Closing File]. Time from start %f ms", T.GetElapsed_sec() * 1000.f);
	return Image.SaveToDds(out_name);;*/
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
