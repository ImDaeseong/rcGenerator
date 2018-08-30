//
// Copyright ?2001 Rui Godinho Lopes <ruiglopes@yahoo.com>
// All rights reserved.
//
// This source file(s) may be redistributed unmodified by any means
// PROVIDING they are not sold for profit without the authors expressed
// written consent, and providing that this notice and the authors name
// and all copyright notices remain intact.
//
// Any use of the software in source or binary forms, with or without
// modification, must include, in the user documentation ("About" box and
// printed documentation) and internal comments to the code, notices to
// the end user as follows:
//
// "Portions Copyright ?2001 Rui Godinho Lopes"
//
// An email letting me know that you are using it would be nice as well.
// That's not much to ask considering the amount of work that went into
// this.
//
// THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
// EXPRESS OR IMPLIED. USE IT AT YOUT OWN RISK. THE AUTHOR ACCEPTS NO
// LIABILITY FOR ANY DATA DAMAGE/LOSS THAT THIS PRODUCT MAY CAUSE.
//


#include "stdafx.h"
#include <fstream>
#include <string>

#ifdef _UNICODE
typedef std::wstring STD_TSTRING;
#else
typedef std::string STD_TSTRING;
#endif


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#pragma pack(push, 1)

const DWORD PSPBLOCKHEADERID= 0x004b427e;

struct TPSPBBLOCKHEADER
{
	DWORD m_dwHeaderId;	///< Allways PSPBLOCKHEADERID
	WORD m_wBlockId;		///< @see PSPBlockID.
	DWORD m_dwTotalBlockLength;
};


const BYTE PSPFILESIGNATURE[32]= {
	0x50, 0x61, 0x69, 0x6E, 0x74, 0x20, 0x53, 0x68, 0x6F, 0x70, 0x20,
	0x50, 0x72, 0x6F, 0x20, 0x49, 0x6D, 0x61, 0x67, 0x65, 0x20, 0x46,
	0x69, 0x6C, 0x65, 0x0A, 0x1A, 0x00, 0x00, 0x00, 0x00
	};

struct TPSPFILEHEADER
{
	BYTE m_bFileSignature[32]; ///< allways PSPFILESIGNATURE
	WORD m_wMajorVersion;
	WORD m_wMinorVersion;
};


struct TGeneralImageAttributesChunk
{
	DWORD m_dwSize;
	LONG m_lImageWidth;
	LONG m_lImageHeight;
	double m_dblResolutionValue;
	BYTE m_bResolutionMetric;		///< one of PSP_METRIC
	WORD m_wCompressionType;		///< one of PSPCompression
	WORD m_wBitDepth;
	WORD m_wPlaneCount;					///< must be 1
	DWORD m_dwColorCount;
	BYTE m_bGrayScaleFlag;
	DWORD m_dwTotalImageSize;
	LONG m_lActiveLayer;
	WORD m_wLayerCount;
	DWORD m_dwGraphicContents;	///< Flag field (@see PSPGraphicContents)
	// Expansion field.
};

struct TCompositeImageBankInformationChunk
{
	DWORD m_dwSize;
	DWORD m_dwImageCount;
	// Expansion field.
};

struct TCompositeImageAttributesInformationChunk
{
	DWORD m_dwSize;
	LONG m_lWidth;
	LONG m_lHeight;
	WORD m_wBitDepth;
	WORD m_wCompressionType;		///< one of @see PSPCompression
	WORD m_wPlaneCount;
	DWORD m_dwColorCount;
	WORD m_wCompositeImageType; ///< one of @see PSPCompositeImageType
	// Expansion field.
};

struct TLayerInformationChunkHeader
{
	DWORD m_dwSize;
	// WORD m_wCharacterCount; ///< Length of following string (the string does NOT contain the end NULL char)
	// char m_LayerName[m_wCharacterCount];
};

struct TLayerInformationChunk
{
	BYTE m_bLayerType;	///< one of PSPLayerType
	RECT m_rectImage;
	RECT m_rectSavedImage;
	BYTE m_bLayerOpacity;
	BYTE m_bBlendingMode;
	BYTE m_bLayerFlags;	///< one of @see PSPLayerProperties
	BYTE m_bTransparencyProtectedFlag;
	BYTE m_bLinkGroupId;
	RECT m_rectMask;
	RECT m_rectSavedMask;
	BYTE m_bMaskLinkedFlag;
	BYTE m_bMaskDisabledFlag;
	BYTE m_bInvertMaskOnBlendFlag;
	WORD m_wBlendRangeCount;
	BYTE m_bSourceBlendRange1[4];
	BYTE m_bDestinationBlendRange1[4];
	BYTE m_bSourceBlendRange2[4];
	BYTE m_bDestinationBlendRange2[4];
	BYTE m_bSourceBlendRange3[4];
	BYTE m_bDestinationBlendRange3[4];
	BYTE m_bSourceBlendRange4[4];
	BYTE m_bDestinationBlendRange4[4];
	BYTE m_bSourceBlendRange5[4];
	BYTE m_bDestinationBlendRange5[4];
	// Expansion field.
};

struct TLayerBitmapChunk
{
	DWORD m_dwSize;
	WORD m_wBitmapCount;
	WORD m_wChannelCount;
	// Expansion field.
};

struct TChannelInformationChunk
{
	DWORD m_dwSize;
	DWORD m_dwCompressedLength;
	DWORD m_dwUncompressedLength;
	WORD m_wBitmapType;
	WORD m_wChannelType;
	// Expansion field.
};

#pragma pack(pop)


/**	Reads a PSP string from stream in.
	* @return 0 if no error.
	*/
LRESULT ReadPspString(std::ifstream &in, STD_TSTRING &strLayerName)
{
#ifdef _UNICODE
#error This function does not yet support unicode..... ZZZZzzz
#endif

	WORD wLength;
	in.read((char *)&wLength, sizeof(wLength));

	strLayerName.resize(wLength);
	in.read(&strLayerName[0], wLength);

	return 0;
}

