#include "stdafx.h"
#pragma hdrstop
#include "NET_utils.h"

NET_Packet_type NET_Packet::packet_type = NET_Packet_type::type_with_tokens;

void     NET_Packet::w_token(NET_Packet_token_type token)
{
    W_guard g(&w_allow);
    w(&token, sizeof(NET_Packet_token_type));
}

void NET_Packet::w_double(double a, bool write_token)
{
    W_guard g(&w_allow);
    if (token_allowed() && write_token)
    {
        w_token(NET_Packet_token_type::token_double);
    }
    w(&a, sizeof(double));
    INI_W(w_double(a));
}

void NET_Packet::w_float(float a, bool write_token)
{
    W_guard g(&w_allow);
    if (token_allowed() && write_token)
    {
        w_token(NET_Packet_token_type::token_float);
    }
    w(&a, 4);
    INI_W(w_float(a));
}

void NET_Packet::w_vec3(const Fvector& a, bool write_token)
{
    W_guard g(&w_allow);
    if (token_allowed() && write_token)
    {
        w_token(NET_Packet_token_type::token_vec3);
    }
    w(&a, 3 * sizeof(float));
    INI_W(w_vec3(a));
}

void NET_Packet::w_vec4(const Fvector4& a, bool write_token)
{
    W_guard g(&w_allow);
    if (token_allowed() && write_token)
    {
        w_token(NET_Packet_token_type::token_vec4);
    }
    w(&a, 4 * sizeof(float));
    INI_W(w_vec4(a));
}

void NET_Packet::w_u64(u64 a, bool write_token)
{
    W_guard g(&w_allow);
    if (token_allowed() && write_token)
    {
        w_token(NET_Packet_token_type::token_u64);
    }
    w(&a, 8);
    INI_W(w_u64(a));
}

void NET_Packet::w_s64(s64 a, bool write_token)
{
    W_guard g(&w_allow);
    if (token_allowed() && write_token)
    {
        w_token(NET_Packet_token_type::token_s64);
    }
    w(&a, 8);
    INI_W(w_s64(a));
}

void NET_Packet::w_u32(u32 a, bool write_token)
{
    W_guard g(&w_allow);
    if (token_allowed() && write_token)
    {
        w_token(NET_Packet_token_type::token_u32);
    }
    w(&a, 4);
    INI_W(w_u32(a));
}

void NET_Packet::w_s32(s32 a, bool write_token)
{
    W_guard g(&w_allow);
    if (token_allowed() && write_token)
    {
        w_token(NET_Packet_token_type::token_s32);
    }
    w(&a, 4);
    INI_W(w_s32(a));
}

void NET_Packet::w_u16(u16 a, bool write_token)
{
    W_guard g(&w_allow);
    if (token_allowed() && write_token)
    {
        w_token(NET_Packet_token_type::token_u16);
    }
    w(&a, 2);
    INI_W(w_u16(a));
}

void NET_Packet::w_s16(s16 a, bool write_token)
{
    W_guard g(&w_allow);
    if (token_allowed() && write_token)
    {
        w_token(NET_Packet_token_type::token_s16);
    }
    w(&a, 2);
    INI_W(w_s16(a));
}

void NET_Packet::w_u8(u8 a, bool write_token)
{
    W_guard g(&w_allow);
    if (token_allowed() && write_token)
    {
        w_token(NET_Packet_token_type::token_u8);
    }
    w(&a, 1);
    INI_W(w_u8(a));
}

void NET_Packet::w_s8(s8 a, bool write_token)
{
    W_guard g(&w_allow);
    if (token_allowed() && write_token)
    {
        w_token(NET_Packet_token_type::token_s8);
    }
    w(&a, 1);
    INI_W(w_s8(a));
}

void NET_Packet::w_float_q16(float a, float min, float max, bool write_token)
{
    if (token_allowed() && write_token)
    {
        w_token(NET_Packet_token_type::token_float_q16);
    }
    VERIFY(a >= min && a <= max);
    float q = (a - min) / (max - min);
    w_u16(u16(iFloor(q * 65535.f + 0.5f)));
}

void NET_Packet::w_float_q8(float a, float min, float max, bool write_token)
{
    if (token_allowed() && write_token)
    {
        w_token(NET_Packet_token_type::token_float_q8);
    }
    VERIFY(a >= min && a <= max);
    float q = (a - min) / (max - min);
    w_u8(u8(iFloor(q * 255.f + 0.5f)));
}

