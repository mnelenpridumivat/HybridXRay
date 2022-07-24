#ifndef D3DX_WraperH
#define D3DX_WraperH
#pragma once

#ifdef XRETOOLS_EXPORTS
#define  __declspec( dllexport )
#else
#define  __declspec( dllimport )
#endif

extern "C" {
	 UINT WINAPI
		D3DX_GetDriverLevel(
		LPDIRECT3DDEVICE9		pDevice);

	 HRESULT WINAPI
		D3DX_GetImageInfoFromFileInMemory(
		LPCVOID					pSrcData,
		UINT					SrcDataSize,
		D3DXIMAGE_INFO*			pSrcInfo);

	 HRESULT WINAPI
		D3DX_CreateCubeTextureFromFileInMemoryEx(
		LPDIRECT3DDEVICE9       pDevice,
		LPCVOID                 pSrcData,
		UINT                    SrcDataSize,
		UINT                    Size,
		UINT                    MipLevels,
		DWORD                   Usage,
		D3DFORMAT               Format,
		D3DPOOL                 Pool,
		DWORD                   Filter,
		DWORD                   MipFilter,
		D3DCOLOR                ColorKey,
		D3DXIMAGE_INFO*         pSrcInfo,
		PALETTEENTRY*           pPalette,
		LPDIRECT3DCUBETEXTURE9* ppCubeTexture);

	 HRESULT WINAPI
		D3DX_CreateTextureFromFileInMemoryEx(
		LPDIRECT3DDEVICE9       pDevice,
		LPCVOID                 pSrcData,
		UINT                    SrcDataSize,
		UINT                    Width,
		UINT                    Height,
		UINT                    MipLevels,
		DWORD                   Usage,
		D3DFORMAT               Format,
		D3DPOOL                 Pool,
		DWORD                   Filter,
		DWORD                   MipFilter,
		D3DCOLOR                ColorKey,
		D3DXIMAGE_INFO*         pSrcInfo,
		PALETTEENTRY*           pPalette,
		LPDIRECT3DTEXTURE9*     ppTexture);

	 HRESULT WINAPI
		D3DX_CreateTexture(
		LPDIRECT3DDEVICE9       pDevice,
		UINT                    Width,
		UINT                    Height,
		UINT                    MipLevels,
		DWORD                   Usage,
		D3DFORMAT               Format,
		D3DPOOL                 Pool,
		LPDIRECT3DTEXTURE9*     ppTexture);

	 HRESULT WINAPI
		D3DX_ComputeNormalMap(
		LPDIRECT3DTEXTURE9 pTexture,
		LPDIRECT3DTEXTURE9 pSrcTexture,
		const PALETTEENTRY *pSrcPalette,
		DWORD Flags,
		DWORD Channel,
		FLOAT Amplitude);

	 HRESULT WINAPI
		D3DX_LoadSurfaceFromSurface(
		LPDIRECT3DSURFACE9      pDestSurface,
		CONST PALETTEENTRY*     pDestPalette,
		CONST RECT*             pDestRect,
		LPDIRECT3DSURFACE9      pSrcSurface,
		CONST PALETTEENTRY*     pSrcPalette,
		CONST RECT*             pSrcRect,
		DWORD					Filter,
		D3DCOLOR				ColorKey);

	 HRESULT WINAPI
		D3DX_CompileShader(
		LPCSTR                  pSrcData,
		UINT                    SrcDataLen,
		CONST D3DXMACRO*        pDefines,
		LPD3DXINCLUDE           pInclude,
		LPCSTR                  pFunctionName,
		LPCSTR                  pTarget,
		DWORD                   Flags,
		LPD3DXBUFFER*           ppShader,
		LPD3DXBUFFER*           ppErrorMsgs,
		LPD3DXCONSTANTTABLE*    ppConstantTable);

	 HRESULT WINAPI
		D3DX_CompileShaderFromFile(
		LPCSTR                  pSrcFile,
		CONST D3DXMACRO*        pDefines,
		LPD3DXINCLUDE           pInclude,
		LPCSTR                  pFunctionName,
		LPCSTR                  pTarget,
		DWORD                   Flags,
		LPD3DXBUFFER*           ppShader,
		LPD3DXBUFFER*           ppErrorMsgs,
		LPD3DXCONSTANTTABLE*    ppConstantTable);

	 HRESULT WINAPI
		D3DX_FindShaderComment(
		CONST DWORD*            pFunction,
		DWORD                   FourCC,
		LPCVOID*                ppData,
		UINT*                   pSizeInBytes);

	 HRESULT WINAPI
		D3DX_DeclaratorFromFVF(
		DWORD FVF,
		D3DVERTEXELEMENT9 pDeclarator[MAX_FVF_DECL_SIZE]);

	 UINT WINAPI
		D3DX_GetDeclVertexSize(
		CONST D3DVERTEXELEMENT9*pDecl,
		DWORD					Stream);

	 UINT WINAPI
		D3DX_GetDeclLength(
		CONST D3DVERTEXELEMENT9*pDecl);

	 UINT WINAPI
		D3DX_GetFVFVertexSize(
		DWORD					FVF);

	 const char*  WINAPI
		DX_GetErrorDescription(
		HRESULT					hr);

	 D3DXMATRIX* WINAPI
		D3DX_MatrixInverse(          
		D3DXMATRIX *pOut,
		FLOAT *pDeterminant,
		CONST D3DXMATRIX *pM);

	 D3DXMATRIX* WINAPI
		D3DX_MatrixTranspose(          
		D3DXMATRIX *pOut,
		CONST D3DXMATRIX *pM);

	 D3DXPLANE* WINAPI
		D3DX_PlaneNormalize(          
		D3DXPLANE *pOut,
		CONST D3DXPLANE *pP);

	 D3DXPLANE* WINAPI
		D3DX_PlaneTransform(          
		D3DXPLANE *pOut,
		CONST D3DXPLANE *pP,
		CONST D3DXMATRIX *pM);
        
	 HRESULT WINAPI
    	D3DX_OptimizeFaces(
		LPCVOID pIndices,
      	UINT NumFaces,
      	UINT NumVertices,
      	BOOL Indices32Bit,
      	DWORD * pFaceRemap);

	 HRESULT WINAPI
		D3DX_OptimizeVertices(
		LPCVOID pIndices,
		UINT NumFaces,
		UINT NumVertices,
		BOOL Indices32Bit,
		DWORD * pVertexRemap);
}

#endif
