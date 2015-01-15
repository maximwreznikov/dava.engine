//==============================================================================
//
//  
//
//==============================================================================
//
//  externals:

    #include "rhi_DX9.h"
    
    #include "Debug/DVAssert.h"
    #include "FileSystem/Logger.h"
    #include "Core/Core.h"
    using DAVA::Logger;

    #include "_dx9.h"
    #include "../rhi_Type.h"

    #include <vector>


#define E_MINSPEC (-3)  // Error code for gfx-card that doesn't meet min.spec


namespace rhi
{
//==============================================================================

struct
DisplayMode
{
    unsigned        width;
    unsigned        height;
//    Texture::Format format;
    unsigned        refresh_rate;
};


D3DPRESENT_PARAMETERS       _PresentParam;
std::vector<DisplayMode>    _DisplayMode;


//------------------------------------------------------------------------------

Api
HostApi()
{
    return RHI_DX9;
}
//------------------------------------------------------------------------------

static bool
_IsValidIntelCard( unsigned vendor_id, unsigned device_id )
{
    return ( ( vendor_id == 0x8086 ) &&  // Intel Architecture

             // These guys are prehistoric :)
             
             ( device_id == 0x2572 ) ||  // 865G
             ( device_id == 0x3582 ) ||  // 855GM
             ( device_id == 0x2562 ) ||  // 845G
             ( device_id == 0x3577 ) ||  // 830M

             // These are from 2005 and later
             
             ( device_id == 0x2A02 ) ||  // GM965 Device 0 
             ( device_id == 0x2A03 ) ||  // GM965 Device 1 
             ( device_id == 0x29A2 ) ||  // G965 Device 0 
             ( device_id == 0x29A3 ) ||  // G965 Device 1 
             ( device_id == 0x27A2 ) ||  // 945GM Device 0 
             ( device_id == 0x27A6 ) ||  // 945GM Device 1 
             ( device_id == 0x2772 ) ||  // 945G Device 0 
             ( device_id == 0x2776 ) ||  // 945G Device 1 
             ( device_id == 0x2592 ) ||  // 915GM Device 0 
             ( device_id == 0x2792 ) ||  // 915GM Device 1 
             ( device_id == 0x2582 ) ||  // 915G Device 0 
             ( device_id == 0x2782 )     // 915G Device 1 
           );
}


//------------------------------------------------------------------------------

void
Initialize()
{
    _D3D9 = Direct3DCreate9( D3D_SDK_VERSION );

    if( _D3D9 )
    {
        HRESULT                 hr;
        HWND                    wnd     = (HWND)DAVA::Core::Instance()->NativeWindowHandle();
        RECT                    bound;  ::GetClientRect( wnd, &bound );
        unsigned                backbuf_width       = bound.right - bound.left;
        unsigned                backbuf_height      = bound.bottom - bound.top;
        bool                    use_vsync           = true;//(vsync)  ? (bool)(*vsync)  : false;
        D3DADAPTER_IDENTIFIER9  info                = {0};
        D3DCAPS9                caps;
        DWORD                   vertex_processing   = E_FAIL;
        
        hr = _D3D9->GetAdapterIdentifier( D3DADAPTER_DEFAULT, 0, &info );    


        // check if running on Intel card

        Logger::Info( "vendor-id : %04X  device-id : %04X\n", info.VendorId, info.DeviceId );

        hr = _D3D9->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps );
        
        if( SUCCEEDED(hr) )
        {
            if( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
            {
                vertex_processing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
            }
            else
            {
                // check vendor and device ID and enable SW vertex processing 
                // for Intel(R) Extreme Graphics cards
                
                if( SUCCEEDED(hr) ) // if GetAdapterIdentifier SUCCEEDED
                {
                    if( _IsValidIntelCard( info.VendorId, info.DeviceId ) )
                    {
                        vertex_processing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
                    }
                    else
                    {
                        // this is something else
                        vertex_processing = E_MINSPEC;
                        Logger::Error( "GPU does not meet minimum specs: Intel(R) 845G or Hardware T&L chip required\n" );
///                        return false;
                        return;
                    }
                }
            }
        }
        else
        {
            Logger::Error( "failed to get device caps:\n%s\n", D3D9ErrorText(hr) );

            if( _IsValidIntelCard( info.VendorId, info.DeviceId ) )
                vertex_processing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
        }

        if( vertex_processing == E_FAIL )
        {
            Logger::Error( "failed to identify GPU\n" );
///            return false;
            return;
        }


        // detect debug DirectX runtime
/*
        _debug_dx_runtime = false;

        HKEY    key_direct3d;

        if( ::RegOpenKeyA( HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Direct3D", &key_direct3d ) == ERROR_SUCCESS )
        {
            DWORD   type;
            DWORD   data    = 0;
            DWORD   data_sz = sizeof(data);

            if( ::RegQueryValueExA( key_direct3d, "LoadDebugRuntime", UNUSED_PARAM, &type, (BYTE*)(&data), &data_sz ) == ERROR_SUCCESS )
            {
                _debug_dx_runtime = (data == 1) ? true : false;
            }
        }
        note( "using %s DirectX runtime\n", (_debug_dx_runtime) ? "debug" : "retail" );
*/

        // create device

        // CRAP: hardcoded params

        _PresentParam.Windowed               = TRUE;
        _PresentParam.BackBufferFormat       = D3DFMT_UNKNOWN;
        _PresentParam.BackBufferWidth        = backbuf_width;
        _PresentParam.BackBufferHeight       = backbuf_height;
        _PresentParam.SwapEffect             = D3DSWAPEFFECT_DISCARD;
        _PresentParam.BackBufferCount        = 1;
        _PresentParam.EnableAutoDepthStencil = TRUE;
        _PresentParam.AutoDepthStencilFormat = D3DFMT_D16;
        _PresentParam.PresentationInterval   = (use_vsync) ? D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_IMMEDIATE;


        // TODO: check z-buf formats and create most suitable

        D3DDEVTYPE  device  = D3DDEVTYPE_HAL;
        UINT        adapter = D3DADAPTER_DEFAULT;



        // check if specified display-mode supported
        
        _D3D9_Adapter = adapter;
///        _DetectVideoModes();
/*
        if( !_PresentParam.Windowed )
        {
            bool    found = false;

            for( unsigned f=0; f<countof(_VideomodeFormat); ++f )
            {            
                D3DFORMAT   fmt = _VideomodeFormat[f];
                
                for( unsigned m=0; m<_DisplayMode.count(); ++m )
                {
                    if(     _DisplayMode[m].width == _PresentParam.BackBufferWidth 
                        &&  _DisplayMode[m].height == _PresentParam.BackBufferHeight 
                        &&  _DisplayMode[m].format == fmt
                      )
                    {
                        found = true;
                        break;
                    }
                }

                if( found )
                {
                    _PresentParam.BackBufferFormat = (D3DFORMAT)fmt;
                    break;
                }
            }

            if( !found )
            {
                Log::Error( "rhi.DX9", "invalid/unsuported display mode %ux%u\n", _PresentParam.BackBufferWidth, _PresentParam.BackBufferHeight );
///                return false;
                return;
            }
        }
*/

        // create device

        if( SUCCEEDED(hr=_D3D9->CreateDevice( adapter,
                                              device,
                                              wnd,
                                              vertex_processing, 
                                              &_PresentParam,
                                              &_D3D9_Device
                                            )
          ))
        {
            if( SUCCEEDED(_D3D9->GetAdapterIdentifier( D3DADAPTER_DEFAULT, 0, &info )) )
            {
            
                Logger::Info( "Adapter[%u]:\n  %s \"%s\"\n", adapter, info.DeviceName, info.Description );
                Logger::Info( "  Driver %u.%u.%u.%u\n", 
                              HIWORD(info.DriverVersion.HighPart),
                              LOWORD(info.DriverVersion.HighPart),
                              HIWORD(info.DriverVersion.LowPart),
                              LOWORD(info.DriverVersion.LowPart)
                            );
            }
            
        }
        else
        {
            Logger::Error( "failed to create device:\n%s\n", D3D9ErrorText(hr) );
        }
    }
    else
    {
        Logger::Error( "failed to create Direct3D object\n" );
    }
}


//------------------------------------------------------------------------------

void
Uninitialize()
{
}


//==============================================================================
} // namespace rhi