void NET_Packet::w_angle16(float a, bool write_token)
{
    if (token_allowed() && write_token)
    {
        w_token(NET_Packet_token_type::token_angle16);
    }
    w_float_q16(angle_normalize(a), 0, PI_MUL_2);
}

void NET_Packet::w_angle8(float a, bool write_token)
{
    if (token_allowed() && write_token)
    {
        w_token(NET_Packet_token_type::token_angle8);
    }
    w_float_q8(angle_normalize(a), 0, PI_MUL_2);
}

void NET_Packet::w_dir(const Fvector& D, bool write_token)
{
    if (token_allowed() && write_token)
    {
        w_token(NET_Packet_token_type::token_dir);
    }
    w_u16(pvCompress(D));
}

void NET_Packet::w_sdir(const Fvector& D, bool write_token)
{
    if (token_allowed() && write_token)
    {
        w_token(NET_Packet_token_type::token_sdir);
    }
    Fvector C;
    float   mag = D.magnitude();
    if (mag > EPS_S)
    {
        C.div(D, mag);
    }
    else
    {
        C.set(0, 0, 1);
        mag = 0;
    }
    w_dir(C);
    w_float(mag);
}

void NET_Packet::w_stringZ(LPCSTR S, bool write_token)
{
    if (token_allowed() && write_token)
    {
        w_token(NET_Packet_token_type::token_stringZ);
    }
    W_guard g(&w_allow);
    w(S, (u32)xr_strlen(S) + 1);
    INI_W(w_stringZ(S));
}

void NET_Packet::w_stringZ(const shared_str& p, bool write_token)
{
    if (token_allowed() && write_token)
    {
        w_token(NET_Packet_token_type::token_stringZ);
    }
    W_guard g(&w_allow);
    if (*p)
        w(*p, p.size() + 1);
    else
    {
        IIniFileStream* tmp = inistream;
        inistream           = NULL;
        w_u8(0);
        inistream = tmp;   // hack -(
    }

    INI_W(w_stringZ(p.c_str()));
}

void NET_Packet::w_matrix(Fmatrix& M, bool write_token)
{
    if (token_allowed() && write_token)
    {
        w_token(NET_Packet_token_type::token_matrix);
    }
    w_vec3(M.i);
    w_vec3(M.j);
    w_vec3(M.k);
    w_vec3(M.c);
}

void NET_Packet::w_clientID(ClientID& C, bool write_token)
{
    if (token_allowed() && write_token)
    {
        w_token(NET_Packet_token_type::token_clientID);
    }
    w_u32(C.value());
}

void NET_Packet::w_chunk_open8(u32& position, bool write_token)
{
    if (token_allowed() && write_token)
    {
        w_token(NET_Packet_token_type::token_chunk8);
    }
    position = w_tell();
    w_u8(0);
    INI_ASSERT(w_chunk_open8)
}

void NET_Packet::w_chunk_close8(u32 position)
{
    u32 size = u32(w_tell() - position) - sizeof(u8);
    if (token_allowed())
    {
        size -= sizeof(NET_Packet_token_type);
    }
    VERIFY(size < 256);
    u8 _size = (u8)size;
    if (token_allowed())
    {
        position += sizeof(NET_Packet_token_type);
    }
    w_seek(position, &_size, sizeof(_size));
    INI_ASSERT(w_chunk_close8)
}

void NET_Packet::w_chunk_open16(u32& position, bool write_token)
{
    if (token_allowed() && write_token)
    {
        w_token(NET_Packet_token_type::token_chunk16);
    }
    position = w_tell();
    w_u16(0);
    INI_ASSERT(w_chunk_open16)
}

void NET_Packet::w_chunk_close16(u32 position)
{
    u32 size = u32(w_tell() - position) - sizeof(u16);
    if (token_allowed())
    {
        size -= sizeof(NET_Packet_token_type);
    }
    VERIFY(size < 65536);
    u16 _size = (u16)size;
    if (token_allowed())
    {
        position += sizeof(NET_Packet_token_type);
    }
    w_seek(position, &_size, sizeof(_size));
    INI_ASSERT(w_chunk_close16)
}

