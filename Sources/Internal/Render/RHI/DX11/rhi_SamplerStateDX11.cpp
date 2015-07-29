
    #include "../Common/rhi_Private.h"
    #include "../Common/rhi_Pool.h"
    #include "rhi_DX11.h"

    #include "Debug/DVAssert.h"
    #include "FileSystem/Logger.h"
    using DAVA::Logger;

    #include "_dx11.h"


namespace rhi
{
//==============================================================================

struct
SamplerStateDX11_t
{
    uint32              fragmentSamplerCount;
    ID3D11SamplerState* fragmentSampler[MAX_FRAGMENT_TEXTURE_SAMPLER_COUNT];
                        
                        SamplerStateDX11_t()
                          : fragmentSamplerCount(0)
                        {}
};

typedef ResourcePool<SamplerStateDX11_t,RESOURCE_SAMPLER_STATE>  SamplerStateDX11Pool;
RHI_IMPL_POOL(SamplerStateDX11_t,RESOURCE_SAMPLER_STATE);


//------------------------------------------------------------------------------

static DWORD
_AddrMode( TextureAddrMode mode )
{
    DWORD   m = D3DTADDRESS_WRAP;

    switch( mode )
    {
        case TEXADDR_WRAP   : m = D3DTADDRESS_WRAP; break;
        case TEXADDR_CLAMP  : m = D3DTADDRESS_CLAMP; break;
        case TEXADDR_MIRROR : m = D3DTADDRESS_MIRROR; break;
    }

    return m;
}


//------------------------------------------------------------------------------

static D3D11_FILTER
_TextureFilterDX11( TextureFilter min_filter, TextureFilter mag_filter, TextureMipFilter mip_filter )
{
    D3D11_FILTER    f = D3D11_FILTER_MIN_MAG_MIP_POINT;

    switch( mip_filter )
    {
        case TEXMIPFILTER_NONE :
        case TEXMIPFILTER_NEAREST :
        {
            if( min_filter == TEXFILTER_NEAREST  &&  mag_filter == TEXFILTER_NEAREST )
                f = D3D11_FILTER_MIN_MAG_MIP_POINT;
            else if( min_filter == TEXFILTER_NEAREST  &&  mag_filter == TEXFILTER_LINEAR )
                f = D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
            else if( min_filter == TEXFILTER_LINEAR  &&  mag_filter == TEXFILTER_NEAREST )            
                f = D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
            else if( min_filter == TEXFILTER_LINEAR  &&  mag_filter == TEXFILTER_LINEAR )  
                f = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
        }   break;

        case TEXMIPFILTER_LINEAR :
        {
            if( min_filter == TEXFILTER_NEAREST  &&  mag_filter == TEXFILTER_NEAREST )
                f = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
            else if( min_filter == TEXFILTER_NEAREST  &&  mag_filter == TEXFILTER_LINEAR )
                f = D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
            else if( min_filter == TEXFILTER_LINEAR  &&  mag_filter == TEXFILTER_NEAREST )            
                f = D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
            else if( min_filter == TEXFILTER_LINEAR  &&  mag_filter == TEXFILTER_LINEAR )  
                f = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        }   break;
    }

    return f;
}


//------------------------------------------------------------------------------

D3D11_TEXTURE_ADDRESS_MODE
_TextureAddrModeDX11( TextureAddrMode mode )
{
    D3D11_TEXTURE_ADDRESS_MODE m = D3D11_TEXTURE_ADDRESS_WRAP;

    switch( mode )
    {
        case TEXADDR_WRAP   : m = D3D11_TEXTURE_ADDRESS_WRAP; break;
        case TEXADDR_CLAMP  : m = D3D11_TEXTURE_ADDRESS_CLAMP; break;
        case TEXADDR_MIRROR : m = D3D11_TEXTURE_ADDRESS_MIRROR; break;
    }
    
    return m;
}


//------------------------------------------------------------------------------

static void
dx11_SamplerState_Delete( Handle hstate )
{
    SamplerStateDX11_t* state  = SamplerStateDX11Pool::Get( hstate );

    if( state )
    {
        DX11Command cmd[MAX_FRAGMENT_TEXTURE_SAMPLER_COUNT];
        uint32      cmdCount = 0;

        for( unsigned s=0; s!=state->fragmentSamplerCount; ++s )    
        {
            if( state->fragmentSampler[s] )
            {
                cmd[cmdCount].func   = DX11Command::RELEASE;
                cmd[cmdCount].arg[0] = uint64(state->fragmentSampler);
                ++cmdCount;
            }
        }

        ExecDX11( cmd, cmdCount );
        SamplerStateDX11Pool::Free( hstate );
    }    
}


//------------------------------------------------------------------------------

static Handle
dx11_SamplerState_Create( const SamplerState::Descriptor& desc )
{
    Handle              handle = SamplerStateDX11Pool::Alloc();
    SamplerStateDX11_t* state  = SamplerStateDX11Pool::Get( handle );
    
    D3D11_SAMPLER_DESC  s_desc[MAX_FRAGMENT_TEXTURE_SAMPLER_COUNT];
    DX11Command         cmd[MAX_FRAGMENT_TEXTURE_SAMPLER_COUNT];

    memset( state->fragmentSampler, 0, sizeof(state->fragmentSampler) );
    
    state->fragmentSamplerCount = desc.fragmentSamplerCount;
    for( unsigned s=0; s!=desc.fragmentSamplerCount; ++s )    
    {        
        s_desc[s].Filter        = _TextureFilterDX11( TextureFilter(desc.fragmentSampler[s].minFilter), TextureFilter(desc.fragmentSampler[s].magFilter), TextureMipFilter(desc.fragmentSampler[s].mipFilter) );
        s_desc[s].AddressU      = _TextureAddrModeDX11( TextureAddrMode(desc.fragmentSampler[s].addrU) );
        s_desc[s].AddressV      = _TextureAddrModeDX11( TextureAddrMode(desc.fragmentSampler[s].addrV) );
        s_desc[s].AddressW      = _TextureAddrModeDX11( TextureAddrMode(desc.fragmentSampler[s].addrW) );
        s_desc[s].MipLODBias    = 0;
        s_desc[s].MaxAnisotropy = 0;
        s_desc[s].MinLOD        = -D3D11_FLOAT32_MAX;
        s_desc[s].MaxLOD        = D3D11_FLOAT32_MAX;

        cmd[s].func = DX11Command::CREATE_SAMPLER;
        cmd[s].arg[0] = (uint64)(s_desc+s);
        cmd[s].arg[1] = (uint64)(state->fragmentSampler+s);
    }

    ExecDX11( cmd, desc.fragmentSamplerCount );

    bool    success = true;

    for( unsigned s=0; s!=desc.fragmentSamplerCount; ++s )    
    {
        if( FAILED(cmd[s].retval) )
        {
            state->fragmentSampler[s] = nullptr;
            success = false;
        }
    }

    if( !success )
    {
        dx11_SamplerState_Delete( handle );
        handle = InvalidHandle;
    }

    return handle;
}


//==============================================================================

namespace SamplerStateDX11
{

void
SetupDispatch( Dispatch* dispatch )
{
    dispatch->impl_SamplerState_Create = &dx11_SamplerState_Create;
    dispatch->impl_SamplerState_Delete = &dx11_SamplerState_Delete;
}

void
SetToRHI( Handle hstate )
{
    SamplerStateDX11_t* state = SamplerStateDX11Pool::Get( hstate );
    
      _D3D11_ImmediateContext->PSSetSamplers( 0, state->fragmentSamplerCount, state->fragmentSampler );
}

}



//==============================================================================
} // namespace rhi

