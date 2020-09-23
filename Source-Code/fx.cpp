// fx.cpp : Defines the entry point for the DLL application.
//
#include "stdafx.h"
#include "fx.h"


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}



////////////////////////////////////////////////////////////////////////////
//
//	fxAlphaBlend()				Displays bitmaps with transparent and
//								semitransparent pixels, first
//								stretching or compressing the bitmap to fit
//								the dimensions of the destination rectangle,
//								if necessary.
//								If the function succeeds, the return value
//								is 1 (true); 
//								if fails, the return value is 0 (false). 
//
//	HDC		DestDC				Handle to the destination device context;
//	int		DestX				Specifies the x-coordinate, 
//								in logical units, of the upper-left 
//								corner of the destination rectangle;
//	int		DestY				Specifies the y-coordinate, 
//								in logical units, of the upper-left 
//								corner of the destination rectangle;
//	int		DestWidth			Specifies the width, in logical units,
//								of the destination rectangle.
//	int		DestHeight			Specifies the height, in logical units,
//								of the destination rectangle.
//	HDC		SrcDC				Handle to the source device context;
//	int		SrcX				Specifies the x-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		SrcY				Specifies the y-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		SrcWidth			Specifies the width, in logical units,
//								of the source rectangle;
//	int		SrcHeight			Specifies the height, in logical units,
//								of the source rectangle.
//	int		Alpha				Alpha value (value in range 0 to 255);
//	int		TransparentColor	The RGB color in the source DC to 
//								treat as transparent;
//								if (TransparentColor = -1) the transparency
//								will not be used;
//	int		Flags				Reserved parameter.
//
////////////////////////////////////////////////////////////////////////////
int __stdcall fxAlphaBlend(HDC DestDC, int DestX, int DestY, int DestWidth, int DestHeight, HDC SrcDC, int SrcX, int SrcY, int SrcWidth, int SrcHeight, int Blending, int TransparentColor, int Flags)
{
	if (Blending < 0)
		Blending = 0;
	if (Blending > 255)
		Blending = 255;

	BITMAPINFOHEADER	BMI;
	BMI.biSize			= sizeof(BITMAPINFOHEADER);
	BMI.biWidth			= DestWidth;
	BMI.biHeight		= DestHeight;
	BMI.biPlanes		= 1;
	BMI.biBitCount		= 32;
	BMI.biCompression	= BI_RGB;
	BMI.biSizeImage		= 0;
	BMI.biXPelsPerMeter = 0;
	BMI.biYPelsPerMeter = 0;
	BMI.biClrUsed		= 0;
	BMI.biClrImportant	= 0;
	
	BYTE				*pDestBits;
	HBITMAP				hbmDest;
	hbmDest = ::CreateDIBSection(DestDC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pDestBits, NULL, 0l);

	BYTE				*pSrcBits;
	HBITMAP				hbmSrc;
	hbmSrc = ::CreateDIBSection(SrcDC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pSrcBits, NULL, 0l);

	HDC					dc = ::CreateCompatibleDC(NULL);
	HBITMAP				hbm = (HBITMAP) ::SelectObject(dc, hbmSrc);
	
	::SetStretchBltMode(dc, HALFTONE);

	if (!::StretchBlt(dc, 0, 0, DestWidth, DestHeight, SrcDC, SrcX, SrcY, SrcWidth, SrcHeight, SRCCOPY))
		return 0;

	::SelectObject(dc, hbmDest);

	if (!::StretchBlt(dc, 0, 0, DestWidth, DestHeight, DestDC, DestX, DestY, DestWidth, DestHeight, SRCCOPY))
		return 0;

	::SelectObject(dc, hbm);

	::DeleteDC(dc);

    for (int y = 0; y < DestHeight; y++)
    {
		LPBYTE pbDestRGB = (LPBYTE)&((DWORD*)pDestBits)[y * DestWidth];
		LPBYTE pbSrcRGB = (LPBYTE)&((DWORD*)pSrcBits)[y * DestWidth];

		for (int x = 0; x < DestWidth; x++)
		{

			if (TransparentColor >= 0)
			{
				if (TransparentColor == fxRGB(pbSrcRGB[2], pbSrcRGB[1], pbSrcRGB[0]))
				{
					pbSrcRGB[0] = pbDestRGB[0];
					pbSrcRGB[1] = pbDestRGB[1];
					pbSrcRGB[2] = pbDestRGB[2];
				}
				else
				{
					pbSrcRGB[0] = fxShadeColors(pbDestRGB[0], pbSrcRGB[0], Blending);
					pbSrcRGB[1] = fxShadeColors(pbDestRGB[1], pbSrcRGB[1], Blending);
					pbSrcRGB[2] = fxShadeColors(pbDestRGB[2], pbSrcRGB[2], Blending);
				}
			}
			else
			{
				pbSrcRGB[0] = fxShadeColors(pbDestRGB[0], pbSrcRGB[0], Blending);
				pbSrcRGB[1] = fxShadeColors(pbDestRGB[1], pbSrcRGB[1], Blending);
				pbSrcRGB[2] = fxShadeColors(pbDestRGB[2], pbSrcRGB[2], Blending);
			}

			pbSrcRGB += 4;
			pbDestRGB += 4;

		}
	}

	dc = ::CreateCompatibleDC(NULL);
	
	hbm = (HBITMAP) ::SelectObject(dc, hbmSrc);
	if (!::BitBlt(DestDC, DestX, DestY, DestWidth, DestHeight, dc, 0, 0, SRCCOPY))
		return 0;

	::DeleteDC(dc);

	::DeleteObject(hbmSrc);	
	::DeleteObject(hbmDest);	
	
	return 1;
}



////////////////////////////////////////////////////////////////////////////
//
//	fxAmbientLight()			Function displays bitmap in 
//								specified ambient light, first
//								stretching or compressing the bitmap to fit
//								the dimensions of the destination rectangle,
//								if necessary.
//								If the function succeeds, the return value
//								is 1 (true); 
//								if fails, the return value is 0 (false). 
//
//	HDC		DestDC				Handle to the destination device context;
//	int		DestX				Specifies the x-coordinate, 
//								in logical units, of the upper-left 
//								corner of the destination rectangle;
//	int		DestY				Specifies the y-coordinate, 
//								in logical units, of the upper-left 
//								corner of the destination rectangle;
//	int		DestWidth			Specifies the width, in logical units,
//								of the destination rectangle.
//	int		DestHeight			Specifies the height, in logical units,
//								of the destination rectangle.
//	HDC		SrcDC				Handle to the source device context;
//	int		SrcX				Specifies the x-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		SrcY				Specifies the y-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		SrcWidth			Specifies the width, in logical units,
//								of the source rectangle;
//	int		SrcHeight			Specifies the height, in logical units,
//								of the source rectangle;
//	int		Color				Ambient light color (RGB);
//	int		Intensity			Ambient light intensity (value in
//								range 0-255);
//	int		Flags				Reserved parameter.
//
////////////////////////////////////////////////////////////////////////////
int __stdcall fxAmbientLight(HDC DestDC, int DestX, int DestY, int DestWidth, int DestHeight, HDC SrcDC, int SrcX, int SrcY, int SrcWidth, int SrcHeight, int Color, int Intensity, int Flags)
{
	if (Intensity < 0)
		Intensity = 0;
	if (Intensity > 255)
		Intensity = 255;

	int fxR				= (255 - Intensity - fxGetRed(Color));
	int fxG				= (255 - Intensity - fxGetGreen(Color));
	int fxB				= (255 - Intensity - fxGetBlue(Color));

	BITMAPINFOHEADER	BMI;
	BMI.biSize			= sizeof(BITMAPINFOHEADER);
	BMI.biWidth			= DestWidth;
	BMI.biHeight		= DestHeight;
	BMI.biPlanes		= 1;
	BMI.biBitCount		= 32;
	BMI.biCompression	= BI_RGB;
	BMI.biSizeImage		= 0;
	BMI.biXPelsPerMeter = 0;
	BMI.biYPelsPerMeter = 0;
	BMI.biClrUsed		= 0;
	BMI.biClrImportant	= 0;
	
	/*
	BYTE				*pDestBits;
	HBITMAP				hbmDest;
	hbmDest = ::CreateDIBSection(DestDC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pDestBits, NULL, 0l);
	*/

	BYTE				*pSrcBits;
	HBITMAP				hbmSrc;
	hbmSrc = ::CreateDIBSection(SrcDC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pSrcBits, NULL, 0l);

	HDC					dc = ::CreateCompatibleDC(NULL);
	HBITMAP				hbm = (HBITMAP) ::SelectObject(dc, hbmSrc);
	
	::SetStretchBltMode(dc, HALFTONE);

	if (!::StretchBlt(dc, 0, 0, DestWidth, DestHeight, SrcDC, SrcX, SrcY, SrcWidth, SrcHeight, SRCCOPY))
		return 0;

	/*
	::SelectObject(dc, hbmDest);

	if (!::StretchBlt(dc, 0, 0, DestWidth, DestHeight, DestDC, DestX, DestY, DestWidth, DestHeight, SRCCOPY))
		return 0;
	*/

	::SelectObject(dc, hbm);

	::DeleteDC(dc);

    for (int y = 0; y < DestHeight; y++)
    {
		LPBYTE pbSrcRGB = (LPBYTE)&((DWORD*)pSrcBits)[y * DestWidth];
		//LPBYTE pbDestRGB = (LPBYTE)&((DWORD*)pDestBits)[y * DestWidth];

		for (int x = 0; x < DestWidth; x++)
		{
			pbSrcRGB[0] = fxByte(pbSrcRGB[0] - fxB);
			pbSrcRGB[1] = fxByte(pbSrcRGB[1] - fxG);
			pbSrcRGB[2] = fxByte(pbSrcRGB[2] - fxR);

			pbSrcRGB += 4;
			//pbDestRGB += 4;
		}
	}

	dc = ::CreateCompatibleDC(NULL);
	
	hbm = (HBITMAP) ::SelectObject(dc, hbmSrc);
	if (!::BitBlt(DestDC, DestX, DestY, DestWidth, DestHeight, dc, 0, 0, SRCCOPY))
		return 0;

	::DeleteDC(dc);

	::DeleteObject(hbmSrc);	
	//::DeleteObject(hbmDest);	
	
	return 1;
}



////////////////////////////////////////////////////////////////////////////
//
//	fxAntiAlias()				Displays anti-aliased bitmap, 
//								first stretching or compressing it to fit
//								the dimensions of the destination rectangle,
//								if necessary.
//								If the function succeeds, the return value
//								is 1 (true); 
//								if fails, the return value is 0 (false). 
//
//	HDC		DestDC				Handle to the destination device context;
//	int		DestX				Specifies the x-coordinate, 
//								in logical units, of the upper-left 
//								corner of the destination rectangle;
//	int		DestY				Specifies the y-coordinate, 
//								in logical units, of the upper-left 
//								corner of the destination rectangle;
//	int		DestWidth			Specifies the width, in logical units,
//								of the destination rectangle.
//	int		DestHeight			Specifies the height, in logical units,
//								of the destination rectangle.
//	HDC		SrcDC				Handle to the source device context;
//	int		SrcX				Specifies the x-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		SrcY				Specifies the y-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		SrcWidth			Specifies the width, in logical units,
//								of the source rectangle;
//	int		SrcHeight			Specifies the height, in logical units,
//								of the source rectangle;
//	int		Flags				Reserved parameter.
//
////////////////////////////////////////////////////////////////////////////
int __stdcall fxAntiAlias(HDC DestDC, int DestX, int DestY, int DestWidth, int DestHeight, HDC SrcDC, int SrcX, int SrcY, int SrcWidth, int SrcHeight, int Flags)
{
	BITMAPINFOHEADER	BMI;
	BMI.biSize			= sizeof(BITMAPINFOHEADER);
	BMI.biWidth			= DestWidth;
	BMI.biHeight		= DestHeight;
	BMI.biPlanes		= 1;
	BMI.biBitCount		= 32;
	BMI.biCompression	= BI_RGB;
	BMI.biSizeImage		= 0;
	BMI.biXPelsPerMeter = 0;
	BMI.biYPelsPerMeter = 0;
	BMI.biClrUsed		= 0;
	BMI.biClrImportant	= 0;
	
	/*
	BYTE				*pDestBits;
	HBITMAP				hbmDest;
	hbmDest = ::CreateDIBSection(DestDC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pDestBits, NULL, 0l);
	*/

	BYTE				*pSrcBits;
	HBITMAP				hbmSrc;
	hbmSrc = ::CreateDIBSection(SrcDC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pSrcBits, NULL, 0l);

	HDC					dc = ::CreateCompatibleDC(NULL);
	HBITMAP				hbm = (HBITMAP) ::SelectObject(dc, hbmSrc);
	
	::SetStretchBltMode(dc, HALFTONE);

	if (!::StretchBlt(dc, 0, 0, DestWidth, DestHeight, SrcDC, SrcX, SrcY, SrcWidth, SrcHeight, SRCCOPY))
		return 0;

	/*
	::SelectObject(dc, hbmDest);

	if (!::StretchBlt(dc, 0, 0, DestWidth, DestHeight, DestDC, DestX, DestY, DestWidth, DestHeight, SRCCOPY))
		return 0;
	*/

	::SelectObject(dc, hbm);

	::DeleteDC(dc);

	for (int y = 0; y < DestHeight - 1; y++)
    {
		LPBYTE pbSrcRGB = (LPBYTE)&((DWORD*)pSrcBits)[y * DestWidth];
		LPBYTE pbSr2RGB = (LPBYTE)&((DWORD*)pSrcBits)[(y + 1) * DestWidth];

		for (int x = 0; x < DestWidth - 1; x++)
		{
			pbSrcRGB[0] = (int)(pbSrcRGB[0] + pbSrcRGB [4] + pbSr2RGB[0] + pbSr2RGB[4]) / 4;
			pbSrcRGB[1] = (int)(pbSrcRGB[1] + pbSrcRGB [5] + pbSr2RGB[1] + pbSr2RGB[5]) / 4;
			pbSrcRGB[2] = (int)(pbSrcRGB[2] + pbSrcRGB [6] + pbSr2RGB[2] + pbSr2RGB[6]) / 4;

			pbSrcRGB += 4;
			pbSr2RGB += 4;
		}
	}

	dc = ::CreateCompatibleDC(NULL);
	
	hbm = (HBITMAP) ::SelectObject(dc, hbmSrc);
	if (!::BitBlt(DestDC, DestX, DestY, DestWidth, DestHeight, dc, 0, 0, SRCCOPY))
		return 0;

	::DeleteDC(dc);

	::DeleteObject(hbmSrc);	
	//::DeleteObject(hbmDest);	
	
	return 1;
}



////////////////////////////////////////////////////////////////////////////
//
//	fxBitBlt()					Function performs a bit-block transfer of
//								the color data corresponding to a rectangle
//								of pixels from the specified source device
//								context into a destination device context.
//								If the function succeeds, the return value
//								is 1 (true); 
//								if fails, the return value is 0 (false). 
//
//	HDC		DestDC				Handle to the destination device context;
//	int		DestX				Specifies the x-coordinate, 
//								in logical units, of the upper-left 
//								corner of the destination rectangle;
//	int		DestY				Specifies the y-coordinate, 
//								in logical units, of the upper-left 
//								corner of the destination rectangle;
//	int		DestWidth			Specifies the width, in logical units,
//								of the destination rectangle.
//	int		DestHeight			Specifies the height, in logical units,
//								of the destination rectangle.
//	HDC		SrcDC				Handle to the source device context;
//	int		SrcX				Specifies the x-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		SrcY				Specifies the y-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		RasterOperation		Specifies a raster-operation code.
//
////////////////////////////////////////////////////////////////////////////
int __stdcall fxBitBlt(HDC DestDC, int DestX, int DestY, int DestWidth, int DestHeight, HDC SrcDC, int SrcX, int SrcY, int RasterOperation)
{
	return ::BitBlt(DestDC, DestX, DestY, DestWidth, DestHeight, SrcDC, SrcX, SrcY, RasterOperation);
}



////////////////////////////////////////////////////////////////////////////
//
//	fxBlur()					Displays blured bitmap, 
//								first stretching or compressing it to fit
//								the dimensions of the destination rectangle,
//								if necessary.
//								If the function succeeds, the return value
//								is 1 (true); 
//								if fails, the return value is 0 (false). 
//
//	HDC		DestDC				Handle to the destination device context;
//	int		DestX				Specifies the x-coordinate, 
//								in logical units, of the upper-left 
//								corner of the destination rectangle;
//	int		DestY				Specifies the y-coordinate, 
//								in logical units, of the upper-left 
//								corner of the destination rectangle;
//	int		DestWidth			Specifies the width, in logical units,
//								of the destination rectangle.
//	int		DestHeight			Specifies the height, in logical units,
//								of the destination rectangle.
//	HDC		SrcDC				Handle to the source device context;
//	int		SrcX				Specifies the x-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		SrcY				Specifies the y-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		SrcWidth			Specifies the width, in logical units,
//								of the source rectangle;
//	int		SrcHeight			Specifies the height, in logical units,
//								of the source rectangle;
//	int		Flags				Reserved parameter.
//
////////////////////////////////////////////////////////////////////////////
int __stdcall fxBlur(HDC DestDC, int DestX, int DestY, int DestWidth, int DestHeight, HDC SrcDC, int SrcX, int SrcY, int SrcWidth, int SrcHeight, int Flags)
{
	BITMAPINFOHEADER	BMI;
	BMI.biSize			= sizeof(BITMAPINFOHEADER);
	BMI.biWidth			= DestWidth;
	BMI.biHeight		= DestHeight;
	BMI.biPlanes		= 1;
	BMI.biBitCount		= 32;
	BMI.biCompression	= BI_RGB;
	BMI.biSizeImage		= 0;
	BMI.biXPelsPerMeter = 0;
	BMI.biYPelsPerMeter = 0;
	BMI.biClrUsed		= 0;
	BMI.biClrImportant	= 0;
	
	/*
	BYTE				*pDestBits;
	HBITMAP				hbmDest;
	hbmDest = ::CreateDIBSection(DestDC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pDestBits, NULL, 0l);
	*/

	BYTE				*pSrcBits;
	HBITMAP				hbmSrc;
	hbmSrc = ::CreateDIBSection(SrcDC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pSrcBits, NULL, 0l);

	HDC					dc = ::CreateCompatibleDC(NULL);
	HBITMAP				hbm = (HBITMAP) ::SelectObject(dc, hbmSrc);
	
	::SetStretchBltMode(dc, HALFTONE);

	if (!::StretchBlt(dc, 0, 0, DestWidth, DestHeight, SrcDC, SrcX, SrcY, SrcWidth, SrcHeight, SRCCOPY))
		return 0;

	/*
	::SelectObject(dc, hbmDest);

	if (!::StretchBlt(dc, 0, 0, DestWidth, DestHeight, DestDC, DestX, DestY, DestWidth, DestHeight, SRCCOPY))
		return 0;
	*/

	::SelectObject(dc, hbm);

	::DeleteDC(dc);

	for (int y = 1; y < DestHeight - 1; y++)
    {
		LPBYTE pbSrcRGB = (LPBYTE)&((DWORD*)pSrcBits)[y * DestWidth];
		LPBYTE pbSrcRGBy1 = (LPBYTE)&((DWORD*)pSrcBits)[(y-1) * DestWidth];
		LPBYTE pbSrcRGBy2 = (LPBYTE)&((DWORD*)pSrcBits)[(y+1) * DestWidth];
		//LPBYTE pbDestRGB = (LPBYTE)&((DWORD*)pDestBits)[y * DestWidth];

		for (int x = 1; x < DestWidth - 1; x++)
		{	
			pbSrcRGB[4] = (pbSrcRGB[0] + pbSrcRGB[4] + pbSrcRGB[8] + pbSrcRGBy1[0] + pbSrcRGBy1[4] + pbSrcRGBy1[8] + pbSrcRGBy2[0] + pbSrcRGBy2[4] + pbSrcRGBy2[8]) / 9;
			pbSrcRGB[5] = (pbSrcRGB[1] + pbSrcRGB[5] + pbSrcRGB[9] + pbSrcRGBy1[1] + pbSrcRGBy1[5] + pbSrcRGBy1[9] + pbSrcRGBy2[1] + pbSrcRGBy2[5] + pbSrcRGBy2[9]) / 9;
			pbSrcRGB[6] = (pbSrcRGB[2] + pbSrcRGB[6] + pbSrcRGB[10] + pbSrcRGBy1[2] + pbSrcRGBy1[6] + pbSrcRGBy1[10] + pbSrcRGBy2[2] + pbSrcRGBy2[6] + pbSrcRGBy2[10]) / 9;

			pbSrcRGB += 4;
			pbSrcRGBy1 += 4;
			pbSrcRGBy2 += 4;
			//pbDestRGB += 4;
		}
	}

	dc = ::CreateCompatibleDC(NULL);
	
	hbm = (HBITMAP) ::SelectObject(dc, hbmSrc);
	if (!::BitBlt(DestDC, DestX, DestY, DestWidth, DestHeight, dc, 0, 0, SRCCOPY))
		return 0;

	::DeleteDC(dc);

	::DeleteObject(hbmSrc);	
	//::DeleteObject(hbmDest);	
	
	return 1;
}