// ---NET_Packet
// reading
void NET_Packet::read_start()
{
    r_pos = 0;
    INI_W(move_begin());
}

u32 NET_Packet::r_begin(u16& type)   // returns time of receiving
{
    r_pos = 0;
    if (!inistream)
        r_u16(type);
    else
        inistream->r_u16(type);

    return timeReceive;
}


void NET_Packet::write_start()
{
    B.count = 0;
    INI_W(move_begin());
}

void NET_Packet::w_begin(u16 type)
{
    B.count = 0;
    w_u16(type);
}

void NET_Packet::w(const void* p, u32 count)
{
    R_ASSERT(inistream == NULL || w_allow);
    VERIFY(p && count);
    VERIFY(B.count + count < NET_PacketSizeLimit);
    CopyMemory(&B.data[B.count], p, count);
    B.count += count;
    VERIFY(B.count < NET_PacketSizeLimit);
}

void NET_Packet::w_seek(u32 pos, const void* p, u32 count)
{
    VERIFY(p && count && (pos + count <= B.count));
    CopyMemory(&B.data[pos], p, count);
    //.	INI_ASSERT		(w_seek)
}

u32 NET_Packet::w_tell()
{
    return B.count;
}

void NET_Packet::r_seek(u32 pos)
{
    INI_ASSERT(r_seek)
    VERIFY(pos < B.count);
    r_pos = pos;
}

u32 NET_Packet::r_tell()
{
    INI_ASSERT(r_tell)
    return r_pos;
}

void NET_Packet::r(void* p, u32 count)
{
    R_ASSERT(inistream == NULL);
    VERIFY(p && count);
    CopyMemory(p, &B.data[r_pos], count);
    r_pos += count;
    VERIFY(r_pos <= B.count);
}

BOOL NET_Packet::r_eof()
{
    INI_ASSERT(r_eof)
    return (r_pos >= B.count);
}

u32 NET_Packet::r_elapsed()
{
    INI_ASSERT(r_elapsed)
    return (B.count - r_pos);
}

void NET_Packet::r_advance(u32 size)
{
    INI_ASSERT(r_advance)
    r_pos += size;
    VERIFY(r_pos <= B.count);
}

void NET_Packet::r_check_token(NET_Packet_token_type token)
{
    static xr_unordered_map<NET_Packet_token_type, xr_string> token_names = {
        {NET_Packet_token_type::token_double, "double"}, 
        {NET_Packet_token_type::token_float, "float"}, 
        {NET_Packet_token_type::token_vec3, "vec3"}, 
        {NET_Packet_token_type::token_vec4, "vec4"}, 
        {NET_Packet_token_type::token_u64, "u64"}, 
        {NET_Packet_token_type::token_s64, "s64"}, 
        {NET_Packet_token_type::token_u32, "u32"}, 
        {NET_Packet_token_type::token_s32, "s32"}, 
        {NET_Packet_token_type::token_u16, "u16"},
        {NET_Packet_token_type::token_s16, "s16"}, 
        {NET_Packet_token_type::token_u8, "u8"}, 
        {NET_Packet_token_type::token_s8, "s8"}, 
        {NET_Packet_token_type::token_float_q16, "float_q16"}, 
        {NET_Packet_token_type::token_float_q8, "float_q8"}, 
        {NET_Packet_token_type::token_angle16, "angle16"}, 
        {NET_Packet_token_type::token_angle8, "angle8"}, 
        {NET_Packet_token_type::token_dir, "dir"}, 
        {NET_Packet_token_type::token_sdir, "sdir"}, 
        {NET_Packet_token_type::token_stringZ, "string"},
        {NET_Packet_token_type::token_matrix, "matrix"}, 
        {NET_Packet_token_type::token_clientID, "clientID"}, 
        {NET_Packet_token_type::token_chunk8, "chunk8"}, 
        {NET_Packet_token_type::token_chunk16, "chunk16"}
    };
    NET_Packet_token_type                                     value;
    r(&value, sizeof(NET_Packet_token_type));
    if (value != token)
    {
        VERIFY(token_names.find(value) != token_names.end());
        xr_string requested_token_name = "expect token: " + token_names[token];
        xr_string got_token_name       = "got token: " + token_names[value];
        VERIFY4(value == token, "Error reading NET packet: token not matching", requested_token_name.c_str(), got_token_name.c_str());
    }
}

