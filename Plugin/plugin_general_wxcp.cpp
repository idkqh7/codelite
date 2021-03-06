//////////////////////////////////////////////////////////////////////
// This file was auto-generated by codelite's wxCrafter Plugin
// Do not modify this file by hand!
//////////////////////////////////////////////////////////////////////

#include "plugin_general_wxcp.h"
#include "codelite_exports.h"


// Declare the bitmap loading function
extern void wxCDEE4InitBitmapResources();

static bool bBitmapLoaded = false;


GeneralImages::GeneralImages()
    : wxImageList(16, 16, true)
{
    if ( !bBitmapLoaded ) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxCDEE4InitBitmapResources();
        bBitmapLoaded = true;
    }
    wxBitmap bmp;
    
    bmp = wxXmlResource::Get()->LoadBitmap(wxT("dbgAsm"));
    this->Add( bmp );
    m_bitmaps.insert( std::make_pair(wxT("dbgAsm"), bmp ) );
    
    bmp = wxXmlResource::Get()->LoadBitmap(wxT("tabClose"));
    this->Add( bmp );
    m_bitmaps.insert( std::make_pair(wxT("tabClose"), bmp ) );
    
    
}

GeneralImages::~GeneralImages()
{
}
