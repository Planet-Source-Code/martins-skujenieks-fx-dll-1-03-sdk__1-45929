
int __stdcall fxAlphaBlend(HDC DestDC, int DestX, int DestY, int DestWidth, int DestHeight, HDC SrcDC, int SrcX, int SrcY, int SrcWidth, int SrcHeight, int Blending, int TransparentColor, int Flags);
int __stdcall fxAmbientLight(HDC DestDC, int DestX, int DestY, int DestWidth, int DestHeight, HDC SrcDC, int SrcX, int SrcY, int SrcWidth, int SrcHeight, int Color, int Intensity, int Flags);
int __stdcall fxAntiAlias(HDC DestDC, int DestX, int DestY, int DestWidth, int DestHeight, HDC SrcDC, int SrcX, int SrcY, int SrcWidth, int SrcHeight, int Flags);
int __stdcall fxBitBlt(HDC DestDC, int DestX, int DestY, int DestWidth, int DestHeight, HDC SrcDC, int SrcX, int SrcY, int RasterOperation);
int __stdcall fxBlur(HDC DestDC, int DestX, int DestY, int DestWidth, int DestHeight, HDC SrcDC, int SrcX, int SrcY, int SrcWidth, int SrcHeight, int Flags);
int __stdcall fxBrightness(HDC DestDC, int DestX, int DestY, int DestWidth, int DestHeight, HDC SrcDC, int SrcX, int SrcY, int SrcWidth, int SrcHeight, int Brightness, int Flags);
int __stdcall fxCMYK(int C, int M, int Y, int K);
int __stdcall fxDrawText(HDC DC, int X, int Y, int Width, int Height, LPCSTR Text, int Color, HFONT Font, int Blending, int Flags);
int __stdcall fxEmboss(HDC DestDC, int DestX, int DestY, int DestWidth, int DestHeight, HDC SrcDC, int SrcX, int SrcY, int SrcWidth, int SrcHeight, double Depth, int Flags);
int __stdcall fxEngrave(HDC DestDC, int DestX, int DestY, int DestWidth, int DestHeight, HDC SrcDC, int SrcX, int SrcY, int SrcWidth, int SrcHeight, double Height, int Flags);
int __stdcall fxGamma(HDC DestDC, int DestX, int DestY, int DestWidth, int DestHeight, HDC SrcDC, int SrcX, int SrcY, int SrcWidth, int SrcHeight, double Gamma, int Flags);
int __stdcall fxGetBlue(int RGB);
int __stdcall fxGetGreen(int RGB);
int __stdcall fxGetRed(int RGB);
int __stdcall fxGreyscale(HDC DestDC, int DestX, int DestY, int DestWidth, int DestHeight, HDC SrcDC, int SrcX, int SrcY, int SrcWidth, int SrcHeight, int Flags);
int __stdcall fxGridelines(HDC DestDC, int DestX, int DestY, int DestWidth, int DestHeight, HDC SrcDC, int SrcX, int SrcY, int SrcWidth, int SrcHeight, int Color, int Transparency, int Distance, int Flags);
int __stdcall fxHSLtoRGB(double H, double S, double L);
int __stdcall fxHue(HDC DestDC, int DestX, int DestY, int DestWidth, int DestHeight, HDC SrcDC, int SrcX, int SrcY, int SrcWidth, int SrcHeight, double Hue, int Flags);
int __stdcall fxInversion(HDC DestDC, int DestX, int DestY, int DestWidth, int DestHeight, HDC SrcDC, int SrcX, int SrcY, int SrcWidth, int SrcHeight, int Inversion, int Flags);
int __stdcall fxInvert(HDC DestDC, int DestX, int DestY, int DestWidth, int DestHeight, HDC SrcDC, int SrcX, int SrcY, int SrcWidth, int SrcHeight, int Flags);
int __stdcall fxLight(HDC DC, int X, int Y, int Color, int Extension, int Intensity, int Definition, int Flags);
int __stdcall fxMirror(HDC DestDC, int DestX, int DestY, int DestWidth, int DestHeight, HDC SrcDC, int SrcX, int SrcY, int SrcWidth, int SrcHeight, bool Horizontal, bool Vertical, int Flags);
int __stdcall fxMosaic(HDC DestDC, int DestX, int DestY, int DestWidth, int DestHeight, HDC SrcDC, int SrcX, int SrcY, int SrcWidth, int SrcHeight, int Size, int Flags);
int __stdcall fxPaletteIndex(int Index);
int __stdcall fxPaletteRGB(int R, int G, int B);
int __stdcall fxReduceColors(HDC DestDC, int DestX, int DestY, int DestWidth, int DestHeight, HDC SrcDC, int SrcX, int SrcY, int SrcWidth, int SrcHeight, int Level, int Flags);
int __stdcall fxRelief(HDC DestDC, int DestX, int DestY, int DestWidth, int DestHeight, HDC SrcDC, int SrcX, int SrcY, int SrcWidth, int SrcHeight, int Flags);
int __stdcall fxRender(HDC DestDC, int CenterX, int CenterY, HDC SrcDC, int SrcX, int SrcY, int SrcWidth, int SrcHeight, int Blending, double Angle, double Zoom, int TransparentColor, int Flags);
int __stdcall fxReplaceColor(HDC DestDC, int DestX, int DestY, int DestWidth, int DestHeight, HDC SrcDC, int SrcX, int SrcY, int SrcWidth, int SrcHeight, int Color, int ByColor, int Flags);
int __stdcall fxReplaceColors(HDC DestDC, int DestX, int DestY, int DestWidth, int DestHeight, HDC SrcDC, int SrcX, int SrcY, int SrcWidth, int SrcHeight, int Color, int ByColor, int Similarity, int Flags);
int __stdcall fxResample(HDC DestDC, int DestX, int DestY, HDC SrcDC, int SrcX, int SrcY, int SrcWidth, int SrcHeight, double Horizontal, double Vertical, int TransparentColor, int Flags);
int __stdcall fxRGB(int R, int G, int B);
int __stdcall fxRGBtoHSL(int RGB, double *H, double *S, double *L);
int __stdcall fxRotate(HDC DestDC, int DestX, int DestY, HDC SrcDC, int SrcX, int SrcY, int SrcWidth, int SrcHeight, double Angle, int TransparentColor, int Flags);
int __stdcall fxSaturation(HDC DestDC, int DestX, int DestY, int DestWidth, int DestHeight, HDC SrcDC, int SrcX, int SrcY, int SrcWidth, int SrcHeight, int Saturation, int Flags);
int __stdcall fxScanlines(HDC DestDC, int DestX, int DestY, int DestWidth, int DestHeight, HDC SrcDC, int SrcX, int SrcY, int SrcWidth, int SrcHeight, int Color, int Transparency, int Step, bool Horizontal, bool Vertical, int Flags);
int __stdcall fxScreenShot(HDC DestDC, int DestX, int DestY, int DestWidth, int DestHeight, bool GetCursor, int Flags);
int __stdcall fxScroll(HDC DC, int X, int Y, LPRECT Scroll, LPRECT Clip, int Flags);
int __stdcall fxSemitransparentBlt(HDC DestDC, int DestX, int DestY, int DestWidth, int DestHeight, HDC SrcDC, int SrcX, int SrcY, int SrcWidth, int SrcHeight, int Color, int Transparency);
int __stdcall fxShadeColors(int DestColor, int SrcColor, int Shade);
int __stdcall fxShift(HDC DestDC, int DestX, int DestY, int DestWidth, int DestHeight, HDC SrcDC, int SrcX, int SrcY, int SrcWidth, int SrcHeight, int Shift, int Flags);
int __stdcall fxStretchBlt(HDC DestDC, int DestX, int DestY, int DestWidth, int DestHeight, HDC SrcDC, int SrcX, int SrcY, int SrcWidth, int SrcHeight, int RasterOperation);
int __stdcall fxText(HDC DC, int X, int Y, LPCSTR Text, int Color, int Alignment, int Flags);
int __stdcall fxTone(HDC DestDC, int DestX, int DestY, int DestWidth, int DestHeight, HDC SrcDC, int SrcX, int SrcY, int SrcWidth, int SrcHeight, int Color, int Tone, int Flags);
int __stdcall fxTransparentBlt(HDC DestDC, int DestX, int DestY, int DestWidth, int DestHeight, HDC SrcDC, int SrcX, int SrcY, int SrcWidth, int SrcHeight, int Color);
int __stdcall fxZoom(HDC DestDC, int CenterX, int CenterY, HDC SrcDC, int SrcX, int SrcY, int SrcWidth, int SrcHeight, double Zoom, int TransparentColor, int Flags);




/******************************************************************/

int fxByte(int Value)
{
	if (Value < 0)
		Value = 0;
	if (Value > 255)
		Value = 255;
	return Value;
}


int fxGetMemory(void *Address, int Size = 1)
{
	int Mem=0;
	int *pMem;
	pMem = &Mem;

	::memcpy(pMem, Address, Size);

	return Mem;
}


void fxSetMemory(void *Address, int Value, int Size = 1)
{
	::memset(Address, Value, Size);
}

/******************************************************************/