// reading - utilities
void NET_Packet::r_vec3(Fvector& A, bool read_token)
{
    if (!inistream && packet_type == NET_Packet_type::type_with_tokens && read_token)
    {
		r_check_token(NET_Packet_token_type::token_vec3);
	}
    if (!inistream)
    {
        r(&A, sizeof(Fvector));
    }
    else
    {
        inistream->r_vec3(A);
    }
    Msg("read vector [%f, %f, %f]", A.x, A.y, A.z);
}   // vec3

void NET_Packet::r_vec4(Fvector4& A, bool read_token)
{
    if (!inistream && packet_type == NET_Packet_type::type_with_tokens && read_token)
    {
        r_check_token(NET_Packet_token_type::token_vec4);
    }
    if (!inistream)
    {
        r(&A, sizeof(Fvector4));
    }
    else
    {
        inistream->r_vec4(A);
    }
    Msg("read vector4 [%f, %f, %f, %f]", A.x, A.y, A.z, A.w);
}   // vec4

void NET_Packet::r_double(double& A, bool read_token)
{
    if (!inistream && packet_type == NET_Packet_type::type_with_tokens && read_token)
    {
        r_check_token(NET_Packet_token_type::token_double);
    }
    if (!inistream)
    {
        r(&A, sizeof(double));
    }
    else
    {
        inistream->r_double(A);
    }
    Msg("read double [%lf]", A);
}   // double

void NET_Packet::r_float(float& A, bool read_token)
{
    if (!inistream && packet_type == NET_Packet_type::type_with_tokens && read_token)
    {
        r_check_token(NET_Packet_token_type::token_float);
    }
    if (!inistream)
    {
        r(&A, sizeof(float));
    }
    else
    {
        inistream->r_float(A);
    }
    Msg("read float [%f]", A);
}   // float

void NET_Packet::r_u64(u64& A, bool read_token)
{
    if (!inistream && packet_type == NET_Packet_type::type_with_tokens && read_token)
    {
        r_check_token(NET_Packet_token_type::token_u64);
    }
    if (!inistream)
    {
        r(&A, sizeof(u64));
    }
    else
    {
        inistream->r_u64(A);
    }
    Msg("read u64 [%llu]", A);
}   // qword (8b)

void NET_Packet::r_s64(s64& A, bool read_token)
{
    if (!inistream && packet_type == NET_Packet_type::type_with_tokens && read_token)
    {
        r_check_token(NET_Packet_token_type::token_s64);
    }
    if (!inistream)
    {
        r(&A, sizeof(s64));
    }
    else
    {
        inistream->r_s64(A);
    }
    Msg("read s64 [%lld]", A);
}   // qword (8b)

void NET_Packet::r_u32(u32& A, bool read_token)
{
    if (!inistream && packet_type == NET_Packet_type::type_with_tokens && read_token)
    {
        r_check_token(NET_Packet_token_type::token_u32);
    }
    if (!inistream)
    {
        r(&A, sizeof(u32));
    }
    else
    {
        inistream->r_u32(A);
    }
    Msg("read u32 [%lu]", A);
}   // dword (4b)

void NET_Packet::r_s32(s32& A, bool read_token)
{
    if (!inistream && packet_type == NET_Packet_type::type_with_tokens && read_token)
    {
        r_check_token(NET_Packet_token_type::token_s32);
    }
    if (!inistream)
    {
        r(&A, sizeof(s32));
    }
    else
    {
        inistream->r_s32(A);
    }
    Msg("read s32 [%ld]", A);
}   // dword (4b)

void NET_Packet::r_u16(u16& A, bool read_token)
{
    if (!inistream && packet_type == NET_Packet_type::type_with_tokens && read_token)
    {
        r_check_token(NET_Packet_token_type::token_u16);
    }
    if (!inistream)
    {
        r(&A, sizeof(u16));
    }
    else {
        inistream->r_u16(A);
    }
    Msg("read u16 [%u]", A);
}   // word (2b)