////////////////////////////////////////////////////////////////////////////
//
//	fxBrightness()				Displays bitmap with specified 
//								brightness value, 
//								first stretching or compressing it to fit
//								the dimensions of the destination rectangle,
//								if necessary.
//								If the function succeeds, the return value
//								is 1 (true); 
//								if fails, the return value is 0 (false). 
//
//	HDC		DestDC				Handle to the destination device context;
//	int		DestX				Specifies the x-coordinate, 
//								in logical units, of the upper-left 
//								corner of the destination rectangle;
//	int		DestY				Specifies the y-coordinate, 
//								in logical units, of the upper-left 
//								corner of the destination rectangle;
//	int		DestWidth			Specifies the width, in logical units,
//								of the destination rectangle.
//	int		DestHeight			Specifies the height, in logical units,
//								of the destination rectangle.
//	HDC		SrcDC				Handle to the source device context;
//	int		SrcX				Specifies the x-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		SrcY				Specifies the y-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		SrcWidth			Specifies the width, in logical units,
//								of the source rectangle;
//	int		SrcHeight			Specifies the height, in logical units,
//								of the source rectangle.
//	int		Brightness			Brightness (value in range -255 to 255);
//	int		Flags				Reserved parameter.
//
////////////////////////////////////////////////////////////////////////////
int __stdcall fxBrightness(HDC DestDC, int DestX, int DestY, int DestWidth, int DestHeight, HDC SrcDC, int SrcX, int SrcY, int SrcWidth, int SrcHeight, int Brightness, int Flags)
{
	if (Brightness < -255)
		Brightness = -255;
	if (Brightness > 255)
		Brightness = 255;

	BITMAPINFOHEADER	BMI;
	BMI.biSize			= sizeof(BITMAPINFOHEADER);
	BMI.biWidth			= DestWidth;
	BMI.biHeight		= DestHeight;
	BMI.biPlanes		= 1;
	BMI.biBitCount		= 32;
	BMI.biCompression	= BI_RGB;
	BMI.biSizeImage		= 0;
	BMI.biXPelsPerMeter = 0;
	BMI.biYPelsPerMeter = 0;
	BMI.biClrUsed		= 0;
	BMI.biClrImportant	= 0;

	/*
	BYTE				*pDestBits;
	HBITMAP				hbmDest;
	hbmDest = ::CreateDIBSection(DestDC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pDestBits, NULL, 0l);
	*/

	BYTE				*pSrcBits;
	HBITMAP				hbmSrc;
	hbmSrc = ::CreateDIBSection(SrcDC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pSrcBits, NULL, 0l);

	HDC					dc = ::CreateCompatibleDC(NULL);
	HBITMAP				hbm = (HBITMAP) ::SelectObject(dc, hbmSrc);
	
	::SetStretchBltMode(dc, HALFTONE);

	if (!::StretchBlt(dc, 0, 0, DestWidth, DestHeight, SrcDC, SrcX, SrcY, SrcWidth, SrcHeight, SRCCOPY))
		return 0;

	/*
	::SelectObject(dc, hbmDest);

	if (!::StretchBlt(dc, 0, 0, DestWidth, DestHeight, DestDC, DestX, DestY, DestWidth, DestHeight, SRCCOPY))
		return 0;
	*/

	::SelectObject(dc, hbm);

	::DeleteDC(dc);

    for (int y = 0; y < DestHeight; y++)
    {
		LPBYTE pbSrcRGB = (LPBYTE)&((DWORD*)pSrcBits)[y * DestWidth];
		//LPBYTE pbDestRGB = (LPBYTE)&((DWORD*)pDestBits)[y * DestWidth];

		for (int x = 0; x < DestWidth; x++)
		{
			pbSrcRGB[0] = fxByte(pbSrcRGB[0] + Brightness);
			pbSrcRGB[1] = fxByte(pbSrcRGB[1] + Brightness);
			pbSrcRGB[2] = fxByte(pbSrcRGB[2] + Brightness);

			pbSrcRGB += 4;
			//pbDestRGB += 4;
		}
	}

	dc = ::CreateCompatibleDC(NULL);
	
	hbm = (HBITMAP) ::SelectObject(dc, hbmSrc);
	if (!::BitBlt(DestDC, DestX, DestY, DestWidth, DestHeight, dc, 0, 0, SRCCOPY))
		return 0;

	::DeleteDC(dc);

	::DeleteObject(hbmSrc);	
	//::DeleteObject(hbmDest);	
	
	return 1;
}


////////////////////////////////////////////////////////////////////////////
//
//	fxCMYK()					Function returns CMYK color from 
//								given C, M, Y, K values.
//
//	int		C					Cyan value (value in range 0 to 255);
//	int		M					Magenta value (value in range 0 to 255);
//	int		Y					Yellow value (value in range 0 to 255);
//	int		K					Black value (value in range 0 to 255);
//
////////////////////////////////////////////////////////////////////////////
int __stdcall fxCMYK(int C, int M, int Y, int K)
{
	return CMYK(C, M, Y, K);
}



////////////////////////////////////////////////////////////////////////////
//
//	fxDrawText()				Renders bitmap first roatating, zooming or
//								alphablending it.
//								If the function succeeds, the return value
//								is 1 (true); 
//								if fails, the return value is 0 (false). 
//
//	HDC		DestDC				Handle to the destination device context;
//	int		DestX				Specifies the x-coordinate, 
//								in logical units, of the upper-left 
//								corner of the destination rectangle;
//	int		DestY				Specifies the y-coordinate, 
//								in logical units, of the upper-left 
//								corner of the destination rectangle;
//	HDC		SrcDC				Handle to the source device context;
//	int		SrcX				Specifies the x-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		SrcY				Specifies the y-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		SrcWidth			Specifies the width, in logical units,
//								of the source and destination rectangles;
//	int		SrcHeight			Specifies the height, in logical units,
//								of the source and destination rectangles;
//	int		Text				Text to write;
//	int		Color				The RGB color of the text;
//	HFONT	Font				Font object to use;
//	int		Blending			Blending value (0-255);
//	int		Flags				Reserved parameter.
//
////////////////////////////////////////////////////////////////////////////
int __stdcall fxDrawText(HDC DC, int X, int Y, int Width, int Height, LPCSTR Text, int Color, HFONT Font, int Blending, int Flags)
{
	HDC DestDC			= DC;
	int DestX			= X;
	int DestY			= Y;
	int DestWidth		= Width;
	int DestHeight		= Height;
	HDC SrcDC			= DC;
	int SrcX			= X;
	int SrcY			= Y;
	int SrcWidth		= Width;
	int SrcHeight		= Height;
	int TransparentColor = fxRGB(255, 255, 255);

	if (Blending < 0)
		Blending = 0;
	if (Blending > 255)
		Blending = 255;

	BITMAPINFOHEADER	BMI;
	BMI.biSize			= sizeof(BITMAPINFOHEADER);
	BMI.biWidth			= DestWidth;
	BMI.biHeight		= DestHeight;
	BMI.biPlanes		= 1;
	BMI.biBitCount		= 32;
	BMI.biCompression	= BI_RGB;
	BMI.biSizeImage		= 0;
	BMI.biXPelsPerMeter = 0;
	BMI.biYPelsPerMeter = 0;
	BMI.biClrUsed		= 0;
	BMI.biClrImportant	= 0;
	
	BYTE				*pDestBits;
	HBITMAP				hbmDest;
	hbmDest				= ::CreateDIBSection(DestDC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pDestBits, NULL, 0l);

	BYTE				*pSrcBits;
	HBITMAP				hbmSrc;
	hbmSrc				= ::CreateDIBSection(SrcDC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pSrcBits, NULL, 0l);

	HDC	dc				= ::CreateCompatibleDC(NULL);
	HBITMAP	hbm			= (HBITMAP)::SelectObject(dc, hbmSrc);
	
	::SetStretchBltMode(dc, HALFTONE);

	if (!::StretchBlt(dc, 0, 0, DestWidth, DestHeight, SrcDC, SrcX, SrcY, SrcWidth, SrcHeight, WHITENESS))
		return 0;
	
	/*
	HFONT Font = ::CreateFont(-MulDiv(Size, ::GetDeviceCaps(dc, LOGPIXELSY), 72), 0, 0, 0, Weight, Italic, Underline, Strikeout, Charset, 0, 0, PROOF_QUALITY, 0, Font);
	*/

	::DeleteObject(::SelectObject(dc, Font));

	::SetTextColor(dc, Color);
	::TextOut(dc, 0, 0, Text, strlen(Text));

	::SelectObject(dc, hbmDest);

	if (!::StretchBlt(dc, 0, 0, DestWidth, DestHeight, DestDC, DestX, DestY, DestWidth, DestHeight, SRCCOPY))
		return 0;

	::SelectObject(dc, hbm);
	::DeleteDC(dc);


    for (int y = 0; y < DestHeight; y++)
    {
		LPBYTE pbDestRGB = (LPBYTE)&((DWORD*)pDestBits)[y * DestWidth];
		LPBYTE pbSrcRGB = (LPBYTE)&((DWORD*)pSrcBits)[y * DestWidth];

		for (int x = 0; x < DestWidth; x++)
		{

			if (TransparentColor >= 0)
			{
				if (TransparentColor == fxRGB(pbSrcRGB[2], pbSrcRGB[1], pbSrcRGB[0]))
				{
					pbSrcRGB[0] = pbDestRGB[0];
					pbSrcRGB[1] = pbDestRGB[1];
					pbSrcRGB[2] = pbDestRGB[2];
				}
				else
				{
					pbSrcRGB[0] = fxShadeColors(pbDestRGB[0], pbSrcRGB[0], Blending);
					pbSrcRGB[1] = fxShadeColors(pbDestRGB[1], pbSrcRGB[1], Blending);
					pbSrcRGB[2] = fxShadeColors(pbDestRGB[2], pbSrcRGB[2], Blending);
				}
			}
			else
			{
				pbSrcRGB[0] = fxShadeColors(pbDestRGB[0], pbSrcRGB[0], Blending);
				pbSrcRGB[1] = fxShadeColors(pbDestRGB[1], pbSrcRGB[1], Blending);
				pbSrcRGB[2] = fxShadeColors(pbDestRGB[2], pbSrcRGB[2], Blending);
			}

			pbSrcRGB += 4;
			pbDestRGB += 4;
		}
	}

	dc = ::CreateCompatibleDC(NULL);
	hbm = (HBITMAP)::SelectObject(dc, hbmSrc);

	if (!::BitBlt(DestDC, DestX, DestY, DestWidth, DestHeight, dc, 0, 0, SRCCOPY))
		return 0;

	::DeleteDC(dc);

	::DeleteObject(hbm);
	::DeleteObject(hbmSrc);	
	::DeleteObject(hbmDest);	
	
	return 1;
}



////////////////////////////////////////////////////////////////////////////
//
//	fxEmboss()					Renders bitmap with emboss effect.
//								If the function succeeds, the return value
//								is 1 (true); 
//								if fails, the return value is 0 (false). 
//
//	HDC		DestDC				Handle to the destination device context;
//	int		DestX				Specifies the x-coordinate, 
//								in logical units, of the upper-left 
//								corner of the destination rectangle;
//	int		DestY				Specifies the y-coordinate, 
//								in logical units, of the upper-left 
//								corner of the destination rectangle;
//	int		DestWidth			Specifies the width, in logical units,
//								of the source and destination rectangles;
//	int		DestHeight			Specifies the height, in logical units,
//								of the source and destination rectangles;
//	HDC		SrcDC				Handle to the source device context;
//	int		SrcX				Specifies the x-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		SrcY				Specifies the y-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		Angle				Rotation angle (value in range -360 to 360);
//	int		TransparentColor	The RGB color in the source device context
//								to treat as transparent;
//								if (TransparentColor = -1) the transparency
//								will not be used;
//	int		Flags				Reserved parameter.
//
////////////////////////////////////////////////////////////////////////////
int __stdcall fxEmboss(HDC DestDC, int DestX, int DestY, int DestWidth, int DestHeight, HDC SrcDC, int SrcX, int SrcY, int SrcWidth, int SrcHeight, double Depth, int Flags)
{
	if (Depth < 0)
		Depth = 0;
	if (Depth > 4)
		Depth = 4;

	BITMAPINFOHEADER	BMI;
	BMI.biSize			= sizeof(BITMAPINFOHEADER);
	BMI.biWidth			= DestWidth;
	BMI.biHeight		= DestHeight;
	BMI.biPlanes		= 1;
	BMI.biBitCount		= 32;
	BMI.biCompression	= BI_RGB;
	BMI.biSizeImage		= 0;
	BMI.biXPelsPerMeter = 0;
	BMI.biYPelsPerMeter = 0;
	BMI.biClrUsed		= 0;
	BMI.biClrImportant	= 0;

	/*
	BYTE				*pDestBits;
	HBITMAP				hbmDest;
	hbmDest = ::CreateDIBSection(DestDC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pDestBits, NULL, 0l);
	*/

	BYTE				*pSrcBits;
	HBITMAP				hbmSrc;
	hbmSrc = ::CreateDIBSection(SrcDC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pSrcBits, NULL, 0l);

	HDC					dc = ::CreateCompatibleDC(NULL);
	HBITMAP				hbm = (HBITMAP) ::SelectObject(dc, hbmSrc);
	
	::SetStretchBltMode(dc, HALFTONE);

	if (!::StretchBlt(dc, 0, 0, DestWidth, DestHeight, SrcDC, SrcX, SrcY, SrcWidth, SrcHeight, SRCCOPY))
		return 0;

	/*
	::SelectObject(dc, hbmDest);

	if (!::StretchBlt(dc, 0, 0, DestWidth, DestHeight, DestDC, DestX, DestY, DestWidth, DestHeight, SRCCOPY))
		return 0;
	*/

	::SelectObject(dc, hbm);

	::DeleteDC(dc);


	int fxR = 0;
	int fxG	= 0;
	int fxB = 0;

	int fxGreyscale;

    for (int y = 0; y < DestHeight - 1; y++)
    {
		LPBYTE pbSrcRGB = (LPBYTE)&((DWORD*)pSrcBits)[y * DestWidth];
		LPBYTE pbSr2RGB = (LPBYTE)&((DWORD*)pSrcBits)[(y + 1) * DestWidth];

		for (int x = 0; x < DestWidth - 1; x++)
		{
			fxB = abs((int)((pbSrcRGB[(x * 4) + 0] - pbSr2RGB[((x + 1) * 4) + 0]) * Depth + 128));
			fxG = abs((int)((pbSrcRGB[(x * 4) + 1] - pbSr2RGB[((x + 1) * 4) + 1]) * Depth + 128));
			fxR = abs((int)((pbSrcRGB[(x * 4) + 2] - pbSr2RGB[((x + 1) * 4) + 2]) * Depth + 128));

			fxGreyscale = fxByte((fxR + fxG + fxB) / 3);

			pbSrcRGB[(x * 4) + 0] = fxGreyscale;
			pbSrcRGB[(x * 4) + 1] = fxGreyscale;
			pbSrcRGB[(x * 4) + 2] = fxGreyscale;
		}
	}

	dc = ::CreateCompatibleDC(NULL);
	
	hbm = (HBITMAP) ::SelectObject(dc, hbmSrc);

	if (!::BitBlt(DestDC, DestX, DestY, DestWidth, DestHeight, dc, 0, 0, SRCCOPY))
		return 0;

	::DeleteDC(dc);

	::DeleteObject(hbmSrc);	
	//::DeleteObject(hbmDest);	
	
	return 1;
}



////////////////////////////////////////////////////////////////////////////
//
//	fxEngrave()					Renders bitmap with engrave effect.
//								If the function succeeds, the return value
//								is 1 (true); 
//								if fails, the return value is 0 (false). 
//
//	HDC		DestDC				Handle to the destination device context;
//	int		DestX				Specifies the x-coordinate, 
//								in logical units, of the upper-left 
//								corner of the destination rectangle;
//	int		DestY				Specifies the y-coordinate, 
//								in logical units, of the upper-left 
//								corner of the destination rectangle;
//	int		DestWidth			Specifies the width, in logical units,
//								of the source and destination rectangles;
//	int		DestHeight			Specifies the height, in logical units,
//								of the source and destination rectangles;
//	HDC		SrcDC				Handle to the source device context;
//	int		SrcX				Specifies the x-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		SrcY				Specifies the y-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		Angle				Rotation angle (value in range -360 to 360);
//	int		TransparentColor	The RGB color in the source device context
//								to treat as transparent;
//								if (TransparentColor = -1) the transparency
//								will not be used;
//	int		Flags				Reserved parameter.
//
////////////////////////////////////////////////////////////////////////////
int __stdcall fxEngrave(HDC DestDC, int DestX, int DestY, int DestWidth, int DestHeight, HDC SrcDC, int SrcX, int SrcY, int SrcWidth, int SrcHeight, double Height, int Flags)
{
	if (Height < 0)
		Height = 0;
	if (Height > 4)
		Height = 4;

	BITMAPINFOHEADER	BMI;
	BMI.biSize			= sizeof(BITMAPINFOHEADER);
	BMI.biWidth			= DestWidth;
	BMI.biHeight		= DestHeight;
	BMI.biPlanes		= 1;
	BMI.biBitCount		= 32;
	BMI.biCompression	= BI_RGB;
	BMI.biSizeImage		= 0;
	BMI.biXPelsPerMeter = 0;
	BMI.biYPelsPerMeter = 0;
	BMI.biClrUsed		= 0;
	BMI.biClrImportant	= 0;

	/*
	BYTE				*pDestBits;
	HBITMAP				hbmDest;
	hbmDest = ::CreateDIBSection(DestDC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pDestBits, NULL, 0l);
	*/

	BYTE				*pSrcBits;
	HBITMAP				hbmSrc;
	hbmSrc = ::CreateDIBSection(SrcDC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pSrcBits, NULL, 0l);

	HDC					dc = ::CreateCompatibleDC(NULL);
	HBITMAP				hbm = (HBITMAP) ::SelectObject(dc, hbmSrc);
	
	::SetStretchBltMode(dc, HALFTONE);

	if (!::StretchBlt(dc, 0, 0, DestWidth, DestHeight, SrcDC, SrcX, SrcY, SrcWidth, SrcHeight, SRCCOPY))
		return 0;

	/*
	::SelectObject(dc, hbmDest);

	if (!::StretchBlt(dc, 0, 0, DestWidth, DestHeight, DestDC, DestX, DestY, DestWidth, DestHeight, SRCCOPY))
		return 0;
	*/

	::SelectObject(dc, hbm);

	::DeleteDC(dc);


	int fxR = 0;
	int fxG	= 0;
	int fxB = 0;

	int fxGreyscale;

    for (int y = 0; y < DestHeight - 1; y++)
    {
		LPBYTE pbSrcRGB = (LPBYTE)&((DWORD*)pSrcBits)[y * DestWidth];
		LPBYTE pbSr2RGB = (LPBYTE)&((DWORD*)pSrcBits)[(y + 1) * DestWidth];

		for (int x = 0; x < DestWidth - 1; x++)
		{
			fxB = abs((int)((pbSr2RGB[((x + 1) * 4) + 0] - pbSrcRGB[(x * 4) + 0]) * Height + 128));
			fxG = abs((int)((pbSr2RGB[((x + 1) * 4) + 1] - pbSrcRGB[(x * 4) + 1]) * Height + 128));
			fxR = abs((int)((pbSr2RGB[((x + 1) * 4) + 2] - pbSrcRGB[(x * 4) + 2]) * Height + 128));

			fxGreyscale = fxByte((fxR + fxG + fxB) / 3);

			pbSrcRGB[(x * 4) + 0] = fxGreyscale;
			pbSrcRGB[(x * 4) + 1] = fxGreyscale;
			pbSrcRGB[(x * 4) + 2] = fxGreyscale;
		}
	}

	dc = ::CreateCompatibleDC(NULL);
	
	hbm = (HBITMAP) ::SelectObject(dc, hbmSrc);

	if (!::BitBlt(DestDC, DestX, DestY, DestWidth, DestHeight, dc, 0, 0, SRCCOPY))
		return 0;

	::DeleteDC(dc);

	::DeleteObject(hbmSrc);	
	//::DeleteObject(hbmDest);	
	
	return 1;
}



////////////////////////////////////////////////////////////////////////////
//
//	fxGamma()					Function displays bitmap with specified 
//								gamma value, 
//								first stretching or compressing it to fit
//								the dimensions of the destination rectangle,
//								if necessary.
//								If the function succeeds, the return value
//								is 1 (true); 
//								if fails, the return value is 0 (false). 
//
//	HDC		DestDC				Handle to the destination device context;
//	int		DestX				Specifies the x-coordinate, 
//								in logical units, of the upper-left 
//								corner of the destination rectangle;
//	int		DestY				Specifies the y-coordinate, 
//								in logical units, of the upper-left 
//								corner of the destination rectangle;
//	int		DestWidth			Specifies the width, in logical units,
//								of the destination rectangle.
//	int		DestHeight			Specifies the height, in logical units,
//								of the destination rectangle.
//	HDC		SrcDC				Handle to the source device context;
//	int		SrcX				Specifies the x-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		SrcY				Specifies the y-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		SrcWidth			Specifies the width, in logical units,
//								of the source rectangle;
//	int		SrcHeight			Specifies the height, in logical units,
//								of the source rectangle.
//	double	Gamma				Gamma (value in range 0 to 10);
//	int		Flags				Reserved parameter.
//
////////////////////////////////////////////////////////////////////////////
int __stdcall fxGamma(HDC DestDC, int DestX, int DestY, int DestWidth, int DestHeight, HDC SrcDC, int SrcX, int SrcY, int SrcWidth, int SrcHeight, double Gamma, int Flags)
{
	if (Gamma < 0)
		Gamma = 0;
	if (Gamma > 10)
		Gamma = 10;

	BITMAPINFOHEADER	BMI;
	BMI.biSize			= sizeof(BITMAPINFOHEADER);
	BMI.biWidth			= DestWidth;
	BMI.biHeight		= DestHeight;
	BMI.biPlanes		= 1;
	BMI.biBitCount		= 32;
	BMI.biCompression	= BI_RGB;
	BMI.biSizeImage		= 0;
	BMI.biXPelsPerMeter = 0;
	BMI.biYPelsPerMeter = 0;
	BMI.biClrUsed		= 0;
	BMI.biClrImportant	= 0;
	
	/*
	BYTE				*pDestBits;
	HBITMAP				hbmDest;
	hbmDest = ::CreateDIBSection(DestDC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pDestBits, NULL, 0l);
	*/

	BYTE				*pSrcBits;
	HBITMAP				hbmSrc;
	hbmSrc = ::CreateDIBSection(SrcDC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pSrcBits, NULL, 0l);

	HDC					dc = ::CreateCompatibleDC(NULL);
	HBITMAP				hbm = (HBITMAP) ::SelectObject(dc, hbmSrc);
	
	::SetStretchBltMode(dc, HALFTONE);

	if (!::StretchBlt(dc, 0, 0, DestWidth, DestHeight, SrcDC, SrcX, SrcY, SrcWidth, SrcHeight, SRCCOPY))
		return 0;

	/*
	::SelectObject(dc, hbmDest);

	if (!::StretchBlt(dc, 0, 0, DestWidth, DestHeight, DestDC, DestX, DestY, DestWidth, DestHeight, SRCCOPY))
		return 0;
	*/

	::SelectObject(dc, hbm);

	::DeleteDC(dc);

    for (int y = 0; y < DestHeight; y++)
    {
		LPBYTE pbSrcRGB = (LPBYTE)&((DWORD*)pSrcBits)[y * DestWidth];
		//LPBYTE pbDestRGB = (LPBYTE)&((DWORD*)pDestBits)[y * DestWidth];

		for (int x = 0; x < DestWidth; x++)
		{
			pbSrcRGB[0] = (int)(255.0 * pow(pbSrcRGB[0] / 255.0, 1.0 / Gamma));
			pbSrcRGB[1] = (int)(255.0 * pow(pbSrcRGB[1] / 255.0, 1.0 / Gamma));
			pbSrcRGB[2] = (int)(255.0 * pow(pbSrcRGB[2] / 255.0, 1.0 / Gamma));

			pbSrcRGB += 4;
			//pbDestRGB += 4;
		}
	}

	dc = ::CreateCompatibleDC(NULL);
	
	hbm = (HBITMAP) ::SelectObject(dc, hbmSrc);
	if (!::BitBlt(DestDC, DestX, DestY, DestWidth, DestHeight, dc, 0, 0, SRCCOPY))
		return 0;

	::DeleteDC(dc);

	::DeleteObject(hbmSrc);	
	//::DeleteObject(hbmDest);	
	
	return 1;
}



