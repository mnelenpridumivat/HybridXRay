#ifndef _INCDEF_NETUTILS_H_
#define _INCDEF_NETUTILS_H_
#pragma once

#include "client_id.h"

#pragma pack(push, 1)

const u32 NET_PacketSizeLimit = 16 * 1024;

enum class NET_Packet_type : u8
{
    type_original = 0,
    type_with_tokens,
};

enum class NET_Packet_token_type : u8
{
    token_double = 0,
    token_float,
    token_vec3,
    token_vec4,
    token_u64,
    token_s64,
    token_u32,
    token_s32,
    token_u16,
    token_s16,
    token_u8,
    token_s8,
    token_float_q16,
    token_float_q8,
    token_angle16,
    token_angle8,
    token_dir,
    token_sdir,
    token_stringZ,
    token_matrix,
    token_clientID,
    token_chunk8,
    token_chunk16,
};

struct XRCORE_API IIniFileStream
{
    virtual void move_begin()                                   = 0;

    virtual void w_double(double a)                   = 0;
    virtual void w_float(float a)                    = 0;
    virtual void w_vec3(const Fvector& a)            = 0;
    virtual void w_vec4(const Fvector4& a)           = 0;
    virtual void w_u64(u64 a)                        = 0;
    virtual void w_s64(s64 a)                        = 0;
    virtual void w_u32(u32 a)                        = 0;
    virtual void w_s32(s32 a)                        = 0;
    virtual void w_u16(u16 a)                        = 0;
    virtual void w_s16(s16 a)                        = 0;
    virtual void w_u8(u8 a)                          = 0;
    virtual void w_s8(s8 a)                          = 0;
    virtual void w_stringZ(LPCSTR S)                 = 0;

    virtual void r_vec3(Fvector&)                    = 0;
    virtual void r_vec4(Fvector4&)                              = 0;
    virtual void r_double(double&)                    = 0;
    virtual void r_float(float&)                     = 0;
    virtual void r_u8(u8&)                           = 0;
    virtual void r_u16(u16&)                         = 0;
    virtual void r_u32(u32&)                         = 0;
    virtual void r_u64(u64&)                         = 0;
    virtual void r_s8(s8&)                           = 0;
    virtual void r_s16(s16&)                         = 0;
    virtual void r_s32(s32&)                         = 0;
    virtual void r_s64(s64&)                         = 0;

    virtual void r_string(LPSTR dest, u32 dest_size) = 0;
    //	virtual void		r_tell			()							= 0;
    //	virtual void		r_seek			(u32 pos)					= 0;
    virtual void skip_stringZ()                      = 0;
};

#define INI_W(what_to_do)      \
    if (inistream)             \
    {                          \
        inistream->what_to_do; \
    }