void NET_Packet::r_s16(s16& A, bool read_token)
{
    if (!inistream && packet_type == NET_Packet_type::type_with_tokens && read_token)
    {
        r_check_token(NET_Packet_token_type::token_s16);
    }
    if (!inistream)
    {
        r(&A, sizeof(s16));
    }
    else
    {
        inistream->r_s16(A);
    }
    Msg("read s16 [%d]", A);
}   // word (2b)

void NET_Packet::r_u8(u8& A, bool read_token)
{
    if (!inistream && packet_type == NET_Packet_type::type_with_tokens && read_token)
    {
        r_check_token(NET_Packet_token_type::token_u8);
    }
    if (!inistream)
    {
        r(&A, sizeof(u8));
    }
    else
    {
        inistream->r_u8(A);
    }
    Msg("read u8 [%u]", A);
}   // byte (1b)

void NET_Packet::r_s8(s8& A, bool read_token)
{
    if (!inistream && packet_type == NET_Packet_type::type_with_tokens && read_token)
    {
        r_check_token(NET_Packet_token_type::token_s8);
    }
    if (!inistream)
    {
        r(&A, sizeof(s8));
    }
    else
    {
        inistream->r_s8(A);
    }
    Msg("read s8 [%d]", A);
}   // byte (1b)

// IReader compatibility
Fvector NET_Packet::r_vec3()
{
    Fvector A;
    r_vec3(A);
    return (A);
}

Fvector4 NET_Packet::r_vec4()
{
    Fvector4 A;
    r_vec4(A);
    return (A);
}

float NET_Packet::r_float_q8(float min, float max)
{
    float A;
    r_float_q8(A, min, max);
    return A;
}

float NET_Packet::r_float_q16(float min, float max)
{
    float A;
    r_float_q16(A, min, max);
    return A;
}

double NET_Packet::r_double()
{
    double A;
    r_double(A);
    return (A);
}   // double

float NET_Packet::r_float()
{
    float A;
    r_float(A);
    return (A);
}   // float

u64 NET_Packet::r_u64()
{
    u64 A;
    r_u64(A);
    return (A);
}   // qword (8b)

s64 NET_Packet::r_s64()
{
    s64 A;
    r_s64(A);
    return (A);
}   // qword (8b)

u32 NET_Packet::r_u32()
{
    u32 A;
    r_u32(A);
    return (A);
}   // dword (4b)

s32 NET_Packet::r_s32()
{
    s32 A;
    r_s32(A);
    return (A);
}   // dword (4b)

u16 NET_Packet::r_u16()
{
    u16 A;
    r_u16(A);
    return (A);
}   // word (2b)

s16 NET_Packet::r_s16()
{
    s16 A;
    r_s16(A);
    return (A);
}   // word (2b)

u8 NET_Packet::r_u8()
{
    u8 A;
    r_u8(A);
    return (A);
}   // byte (1b)

s8 NET_Packet::r_s8()
{
    s8 A;
    r_s8(A);
    return (A);
}

void NET_Packet::r_float_q16(float& A, float min, float max, bool read_token)
{
    if (!inistream && packet_type == NET_Packet_type::type_with_tokens && read_token)
    {
        r_check_token(NET_Packet_token_type::token_float_q16);
    }
    u16 val;
    r_u16(val);
    A = (float(val) * (max - min)) / 65535.f + min;   // floating-point-error possible
    VERIFY((A >= min - EPS_S) && (A <= max + EPS_S));
}

void NET_Packet::r_float_q8(float& A, float min, float max, bool read_token)
{
    if (!inistream && packet_type == NET_Packet_type::type_with_tokens && read_token)
    {
        r_check_token(NET_Packet_token_type::token_float_q8);
    }
    u8 val;
    r_u8(val);
    A = (float(val) / 255.0001f) * (max - min) + min;   // floating-point-error possible
    VERIFY((A >= min) && (A <= max));
}

void NET_Packet::r_angle16(float& A, bool read_token)
{
    if (!inistream && packet_type == NET_Packet_type::type_with_tokens && read_token)
    {
        r_check_token(NET_Packet_token_type::token_angle16);
    }
    r_float_q16(A, 0, PI_MUL_2);
}

void NET_Packet::r_angle8(float& A, bool read_token)
{
    if (!inistream && packet_type == NET_Packet_type::type_with_tokens && read_token)
    {
        r_check_token(NET_Packet_token_type::token_angle8);
    }
    r_float_q8(A, 0, PI_MUL_2);
}