////////////////////////////////////////////////////////////////////////////
//
//	fxGetBlue()					These function returns the red, green or
//	fxGetGreen()				blue values (value in range 0 to 255)
//  fxGetRed()					from the given RGB color value.
//  
//	int		RGB					Specifies the RGB color.
//
////////////////////////////////////////////////////////////////////////////
int __stdcall fxGetBlue(int RGB)
{
	//return ((RGB & 16711680) / 65536);
	return GetBValue(RGB);
}


int __stdcall fxGetGreen(int RGB)
{
	//return ((RGB & 65280) / 256);
	return GetGValue(RGB);
}


int __stdcall fxGetRed(int RGB)
{
	//return (RGB & 255);
	return GetRValue(RGB);
}



////////////////////////////////////////////////////////////////////////////
//
//	fxGreyscale()				Displays greyscaled bitmap, 
//								first stretching or compressing it to fit
//								the dimensions of the destination rectangle,
//								if necessary.
//								If the function succeeds, the return value
//								is 1 (true); 
//								if fails, the return value is 0 (false). 
//
//	HDC		DestDC				Handle to the destination device context;
//	int		DestX				Specifies the x-coordinate, 
//								in logical units, of the upper-left 
//								corner of the destination rectangle;
//	int		DestY				Specifies the y-coordinate, 
//								in logical units, of the upper-left 
//								corner of the destination rectangle;
//	int		DestWidth			Specifies the width, in logical units,
//								of the destination rectangle.
//	int		DestHeight			Specifies the height, in logical units,
//								of the destination rectangle.
//	HDC		SrcDC				Handle to the source device context;
//	int		SrcX				Specifies the x-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		SrcY				Specifies the y-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		SrcWidth			Specifies the width, in logical units,
//								of the source rectangle;
//	int		SrcHeight			Specifies the height, in logical units,
//								of the source rectangle;
//	int		Flags				Reserved parameter.
//
////////////////////////////////////////////////////////////////////////////
int __stdcall fxGreyscale(HDC DestDC, int DestX, int DestY, int DestWidth, int DestHeight, HDC SrcDC, int SrcX, int SrcY, int SrcWidth, int SrcHeight, int Flags)
{
	int	fxG				= 0;

	BITMAPINFOHEADER	BMI;
	BMI.biSize			= sizeof(BITMAPINFOHEADER);
	BMI.biWidth			= DestWidth;
	BMI.biHeight		= DestHeight;
	BMI.biPlanes		= 1;
	BMI.biBitCount		= 32;
	BMI.biCompression	= BI_RGB;
	BMI.biSizeImage		= 0;
	BMI.biXPelsPerMeter = 0;
	BMI.biYPelsPerMeter = 0;
	BMI.biClrUsed		= 0;
	BMI.biClrImportant	= 0;
	
	/*
	BYTE				*pDestBits;
	HBITMAP				hbmDest;
	hbmDest = ::CreateDIBSection(DestDC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pDestBits, NULL, 0l);
	*/

	BYTE				*pSrcBits;
	HBITMAP				hbmSrc;
	hbmSrc = ::CreateDIBSection(SrcDC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pSrcBits, NULL, 0l);

	HDC					dc = ::CreateCompatibleDC(NULL);
	HBITMAP				hbm = (HBITMAP) ::SelectObject(dc, hbmSrc);
	
	::SetStretchBltMode(dc, HALFTONE);

	if (!::StretchBlt(dc, 0, 0, DestWidth, DestHeight, SrcDC, SrcX, SrcY, SrcWidth, SrcHeight, SRCCOPY))
		return 0;

	/*
	::SelectObject(dc, hbmDest);

	if (!::StretchBlt(dc, 0, 0, DestWidth, DestHeight, DestDC, DestX, DestY, DestWidth, DestHeight, SRCCOPY))
		return 0;
	*/

	::SelectObject(dc, hbm);

	::DeleteDC(dc);

    for (int y = 0; y < DestHeight; y++)
    {
		LPBYTE pbSrcRGB = (LPBYTE)&((DWORD*)pSrcBits)[y * DestWidth];
		//LPBYTE pbDestRGB = (LPBYTE)&((DWORD*)pDestBits)[y * DestWidth];

		for (int x = 0; x < DestWidth; x++)
		{
			fxG = ((pbSrcRGB[0] + pbSrcRGB[1] + pbSrcRGB[2]) / 3);

			pbSrcRGB[0] = fxG;
			pbSrcRGB[1] = fxG;
			pbSrcRGB[2] = fxG;

			pbSrcRGB += 4;
			//pbDestRGB += 4;
		}
	}

	dc = ::CreateCompatibleDC(NULL);
	
	hbm = (HBITMAP) ::SelectObject(dc, hbmSrc);
	if (!::BitBlt(DestDC, DestX, DestY, DestWidth, DestHeight, dc, 0, 0, SRCCOPY))
		return 0;

	::DeleteDC(dc);

	::DeleteObject(hbmSrc);	
	//::DeleteObject(hbmDest);	
	
	return 1;
}



////////////////////////////////////////////////////////////////////////////
//
//	fxGridelines()				Displays gride-lined bitmap,
//								first stretching or compressing it to fit
//								the dimensions of the destination rectangle,
//								if necessary.
//								If the function succeeds, the return value
//								is 1 (true); 
//								if fails, the return value is 0 (false). 
//
//	HDC		DestDC				Handle to the destination device context;
//	int		DestX				Specifies the x-coordinate, 
//								in logical units, of the upper-left 
//								corner of the destination rectangle;
//	int		DestY				Specifies the y-coordinate, 
//								in logical units, of the upper-left 
//								corner of the destination rectangle;
//	int		DestWidth			Specifies the width, in logical units,
//								of the destination rectangle.
//	int		DestHeight			Specifies the height, in logical units,
//								of the destination rectangle.
//	HDC		SrcDC				Handle to the source device context;
//	int		SrcX				Specifies the x-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		SrcY				Specifies the y-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		SrcWidth			Specifies the width, in logical units,
//								of the source rectangle;
//	int		SrcHeight			Specifies the height, in logical units,
//								of the source rectangle;
//	int		Color				Color of scan-lines (RGB);
//	int		Transparency		Transparency of scanlines (value in
//								range 0 to 255);
//	int		Distance			Distance between scanlines;
//	int		Flags				Reserved parameter.
//
////////////////////////////////////////////////////////////////////////////
int __stdcall fxGridelines(HDC DestDC, int DestX, int DestY, int DestWidth, int DestHeight, HDC SrcDC, int SrcX, int SrcY, int SrcWidth, int SrcHeight, int Color, int Transparency, int Step, int Flags)
{
	Step				= Step + 1;

	if (Transparency < 0)
		Transparency = 0;
	if (Transparency > 255)
		Transparency = 255;

	if (Step < 1)
		Step = 1;
	if (Step > 255)
		Step = 255;

	int fxR				= fxByte(fxGetRed(Color));
	int fxG				= fxByte(fxGetGreen(Color));
	int fxB				= fxByte(fxGetBlue(Color));

	BITMAPINFOHEADER	BMI;
	BMI.biSize			= sizeof(BITMAPINFOHEADER);
	BMI.biWidth			= DestWidth;
	BMI.biHeight		= DestHeight;
	BMI.biPlanes		= 1;
	BMI.biBitCount		= 32;
	BMI.biCompression	= BI_RGB;
	BMI.biSizeImage		= 0;
	BMI.biXPelsPerMeter = 0;
	BMI.biYPelsPerMeter = 0;
	BMI.biClrUsed		= 0;
	BMI.biClrImportant	= 0;
	
	/*
	BYTE				*pDestBits;
	HBITMAP				hbmDest;
	hbmDest = ::CreateDIBSection(DestDC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pDestBits, NULL, 0l);
	*/

	BYTE				*pSrcBits;
	HBITMAP				hbmSrc;
	hbmSrc = ::CreateDIBSection(SrcDC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pSrcBits, NULL, 0l);

	HDC					dc = ::CreateCompatibleDC(NULL);
	HBITMAP				hbm = (HBITMAP) ::SelectObject(dc, hbmSrc);
	
	::SetStretchBltMode(dc, HALFTONE);

	if (!::StretchBlt(dc, 0, 0, DestWidth, DestHeight, SrcDC, SrcX, SrcY, SrcWidth, SrcHeight, SRCCOPY))
		return 0;

	/*
	::SelectObject(dc, hbmDest);

	if (!::StretchBlt(dc, 0, 0, DestWidth, DestHeight, DestDC, DestX, DestY, DestWidth, DestHeight, SRCCOPY))
		return 0;
	*/

	::SelectObject(dc, hbm);

	::DeleteDC(dc);

	if (Step > 1)
	{
		for (int y = 0; y < DestHeight; y++)
		{
			LPBYTE pbSrcRGB = (LPBYTE)&((DWORD*)pSrcBits)[y * DestWidth];
			//LPBYTE pbDestRGB = (LPBYTE)&((DWORD*)pDestBits)[y * DestWidth];

			for (int x = 0; x < DestWidth; x++)
			{
				if (((DestHeight - y) % Step) == 1)
					pbSrcRGB[0] = fxShadeColors(fxB, pbSrcRGB[0], Transparency),
					pbSrcRGB[1] = fxShadeColors(fxG, pbSrcRGB[1], Transparency),
					pbSrcRGB[2] = fxShadeColors(fxR, pbSrcRGB[2], Transparency);
					
				if ((x % Step) == 0) 
					pbSrcRGB[0] = fxShadeColors(fxB, pbSrcRGB[0], Transparency),
					pbSrcRGB[1] = fxShadeColors(fxG, pbSrcRGB[1], Transparency),
					pbSrcRGB[2] = fxShadeColors(fxR, pbSrcRGB[2], Transparency);

				pbSrcRGB += 4;
				//pbDestRGB += 4;
			}
		}
	}
	else
	{
		for (int y = 0; y < DestHeight; y++)
		{
			LPBYTE pbSrcRGB = (LPBYTE)&((DWORD*)pSrcBits)[y * DestWidth];
			//LPBYTE pbDestRGB = (LPBYTE)&((DWORD*)pDestBits)[y * DestWidth];

			for (int x = 0; x < DestWidth; x++)
			{
				pbSrcRGB[0] = fxShadeColors(fxB, pbSrcRGB[0], Transparency);
				pbSrcRGB[1] = fxShadeColors(fxG, pbSrcRGB[1], Transparency);
				pbSrcRGB[2] = fxShadeColors(fxR, pbSrcRGB[2], Transparency);

				pbSrcRGB += 4;
				//pbDestRGB += 4;
			}
		}
	}

	dc = ::CreateCompatibleDC(NULL);
	
	hbm = (HBITMAP) ::SelectObject(dc, hbmSrc);
	if (!::BitBlt(DestDC, DestX, DestY, DestWidth, DestHeight, dc, 0, 0, SRCCOPY))
		return 0;

	::DeleteDC(dc);

	::DeleteObject(hbmSrc);	
	//::DeleteObject(hbmDest);	
	
	return 1;
}



////////////////////////////////////////////////////////////////////////////
//
//	fxHSL()						Function returns RGB color value
//								from given HSL values.
//
//	int		H					Hue value (in range 0 to 1);
//	int		S					Saturation value (in range 0 to 1);
//	int		L					Luminiscency value (in range 0 to 1).
//
////////////////////////////////////////////////////////////////////////////
double HuetoRGB(double m1, double m2, double h)
{
	if (h < 0) h += 1.0;
	if (h > 1) h -= 1.0;
	if (6.0 * h < 1)
		return (m1+(m2-m1)*h*6.0);
	if (2.0 * h < 1)
		return m2;
	if (3.0 * h < 2.0 )
		return (m1+(m2-m1)*((2.0/3.0)-h)*6.0);
	return m1;
}

int __stdcall fxHSLtoRGB(double H, double S, double L)
{
	double r, g, b;
	double m1, m2;

	if (S == 0)
	{
		r=g=b=L;
	} 
	else 
	{
		if (L <= 0.5)
		   m2 = L*(1.0+S);
		else
		   m2 = L+S-L*S;

		m1 = 2.0*L-m2;

		r = HuetoRGB(m1,m2,H+1.0/3.0);
		g = HuetoRGB(m1,m2,H);
		b = HuetoRGB(m1,m2,H-1.0/3.0);
	}

	return RGB((BYTE)(r * 255), (BYTE)(g * 255), (BYTE)(b * 255));
}



////////////////////////////////////////////////////////////////////////////
//
//	fxHue()						Displays bitmap applying specified hue value, 
//								first stretching or compressing it to fit
//								the dimensions of the destination rectangle,
//								if necessary.
//								If the function succeeds, the return value
//								is 1 (true); 
//								if fails, the return value is 0 (false). 
//
//	HDC		DestDC				Handle to the destination device context;
//	int		DestX				Specifies the x-coordinate, 
//								in logical units, of the upper-left 
//								corner of the destination rectangle;
//	int		DestY				Specifies the y-coordinate, 
//								in logical units, of the upper-left 
//								corner of the destination rectangle;
//	int		DestWidth			Specifies the width, in logical units,
//								of the destination rectangle.
//	int		DestHeight			Specifies the height, in logical units,
//								of the destination rectangle.
//	HDC		SrcDC				Handle to the source device context;
//	int		SrcX				Specifies the x-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		SrcY				Specifies the y-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		SrcWidth			Specifies the width, in logical units,
//								of the source rectangle;
//	int		SrcHeight			Specifies the height, in logical units,
//								of the source rectangle.
//	double	Hue					Hue (value in range -1.00 to 1.00);
//	int		Flags				Reserved parameter.
//
////////////////////////////////////////////////////////////////////////////
int __stdcall fxHue(HDC DestDC, int DestX, int DestY, int DestWidth, int DestHeight, HDC SrcDC, int SrcX, int SrcY, int SrcWidth, int SrcHeight, double Hue, int Flags)
{
	if (Hue < -1)
		Hue = -1;
	if (Hue > 1)
		Hue = 1;

	Hue					= Hue * 100;

	double c16			= (100 / 6.);
	double c33			= (100 / 3.);
	double c67			= (200 / 3.);
	double c83			= (250 / 3.);

	int fxR				= 0;
	int fxG				= 0;
	int fxB				= 0;

	int Sign			= 1;

	if (Hue < 0)
		Sign = -1,
		Hue	= Hue * (-1);

	if ((Hue > 0) & (Hue <= c16))
		fxR = 255,
		fxG = (int)((Hue / c16) * 255),
		fxB = 0;

	if ((Hue > c16) & (Hue <= c33))
		fxR = (int)(((c33 - Hue) / c16) * 255),
		fxG = 255,
		fxB = 0;

	if ((Hue > c33) & (Hue <= 50))
		fxR = 0,
		fxG = 255,
		fxB = (int)(((Hue - c33) / c16) * 255);

	if ((Hue > 50) & (Hue <= c67))
		fxR = 0,
		fxG = (int)(((c67 - Hue) / c16) * 255),
		fxB = 255;

	if ((Hue > c67) & (Hue <= c83))
		fxR = (int)(((Hue - c67) / c16) * 255),
		fxG = 0,
		fxB = 255;

	if ((Hue > c83) & (Hue <= 100))
		fxR = 255,
		fxG = 0,
		fxB = (int)(((100 - Hue) / c16) * 255);
	
	fxR					= fxR * Sign;
	fxG					= fxG * Sign;
	fxB					= fxB * Sign;

	BITMAPINFOHEADER	BMI;
	BMI.biSize			= sizeof(BITMAPINFOHEADER);
	BMI.biWidth			= DestWidth;
	BMI.biHeight		= DestHeight;
	BMI.biPlanes		= 1;
	BMI.biBitCount		= 32;
	BMI.biCompression	= BI_RGB;
	BMI.biSizeImage		= 0;
	BMI.biXPelsPerMeter = 0;
	BMI.biYPelsPerMeter = 0;
	BMI.biClrUsed		= 0;
	BMI.biClrImportant	= 0;
	
	/*
	BYTE				*pDestBits;
	HBITMAP				hbmDest;
	hbmDest = ::CreateDIBSection(DestDC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pDestBits, NULL, 0l);
	*/

	BYTE				*pSrcBits;
	HBITMAP				hbmSrc;
	hbmSrc = ::CreateDIBSection(SrcDC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pSrcBits, NULL, 0l);

	HDC					dc = ::CreateCompatibleDC(NULL);
	HBITMAP				hbm = (HBITMAP) ::SelectObject(dc, hbmSrc);
	
	::SetStretchBltMode(dc, HALFTONE);

	if (!::StretchBlt(dc, 0, 0, DestWidth, DestHeight, SrcDC, SrcX, SrcY, SrcWidth, SrcHeight, SRCCOPY))
		return 0;

	/*
	::SelectObject(dc, hbmDest);

	if (!::StretchBlt(dc, 0, 0, DestWidth, DestHeight, DestDC, DestX, DestY, DestWidth, DestHeight, SRCCOPY))
		return 0;
	*/

	::SelectObject(dc, hbm);

	::DeleteDC(dc);

    for (int y = 0; y < DestHeight; y++)
    {
		LPBYTE pbSrcRGB = (LPBYTE)&((DWORD*)pSrcBits)[y * DestWidth];
		//LPBYTE pbDestRGB = (LPBYTE)&((DWORD*)pDestBits)[y * DestWidth];

		for (int x = 0; x < DestWidth; x++)
		{
			pbSrcRGB[0] = fxByte(pbSrcRGB[0] + fxB);
			pbSrcRGB[1] = fxByte(pbSrcRGB[1] + fxG);
			pbSrcRGB[2] = fxByte(pbSrcRGB[2] + fxR);

			pbSrcRGB += 4;
			//pbDestRGB += 4;
		}
	}

	dc = ::CreateCompatibleDC(NULL);
	
	hbm = (HBITMAP) ::SelectObject(dc, hbmSrc);
	if (!::BitBlt(DestDC, DestX, DestY, DestWidth, DestHeight, dc, 0, 0, SRCCOPY))
		return 0;

	::DeleteDC(dc);

	::DeleteObject(hbmSrc);	
	//::DeleteObject(hbmDest);	
	
	return 1;
}