#define INI_ASSERT(what_to_do)                            \
    {                                                     \
        if (inistream)                                    \
            R_ASSERT3(0, #what_to_do, "not implemented"); \
    }

struct NET_Buffer
{
    BYTE data[NET_PacketSizeLimit];
    u32  count;
};

class XRCORE_API NET_Packet
{
public:
    IIniFileStream* inistream;

    void            construct(const void* data, unsigned size)
    {
        memcpy(B.data, data, size);
        B.count = size;
    }

    NET_Buffer B;
    u32        r_pos;
    u32        timeReceive;
    bool       w_allow;
    static NET_Packet_type packet_type;   //NET_Packet_type::type_original;

public:
    NET_Packet(): inistream(NULL), w_allow(true) {}

    void write_start();
    void w_begin(u16 type);

    struct W_guard
    {
        bool* guarded;
        W_guard(bool* b): guarded(b)
        {
            *b = true;
        }
        ~W_guard()
        {
            *guarded = false;
        }
    };
    void                           w(const void* p, u32 count);
    void   w_seek(u32 pos, const void* p, u32 count);
    u32                            w_tell();

    void                           w_token(NET_Packet_token_type token);
    void                           w_double(double a, bool write_token = true);
    void                           w_float(float a, bool write_token = true);
    void                           w_vec3(const Fvector& a, bool write_token = true);
    void                           w_vec4(const Fvector4& a, bool write_token = true);
    void                           w_u64(u64 a, bool write_token = true);
    void                           w_s64(s64 a, bool write_token = true);
    void                           w_u32(u32 a, bool write_token = true);
    void                           w_s32(s32 a, bool write_token = true);
    void                           w_u16(u16 a, bool write_token = true);
    void                           w_s16(s16 a, bool write_token = true);
    void                           w_u8(u8 a, bool write_token = true);
    void                           w_s8(s8 a, bool write_token = true);
    void                           w_float_q16(float a, float min, float max, bool write_token = true);
    void                           w_float_q8(float a, float min, float max, bool write_token = true);
    void                           w_angle16(float a, bool write_token = true);
    void                           w_angle8(float a, bool write_token = true);
    void                           w_dir(const Fvector& D, bool write_token = true);
    void                           w_sdir(const Fvector& D, bool write_token = true);
    void                           w_stringZ(LPCSTR S, bool write_token = true);
    void                           w_stringZ(const shared_str& p, bool write_token = true);
    void                           w_matrix(Fmatrix& M, bool write_token = true);
    void                           w_clientID(ClientID& C, bool write_token = true);
    void                           w_chunk_open8(u32& position, bool write_token = true);
    void                           w_chunk_close8(u32 position);
    void                           w_chunk_open16(u32& position, bool write_token = true);
    void                           w_chunk_close16(u32 position);

    // reading
    void    read_start();
    u32     r_begin(u16& type);
    void    r_seek(u32 pos);
    u32     r_tell();

    void                           r(void* p, u32 count);
    BOOL                           r_eof();
    u32                            r_elapsed();
    void                           r_advance(u32 size);

    bool                           token_allowed(){ return !inistream && packet_type == NET_Packet_type::type_with_tokens; }
    void                           r_check_token(NET_Packet_token_type token);

    // reading - utilities
    void                           r_vec3(Fvector& A, bool read_token = true);
    void                           r_vec4(Fvector4& A, bool read_token = true);
    void                           r_double(double& A, bool read_token = true);
    void                           r_float(float& A, bool read_token = true);
    void                           r_u64(u64& A, bool read_token = true);
    void                           r_s64(s64& A, bool read_token = true);
    void                           r_u32(u32& A, bool read_token = true);
    void                           r_s32(s32& A, bool read_token = true);
    void                           r_u16(u16& A, bool read_token = true);
    void                           r_s16(s16& A, bool read_token = true);
    void                           r_u8(u8& A, bool read_token = true);
    void                           r_s8(s8& A, bool read_token = true);

    // IReader compatibility
    Fvector                        r_vec3();
    Fvector4                       r_vec4();
    float                          r_float_q8(float min, float max);
    float                          r_float_q16(float min, float max);
    double                         r_double();
    float                          r_float();
    u64                            r_u64();
    s64                            r_s64();
    u32                            r_u32();
    s32                            r_s32();
    u16                            r_u16();
    s16                            r_s16();
    u8                             r_u8();
    s8                             r_s8();

    void                           r_float_q16(float& A, float min, float max, bool read_token = true);
    void                           r_float_q8(float& A, float min, float max, bool read_token = true);
    void                           r_angle16(float& A, bool read_token = true);
    void                           r_angle8(float& A, bool read_token = true);
    void                           r_dir(Fvector& A, bool read_token = true);

    void                           r_sdir(Fvector& A, bool read_token = true);
    void                           r_stringZ(LPSTR S, bool read_token = true);
    void                           r_stringZ(xr_string& dest, bool read_token = true);
    void                           r_stringZ(shared_str& dest, bool read_token = true);

    void                           skip_stringZ(bool read_token = true);

    void                           r_stringZ_s(LPSTR string, u32 size, bool read_token = true);

    template<u32 size> inline void r_stringZ_s(char (&string)[size])
    {
        r_stringZ_s(string, size);
    }

    void r_matrix(Fmatrix& M, bool read_token = true);
    void r_clientID(ClientID& C, bool read_token = true);
};

#pragma pack(pop)

#endif /*_INCDEF_NETUTILS_H_*/