void NET_Packet::r_dir(Fvector& A, bool read_token)
{
    if (!inistream && packet_type == NET_Packet_type::type_with_tokens && read_token)
    {
        r_check_token(NET_Packet_token_type::token_dir);
    }
    u16 t;
    r_u16(t);
    pvDecompress(A, t);
}

void NET_Packet::r_sdir(Fvector& A, bool read_token)
{
    if (!inistream && packet_type == NET_Packet_type::type_with_tokens && read_token)
    {
        r_check_token(NET_Packet_token_type::token_sdir);
    }
    u16   t;
    float s;
    r_u16(t);
    r_float(s);
    pvDecompress(A, t);
    A.mul(s);
}

void NET_Packet::r_stringZ(LPSTR S, bool read_token)
{
    if (!inistream && packet_type == NET_Packet_type::type_with_tokens && read_token)
    {
        r_check_token(NET_Packet_token_type::token_stringZ);
    }
    if (!inistream)
    {
        LPCSTR data = LPCSTR(&B.data[r_pos]);
        size_t len  = xr_strlen(data);
        r(S, (u32)len + 1);
    }
    else
    {
        inistream->r_string(S, 4096);   //???
    }
    Msg("read string [%s]", S);
}

void NET_Packet::r_stringZ(xr_string& dest, bool read_token)
{
    if (!inistream && packet_type == NET_Packet_type::type_with_tokens && read_token)
    {
        r_check_token(NET_Packet_token_type::token_stringZ);
    }
    if (!inistream)
    {
        dest = LPCSTR(&B.data[r_pos]);
        r_advance(u32(dest.size() + 1));
    }
    else
    {
        string4096 buff;
        inistream->r_string(buff, sizeof(buff));
        dest = buff;
    }
    Msg("read string [%s]", dest.c_str());
}

void NET_Packet::r_stringZ(shared_str& dest, bool read_token)
{
    if (!inistream && packet_type == NET_Packet_type::type_with_tokens && read_token)
    {
        r_check_token(NET_Packet_token_type::token_stringZ);
    }
    if (!inistream)
    {
        dest = LPCSTR(&B.data[r_pos]);
        r_advance(dest.size() + 1);
    }
    else
    {
        string4096 buff;
        inistream->r_string(buff, sizeof(buff));
        dest = buff;
    }
    Msg("read string [%s]", dest.c_str());
}

void NET_Packet::skip_stringZ(bool read_token)
{
    if (!inistream && packet_type == NET_Packet_type::type_with_tokens && read_token)
    {
        r_check_token(NET_Packet_token_type::token_stringZ);
    }
    if (!inistream)
    {
        LPCSTR data = LPCSTR(&B.data[r_pos]);
        u32    len  = xr_strlen(data);
        r_advance(len + 1);
    }
    else
    {
        inistream->skip_stringZ();
    }
}

void NET_Packet::r_matrix(Fmatrix& M, bool read_token)
{
    if (!inistream && packet_type == NET_Packet_type::type_with_tokens && read_token)
    {
        r_check_token(NET_Packet_token_type::token_matrix);
    }
    r_vec3(M.i, false);
    M._14_ = 0;
    r_vec3(M.j, false);
    M._24_ = 0;
    r_vec3(M.k, false);
    M._34_ = 0;
    r_vec3(M.c, false);
    M._44_ = 1;
}

void NET_Packet::r_clientID(ClientID& C, bool read_token)
{
    if (!inistream && packet_type == NET_Packet_type::type_with_tokens && read_token)
    {
        r_check_token(NET_Packet_token_type::token_clientID);
    }
    u32 tmp;
    r_u32(tmp, false);
    C.set(tmp);
}

void NET_Packet::r_stringZ_s(LPSTR string, u32 const size, bool read_token)
{
    if (!inistream && packet_type == NET_Packet_type::type_with_tokens && read_token)
    {
        r_check_token(NET_Packet_token_type::token_stringZ);
    }
    if (inistream)
    {
        inistream->r_string(string, size);
        return;
    }

    LPCSTR data   = LPCSTR(B.data + r_pos);
    u32    length = xr_strlen(data);
    R_ASSERT2((length + 1) <= size, "buffer overrun");
    r(string, length + 1);
    Msg("read string [%s]", string);
}