////////////////////////////////////////////////////////////////////////////
//
//	fxInversion()				Function displays bitmap with specified
//								inversion value, first
//								stretching or compressing the bitmap to fit
//								the dimensions of the destination rectangle,
//								if necessary.
//								If the function succeeds, the return value
//								is 1 (true); 
//								if fails, the return value is 0 (false). 
//
//	HDC		DestDC				Handle to the destination device context;
//	int		DestX				Specifies the x-coordinate, 
//								in logical units, of the upper-left 
//								corner of the destination rectangle;
//	int		DestY				Specifies the y-coordinate, 
//								in logical units, of the upper-left 
//								corner of the destination rectangle;
//	int		DestWidth			Specifies the width, in logical units,
//								of the destination rectangle.
//	int		DestHeight			Specifies the height, in logical units,
//								of the destination rectangle.
//	HDC		SrcDC				Handle to the source device context;
//	int		SrcX				Specifies the x-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		SrcY				Specifies the y-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		SrcWidth			Specifies the width, in logical units,
//								of the source rectangle;
//	int		SrcHeight			Specifies the height, in logical units,
//								of the source rectangle;
//	int		Inversion			Inversion (value in range 0 to 255);
//	int		Flags				Reserved parameter.
//
////////////////////////////////////////////////////////////////////////////
int __stdcall fxInversion(HDC DestDC, int DestX, int DestY, int DestWidth, int DestHeight, HDC SrcDC, int SrcX, int SrcY, int SrcWidth, int SrcHeight, int Inversion, int Flags)
{
	if (Inversion < 0)
		Inversion = 0;
	if (Inversion > 255)
		Inversion = 255;

	BITMAPINFOHEADER	BMI;
	BMI.biSize			= sizeof(BITMAPINFOHEADER);
	BMI.biWidth			= DestWidth;
	BMI.biHeight		= DestHeight;
	BMI.biPlanes		= 1;
	BMI.biBitCount		= 32;
	BMI.biCompression	= BI_RGB;
	BMI.biSizeImage		= 0;
	BMI.biXPelsPerMeter = 0;
	BMI.biYPelsPerMeter = 0;
	BMI.biClrUsed		= 0;
	BMI.biClrImportant	= 0;
	
	/*
	BYTE				*pDestBits;
	HBITMAP				hbmDest;
	hbmDest = ::CreateDIBSection(DestDC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pDestBits, NULL, 0l);
	*/

	BYTE				*pSrcBits;
	HBITMAP				hbmSrc;
	hbmSrc = ::CreateDIBSection(SrcDC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pSrcBits, NULL, 0l);

	HDC					dc = ::CreateCompatibleDC(NULL);
	HBITMAP				hbm = (HBITMAP) ::SelectObject(dc, hbmSrc);
	
	::SetStretchBltMode(dc, HALFTONE);

	if (!::StretchBlt(dc, 0, 0, DestWidth, DestHeight, SrcDC, SrcX, SrcY, SrcWidth, SrcHeight, SRCCOPY))
		return 0;

	/*
	::SelectObject(dc, hbmDest);

	if (!::StretchBlt(dc, 0, 0, DestWidth, DestHeight, DestDC, DestX, DestY, DestWidth, DestHeight, SRCCOPY))
		return 0;
	*/

	::SelectObject(dc, hbm);

	::DeleteDC(dc);

    for (int y = 0; y < DestHeight; y++)
    {
		LPBYTE pbSrcRGB = (LPBYTE)&((DWORD*)pSrcBits)[y * DestWidth];
		//LPBYTE pbDestRGB = (LPBYTE)&((DWORD*)pDestBits)[y * DestWidth];

		for (int x = 0; x < DestWidth; x++)
		{
			pbSrcRGB[0] = fxByte(abs(Inversion - pbSrcRGB[0]));
			pbSrcRGB[1] = fxByte(abs(Inversion - pbSrcRGB[1]));
			pbSrcRGB[2] = fxByte(abs(Inversion - pbSrcRGB[2]));

			pbSrcRGB += 4;
			//pbDestRGB += 4;
		}
	}

	dc = ::CreateCompatibleDC(NULL);
	
	hbm = (HBITMAP) ::SelectObject(dc, hbmSrc);
	if (!::BitBlt(DestDC, DestX, DestY, DestWidth, DestHeight, dc, 0, 0, SRCCOPY))
		return 0;

	::DeleteDC(dc);

	::DeleteObject(hbmSrc);	
	//::DeleteObject(hbmDest);	
	
	return 1;
}



////////////////////////////////////////////////////////////////////////////
//
//	fxInvert()					Displays inverted bitmap, 
//								first stretching or compressing it to fit
//								the dimensions of the destination rectangle,
//								if necessary.
//								If the function succeeds, the return value
//								is 1 (true); 
//								if fails, the return value is 0 (false). 
//
//	HDC		DestDC				Handle to the destination device context;
//	int		DestX				Specifies the x-coordinate, 
//								in logical units, of the upper-left 
//								corner of the destination rectangle;
//	int		DestY				Specifies the y-coordinate, 
//								in logical units, of the upper-left 
//								corner of the destination rectangle;
//	int		DestWidth			Specifies the width, in logical units,
//								of the destination rectangle.
//	int		DestHeight			Specifies the height, in logical units,
//								of the destination rectangle.
//	HDC		SrcDC				Handle to the source device context;
//	int		SrcX				Specifies the x-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		SrcY				Specifies the y-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		SrcWidth			Specifies the width, in logical units,
//								of the source rectangle;
//	int		SrcHeight			Specifies the height, in logical units,
//								of the source rectangle;
//	int		Flags				Reserved parameter.
//
////////////////////////////////////////////////////////////////////////////
int __stdcall fxInvert(HDC DestDC, int DestX, int DestY, int DestWidth, int DestHeight, HDC SrcDC, int SrcX, int SrcY, int SrcWidth, int SrcHeight, int Flags)
{
	BITMAPINFOHEADER	BMI;
	BMI.biSize			= sizeof(BITMAPINFOHEADER);
	BMI.biWidth			= DestWidth;
	BMI.biHeight		= DestHeight;
	BMI.biPlanes		= 1;
	BMI.biBitCount		= 32;
	BMI.biCompression	= BI_RGB;
	BMI.biSizeImage		= 0;
	BMI.biXPelsPerMeter = 0;
	BMI.biYPelsPerMeter = 0;
	BMI.biClrUsed		= 0;
	BMI.biClrImportant	= 0;
	
	/*
	BYTE				*pDestBits;
	HBITMAP				hbmDest;
	hbmDest = ::CreateDIBSection(DestDC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pDestBits, NULL, 0l);
	*/

	BYTE				*pSrcBits;
	HBITMAP				hbmSrc;
	hbmSrc = ::CreateDIBSection(SrcDC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pSrcBits, NULL, 0l);

	HDC					dc = ::CreateCompatibleDC(NULL);
	HBITMAP				hbm = (HBITMAP) ::SelectObject(dc, hbmSrc);
	
	::SetStretchBltMode(dc, HALFTONE);

	if (!::StretchBlt(dc, 0, 0, DestWidth, DestHeight, SrcDC, SrcX, SrcY, SrcWidth, SrcHeight, SRCCOPY))
		return 0;

	/*
	::SelectObject(dc, hbmDest);

	if (!::StretchBlt(dc, 0, 0, DestWidth, DestHeight, DestDC, DestX, DestY, DestWidth, DestHeight, SRCCOPY))
		return 0;
	*/

	::SelectObject(dc, hbm);

	::DeleteDC(dc);

    for (int y = 0; y < DestHeight; y++)
    {
		LPBYTE pbSrcRGB = (LPBYTE)&((DWORD*)pSrcBits)[y * DestWidth];
		//LPBYTE pbDestRGB = (LPBYTE)&((DWORD*)pDestBits)[y * DestWidth];

		for (int x = 0; x < DestWidth; x++)
		{
			pbSrcRGB[0] = 255 - pbSrcRGB[0];
			pbSrcRGB[1] = 255 - pbSrcRGB[1];
			pbSrcRGB[2] = 255 - pbSrcRGB[2];

			pbSrcRGB += 4;
			//pbDestRGB += 4;
		}
	}

	dc = ::CreateCompatibleDC(NULL);
	
	hbm = (HBITMAP) ::SelectObject(dc, hbmSrc);
	if (!::BitBlt(DestDC, DestX, DestY, DestWidth, DestHeight, dc, 0, 0, SRCCOPY))
		return 0;

	::DeleteDC(dc);

	::DeleteObject(hbmSrc);	
	//::DeleteObject(hbmDest);	
	
	return 1;
}



////////////////////////////////////////////////////////////////////////////
//
//	fxLight()					Displays bitmaps with light effect.
//								If the function succeeds, the return value
//								is 1 (true); 
//								if fails, the return value is 0 (false). 
//
//	HDC		DC					Handle to the destination device context;
//	int		X					Specifies the x-coordinate, 
//								in logical units, of the light center;
//	int		Y					Specifies the y-coordinate, 
//								in logical units, of the light center;
//	int		Color				Specifies the RGB color of the light;
//	int		Extension			Specifies the light extension;
//	int		Intensity			Specifies light intensity;
//	int		Definition			Specifies light definiton (if Flags == 0);
//	int		Flags				Light rendering flags:
//									FX_LIGHT_DEFAULT = 0
//									FX_LIGHT_RGB = 1
//
////////////////////////////////////////////////////////////////////////////
int __stdcall fxLight(HDC DC, int X, int Y, int Color, int Extension, int Intensity, int Definition, int Flags)
{
	int ExtensionPOW2	= Extension * Extension;

	int DestX			= X - Extension;
	int DestY			= Y - Extension;
	int	DestWidth		= Extension * 2;
	int DestHeight		= Extension * 2;

	if (Extension < 0)
		Extension = 0;
		
	if (Intensity < 0)
		Intensity = 0;
	if (Intensity > 255)
		Intensity = 255;

	if (Definition < 0)
		Definition = 0;
	if (Definition > 255)
		Definition = 255;


	int fxR				= fxGetRed(Color);
	int fxG				= fxGetGreen(Color);
	int fxB				= fxGetBlue(Color);


	BITMAPINFOHEADER	BMI;
	BMI.biSize			= sizeof(BITMAPINFOHEADER);
	BMI.biWidth			= DestWidth;
	BMI.biHeight		= DestHeight;
	BMI.biPlanes		= 1;
	BMI.biBitCount		= 32;
	BMI.biCompression	= BI_RGB;
	BMI.biSizeImage		= 0;
	BMI.biXPelsPerMeter = 0;
	BMI.biYPelsPerMeter = 0;
	BMI.biClrUsed		= 0;
	BMI.biClrImportant	= 0;

	/*
	BYTE				*pDestBits;
	HBITMAP				hbmDest;
	hbmDest = ::CreateDIBSection(DestDC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pDestBits, NULL, 0l);
	*/

	BYTE				*pSrcBits;
	HBITMAP				hbmSrc;
	hbmSrc = ::CreateDIBSection(DC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pSrcBits, NULL, 0l);

	HDC					dc = ::CreateCompatibleDC(NULL);
	HBITMAP				hbm = (HBITMAP) ::SelectObject(dc, hbmSrc);
	
	if (!::BitBlt(dc, 0, 0, DestWidth, DestHeight, DC, DestX, DestY, SRCCOPY))
		return 0;

	/*
	::SelectObject(dc, hbmDest);

	if (!::StretchBlt(dc, 0, 0, DestWidth, DestHeight, DestDC, DestX, DestY, DestWidth, DestHeight, SRCCOPY))
		return 0;
	*/

	::SelectObject(dc, hbm);

	::DeleteDC(dc);


	POINT pt;
	RECT rect;

	rect.left = 0;
	rect.top = 0;
	rect.right = DestWidth;
	rect.bottom = DestHeight;


	double d;

	if (Flags == 1)
	{
		d = ((double)Definition) / 100;
		Definition = 0;
	}

    for (int y = -Extension; y < Extension; y++)
    {
		pt.y = y + Extension;

		LPBYTE pbSrcRGB = (LPBYTE)&((DWORD*)pSrcBits)[pt.y * DestWidth];

		for (int x = -Extension; x < Extension; x++)
		{
			pt.x = x + Extension;

			if ((x * x + y * y) < (ExtensionPOW2))
			{
				if (::PtInRect(&rect, pt))
				{
					int Light = (int)((1 - 1 * sqrt(x * x + y * y) / Extension) * Intensity);
					
					switch (Flags)
					{
						case 0:	
							pbSrcRGB[(pt.x * 4) + 0] = fxShadeColors(pbSrcRGB[(pt.x * 4) + 0], fxB, Light);
							pbSrcRGB[(pt.x * 4) + 1] = fxShadeColors(pbSrcRGB[(pt.x * 4) + 1], fxG, Light);
							pbSrcRGB[(pt.x * 4) + 2] = fxShadeColors(pbSrcRGB[(pt.x * 4) + 2], fxR, Light);
							break;
						case 1:	
							pbSrcRGB[(pt.x * 4) + 0] = fxByte(pbSrcRGB[(pt.x * 4) + 0] + (int)(Light * d) + Definition + fxShadeColors(Definition, fxB, Light));
							pbSrcRGB[(pt.x * 4) + 1] = fxByte(pbSrcRGB[(pt.x * 4) + 1] + (int)(Light * d) + Definition + fxShadeColors(Definition, fxG, Light));
							pbSrcRGB[(pt.x * 4) + 2] = fxByte(pbSrcRGB[(pt.x * 4) + 2] + (int)(Light * d) + Definition + fxShadeColors(Definition, fxR, Light));
							break;
						case 2:	
							pbSrcRGB[(pt.x * 4) + 0] = fxByte(fxShadeColors(pbSrcRGB[(pt.x * 4) + 0], fxB, Light) + fxShadeColors(Definition, fxB, Light));
							pbSrcRGB[(pt.x * 4) + 1] = fxByte(fxShadeColors(pbSrcRGB[(pt.x * 4) + 1], fxG, Light) + fxShadeColors(Definition, fxG, Light));
							pbSrcRGB[(pt.x * 4) + 2] = fxByte(fxShadeColors(pbSrcRGB[(pt.x * 4) + 2], fxR, Light) + fxShadeColors(Definition, fxR, Light));
							break;
						case 3:	
							pbSrcRGB[(pt.x * 4) + 0] = fxByte(pbSrcRGB[(pt.x * 4) + 0] + Light + Definition + fxShadeColors(Definition, fxB, Light));
							pbSrcRGB[(pt.x * 4) + 1] = fxByte(pbSrcRGB[(pt.x * 4) + 1] + Light + Definition + fxShadeColors(Definition, fxG, Light));
							pbSrcRGB[(pt.x * 4) + 2] = fxByte(pbSrcRGB[(pt.x * 4) + 2] + Light + Definition + fxShadeColors(Definition, fxR, Light));
							break;
						case 4:	
							pbSrcRGB[(pt.x * 4) + 0] = fxByte(pbSrcRGB[(pt.x * 4) + 0] + Light + Definition);
							pbSrcRGB[(pt.x * 4) + 1] = fxByte(pbSrcRGB[(pt.x * 4) + 1] + Light + Definition);
							pbSrcRGB[(pt.x * 4) + 2] = fxByte(pbSrcRGB[(pt.x * 4) + 2] + Light + Definition);
							break;
						default:
							pbSrcRGB[(pt.x * 4) + 0] = fxByte(pbSrcRGB[(pt.x * 4) + 0] + (int)(Light * d) + Definition + fxShadeColors(Definition, fxB, Light));
							pbSrcRGB[(pt.x * 4) + 1] = fxByte(pbSrcRGB[(pt.x * 4) + 1] + (int)(Light * d) + Definition + fxShadeColors(Definition, fxG, Light));
							pbSrcRGB[(pt.x * 4) + 2] = fxByte(pbSrcRGB[(pt.x * 4) + 2] + (int)(Light * d) + Definition + fxShadeColors(Definition, fxR, Light));
							break;
					}
				}
			}
		}
	}


	dc = ::CreateCompatibleDC(NULL);
	
	hbm = (HBITMAP) ::SelectObject(dc, hbmSrc);
	if (!::BitBlt(DC, DestX, DestY, DestWidth, DestHeight, dc, 0, 0, SRCCOPY))
		return 0;

	::DeleteDC(dc);

	::DeleteObject(hbmSrc);	
	//::DeleteObject(hbmDest);	
	
	return 1;
}



////////////////////////////////////////////////////////////////////////////
//
//	fxMirror()					Displays mirrored bitmap, first
//								stretching or compressing the bitmap to fit
//								the dimensions of the destination rectangle,
//								if necessary.
//								If the function succeeds, the return value
//								is 1 (true); 
//								if fails, the return value is 0 (false). 
//
//	HDC		DestDC				Handle to the destination device context;
//	int		DestX				Specifies the x-coordinate, 
//								in logical units, of the upper-left 
//								corner of the destination rectangle;
//	int		DestY				Specifies the y-coordinate, 
//								in logical units, of the upper-left 
//								corner of the destination rectangle;
//	int		DestWidth			Specifies the width, in logical units,
//								of the destination rectangle.
//	int		DestHeight			Specifies the height, in logical units,
//								of the destination rectangle.
//	HDC		SrcDC				Handle to the source device context;
//	int		SrcX				Specifies the x-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		SrcY				Specifies the y-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		SrcWidth			Specifies the width, in logical units,
//								of the source rectangle;
//	int		SrcHeight			Specifies the height, in logical units,
//								of the source rectangle.
//	bool	Horizontal			Specifies wether to render
//								horizontal mirroring.
//	bool	Vertical			Specifies wether to render
//								vertical mirroring.
//	int		Flags				Reserved parameter.
//
////////////////////////////////////////////////////////////////////////////
int __stdcall fxMirror(HDC DestDC, int DestX, int DestY, int DestWidth, int DestHeight, HDC SrcDC, int SrcX, int SrcY, int SrcWidth, int SrcHeight, bool Horizontal, bool Vertical, int Flags)
{
	BITMAPINFOHEADER	BMI;
	BMI.biSize			= sizeof(BITMAPINFOHEADER);
	BMI.biWidth			= DestWidth;
	BMI.biHeight		= DestHeight;
	BMI.biPlanes		= 1;
	BMI.biBitCount		= 32;
	BMI.biCompression	= BI_RGB;
	BMI.biSizeImage		= 0;
	BMI.biXPelsPerMeter = 0;
	BMI.biYPelsPerMeter = 0;
	BMI.biClrUsed		= 0;
	BMI.biClrImportant	= 0;
	
	BYTE				*pDstBits;
	HBITMAP				hbmDst;
	hbmDst = ::CreateDIBSection(SrcDC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pDstBits, NULL, 0l);

	BYTE				*pSrcBits;
	HBITMAP				hbmSrc;
	hbmSrc = ::CreateDIBSection(SrcDC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pSrcBits, NULL, 0l);

	HDC					dc = ::CreateCompatibleDC(NULL);
	HBITMAP				hbm = (HBITMAP) ::SelectObject(dc, hbmSrc);
	
	::SetStretchBltMode(dc, HALFTONE);

	if (!::StretchBlt(dc, 0, 0, DestWidth, DestHeight, SrcDC, SrcX, SrcY, SrcWidth, SrcHeight, SRCCOPY))
		return 0;

	::SelectObject(dc, hbmDst);

	if (!::StretchBlt(dc, 0, 0, DestWidth, DestHeight, DestDC, DestX, DestY, DestWidth, DestHeight, SRCCOPY))
		return 0;

	::SelectObject(dc, hbm);

	::DeleteDC(dc);

	int Bit;

	if (Vertical)
	{
		for (int y = 0; y < DestHeight; y++)
		{
			LPBYTE pbSrcRGB = (LPBYTE)&((DWORD*)pSrcBits)[y * DestWidth];
			LPBYTE pbDstRGB = (LPBYTE)&((DWORD*)pDstBits)[(DestHeight - y) * DestWidth];

			for (int x = 0; x < DestWidth; x++)
			{	

				if (Horizontal)
				{
					Bit = (DestWidth - x) * 4;
					pbDstRGB[Bit + 0] = pbSrcRGB[0];
					pbDstRGB[Bit + 1] = pbSrcRGB[1];
					pbDstRGB[Bit + 2] = pbSrcRGB[2];
					pbSrcRGB += 4;
				}
				else
				{
					pbDstRGB[0] = pbSrcRGB[0];
					pbDstRGB[1] = pbSrcRGB[1];
					pbDstRGB[2] = pbSrcRGB[2];
					pbSrcRGB += 4;
					pbDstRGB += 4;
				}
			}
		}
	}
	else
	{
		for (int y = 0; y < DestHeight; y++)
		{
			LPBYTE pbSrcRGB = (LPBYTE)&((DWORD*)pSrcBits)[y * DestWidth];
			LPBYTE pbDstRGB = (LPBYTE)&((DWORD*)pDstBits)[y * DestWidth];

			for (int x = 0; x < DestWidth; x++)
			{	

				if (Horizontal)
				{
					Bit = (DestWidth - x) * 4;
					pbDstRGB[Bit + 0] = pbSrcRGB[0];
					pbDstRGB[Bit + 1] = pbSrcRGB[1];
					pbDstRGB[Bit + 2] = pbSrcRGB[2];
					pbSrcRGB += 4;
				}
				else
				{
					pbDstRGB[0] = pbSrcRGB[0];
					pbDstRGB[1] = pbSrcRGB[1];
					pbDstRGB[2] = pbSrcRGB[2];
					pbSrcRGB += 4;
					pbDstRGB += 4;
				}
			}
		}
	}

	dc = ::CreateCompatibleDC(NULL);
	
	hbm = (HBITMAP) ::SelectObject(dc, hbmDst);
	if (!::BitBlt(DestDC, DestX, DestY, DestWidth, DestHeight, dc, 0, 0, SRCCOPY))
		return 0;

	::DeleteDC(dc);

	::DeleteObject(hbmSrc);	
	::DeleteObject(hbmDst);	
	
	return 1;
}



////////////////////////////////////////////////////////////////////////////
//
//	fxMosaic()					Draws bitmap applying mosaic effect, 
//								first stretching or compressing it
//								to fit the dimensions of the destination
//								rectangle, if necessary.
//								If the function succeeds, the return value
//								is 1 (true); 
//								if fails, the return value is 0 (false). 
//
//	HDC		DestDC				Handle to the destination device context;
//	int		DestX				Specifies the x-coordinate, 
//								in logical units, of the upper-left 
//								corner of the destination rectangle;
//	int		DestY				Specifies the y-coordinate, 
//								in logical units, of the upper-left 
//								corner of the destination rectangle;
//	int		DestWidth			Specifies the width, in logical units,
//								of the destination rectangle.
//	int		DestHeight			Specifies the height, in logical units,
//								of the destination rectangle.
//	HDC		SrcDC				Handle to the source device context;
//	int		SrcX				Specifies the x-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		SrcY				Specifies the y-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		SrcWidth			Specifies the width, in logical units,
//								of the source rectangle;
//	int		SrcHeight			Specifies the height, in logical units,
//								of the source rectangle;
//	int		Size				Size, in logical units, of each of the
//								mosaic squares;
//	int		Flags				Reserved parameter.
//
////////////////////////////////////////////////////////////////////////////
int __stdcall fxMosaic(HDC DestDC, int DestX, int DestY, int DestWidth, int DestHeight, HDC SrcDC, int SrcX, int SrcY, int SrcWidth, int SrcHeight, int Size, int Flags)
{
	if (Size < 1)
		Size = 1;

	BITMAPINFOHEADER	BMI;
	BMI.biSize			= sizeof(BITMAPINFOHEADER);
	BMI.biWidth			= DestWidth;
	BMI.biHeight		= DestHeight;
	BMI.biPlanes		= 1;
	BMI.biBitCount		= 32;
	BMI.biCompression	= BI_RGB;
	BMI.biSizeImage		= 0;
	BMI.biXPelsPerMeter = 0;
	BMI.biYPelsPerMeter = 0;
	BMI.biClrUsed		= 0;
	BMI.biClrImportant	= 0;
	
	/*
	BYTE				*pDestBits;
	HBITMAP				hbmDest;
	hbmDest = ::CreateDIBSection(DestDC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pDestBits, NULL, 0l);
	*/

	BYTE				*pSrcBits;
	HBITMAP				hbmSrc;
	hbmSrc = ::CreateDIBSection(SrcDC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pSrcBits, NULL, 0l);

	HDC					dc = ::CreateCompatibleDC(NULL);
	HBITMAP				hbm = (HBITMAP) ::SelectObject(dc, hbmSrc);
	
	::SetStretchBltMode(dc, HALFTONE);

	if (!::StretchBlt(dc, 0, 0, DestWidth, DestHeight, SrcDC, SrcX, SrcY, SrcWidth, SrcHeight, SRCCOPY))
		return 0;

	/*
	::SelectObject(dc, hbmDest);

	if (!::StretchBlt(dc, 0, 0, DestWidth, DestHeight, DestDC, DestX, DestY, DestWidth, DestHeight, SRCCOPY))
		return 0;
	*/

	::SelectObject(dc, hbm);

	::DeleteDC(dc);

    for (int y = 0; y < DestHeight; y++)
    {
		LPBYTE pbDestRGB = (LPBYTE)&((DWORD*)pSrcBits)[y * SrcWidth];
		LPBYTE pbSrcRGB = (LPBYTE)&((DWORD*)pSrcBits)[(y - (y % Size)) * SrcWidth];

		for (int x = 0; x < DestWidth; x++)
		{
			pbDestRGB[(4 * x) + 0] = pbSrcRGB[(4 * (x - (x % Size))) + 0];
			pbDestRGB[(4 * x) + 1] = pbSrcRGB[(4 * (x - (x % Size))) + 1];
			pbDestRGB[(4 * x) + 2] = pbSrcRGB[(4 * (x - (x % Size))) + 2];
		}
	}

	dc = ::CreateCompatibleDC(NULL);
	
	hbm = (HBITMAP) ::SelectObject(dc, hbmSrc);
	if (!::BitBlt(DestDC, DestX, DestY, DestWidth, DestHeight, dc, 0, 0, SRCCOPY))
		return 0;

	::DeleteDC(dc);

	::DeleteObject(hbmSrc);	
	//::DeleteObject(hbmDest);	
	
	return 1;
}



////////////////////////////////////////////////////////////////////////////
//
//	fxPaletteIndex()			Function returns the palette color 
//								from the given index value.
//
//	int		Index				Palette index value (long value);
//
////////////////////////////////////////////////////////////////////////////
int __stdcall fxPaletteIndex(int Index)
{
	return PALETTEINDEX(Index);
}



////////////////////////////////////////////////////////////////////////////
//
//	fxPaletteRGB()				Function returns the palette RGB color 
//								from given R, G, B values.
//
//	int		R					Red value (value in range 0 to 255);
//	int		G					Green value (value in range 0 to 255);
//	int		B					Blue value (value in range 0 to 255).
//
////////////////////////////////////////////////////////////////////////////
int __stdcall fxPaletteRGB(int R, int G, int B)
{
	return PALETTERGB(R, G, B);
}



////////////////////////////////////////////////////////////////////////////
//
//	fxReduceColors()			Displays color reduced bitmap, 
//								first stretching or compressing it to fit
//								the dimensions of the destination rectangle,
//								if necessary.
//								If the function succeeds, the return value
//								is 1 (true); 
//								if fails, the return value is 0 (false). 
//
//	HDC		DestDC				Handle to the destination device context;
//	int		DestX				Specifies the x-coordinate, 
//								in logical units, of the upper-left 
//								corner of the destination rectangle;
//	int		DestY				Specifies the y-coordinate, 
//								in logical units, of the upper-left 
//								corner of the destination rectangle;
//	int		DestWidth			Specifies the width, in logical units,
//								of the destination rectangle.
//	int		DestHeight			Specifies the height, in logical units,
//								of the destination rectangle.
//	HDC		SrcDC				Handle to the source device context;
//	int		SrcX				Specifies the x-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		SrcY				Specifies the y-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		SrcWidth			Specifies the width, in logical units,
//								of the source rectangle;
//	int		SrcHeight			Specifies the height, in logical units,
//								of the source rectangle.
//	int		Level				Reduce level of colors (value in range 
//								0 to 255);
//								Value 0 represents full colors;
//	int		Flags				Reserved parameter.
//
////////////////////////////////////////////////////////////////////////////
int __stdcall fxReduceColors(HDC DestDC, int DestX, int DestY, int DestWidth, int DestHeight, HDC SrcDC, int SrcX, int SrcY, int SrcWidth, int SrcHeight, int Level, int Flags)
{
	if (Level < 0)
		Level = 0;

	Level++;

	BITMAPINFOHEADER	BMI;
	BMI.biSize			= sizeof(BITMAPINFOHEADER);
	BMI.biWidth			= DestWidth;
	BMI.biHeight		= DestHeight;
	BMI.biPlanes		= 1;
	BMI.biBitCount		= 32;
	BMI.biCompression	= BI_RGB;
	BMI.biSizeImage		= 0;
	BMI.biXPelsPerMeter = 0;
	BMI.biYPelsPerMeter = 0;
	BMI.biClrUsed		= 0;
	BMI.biClrImportant	= 0;

	/*
	BYTE				*pDestBits;
	HBITMAP				hbmDest;
	hbmDest = ::CreateDIBSection(DestDC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pDestBits, NULL, 0l);
	*/

	BYTE				*pSrcBits;
	HBITMAP				hbmSrc;
	hbmSrc = ::CreateDIBSection(SrcDC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pSrcBits, NULL, 0l);

	HDC					dc = ::CreateCompatibleDC(NULL);
	HBITMAP				hbm = (HBITMAP) ::SelectObject(dc, hbmSrc);
	
	::SetStretchBltMode(dc, HALFTONE);

	if (!::StretchBlt(dc, 0, 0, DestWidth, DestHeight, SrcDC, SrcX, SrcY, SrcWidth, SrcHeight, SRCCOPY))
		return 0;

	/*
	::SelectObject(dc, hbmDest);

	if (!::StretchBlt(dc, 0, 0, DestWidth, DestHeight, DestDC, DestX, DestY, DestWidth, DestHeight, SRCCOPY))
		return 0;
	*/

	::SelectObject(dc, hbm);

	::DeleteDC(dc);

    for (int y = 0; y < DestHeight; y++)
    {
		LPBYTE pbSrcRGB = (LPBYTE)&((DWORD*)pSrcBits)[y * DestWidth];
		//LPBYTE pbDestRGB = (LPBYTE)&((DWORD*)pDestBits)[y * DestWidth];

		for (int x = 0; x < DestWidth; x++)
		{
			pbSrcRGB[0] = pbSrcRGB[0] - (pbSrcRGB[0] % Level);
			pbSrcRGB[1] = pbSrcRGB[1] - (pbSrcRGB[1] % Level);
			pbSrcRGB[2] = pbSrcRGB[2] - (pbSrcRGB[2] % Level);

			pbSrcRGB += 4;
			//pbDestRGB += 4;
		}
	}

	dc = ::CreateCompatibleDC(NULL);
	
	hbm = (HBITMAP) ::SelectObject(dc, hbmSrc);
	if (!::BitBlt(DestDC, DestX, DestY, DestWidth, DestHeight, dc, 0, 0, SRCCOPY))
		return 0;

	::DeleteDC(dc);

	::DeleteObject(hbmSrc);	
	//::DeleteObject(hbmDest);	
	
	return 1;
}



////////////////////////////////////////////////////////////////////////////
//
//	fxRelief()					Displays bitmap with relief effect, 
//								first stretching or compressing it to fit
//								the dimensions of the destination rectangle,
//								if necessary.
//								If the function succeeds, the return value
//								is 1 (true); 
//								if fails, the return value is 0 (false). 
//
//	HDC		DestDC				Handle to the destination device context;
//	int		DestX				Specifies the x-coordinate, 
//								in logical units, of the upper-left 
//								corner of the destination rectangle;
//	int		DestY				Specifies the y-coordinate, 
//								in logical units, of the upper-left 
//								corner of the destination rectangle;
//	int		DestWidth			Specifies the width, in logical units,
//								of the destination rectangle.
//	int		DestHeight			Specifies the height, in logical units,
//								of the destination rectangle.
//	HDC		SrcDC				Handle to the source device context;
//	int		SrcX				Specifies the x-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		SrcY				Specifies the y-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		SrcWidth			Specifies the width, in logical units,
//								of the source rectangle;
//	int		SrcHeight			Specifies the height, in logical units,
//								of the source rectangle;
//	int		Flags				Reserved parameter.
//
////////////////////////////////////////////////////////////////////////////
int __stdcall fxRelief(HDC DestDC, int DestX, int DestY, int DestWidth, int DestHeight, HDC SrcDC, int SrcX, int SrcY, int SrcWidth, int SrcHeight, int Flags)
{
	DestX				= DestX;
	DestY				= DestY - 2;

	SrcHeight			= SrcHeight + 2;
	SrcWidth			= SrcWidth + 2;
	DestHeight			= DestHeight + 2;
	DestWidth			= DestWidth + 2;

	BITMAPINFOHEADER	BMI;
	BMI.biSize			= sizeof(BITMAPINFOHEADER);
	BMI.biWidth			= DestWidth;
	BMI.biHeight		= DestHeight;
	BMI.biPlanes		= 1;
	BMI.biBitCount		= 32;
	BMI.biCompression	= BI_RGB;
	BMI.biSizeImage		= 0;
	BMI.biXPelsPerMeter = 0;
	BMI.biYPelsPerMeter = 0;
	BMI.biClrUsed		= 0;
	BMI.biClrImportant	= 0;

	/*
	BYTE				*pDestBits;
	HBITMAP				hbmDest;
	hbmDest = ::CreateDIBSection(DestDC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pDestBits, NULL, 0l);
	*/

	BYTE				*pSrcBits;
	HBITMAP				hbmSrc;
	hbmSrc = ::CreateDIBSection(SrcDC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pSrcBits, NULL, 0l);

	HDC					dc = ::CreateCompatibleDC(NULL);
	HBITMAP				hbm = (HBITMAP) ::SelectObject(dc, hbmSrc);
	
	::SetStretchBltMode(dc, HALFTONE);

	if (!::StretchBlt(dc, 0, 0, DestWidth, DestHeight, SrcDC, SrcX, SrcY, SrcWidth, SrcHeight, SRCCOPY))
		return 0;

	/*
	::SelectObject(dc, hbmDest);

	if (!::StretchBlt(dc, 0, 0, DestWidth, DestHeight, DestDC, DestX, DestY, DestWidth, DestHeight, SRCCOPY))
		return 0;
	*/

	::SelectObject(dc, hbm);

	::DeleteDC(dc);

    for (int y = 0; y < DestHeight - 2; y++)
    {
		int scanline = 4 * y * DestWidth;

		for (int x = 0; x < DestWidth - 2; x++)
		{
			int Address = scanline + (4 * x);
			int Address2 = (4 * (y + 2) * DestWidth) + (4 * (x + 2));

			int col1 = fxGetMemory(pSrcBits + Address, 4);			
			int col2 = fxGetMemory(pSrcBits + Address2, 4);

			col1 = col1 - col2 + 0x808080;

			if (col1 < 0) 
				col1 = 0;

			if (col1 > 0xFFFFFF)
				col1 = 0xFFFFFF;

			memcpy(pSrcBits + Address, &col1, 4);


			/*
				LPBYTE pbDestRGB = (LPBYTE)&((DWORD*)pDestBits)[y * SrcWidth];
				LPBYTE pbSrcRGB = (LPBYTE)&((DWORD*)pSrcBits)[pt.y * SrcWidth];

				if (RGB(pbSrcRGB[(4 * pt.x)], pbSrcRGB[(4 * pt.x)+1], pbSrcRGB[(4 * pt.x)+2])!=0)
					pbDestRGB[(4 * x)] = pbSrcRGB[(4 * pt.x)],
					pbDestRGB[(4 * x)+1] = pbSrcRGB[(4 * pt.x)+1],
					pbDestRGB[(4 * x)+2] = pbSrcRGB[(4 * pt.x)+2];
					*/
		}
	}

	dc = ::CreateCompatibleDC(NULL);
	
	hbm = (HBITMAP) ::SelectObject(dc, hbmSrc);
	if (!::BitBlt(DestDC, DestX, DestY, DestWidth, DestHeight, dc, 0, 0, SRCCOPY))
		return 0;

	::DeleteDC(dc);

	::DeleteObject(hbmSrc);	
	//::DeleteObject(hbmDest);	
	
	return 1;
}



////////////////////////////////////////////////////////////////////////////
//
//	fxRender()					Renders bitmap first roatating, zooming or
//								alphablending it.
//								If the function succeeds, the return value
//								is 1 (true); 
//								if fails, the return value is 0 (false). 
//
//	HDC		DestDC				Handle to the destination device context;
//	int		DestX				Specifies the x-coordinate, 
//								in logical units, of the upper-left 
//								corner of the destination rectangle;
//	int		DestY				Specifies the y-coordinate, 
//								in logical units, of the upper-left 
//								corner of the destination rectangle;
//	HDC		SrcDC				Handle to the source device context;
//	int		SrcX				Specifies the x-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		SrcY				Specifies the y-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		SrcWidth			Specifies the width, in logical units,
//								of the source and destination rectangles;
//	int		SrcHeight			Specifies the height, in logical units,
//								of the source and destination rectangles;
//	int		Angle				Rotation angle (value in range -360 to 360);
//	int		TransparentColor	The RGB color in the source device context
//								to treat as transparent;
//								if (TransparentColor = -1) the transparency
//								will not be used;
//	int		Flags				Reserved parameter.
//
////////////////////////////////////////////////////////////////////////////
int __stdcall fxRender(HDC DestDC, int CenterX, int CenterY, HDC SrcDC, int SrcX, int SrcY, int SrcWidth, int SrcHeight, int Blending, double Angle, double Zoom, int TransparentColor, int Flags)
{
	if (Blending < 0)
		Blending = 0;
	if (Blending > 255)
		Blending = 255;

	if (Angle < -360)
		Angle = -360;
	if (Angle > 360)
		Angle = 360;

	if (Zoom < 0)
		Zoom = 0;
	if (Zoom > 16)
		Zoom = 16;


	double PI			= 3.14;
	double Radians		= PI / 180; //1.74532925199433E-02;

	double Theta		= 0;

	switch (int(Angle))
	{
		case -360: 
			Theta = 0;
			break;
		case 0:
			Theta = 0;
			break;
		case 360:
			Theta = 0;
			break;
		default:
			Theta = Angle * Radians;
			break;
	}

	
	if (SrcWidth > SrcHeight)
		SrcHeight = SrcWidth;
	else
		SrcWidth = SrcHeight;

	int DestWidth		= (int)(SrcWidth * Zoom);
	int DestHeight		= (int)(SrcHeight * Zoom);

	int	Center			= (int)(float(DestWidth) / 2);

	int DestX			= CenterX - Center;
	int DestY			= CenterY - Center;


	BITMAPINFOHEADER	BMI;
	BMI.biSize			= sizeof(BITMAPINFOHEADER);
	BMI.biWidth			= DestWidth;
	BMI.biHeight		= DestHeight;
	BMI.biPlanes		= 1;
	BMI.biBitCount		= 32;
	BMI.biCompression	= BI_RGB;
	BMI.biSizeImage		= 0;
	BMI.biXPelsPerMeter = 0;
	BMI.biYPelsPerMeter = 0;
	BMI.biClrUsed		= 0;
	BMI.biClrImportant	= 0;
	
	BYTE				*pDestBits;
	HBITMAP				hbmDest;
	hbmDest = ::CreateDIBSection(DestDC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pDestBits, NULL, 0l);

	BYTE				*pSrcBits;
	HBITMAP				hbmSrc;
	hbmSrc = ::CreateDIBSection(SrcDC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pSrcBits, NULL, 0l);

	HDC					dc = ::CreateCompatibleDC(NULL);
	HBITMAP				hbm = (HBITMAP) ::SelectObject(dc, hbmSrc);
	
	::SetStretchBltMode(dc, HALFTONE);

	if (!::StretchBlt(dc, 0, 0, DestWidth, DestHeight, SrcDC, SrcX, SrcY, SrcWidth, SrcHeight, SRCCOPY))
		return 0;

	::SelectObject(dc, hbmDest);

	if (!::StretchBlt(dc, 0, 0, DestWidth, DestHeight, DestDC, DestX, DestY, DestWidth, DestHeight, SRCCOPY))
		return 0;

	::SelectObject(dc, hbm);

	::DeleteDC(dc);


	double SinTheta		= sin(Theta);
	double CosTheta		= cos(Theta);
	
	POINT pt;
	RECT rect;

	rect.left = 0;
	rect.top = 0;
	rect.right = DestWidth;
	rect.bottom = DestHeight;

	for (int y = 0; y < DestHeight; y++)
    {
		for (int x = 0; x < DestWidth; x++)
		{
			//pt.x = int(Center + ((x - CenterX) * CosTheta - (y - CenterY) * SinTheta) + .5);
			//pt.y = int(Center + ((x - CenterX) * SinTheta + (y - CenterY) * CosTheta) + .5);			

			pt.x = int(Center + ((x - Center) * CosTheta - (y - Center) * SinTheta) + .5);
			pt.y = int(Center + ((x - Center) * SinTheta + (y - Center) * CosTheta) + .5);			
			

			if (::PtInRect(&rect, pt))
			{
				LPBYTE pbDestRGB = (LPBYTE)&((DWORD*)pDestBits)[y * DestWidth];
				LPBYTE pbSrcRGB = (LPBYTE)&((DWORD*)pSrcBits)[pt.y * DestWidth];

				if (Blending == 255)
				{
					if (!(TransparentColor == fxRGB(pbSrcRGB[(4 * pt.x)], pbSrcRGB[(4 * pt.x)+1], pbSrcRGB[(4 * pt.x)+2])))
						pbDestRGB[(4 * x)] = pbSrcRGB[(4 * pt.x)],
						pbDestRGB[(4 * x) + 1] = pbSrcRGB[(4 * pt.x) + 1],
						pbDestRGB[(4 * x) + 2] = pbSrcRGB[(4 * pt.x) + 2];
				}
				else
				{
					if (!(TransparentColor == fxRGB(pbSrcRGB[(4 * pt.x)], pbSrcRGB[(4 * pt.x)+1], pbSrcRGB[(4 * pt.x)+2])))
						pbDestRGB[(4 * x)] = fxShadeColors(pbDestRGB[(4 * x)], pbSrcRGB[(4 * pt.x)], Blending),
						pbDestRGB[(4 * x) + 1] = fxShadeColors(pbDestRGB[(4 * x) + 1], pbSrcRGB[(4 * pt.x) + 1], Blending),
						pbDestRGB[(4 * x) + 2] = fxShadeColors(pbDestRGB[(4 * x) + 2], pbSrcRGB[(4 * pt.x) + 2], Blending);
				}
			}
		}
	}


	dc = ::CreateCompatibleDC(NULL);
	
	hbm = (HBITMAP) ::SelectObject(dc, hbmDest);

	if (!::BitBlt(DestDC, DestX, DestY, DestHeight, DestHeight, dc, 0, 0, SRCCOPY))
		return 0;

	::DeleteDC(dc);

	::DeleteObject(hbmDest);	
	::DeleteObject(hbmSrc);	
	
	return 1;
}



////////////////////////////////////////////////////////////////////////////
//
//	fxReplaceColor()			Function replaces given color in 
//								destination device context with specified
//								replace color, first stretching or 
//								compressing bitmap to fit the dimensions
//								of the destination rectangle, if necessary.
//								If the function succeeds, the return value
//								is 1 (true); 
//								if fails, the return value is 0 (false). 
//
//	HDC		DestDC				Handle to the destination device context;
//	int		DestX				Specifies the x-coordinate, 
//								in logical units, of the upper-left 
//								corner of the destination rectangle;
//	int		DestY				Specifies the y-coordinate, 
//								in logical units, of the upper-left 
//								corner of the destination rectangle;
//	int		DestWidth			Specifies the width, in logical units,
//								of the destination rectangle.
//	int		DestHeight			Specifies the height, in logical units,
//								of the destination rectangle.
//	HDC		SrcDC				Handle to the source device context;
//	int		SrcX				Specifies the x-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		SrcY				Specifies the y-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		SrcWidth			Specifies the width, in logical units,
//								of the source rectangle;
//	int		SrcHeight			Specifies the height, in logical units,
//								of the source rectangle;
//	int		Color				Color to be replaced (RGB);
//	int		ByColor				Color to replace with (RGB);
//	int		Flags				Reserved parameter.
//
////////////////////////////////////////////////////////////////////////////
int __stdcall fxReplaceColor(HDC DestDC, int DestX, int DestY, int DestWidth, int DestHeight, HDC SrcDC, int SrcX, int SrcY, int SrcWidth, int SrcHeight, int Color, int ByColor, int Flags)
{
	int fxR				= fxGetRed(Color);
	int fxG				= fxGetGreen(Color);
	int fxB				= fxGetBlue(Color);

	int fxR_r			= fxGetRed(ByColor);
	int fxG_r			= fxGetGreen(ByColor);
	int fxB_r			= fxGetBlue(ByColor);

	BITMAPINFOHEADER	BMI;
	BMI.biSize			= sizeof(BITMAPINFOHEADER);
	BMI.biWidth			= DestWidth;
	BMI.biHeight		= DestHeight;
	BMI.biPlanes		= 1;
	BMI.biBitCount		= 32;
	BMI.biCompression	= BI_RGB;
	BMI.biSizeImage		= 0;
	BMI.biXPelsPerMeter = 0;
	BMI.biYPelsPerMeter = 0;
	BMI.biClrUsed		= 0;
	BMI.biClrImportant	= 0;
	
	/*
	BYTE				*pDestBits;
	HBITMAP				hbmDest;
	hbmDest = ::CreateDIBSection(DestDC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pDestBits, NULL, 0l);
	*/

	BYTE				*pSrcBits;
	HBITMAP				hbmSrc;
	hbmSrc = ::CreateDIBSection(SrcDC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pSrcBits, NULL, 0l);

	HDC					dc = ::CreateCompatibleDC(NULL);
	HBITMAP				hbm = (HBITMAP) ::SelectObject(dc, hbmSrc);
	
	::SetStretchBltMode(dc, HALFTONE);

	if (!::StretchBlt(dc, 0, 0, DestWidth, DestHeight, SrcDC, SrcX, SrcY, SrcWidth, SrcHeight, SRCCOPY))
		return 0;

	/*
	::SelectObject(dc, hbmDest);

	if (!::StretchBlt(dc, 0, 0, DestWidth, DestHeight, DestDC, DestX, DestY, DestWidth, DestHeight, SRCCOPY))
		return 0;
	*/

	::SelectObject(dc, hbm);

	::DeleteDC(dc);

    for (int y = 0; y < DestHeight; y++)
    {
		LPBYTE pbSrcRGB = (LPBYTE)&((DWORD*)pSrcBits)[y * DestWidth];
		//LPBYTE pbDestRGB = (LPBYTE)&((DWORD*)pDestBits)[y * DestWidth];

		for (int x = 0; x < DestWidth; x++)
		{
			if ((fxR == pbSrcRGB[2]) & (fxG == pbSrcRGB[1]) & (fxB == pbSrcRGB[0]))
				pbSrcRGB[0] = fxB_r,
				pbSrcRGB[1] = fxG_r,
				pbSrcRGB[2] = fxR_r;

			pbSrcRGB += 4;
			//pbDestRGB += 4;
		}
	}

	dc = ::CreateCompatibleDC(NULL);
	
	hbm = (HBITMAP) ::SelectObject(dc, hbmSrc);
	if (!::BitBlt(DestDC, DestX, DestY, DestWidth, DestHeight, dc, 0, 0, SRCCOPY))
		return 0;

	::DeleteDC(dc);

	::DeleteObject(hbmSrc);	
	//::DeleteObject(hbmDest);	
	
	return 1;
}



////////////////////////////////////////////////////////////////////////////
//
//	fxReplaceColors()			Function replaces given color in 
//								destination device context with specified
//								replace color, 
//								first stretching or compressing bitmap to 
//								fit the dimensions of the destination 
//								rectangle, if necessary.
//								If the function succeeds, the return value
//								is 1 (true); 
//								if fails, the return value is 0 (false). 
//
//	HDC		DestDC				Handle to the destination device context;
//	int		DestX				Specifies the x-coordinate, 
//								in logical units, of the upper-left 
//								corner of the destination rectangle;
//	int		DestY				Specifies the y-coordinate, 
//								in logical units, of the upper-left 
//								corner of the destination rectangle;
//	int		DestWidth			Specifies the width, in logical units,
//								of the destination rectangle.
//	int		DestHeight			Specifies the height, in logical units,
//								of the destination rectangle.
//	HDC		SrcDC				Handle to the source device context;
//	int		SrcX				Specifies the x-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		SrcY				Specifies the y-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		SrcWidth			Specifies the width, in logical units,
//								of the source rectangle;
//	int		SrcHeight			Specifies the height, in logical units,
//								of the source rectangle;
//	int		Color				Color to be replaced (RGB);
//	int		NewColor			Color to replace with (RGB); 
//	int		Similarity			Similarity within the colors will be
//								replaced (value in range 0(1) to 255);
//	int		Flags				Reserved parameter.
//
////////////////////////////////////////////////////////////////////////////
int __stdcall fxReplaceColors(HDC DestDC, int DestX, int DestY, int DestWidth, int DestHeight, HDC SrcDC, int SrcX, int SrcY, int SrcWidth, int SrcHeight, int Color, int ByColor, int Similarity, int Flags)
{
	if (Similarity < 0)
		Similarity = 0;
	if (Similarity > 255)
		Similarity = 255;

	int fxR				= fxGetRed(Color);
	int fxG				= fxGetGreen(Color);
	int fxB				= fxGetBlue(Color);

	int fxR_r			= fxGetRed(ByColor);
	int fxG_r			= fxGetGreen(ByColor);
	int fxB_r			= fxGetBlue(ByColor);

	BITMAPINFOHEADER	BMI;
	BMI.biSize			= sizeof(BITMAPINFOHEADER);
	BMI.biWidth			= DestWidth;
	BMI.biHeight		= DestHeight;
	BMI.biPlanes		= 1;
	BMI.biBitCount		= 32;
	BMI.biCompression	= BI_RGB;
	BMI.biSizeImage		= 0;
	BMI.biXPelsPerMeter = 0;
	BMI.biYPelsPerMeter = 0;
	BMI.biClrUsed		= 0;
	BMI.biClrImportant	= 0;
	
	/*
	BYTE				*pDestBits;
	HBITMAP				hbmDest;
	hbmDest = ::CreateDIBSection(DestDC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pDestBits, NULL, 0l);
	*/

	BYTE				*pSrcBits;
	HBITMAP				hbmSrc;
	hbmSrc = ::CreateDIBSection(SrcDC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pSrcBits, NULL, 0l);

	HDC					dc = ::CreateCompatibleDC(NULL);
	HBITMAP				hbm = (HBITMAP) ::SelectObject(dc, hbmSrc);
	
	::SetStretchBltMode(dc, HALFTONE);

	if (!::StretchBlt(dc, 0, 0, DestWidth, DestHeight, SrcDC, SrcX, SrcY, SrcWidth, SrcHeight, SRCCOPY))
		return 0;

	/*
	::SelectObject(dc, hbmDest);

	if (!::StretchBlt(dc, 0, 0, DestWidth, DestHeight, DestDC, DestX, DestY, DestWidth, DestHeight, SRCCOPY))
		return 0;
	*/

	::SelectObject(dc, hbm);

	::DeleteDC(dc);

    for (int y = 0; y < DestHeight; y++)
    {
		LPBYTE pbSrcRGB = (LPBYTE)&((DWORD*)pSrcBits)[y * DestWidth];
		//LPBYTE pbDestRGB = (LPBYTE)&((DWORD*)pDestBits)[y * DestWidth];

		for (int x = 0; x < DestWidth; x++)
		{
			if ((abs(fxR - pbSrcRGB[2]) < Similarity) & (abs(fxG - pbSrcRGB[1]) < Similarity) & (abs(fxB - pbSrcRGB[0]) < Similarity))
				pbSrcRGB[0] = fxB_r,
				pbSrcRGB[1] = fxG_r,
				pbSrcRGB[2] = fxR_r;

			pbSrcRGB += 4;
			//pbDestRGB += 4;
		}
	}

	dc = ::CreateCompatibleDC(NULL);
	
	hbm = (HBITMAP) ::SelectObject(dc, hbmSrc);
	if (!::BitBlt(DestDC, DestX, DestY, DestWidth, DestHeight, dc, 0, 0, SRCCOPY))
		return 0;

	::DeleteDC(dc);

	::DeleteObject(hbmSrc);	
	//::DeleteObject(hbmDest);	
	
	return 1;
}



////////////////////////////////////////////////////////////////////////////
//
//	fxResample()				Function resamples bitmap by specified 
//								horizontal and vertical zoom factors.
//								If the function succeeds, the return value
//								is 1 (true); 
//								if fails, the return value is 0 (false). 
//
//	HDC		DestDC				Handle to the destination device context;
//	int		DestX				Specifies the x-coordinate, 
//								in logical units, of the left top corner
//								of the destination rectangle;
//	int		DestY				Specifies the y-coordinate, 
//								in logical units, of the left top corner
//								of the destination rectangle;
//	HDC		SrcDC				Handle to the source device context;
//	int		SrcX				Specifies the x-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		SrcY				Specifies the y-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		SrcWidth			Specifies the width, in logical units,
//								of the source rectangle;
//	int		SrcHeight			Specifies the height, in logical units,
//								of the source rectangle;
//	double	Horizontal			Horizontal resample factor (0.00-16.00);
//	double	Vertical			Vertical resample factor (0.00-16.00);
//	int		TransparentColor	The RGB color in the source device context
//								to treat as transparent;
//								if (TransparentColor = -1) the transparency
//								will not be used;
//	int		Flags				Reserved parameter.
//
////////////////////////////////////////////////////////////////////////////
int __stdcall fxResample(HDC DestDC, int DestX, int DestY, HDC SrcDC, int SrcX, int SrcY, int SrcWidth, int SrcHeight, double Horizontal, double Vertical, int TransparentColor, int Flags)
{
	if (Horizontal < 0)
		Horizontal = 0;
	if (Horizontal > 16)
		Horizontal = 16;

	if (Vertical < 0)
		Vertical = 0;
	if (Vertical > 16)
		Vertical = 16;

	int DestWidth		= (int)(SrcWidth * Horizontal);
	int DestHeight		= (int)(SrcHeight * Vertical);

	BITMAPINFOHEADER	BMI;
	BMI.biSize			= sizeof(BITMAPINFOHEADER);
	BMI.biWidth			= DestWidth;
	BMI.biHeight		= DestHeight;
	BMI.biPlanes		= 1;
	BMI.biBitCount		= 32;
	BMI.biCompression	= BI_RGB;
	BMI.biSizeImage		= 0;
	BMI.biXPelsPerMeter = 0;
	BMI.biYPelsPerMeter = 0;
	BMI.biClrUsed		= 0;
	BMI.biClrImportant	= 0;
	
	BYTE				*pDestBits;
	HBITMAP				hbmDest;
	hbmDest = ::CreateDIBSection(DestDC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pDestBits, NULL, 0l);

	BYTE				*pSrcBits;
	HBITMAP				hbmSrc;
	hbmSrc = ::CreateDIBSection(SrcDC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pSrcBits, NULL, 0l);

	HDC					dc = ::CreateCompatibleDC(NULL);
	HBITMAP				hbm = (HBITMAP) ::SelectObject(dc, hbmSrc);
	
	::SetStretchBltMode(dc, HALFTONE);

	if (!::StretchBlt(dc, 0, 0, DestWidth, DestHeight, SrcDC, SrcX, SrcY, SrcWidth, SrcHeight, SRCCOPY))
		return 0;

	::SelectObject(dc, hbmDest);

	if (!::StretchBlt(dc, 0, 0, DestWidth, DestHeight, DestDC, DestX, DestY, DestWidth, DestHeight, SRCCOPY))
		return 0;

	::SelectObject(dc, hbm);

	::DeleteDC(dc);

	if (!(TransparentColor < 0))
	{
		for (int y = 0; y < DestHeight; y++)
		{
			LPBYTE pbDestRGB = (LPBYTE)&((DWORD*)pDestBits)[y * DestWidth];
			LPBYTE pbSrcRGB = (LPBYTE)&((DWORD*)pSrcBits)[y * DestWidth];

			for (int x = 0; x < DestWidth; x++)
			{		

				if (TransparentColor == fxRGB(pbSrcRGB[2], pbSrcRGB[1], pbSrcRGB[0]))
					pbSrcRGB[0] = pbDestRGB[0],
					pbSrcRGB[1] = pbDestRGB[1],
					pbSrcRGB[2] = pbDestRGB[2];
				

				pbSrcRGB += 4;
				pbDestRGB += 4;
			}
		}
	}

	dc = ::CreateCompatibleDC(NULL);
	
	hbm = (HBITMAP) ::SelectObject(dc, hbmSrc);
	if (!::BitBlt(DestDC, DestX, DestY, DestWidth, DestHeight, dc, 0, 0, SRCCOPY))
		return 0;

	::DeleteDC(dc);

	::DeleteObject(hbmSrc);	
	::DeleteObject(hbmDest);	
	
	return 1;
}



////////////////////////////////////////////////////////////////////////////
//
//	fxRGB()						Function returns the RGB color value from
//								given R, G, B values.
//
//	int		R					Red value (value in range 0 to 255);
//	int		G					Green value (value in range 0 to 255);
//	int		B					Blue value (value in range 0 to 255).
//
////////////////////////////////////////////////////////////////////////////
int __stdcall fxRGB(int R, int G, int B)
{
	//return (B * 65536 + G * 256 + R);
	return RGB(R, G, B);
}



////////////////////////////////////////////////////////////////////////////
//
//	fxRGBtoHSL()				Function returns the H, S, L values from
//								given RGB color value.
//
//	int		RGB					RGB color value to convert;
//	double	H					Reference to hue value;
//	double	S					Reference to saturation value;
//	double	L					Reference to luminiscency value.
//
////////////////////////////////////////////////////////////////////////////
int __stdcall fxRGBtoHSL(int RGB, double *H, double *S, double *L)
{
	double delta;
	double r			= (double)GetRValue(RGB) / 255;
	double g			= (double)GetGValue(RGB) / 255;
	double b			= (double)GetBValue(RGB) / 255;

	double cmax			= max(r, max(g, b));
	double cmin			= min(r, min(g, b));

	*L = (cmax + cmin) / 2.0;
  
	if (cmax == cmin)
	{
		*S = 0;
		*H = 0; // it's really undefined
	} 
	else 
	{
		if (*L < 0.5) 
			*S = (cmax - cmin) / (cmax + cmin);
		else
			*S = (cmax - cmin) / (2.0 - cmax - cmin);

		delta = cmax - cmin;

		if (r == cmax)
			*H = (g - b) / delta;
		else if(g == cmax)
			*H = 2.0 + (b - r) / delta;
		else
			*H = 4.0 + (r - g) / delta;
		
		*H /= 6.0;

		if (*H < 0.0)
			*H += 1;
	}

	return 1;
}



////////////////////////////////////////////////////////////////////////////
//
//	fxRotate()					Rotates bitmap by specified angle.
//								If the function succeeds, the return value
//								is 1 (true); 
//								if fails, the return value is 0 (false). 
//
//	HDC		DestDC				Handle to the destination device context;
//	int		CenterY				Specifies the x-coordinate, 
//								in logical units, of the center
//								of the destination rectangle;
//	int		CenterY				Specifies the y-coordinate, 
//								in logical units, of the center
//								of the destination rectangle;
//	HDC		SrcDC				Handle to the source device context;
//	int		SrcX				Specifies the x-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		SrcY				Specifies the y-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		SrcWidth			Specifies the width, in logical units,
//								of the source and destination rectangles;
//	int		SrcHeight			Specifies the height, in logical units,
//								of the source and destination rectangles;
//	int		Angle				Rotation angle (value in range -360 to 360);
//	int		TransparentColor	The RGB color in the source device context
//								to treat as transparent;
//								if (TransparentColor = -1) the transparency
//								will not be used;
//	int		Flags				Reserved parameter.
//
////////////////////////////////////////////////////////////////////////////
int __stdcall fxRotate(HDC DestDC, int CenterX, int CenterY, HDC SrcDC, int SrcX, int SrcY, int SrcWidth, int SrcHeight, double Angle, int TransparentColor, int Flags)
{
	if (Angle < -360)
		Angle = -360;
	if (Angle > 360)
		Angle = 360;


	double PI			= 3.14;
	double Radians		= PI / 180; //1.74532925199433E-02;
	
	double Theta		= 0;

	switch (int(Angle))
	{
		case -360: 
			Theta = 0;
			break;
		case 0:
			Theta = 0;
			break;
		case 360:
			Theta = 0;
			break;
		default:
			Theta = Angle * Radians;
			break;
	}


	if (SrcWidth > SrcHeight)
		SrcHeight = SrcWidth;
	else
		SrcWidth = SrcHeight;

	int	Center			= (int)(float(SrcWidth) / 2);

	int DestX			= CenterX - Center;
	int DestY			= CenterY - Center;


	BITMAPINFOHEADER	BMI;
	BMI.biSize			= sizeof(BITMAPINFOHEADER);
	BMI.biWidth			= SrcWidth;
	BMI.biHeight		= SrcHeight;
	BMI.biPlanes		= 1;
	BMI.biBitCount		= 32;
	BMI.biCompression	= BI_RGB;
	BMI.biSizeImage		= 0;
	BMI.biXPelsPerMeter = 0;
	BMI.biYPelsPerMeter = 0;
	BMI.biClrUsed		= 0;
	BMI.biClrImportant	= 0;
	
	BYTE				*pDestBits;
	HBITMAP				hbmDest;
	hbmDest = ::CreateDIBSection(DestDC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pDestBits, NULL, 0l);

	BYTE				*pSrcBits;
	HBITMAP				hbmSrc;
	hbmSrc = ::CreateDIBSection(SrcDC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pSrcBits, NULL, 0l);

	HDC					dc = ::CreateCompatibleDC(NULL);
	HBITMAP				hbm = (HBITMAP) ::SelectObject(dc, hbmSrc);
	
	if (!::BitBlt(dc, 0, 0, SrcWidth, SrcHeight, SrcDC, SrcX, SrcY, SRCCOPY))
		return 0;

	::SelectObject(dc, hbmDest);

	if (!::BitBlt(dc, 0, 0, SrcWidth, SrcHeight, DestDC, DestX, DestY, SRCCOPY))
		return 0;

	::SelectObject(dc, hbm);

	::DeleteDC(dc);

	double SinTheta		= sin(Theta);
	double CosTheta		= cos(Theta);
	
	POINT pt;
	RECT rect;

	rect.left = 0;
	rect.top = 0;
	rect.right = SrcWidth;
	rect.bottom = SrcHeight;

	for (int y = 0; y < SrcHeight; y++)
    {
		for (int x = 0; x < SrcWidth; x++)
		{
			//pt.x = int(Center + ((x - CenterX) * CosTheta - (y - CenterY) * SinTheta) + .5);
			//pt.y = int(Center + ((x - CenterX) * SinTheta + (y - CenterY) * CosTheta) + .5);			

			pt.x = int(Center + ((x - Center) * CosTheta - (y - Center) * SinTheta) + .5);
			pt.y = int(Center + ((x - Center) * SinTheta + (y - Center) * CosTheta) + .5);			
			
			if (::PtInRect(&rect, pt))
			{
				LPBYTE pbDestRGB = (LPBYTE)&((DWORD*)pDestBits)[y * SrcWidth];
				LPBYTE pbSrcRGB = (LPBYTE)&((DWORD*)pSrcBits)[pt.y * SrcWidth];

				if (!(TransparentColor == fxRGB(pbSrcRGB[(4 * pt.x)], pbSrcRGB[(4 * pt.x)+1], pbSrcRGB[(4 * pt.x)+2])))
					pbDestRGB[(4 * x)] = pbSrcRGB[(4 * pt.x)],
					pbDestRGB[(4 * x) + 1] = pbSrcRGB[(4 * pt.x) + 1],
					pbDestRGB[(4 * x) + 2] = pbSrcRGB[(4 * pt.x) + 2];
			}
		}
	}

	dc = ::CreateCompatibleDC(NULL);
	
	hbm = (HBITMAP) ::SelectObject(dc, hbmDest);
	if (!::BitBlt(DestDC, DestX, DestY, SrcWidth, SrcHeight, dc, 0, 0, SRCCOPY))
		return 0;

	::DeleteDC(dc);

	::DeleteObject(hbmDest);	
	::DeleteObject(hbmSrc);	
	
	return 1;
}



////////////////////////////////////////////////////////////////////////////
//
//	fxSaturation()				Function displays bitmap with specified
//								saturation value, first
//								stretching or compressing the bitmap to fit
//								the dimensions of the destination rectangle,
//								if necessary.
//								If the function succeeds, the return value
//								is 1 (true); 
//								if fails, the return value is 0 (false). 
//
//	HDC		DestDC				Handle to the destination device context;
//	int		DestX				Specifies the x-coordinate, 
//								in logical units, of the upper-left 
//								corner of the destination rectangle;
//	int		DestY				Specifies the y-coordinate, 
//								in logical units, of the upper-left 
//								corner of the destination rectangle;
//	int		DestWidth			Specifies the width, in logical units,
//								of the destination rectangle.
//	int		DestHeight			Specifies the height, in logical units,
//								of the destination rectangle.
//	HDC		SrcDC				Handle to the source device context;
//	int		SrcX				Specifies the x-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		SrcY				Specifies the y-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		SrcWidth			Specifies the width, in logical units,
//								of the source rectangle;
//	int		SrcHeight			Specifies the height, in logical units,
//								of the source rectangle;
//	int		Saturation			Saturation (value in range 0 to 255);
//	int		Flags				Reserved parameter.
//
////////////////////////////////////////////////////////////////////////////
int __stdcall fxSaturation(HDC DestDC, int DestX, int DestY, int DestWidth, int DestHeight, HDC SrcDC, int SrcX, int SrcY, int SrcWidth, int SrcHeight, int Saturation, int Flags)
{
	if (Saturation < 0)
		Saturation = 0;
	if (Saturation > 255)
		Saturation = 255;

	int fxGreyscale		= 0;

	BITMAPINFOHEADER	BMI;
	BMI.biSize			= sizeof(BITMAPINFOHEADER);
	BMI.biWidth			= DestWidth;
	BMI.biHeight		= DestHeight;
	BMI.biPlanes		= 1;
	BMI.biBitCount		= 32;
	BMI.biCompression	= BI_RGB;
	BMI.biSizeImage		= 0;
	BMI.biXPelsPerMeter = 0;
	BMI.biYPelsPerMeter = 0;
	BMI.biClrUsed		= 0;
	BMI.biClrImportant	= 0;
	
	/*
	BYTE				*pDestBits;
	HBITMAP				hbmDest;
	hbmDest = ::CreateDIBSection(DestDC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pDestBits, NULL, 0l);
	*/

	BYTE				*pSrcBits;
	HBITMAP				hbmSrc;
	hbmSrc = ::CreateDIBSection(SrcDC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pSrcBits, NULL, 0l);

	HDC					dc = ::CreateCompatibleDC(NULL);
	HBITMAP				hbm = (HBITMAP) ::SelectObject(dc, hbmSrc);
	
	::SetStretchBltMode(dc, HALFTONE);

	if (!::StretchBlt(dc, 0, 0, DestWidth, DestHeight, SrcDC, SrcX, SrcY, SrcWidth, SrcHeight, SRCCOPY))
		return 0;

	/*
	::SelectObject(dc, hbmDest);

	if (!::StretchBlt(dc, 0, 0, DestWidth, DestHeight, DestDC, DestX, DestY, DestWidth, DestHeight, SRCCOPY))
		return 0;
	*/

	::SelectObject(dc, hbm);

	::DeleteDC(dc);

    for (int y = 0; y < DestHeight; y++)
    {
		LPBYTE pbSrcRGB = (LPBYTE)&((DWORD*)pSrcBits)[y * DestWidth];
		//LPBYTE pbDestRGB = (LPBYTE)&((DWORD*)pDestBits)[y * DestWidth];

		for (int x = 0; x < DestWidth; x++)
		{
			fxGreyscale = (pbSrcRGB[0] + pbSrcRGB[1] + pbSrcRGB[2]) / 3;

			pbSrcRGB[0] = fxShadeColors(fxGreyscale, pbSrcRGB[0], Saturation);
			pbSrcRGB[1] = fxShadeColors(fxGreyscale, pbSrcRGB[1], Saturation);
			pbSrcRGB[2] = fxShadeColors(fxGreyscale, pbSrcRGB[2], Saturation);

			pbSrcRGB += 4;
			//pbDestRGB += 4;
		}
	}

	dc = ::CreateCompatibleDC(NULL);
	
	hbm = (HBITMAP) ::SelectObject(dc, hbmSrc);
	if (!::BitBlt(DestDC, DestX, DestY, DestWidth, DestHeight, dc, 0, 0, SRCCOPY))
		return 0;

	::DeleteDC(dc);

	::DeleteObject(hbmSrc);	
	//::DeleteObject(hbmDest);	
	
	return 1;
}



////////////////////////////////////////////////////////////////////////////
//
//	fxScanlines()				Displays scan-lined bitmap,
//								first stretching or compressing it to fit
//								the dimensions of the destination rectangle,
//								if necessary.
//								If the function succeeds, the return value
//								is 1 (true); 
//								if fails, the return value is 0 (false). 
//
//	HDC		DestDC				Handle to the destination device context;
//	int		DestX				Specifies the x-coordinate, 
//								in logical units, of the upper-left 
//								corner of the destination rectangle;
//	int		DestY				Specifies the y-coordinate, 
//								in logical units, of the upper-left 
//								corner of the destination rectangle;
//	int		DestWidth			Specifies the width, in logical units,
//								of the destination rectangle.
//	int		DestHeight			Specifies the height, in logical units,
//								of the destination rectangle.
//	HDC		SrcDC				Handle to the source device context;
//	int		SrcX				Specifies the x-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		SrcY				Specifies the y-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		SrcWidth			Specifies the width, in logical units,
//								of the source rectangle;
//	int		SrcHeight			Specifies the height, in logical units,
//								of the source rectangle;
//	int		Color				Color of scan-lines (RGB);
//	int		Transparency		Transparency of scanlines (value in
//								range 0 to 255);
//	int		Step				Gap between scanlines;
//	bool	Horizontal			Specifies wether to draw horizontal lines;
//	bool	Vertical			Specifies wether to draw vertical lines;
//	int		Flags				Reserved parameter.
//
////////////////////////////////////////////////////////////////////////////
int __stdcall fxScanlines(HDC DestDC, int DestX, int DestY, int DestWidth, int DestHeight, HDC SrcDC, int SrcX, int SrcY, int SrcWidth, int SrcHeight, int Color, int Transparency, int Step, bool Horizontal, bool Vertical, int Flags)
{
	Step				= Step + 1;

	if (Transparency < 0)
		Transparency = 0;
	if (Transparency > 255)
		Transparency = 255;

	if (Step < 1)
		Step = 1;
	if (Step > 255)
		Step = 255;

	int fxR				= fxByte(fxGetRed(Color));
	int fxG				= fxByte(fxGetGreen(Color));
	int fxB				= fxByte(fxGetBlue(Color));

	BITMAPINFOHEADER	BMI;
	BMI.biSize			= sizeof(BITMAPINFOHEADER);
	BMI.biWidth			= DestWidth;
	BMI.biHeight		= DestHeight;
	BMI.biPlanes		= 1;
	BMI.biBitCount		= 32;
	BMI.biCompression	= BI_RGB;
	BMI.biSizeImage		= 0;
	BMI.biXPelsPerMeter = 0;
	BMI.biYPelsPerMeter = 0;
	BMI.biClrUsed		= 0;
	BMI.biClrImportant	= 0;
	
	/*
	BYTE				*pDestBits;
	HBITMAP				hbmDest;
	hbmDest = ::CreateDIBSection(DestDC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pDestBits, NULL, 0l);
	*/

	BYTE				*pSrcBits;
	HBITMAP				hbmSrc;
	hbmSrc = ::CreateDIBSection(SrcDC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pSrcBits, NULL, 0l);

	HDC					dc = ::CreateCompatibleDC(NULL);
	HBITMAP				hbm = (HBITMAP) ::SelectObject(dc, hbmSrc);
	
	::SetStretchBltMode(dc, HALFTONE);

	if (!::StretchBlt(dc, 0, 0, DestWidth, DestHeight, SrcDC, SrcX, SrcY, SrcWidth, SrcHeight, SRCCOPY))
		return 0;

	/*
	::SelectObject(dc, hbmDest);

	if (!::StretchBlt(dc, 0, 0, DestWidth, DestHeight, DestDC, DestX, DestY, DestWidth, DestHeight, SRCCOPY))
		return 0;
	*/

	::SelectObject(dc, hbm);

	::DeleteDC(dc);

	if (Step > 1)
	{
		for (int y = 0; y < DestHeight; y++)
		{
			LPBYTE pbSrcRGB = (LPBYTE)&((DWORD*)pSrcBits)[y * DestWidth];
			//LPBYTE pbDestRGB = (LPBYTE)&((DWORD*)pDestBits)[y * DestWidth];

			for (int x = 0; x < DestWidth; x++)
			{
				if (Horizontal)
				{
					if (((DestHeight - y) % Step) == 1)
						pbSrcRGB[0] = fxShadeColors(fxB, pbSrcRGB[0], Transparency),
						pbSrcRGB[1] = fxShadeColors(fxG, pbSrcRGB[1], Transparency),
						pbSrcRGB[2] = fxShadeColors(fxR, pbSrcRGB[2], Transparency);
				}
				
				if (Vertical)
				{
					if ((x % Step) == 0) 
						pbSrcRGB[0] = fxShadeColors(fxB, pbSrcRGB[0], Transparency),
						pbSrcRGB[1] = fxShadeColors(fxG, pbSrcRGB[1], Transparency),
						pbSrcRGB[2] = fxShadeColors(fxR, pbSrcRGB[2], Transparency);
				}

				pbSrcRGB += 4;
				//pbDestRGB += 4;
			}
		}
	}
	else
	{
		for (int y = 0; y < DestHeight; y++)
		{
			LPBYTE pbSrcRGB = (LPBYTE)&((DWORD*)pSrcBits)[y * DestWidth];
			//LPBYTE pbDestRGB = (LPBYTE)&((DWORD*)pDestBits)[y * DestWidth];

			for (int x = 0; x < DestWidth; x++)
			{
				pbSrcRGB[0] = fxShadeColors(fxB, pbSrcRGB[0], Transparency);
				pbSrcRGB[1] = fxShadeColors(fxG, pbSrcRGB[1], Transparency);
				pbSrcRGB[2] = fxShadeColors(fxR, pbSrcRGB[2], Transparency);

				pbSrcRGB += 4;
				//pbDestRGB += 4;
			}
		}
	}

	dc = ::CreateCompatibleDC(NULL);
	
	hbm = (HBITMAP) ::SelectObject(dc, hbmSrc);
	if (!::BitBlt(DestDC, DestX, DestY, DestWidth, DestHeight, dc, 0, 0, SRCCOPY))
		return 0;

	::DeleteDC(dc);

	::DeleteObject(hbmSrc);	
	//::DeleteObject(hbmDest);	
	
	return 1;
}



////////////////////////////////////////////////////////////////////////////
//
//	fxScreenShot()				Takes screen-shot of the desktop
//								window and displays it in the
//								specified device context, first
//								stretching or compressing the bitmap to fit
//								the dimensions of the destination rectangle,
//								if necessary.
//								If the function succeeds, the return value
//								is 1 (true); 
//								if fails, the return value is 0 (false). 
//
//	HDC		DestDC				Handle to the destination device context;
//	int		DestX				Specifies the x-coordinate, 
//								in logical units, of the upper-left 
//								corner of the destination rectangle;
//	int		DestY				Specifies the y-coordinate, 
//								in logical units, of the upper-left 
//								corner of the destination rectangle;
//	bool	GetCursor			Shows wether to take cursor shot.
//	int		Flags				Reserved parameter.
//
////////////////////////////////////////////////////////////////////////////
int __stdcall fxScreenShot(HDC DestDC, int DestX, int DestY, int DestWidth, int DestHeight, bool GetCursor, int Flags)
{	
	HDC	SrcDC			= ::GetDC(::GetDesktopWindow());
	int	SrcX			= 0;
	int	SrcY			= 0;
	int	SrcWidth		= ::GetDeviceCaps(SrcDC, HORZRES);
	int	SrcHeight		= ::GetDeviceCaps(SrcDC, VERTRES);

	if (DestWidth <= 0)
		DestWidth = SrcWidth;

	if (DestHeight <= 0)
		DestHeight = SrcHeight;

	/*
	if (::IsRectEmpty(Shot) != NULL)
	{
		SrcX			= 0;
		SrcY			= 0;
		SrcWidth		= ::GetDeviceCaps(SrcDC, HORZRES);
		SrcHeight		= ::GetDeviceCaps(SrcDC, VERTRES);
	}
	else
	{
		SrcX			= Shot->left;
		SrcY			= Shot->top;
		SrcWidth		= Shot->right - Shot->left;
		SrcHeight		= Shot->bottom - Shot->top;	
	}
	*/

	BITMAPINFOHEADER	BMI;
	BMI.biSize			= sizeof(BITMAPINFOHEADER);
	BMI.biWidth			= DestWidth;
	BMI.biHeight		= DestHeight;
	BMI.biPlanes		= 1;
	BMI.biBitCount		= 32;
	BMI.biCompression	= BI_RGB;
	BMI.biSizeImage		= 0;
	BMI.biXPelsPerMeter = 0;
	BMI.biYPelsPerMeter = 0;
	BMI.biClrUsed		= 0;
	BMI.biClrImportant	= 0;
	
	/*
	BYTE				*pDestBits;
	HBITMAP				hbmDest;
	hbmDest = ::CreateDIBSection(DestDC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pDestBits, NULL, 0l);
	*/

	BYTE				*pSrcBits;
	HBITMAP				hbmSrc;
	hbmSrc = ::CreateDIBSection(SrcDC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pSrcBits, NULL, 0l);

	HDC					dc = ::CreateCompatibleDC(NULL);
	HBITMAP				hbm = (HBITMAP) ::SelectObject(dc, hbmSrc);

	if (!::BitBlt(dc, 0, 0, SrcWidth, SrcHeight, SrcDC, SrcX, SrcY, SRCCOPY))
		return 0;

	if (GetCursor)
	{
		POINT pt;
		RECT rect;

		::GetCursorPos(&pt);		

		::SetRect(&rect, SrcX, SrcY, SrcWidth, SrcHeight);

		if (::PtInRect(&rect, pt) == TRUE)
			::DrawIconEx(dc, pt.x, pt.y, ::GetCursor(), 0, 0, 0, NULL, DI_NORMAL);
	}
	
	/*
	::SelectObject(dc, hbmDest);

	if (!::StretchBlt(dc, 0, 0, DestWidth, DestHeight, DestDC, DestX, DestY, DestWidth, DestHeight, SRCCOPY))
		return 0;
	*/

	::SelectObject(dc, hbm);

	::DeleteDC(dc);
	
	dc = ::CreateCompatibleDC(NULL);
	
	hbm = (HBITMAP) ::SelectObject(dc, hbmSrc);
	if (!::StretchBlt(DestDC, DestX, DestY, DestWidth, DestHeight, dc, 0, 0, SrcWidth, SrcHeight, SRCCOPY))
		return 0;

	::DeleteDC(dc);

	::DeleteObject(hbmSrc);	
	//::DeleteObject(hbmDest);	
	
	return 1;
}



////////////////////////////////////////////////////////////////////////////
//
//	fxScroll()					Function scrolls specified rectangle in 
//								device context.
//								If the function succeeds, the return value
//								is 1 (true); 
//								if fails, the return value is 0 (false). 
//
//	HDC		DC					Specifies the device context where scrolling
//								takes place;
//	int		X					Specifies the horizontal scroll value, in
//								logical units;
//	int		Y					Specifies the vertical scroll value, in
//								logical units;
//	LPRECT	Scroll				Specifies the rectangle, in logical units, 
//								of the area to scroll;
//	LPRECT	Clip				Specifies the rectangle, in logical units, 
//								of the area in which to display scrolled
//								rectangle;
//	int		Flags				Reserved parameter.
//
////////////////////////////////////////////////////////////////////////////
int __stdcall fxScroll(HDC DC, int X, int Y, LPRECT Scroll, LPRECT Clip, int Flags)
{
	int Width			= Clip->right - Clip->left;
	int Height			= Clip->bottom - Clip->top;

	return ::BitBlt(DC, Clip->left, Clip->top, Width, Height, DC, Scroll->left + X, Scroll->top + Y, SRCCOPY);
}



////////////////////////////////////////////////////////////////////////////
//
//	fxSemitransparentBlt()		Function performs a bit-block transfer 
//								of the color data corresponding to a
//								rectangle of pixels from the specified
//								source device context into a destination
//								device context,	stretching or compressing
//								the bitmap to fit the dimensions of the
//								destination rectangle, if necessary.
//								If the function succeeds, the return value
//								is 1 (true); 
//								if fails, the return value is 0 (false). 
//
//	HDC		DestDC				Handle to the destination device context;
//	int		DestX				Specifies the x-coordinate, 
//								in logical units, of the upper-left 
//								corner of the destination rectangle;
//	int		DestY				Specifies the y-coordinate, 
//								in logical units, of the upper-left 
//								corner of the destination rectangle;
//	int		DestWidth			Specifies the width, in logical units,
//								of the destination rectangle.
//	int		DestHeight			Specifies the height, in logical units,
//								of the destination rectangle.
//	HDC		SrcDC				Handle to the source device context;
//	int		SrcX				Specifies the x-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		SrcY				Specifies the y-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		SrcWidth			Specifies the width, in logical units,
//								of the source rectangle;
//	int		SrcHeight			Specifies the height, in logical units,
//								of the source rectangle;
//	int		Color				The RGB color in the source DC to 
//								treat as transparent;
//	int		Transparency		Transparency of the (transparent) color
//								(value in range 0 to 255).	
//
////////////////////////////////////////////////////////////////////////////
int __stdcall fxSemitransparentBlt(HDC DestDC, int DestX, int DestY, int DestWidth, int DestHeight, HDC SrcDC, int SrcX, int SrcY, int SrcWidth, int SrcHeight, int Color, int Transparency)
{
	if (Transparency < 0)
		Transparency = 0;
	if (Transparency > 255)
		Transparency = 255;

	BITMAPINFOHEADER	BMI;
	BMI.biSize			= sizeof(BITMAPINFOHEADER);
	BMI.biWidth			= DestWidth;
	BMI.biHeight		= DestHeight;
	BMI.biPlanes		= 1;
	BMI.biBitCount		= 32;
	BMI.biCompression	= BI_RGB;
	BMI.biSizeImage		= 0;
	BMI.biXPelsPerMeter = 0;
	BMI.biYPelsPerMeter = 0;
	BMI.biClrUsed		= 0;
	BMI.biClrImportant	= 0;
	
	BYTE				*pDestBits;
	HBITMAP				hbmDest;
	hbmDest = ::CreateDIBSection(DestDC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pDestBits, NULL, 0l);

	BYTE				*pSrcBits;
	HBITMAP				hbmSrc;
	hbmSrc = ::CreateDIBSection(SrcDC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pSrcBits, NULL, 0l);

	HDC					dc = ::CreateCompatibleDC(NULL);
	HBITMAP				hbm = (HBITMAP) ::SelectObject(dc, hbmSrc);
	
	::SetStretchBltMode(dc, HALFTONE);

	if (!::StretchBlt(dc, 0, 0, DestWidth, DestHeight, SrcDC, SrcX, SrcY, SrcWidth, SrcHeight, SRCCOPY))
		return 0;

	::SelectObject(dc, hbmDest);

	if (!::StretchBlt(dc, 0, 0, DestWidth, DestHeight, DestDC, DestX, DestY, DestWidth, DestHeight, SRCCOPY))
		return 0;

	::SelectObject(dc, hbm);

	::DeleteDC(dc);

    for (int y = 0; y < DestHeight; y++)
    {
		LPBYTE pbDestRGB = (LPBYTE)&((DWORD*)pDestBits)[y * DestWidth];
		LPBYTE pbSrcRGB = (LPBYTE)&((DWORD*)pSrcBits)[y * DestWidth];

		for (int x = 0; x < DestWidth; x++)
		{
			if (Color == fxRGB(pbSrcRGB[2], pbSrcRGB[1], pbSrcRGB[0]))
				pbSrcRGB[0] = fxShadeColors(pbSrcRGB[0], pbDestRGB[0], Transparency),
				pbSrcRGB[1] = fxShadeColors(pbSrcRGB[1], pbDestRGB[1], Transparency),
				pbSrcRGB[2] = fxShadeColors(pbSrcRGB[2], pbDestRGB[2], Transparency);

			pbSrcRGB += 4;
			pbDestRGB += 4;
		}
	}

	dc = ::CreateCompatibleDC(NULL);
	
	hbm = (HBITMAP) ::SelectObject(dc, hbmSrc);
	if (!::BitBlt(DestDC, DestX, DestY, DestWidth, DestHeight, dc, 0, 0, SRCCOPY))
		return 0;

	::DeleteDC(dc);

	::DeleteObject(hbmSrc);	
	::DeleteObject(hbmDest);	
	
	return 1;
}



////////////////////////////////////////////////////////////////////////////
//
//	fxShadeColors()				Function returns the shaded color value
//								that is calculated from destination,
//								source colors and shade values.
//
//	int		DestColor			Specifies the destination color value;
//	int		SrcColor			Specifies the source color value;
//	int		ShadeValue			Shade value (value in range 0 to 255).
//
////////////////////////////////////////////////////////////////////////////
int __stdcall fxShadeColors(int DestColor, int SrcColor, int Shade)
{
	if (Shade == 0)
		return DestColor;
	if (Shade == 255)
		return SrcColor;

	return (DestColor * (255 - Shade) + SrcColor * Shade) >> 8;
}


////////////////////////////////////////////////////////////////////////////
//
//	fxShift()					Displays shifted bitmap,
//								first stretching or compressing it to fit
//								the dimensions of the destination rectangle,
//								if necessary.
//								If the function succeeds, the return value
//								is 1 (true); 
//								if fails, the return value is 0 (false). 
//
//	HDC		DestDC				Handle to the destination device context;
//	int		DestX				Specifies the x-coordinate, 
//								in logical units, of the upper-left 
//								corner of the destination rectangle;
//	int		DestY				Specifies the y-coordinate, 
//								in logical units, of the upper-left 
//								corner of the destination rectangle;
//	int		DestWidth			Specifies the width, in logical units,
//								of the destination rectangle.
//	int		DestHeight			Specifies the height, in logical units,
//								of the destination rectangle.
//	HDC		SrcDC				Handle to the source device context;
//	int		SrcX				Specifies the x-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		SrcY				Specifies the y-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		SrcWidth			Specifies the width, in logical units,
//								of the source rectangle;
//	int		SrcHeight			Specifies the height, in logical units,
//								of the source rectangle.
//	int		Shift				Shift value (value in range 0 to 255);
//	int		Flags				Reserved parameter.
//
////////////////////////////////////////////////////////////////////////////
int __stdcall fxShift(HDC DestDC, int DestX, int DestY, int DestWidth, int DestHeight, HDC SrcDC, int SrcX, int SrcY, int SrcWidth, int SrcHeight, int Shift, int Flags)
{
	if (Shift < 0)
		Shift = 0;
	if (Shift > 255)
		Shift = 255;

	BITMAPINFOHEADER	BMI;
	BMI.biSize			= sizeof(BITMAPINFOHEADER);
	BMI.biWidth			= DestWidth;
	BMI.biHeight		= DestHeight;
	BMI.biPlanes		= 1;
	BMI.biBitCount		= 32;
	BMI.biCompression	= BI_RGB;
	BMI.biSizeImage		= 0;
	BMI.biXPelsPerMeter = 0;
	BMI.biYPelsPerMeter = 0;
	BMI.biClrUsed		= 0;
	BMI.biClrImportant	= 0;

	BYTE				*pDestBits;
	HBITMAP				hbmDest;
	hbmDest = ::CreateDIBSection(DestDC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pDestBits, NULL, 0l);

	BYTE				*pSrcBits;
	HBITMAP				hbmSrc;
	hbmSrc = ::CreateDIBSection(SrcDC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pSrcBits, NULL, 0l);

	HDC					dc = ::CreateCompatibleDC(NULL);
	HBITMAP				hbm = (HBITMAP) ::SelectObject(dc, hbmSrc);
	
	::SetStretchBltMode(dc, HALFTONE);

	if (!::StretchBlt(dc, 0, 0, DestWidth, DestHeight, SrcDC, SrcX, SrcY, SrcWidth, SrcHeight, SRCCOPY))
		return 0;

	::SelectObject(dc, hbmDest);

	if (!::StretchBlt(dc, 0, 0, DestWidth, DestHeight, DestDC, DestX, DestY, DestWidth, DestHeight, SRCCOPY))
		return 0;

	::SelectObject(dc, hbm);

	::DeleteDC(dc);

    for (int y = 0; y < DestHeight; y++)
    {
		LPBYTE pbSrcRGB = (LPBYTE)&((DWORD*)pSrcBits)[y * DestWidth];
		LPBYTE pbDestRGB = (LPBYTE)&((DWORD*)pDestBits)[y * DestWidth];

		for (int x = 0; x < DestWidth; x++)
		{
			pbDestRGB[0] = fxShadeColors(pbSrcRGB[0], pbSrcRGB[1], Shift);
			pbDestRGB[1] = fxShadeColors(pbSrcRGB[1], pbSrcRGB[0], Shift);
			pbDestRGB[2] = fxShadeColors(pbSrcRGB[2], pbSrcRGB[2], Shift);

			pbSrcRGB += 4;
			pbDestRGB += 4;
		}
	}

	dc = ::CreateCompatibleDC(NULL);
	
	hbm = (HBITMAP) ::SelectObject(dc, hbmDest);

	if (!::BitBlt(DestDC, DestX, DestY, DestWidth, DestHeight, dc, 0, 0, SRCCOPY))
		return 0;

	::DeleteDC(dc);

	::DeleteObject(hbmSrc);	
	::DeleteObject(hbmDest);	
	
	return 1;
}



////////////////////////////////////////////////////////////////////////////
//
//	fxStretchBlt()				Function copies a bitmap from a source 
//								rectangle into a destination rectangle, 
//								stretching or compressing the bitmap to fit
//								the dimensions of the destination rectangle,
//								if necessary. Windows stretches or 
//								compresses the bitmap according to the 
//								stretching mode currently set in the 
//								destination device context.
//								If the function succeeds, the return value
//								is 1 (true); 
//								if fails, the return value is 0 (false). 
//
//	HDC		DestDC				Handle to the destination device context;
//	int		DestX				Specifies the x-coordinate, 
//								in logical units, of the upper-left 
//								corner of the destination rectangle;
//	int		DestY				Specifies the y-coordinate, 
//								in logical units, of the upper-left 
//								corner of the destination rectangle;
//	int		DestWidth			Specifies the width, in logical units,
//								of the destination rectangle.
//	int		DestHeight			Specifies the height, in logical units,
//								of the destination rectangle.
//	HDC		SrcDC				Handle to the source device context;
//	int		SrcX				Specifies the x-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		SrcY				Specifies the y-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		SrcWidth			Specifies the width, in logical units,
//								of the source rectangle;
//	int		SrcHeight			Specifies the height, in logical units,
//								of the source rectangle.
//	int		RasterOperation		Specifies a raster-operation code.
//
////////////////////////////////////////////////////////////////////////////
int __stdcall fxStretchBlt(HDC DestDC, int DestX, int DestY, int DestWidth, int DestHeight, HDC SrcDC, int SrcX, int SrcY, int SrcWidth, int SrcHeight, int RasterOperation)
{
	::SetStretchBltMode(DestDC, HALFTONE);
	return ::StretchBlt(DestDC, DestX, DestY, DestWidth, DestHeight, SrcDC, SrcX, SrcY, SrcWidth, SrcHeight, RasterOperation);
}



////////////////////////////////////////////////////////////////////////////
//
//	fxTextOut()					Function writes a character string with the 
//								specified color at the specified location,
//								using the currently selected font.
//								If the function succeeds, the return value
//								is 1 (true); 
//								if fails, the return value is 0 (false). 
//
//	HDC		DC					Handle of the destination device context;
//	int		X					Specifies the logical x-coordinate of the
//								reference point that will be used to align
//								the string;
//	int		Y					Specifies the logical y-coordinate of the
//								reference point that will be used to align
//								the string;
//	LPCSTR	Text				Specifies the text to write;
//	int		Color				Specifies the RGB color of the text;
//	int		Alignment			Text Alignment.
//	int		Flags				Reserved parameter.
//
////////////////////////////////////////////////////////////////////////////
int __stdcall fxText(HDC DC, int X, int Y, LPCSTR Text, int Color, int Alignment, int Flags)
{
	::SetTextAlign(DC, Alignment);

	::SetTextColor(DC, (COLORREF)Color);
	
	if (!::TextOut(DC, X, Y, Text, strlen(Text)))
		return 0;

	return 1;
}



////////////////////////////////////////////////////////////////////////////
//
//	fxTone()					Function displays toned bitmap, first
//								stretching or compressing the bitmap to fit
//								the dimensions of the destination rectangle,
//								if necessary.
//								If the function succeeds, the return value
//								is 1 (true); 
//								if fails, the return value is 0 (false). 
//
//	HDC		DestDC				Handle to the destination device context;
//	int		DestX				Specifies the x-coordinate, 
//								in logical units, of the upper-left 
//								corner of the destination rectangle;
//	int		DestY				Specifies the y-coordinate, 
//								in logical units, of the upper-left 
//								corner of the destination rectangle;
//	int		DestWidth			Specifies the width, in logical units,
//								of the destination rectangle.
//	int		DestHeight			Specifies the height, in logical units,
//								of the destination rectangle.
//	HDC		SrcDC				Handle to the source device context;
//	int		SrcX				Specifies the x-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		SrcY				Specifies the y-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		SrcWidth			Specifies the width, in logical units,
//								of the source rectangle;
//	int		SrcHeight			Specifies the height, in logical units,
//								of the source rectangle;
//	int		Color				Tone color (RGB);
//	int		Tone				Tone (value in range 0 to 255);
//	int		Flags				Reserved parameter.
//
////////////////////////////////////////////////////////////////////////////
int __stdcall fxTone(HDC DestDC, int DestX, int DestY, int DestWidth, int DestHeight, HDC SrcDC, int SrcX, int SrcY, int SrcWidth, int SrcHeight, int Color, int Tone, int Flags)
{
	if (Tone < 0)
		Tone = 0;
	if (Tone > 255)
		Tone = 255;

	int fxR				= fxByte(fxGetRed(Color));
	int fxG				= fxByte(fxGetGreen(Color));
	int fxB				= fxByte(fxGetBlue(Color));

	Tone				= 255 - Tone;

	BITMAPINFOHEADER	BMI;
	BMI.biSize			= sizeof(BITMAPINFOHEADER);
	BMI.biWidth			= DestWidth;
	BMI.biHeight		= DestHeight;
	BMI.biPlanes		= 1;
	BMI.biBitCount		= 32;
	BMI.biCompression	= BI_RGB;
	BMI.biSizeImage		= 0;
	BMI.biXPelsPerMeter = 0;
	BMI.biYPelsPerMeter = 0;
	BMI.biClrUsed		= 0;
	BMI.biClrImportant	= 0;
	
	/*
	BYTE				*pDestBits;
	HBITMAP				hbmDest;
	hbmDest = ::CreateDIBSection(DestDC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pDestBits, NULL, 0l);
	*/

	BYTE				*pSrcBits;
	HBITMAP				hbmSrc;
	hbmSrc = ::CreateDIBSection(SrcDC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pSrcBits, NULL, 0l);

	HDC					dc = ::CreateCompatibleDC(NULL);
	HBITMAP				hbm = (HBITMAP) ::SelectObject(dc, hbmSrc);
	
	::SetStretchBltMode(dc, HALFTONE);

	if (!::StretchBlt(dc, 0, 0, DestWidth, DestHeight, SrcDC, SrcX, SrcY, SrcWidth, SrcHeight, SRCCOPY))
		return 0;

	/*
	::SelectObject(dc, hbmDest);

	if (!::StretchBlt(dc, 0, 0, DestWidth, DestHeight, DestDC, DestX, DestY, DestWidth, DestHeight, SRCCOPY))
		return 0;
	*/

	::SelectObject(dc, hbm);

	::DeleteDC(dc);

    for (int y = 0; y < DestHeight; y++)
    {
		LPBYTE pbSrcRGB = (LPBYTE)&((DWORD*)pSrcBits)[y * DestWidth];
		//LPBYTE pbDestRGB = (LPBYTE)&((DWORD*)pDestBits)[y * DestWidth];

		for (int x = 0; x < DestWidth; x++)
		{
				pbSrcRGB[0] = fxShadeColors(fxB, pbSrcRGB[0], Tone);
				pbSrcRGB[1] = fxShadeColors(fxG, pbSrcRGB[1], Tone);
				pbSrcRGB[2] = fxShadeColors(fxR, pbSrcRGB[2], Tone);

			pbSrcRGB += 4;
			//pbDestRGB += 4;
		}
	}

	dc = ::CreateCompatibleDC(NULL);
	
	hbm = (HBITMAP) ::SelectObject(dc, hbmSrc);
	if (!::BitBlt(DestDC, DestX, DestY, DestWidth, DestHeight, dc, 0, 0, SRCCOPY))
		return 0;

	::DeleteDC(dc);

	::DeleteObject(hbmSrc);	
	//::DeleteObject(hbmDest);	
	
	return 1;
}



////////////////////////////////////////////////////////////////////////////
//
//	fxTransparentBlt()			Function performs a bit-block transfer 
//								of the color data corresponding to a
//								rectangle of pixels from the specified
//								source device context into a destination
//								device context,	stretching or compressing
//								the bitmap to fit the dimensions of the
//								destination rectangle, if necessary.
//								If the function succeeds, the return value
//								is 1 (true); 
//								if fails, the return value is 0 (false). 
//
//	HDC		DestDC				Handle to the destination device context;
//	int		DestX				Specifies the x-coordinate, 
//								in logical units, of the upper-left 
//								corner of the destination rectangle;
//	int		DestY				Specifies the y-coordinate, 
//								in logical units, of the upper-left 
//								corner of the destination rectangle;
//	int		DestWidth			Specifies the width, in logical units,
//								of the destination rectangle.
//	int		DestHeight			Specifies the height, in logical units,
//								of the destination rectangle.
//	HDC		SrcDC				Handle to the source device context;
//	int		SrcX				Specifies the x-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		SrcY				Specifies the y-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		SrcWidth			Specifies the width, in logical units,
//								of the source rectangle;
//	int		SrcHeight			Specifies the height, in logical units,
//								of the source rectangle;
//	int		Color				The RGB color in the source DC to 
//								treat as transparent.
//
////////////////////////////////////////////////////////////////////////////
int __stdcall fxTransparentBlt(HDC DestDC, int DestX, int DestY, int DestWidth, int DestHeight, HDC SrcDC, int SrcX, int SrcY, int SrcWidth, int SrcHeight, int Color)
{
	BITMAPINFOHEADER	BMI;
	BMI.biSize			= sizeof(BITMAPINFOHEADER);
	BMI.biWidth			= DestWidth;
	BMI.biHeight		= DestHeight;
	BMI.biPlanes		= 1;
	BMI.biBitCount		= 32;
	BMI.biCompression	= BI_RGB;
	BMI.biSizeImage		= 0;
	BMI.biXPelsPerMeter = 0;
	BMI.biYPelsPerMeter = 0;
	BMI.biClrUsed		= 0;
	BMI.biClrImportant	= 0;
	
	BYTE				*pDestBits;
	HBITMAP				hbmDest;
	hbmDest = ::CreateDIBSection(DestDC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pDestBits, NULL, 0l);

	BYTE				*pSrcBits;
	HBITMAP				hbmSrc;
	hbmSrc = ::CreateDIBSection(SrcDC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pSrcBits, NULL, 0l);

	HDC					dc = ::CreateCompatibleDC(NULL);
	HBITMAP				hbm = (HBITMAP) ::SelectObject(dc, hbmSrc);
	
	::SetStretchBltMode(dc, HALFTONE);

	if (!::StretchBlt(dc, 0, 0, DestWidth, DestHeight, SrcDC, SrcX, SrcY, SrcWidth, SrcHeight, SRCCOPY))
		return 0;

	::SelectObject(dc, hbmDest);

	if (!::StretchBlt(dc, 0, 0, DestWidth, DestHeight, DestDC, DestX, DestY, DestWidth, DestHeight, SRCCOPY))
		return 0;

	::SelectObject(dc, hbm);

	::DeleteDC(dc);

    for (int y = 0; y < DestHeight; y++)
    {
		LPBYTE pbDestRGB = (LPBYTE)&((DWORD*)pDestBits)[y * DestWidth];
		LPBYTE pbSrcRGB = (LPBYTE)&((DWORD*)pSrcBits)[y * DestWidth];

		for (int x = 0; x < DestWidth; x++)
		{
			if (Color == fxRGB(pbSrcRGB[2], pbSrcRGB[1], pbSrcRGB[0]))
				pbSrcRGB[0] = pbDestRGB[0],
				pbSrcRGB[1] = pbDestRGB[1],
				pbSrcRGB[2] = pbDestRGB[2];
			
			pbSrcRGB += 4;
			pbDestRGB += 4;
		}
	}

	dc = ::CreateCompatibleDC(NULL);
	
	hbm = (HBITMAP) ::SelectObject(dc, hbmSrc);
	if (!::BitBlt(DestDC, DestX, DestY, DestWidth, DestHeight, dc, 0, 0, SRCCOPY))
		return 0;

	::DeleteDC(dc);

	::DeleteObject(hbmSrc);	
	::DeleteObject(hbmDest);	
	
	return 1;
}



////////////////////////////////////////////////////////////////////////////
//
//	fxZoom()					Function zooms bitmap by specified zoom
//								value.
//								If the function succeeds, the return value
//								is 1 (true); 
//								if fails, the return value is 0 (false). 
//
//	HDC		DestDC				Handle to the destination device context;
//	int		CenterY				Specifies the x-coordinate, 
//								in logical units, of the center
//								of the destination rectangle;
//	int		CenterY				Specifies the y-coordinate, 
//								in logical units, of the center
//								of the destination rectangle;
//	HDC		SrcDC				Handle to the source device context;
//	int		SrcX				Specifies the x-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		SrcY				Specifies the y-coordinate, 
//								in logical units, of the upper-left 
//								corner of the source rectangle;
//	int		SrcWidth			Specifies the width, in logical units,
//								of the source rectangle;
//	int		SrcHeight			Specifies the height, in logical units,
//								of the source rectangle;
//	double	Zoom				Zoom factor (value in range 0.00 to 4.00);
//	int		TransparentColor	The RGB color in the source device context
//								to treat as transparent;
//								if (TransparentColor = -1) the transparency
//								will not be used;
//	int		Flags				Reserved parameter.
//
////////////////////////////////////////////////////////////////////////////
int __stdcall fxZoom(HDC DestDC, int CenterX, int CenterY, HDC SrcDC, int SrcX, int SrcY, int SrcWidth, int SrcHeight, double Zoom, int TransparentColor, int Flags)
{
	if (Zoom < 0)
		Zoom = 0;
	if (Zoom > 16)
		Zoom = 16;

	int DestWidth		= (int)(SrcWidth * Zoom);
	int DestHeight		= (int)(SrcHeight * Zoom);

	int DestX			= CenterX - (int)(float(DestWidth) / 2);
	int DestY			= CenterY - (int)(float(DestHeight) / 2);

	BITMAPINFOHEADER	BMI;
	BMI.biSize			= sizeof(BITMAPINFOHEADER);
	BMI.biWidth			= DestWidth;
	BMI.biHeight		= DestHeight;
	BMI.biPlanes		= 1;
	BMI.biBitCount		= 32;
	BMI.biCompression	= BI_RGB;
	BMI.biSizeImage		= 0;
	BMI.biXPelsPerMeter = 0;
	BMI.biYPelsPerMeter = 0;
	BMI.biClrUsed		= 0;
	BMI.biClrImportant	= 0;
	
	BYTE				*pDestBits;
	HBITMAP				hbmDest;
	hbmDest = ::CreateDIBSection(DestDC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pDestBits, NULL, 0l);

	BYTE				*pSrcBits;
	HBITMAP				hbmSrc;
	hbmSrc = ::CreateDIBSection(SrcDC, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pSrcBits, NULL, 0l);

	HDC					dc = ::CreateCompatibleDC(NULL);
	HBITMAP				hbm = (HBITMAP) ::SelectObject(dc, hbmSrc);
	
	::SetStretchBltMode(dc, HALFTONE);

	if (!::StretchBlt(dc, 0, 0, DestWidth, DestHeight, SrcDC, SrcX, SrcY, SrcWidth, SrcHeight, SRCCOPY))
		return 0;

	::SelectObject(dc, hbmDest);

	if (!::StretchBlt(dc, 0, 0, DestWidth, DestHeight, DestDC, DestX, DestY, DestWidth, DestHeight, SRCCOPY))
		return 0;

	::SelectObject(dc, hbm);

	::DeleteDC(dc);

	if (!(TransparentColor < 0))
	{
		for (int y = 0; y < DestHeight; y++)
		{
			LPBYTE pbDestRGB = (LPBYTE)&((DWORD*)pDestBits)[y * DestWidth];
			LPBYTE pbSrcRGB = (LPBYTE)&((DWORD*)pSrcBits)[y * DestWidth];

			for (int x = 0; x < DestWidth; x++)
			{		

				if (TransparentColor == fxRGB(pbSrcRGB[2], pbSrcRGB[1], pbSrcRGB[0]))
					pbSrcRGB[0] = pbDestRGB[0],
					pbSrcRGB[1] = pbDestRGB[1],
					pbSrcRGB[2] = pbDestRGB[2];
				

				pbSrcRGB += 4;
				pbDestRGB += 4;
			}
		}
	}

	dc = ::CreateCompatibleDC(NULL);
	
	hbm = (HBITMAP) ::SelectObject(dc, hbmSrc);
	if (!::BitBlt(DestDC, DestX, DestY, DestWidth, DestHeight, dc, 0, 0, SRCCOPY))
		return 0;

	::DeleteDC(dc);

	::DeleteObject(hbmSrc);	
	::DeleteObject(hbmDest);	
	
	return 1;
}




	/**************************************************************************
	// Copy source device context to memory device context:

	HDC	sDC				= ::CreateCompatibleDC(NULL);
	HBITMAP sBMP		= ::CreateCompatibleBitmap(SrcDC, SrcWidth, SrcHeight);
	HBITMAP sOBJ		= (HBITMAP)::SelectObject(sDC, sBMP);

	if (!::BitBlt(sDC, 0, 0, SrcWidth, SrcHeight, SrcDC, SrcX, SrcY, SRCCOPY)) //wh
		return 0;


	
	// Copy destination device context to second memory device context:

	HDC dDC				= ::CreateCompatibleDC(NULL);
	HBITMAP dBMP		= ::CreateCompatibleBitmap(DestDC, SrcWidth, SrcHeight); //dest w,h
	HBITMAP dOBJ		= (HBITMAP)::SelectObject(dDC, dBMP);

	if (!::BitBlt(dDC, 0, 0, SrcWidth, SrcHeight, DestDC, SrcX, SrcY, SRCCOPY)) //wh
		return 0;



	// Confirm neccessary bit-block-transfer between both memory device contexts:

	double angle		= (Angle / 100);

	POINT pt;
	RECT rect;

	rect.left = 0;
	rect.top = 0;
	rect.right = SrcWidth;
	rect.bottom = SrcHeight;

    for (int x = 0; x < SrcWidth; x++)
    {
		for (int y = 0; y < SrcHeight; y++)
		{
			pt.x = x * 2;
			pt.y = y * 2;

			if (::PtInRect(&rect, pt))
				::BitBlt(dDC, pt.x, pt.y, 1, 1, sDC, x, y, SRCCOPY);
		}
	}

	

	// Copy resulting memory device context back to destination device context:

	if (!::BitBlt(DestDC, DestX, DestY, SrcWidth, SrcHeight, dDC, 0, 0, SRCCOPY))
		return 0;



	// Delete memory device contexts:

	::DeleteDC(sDC);
	::DeleteDC(dDC);

	::DeleteObject(sBMP);
	::DeleteObject(sOBJ);
	
	::DeleteObject(dBMP);
	::DeleteObject(dOBJ);

	return 1;

	*******************************************************************************/

	


/*
	// SAFE-MODE (1.2-1.5x SLOWER);

	double angle		= (double)Angle;

	double fxX				= cos(angle * 0.017453); 
	double fxY				= sin(angle * 0.017453);

	int DestSize = 200;

	POINT pt;

    for (int y = SrcY; y < (SrcY + SrcHeight); y++)
    {
		for (int x = SrcX; x < (SrcX + SrcWidth); x++)
		{
			pt.x = DestX + (int)(fxX * x) - (int)(fxX * y);
			pt.y = DestY + (int)(fxY * y) - (int)(fxY * x);

			//pt.x = DestX + (SrcWidth / 2) + (int)(fxX * (x / 4)) - (int)(fxX * (y - SrcHeight / 2));
			//pt.y = DestY + (SrcHeight / 2) + (int)(fxY * (y / 4)) - (int)(fxY * (x - SrcWidth / 2));

			//pt.x = DestX + (SrcWidth / 2) + (int)(fxX * (x - SrcWidth / 2)) - (int)(fxX * (y - SrcHeight / 2));
			//pt.y = DestY + (SrcHeight / 2) + (int)(fxY * (y - SrcHeight / 2)) - (int)(fxY * (x - SrcWidth / 2));

			::BitBlt(DestDC, pt.x, pt.y, 1, 1, SrcDC, x, y, SRCCOPY);
		}
	}
	*/







//####################################################



			//int scanline = 4 * y * SrcWidth;
			//int Address = scanline + (4 * x);
			//int NewAddress = (4 * pt.y * SrcWidth) + (4 * pt.x);
				//memcpy(pDestBits + Address, pSrcBits + NewAddress, 4);