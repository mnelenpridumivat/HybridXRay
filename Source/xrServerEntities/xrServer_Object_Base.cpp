////////////////////////////////////////////////////////////////////////////
// Module      : xrServer_Object_Base.cpp
// Created     : 19.09.2002
// Modified    : 16.07.2004
// Author      : Oles Shyshkovtsov, Alexander Maksimchuk, Victor Reutskiy and Dmitriy Iassenev
// Description : Server base object
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "xrServer_Objects.h"
#include "xrMessages.h"
#include "game_base_space.h"
#include "script_value_container_impl.h"
#include "clsid_game.h"
#include "../../xrEngine/xrISEAbstract.h"

#pragma warning(push)
#pragma warning(disable:4995)
#include <malloc.h>
#pragma warning(pop)

#ifndef AI_COMPILER
#include "object_factory.h"
#endif

#ifndef XRSEFACTORY_EXPORTS
#include "..\Editors\Public\xrEProps.h"

IPropHelper& PHelper()
{
    NODEFAULT;
#ifdef DEBUG
    return (*(IPropHelper*)0);
#endif
}

#ifdef XRGAME_EXPORTS
#include "ai_space.h"
#include "alife_simulator.h"
#endif   // #ifdef XRGAME_EXPORTS
#endif

LPCSTR script_section  = "script";
LPCSTR current_version = "current_server_entity_version";

IC u16 script_server_object_version()
{
    static bool initialized    = false;
    static u16  script_version = 0;
    if (!initialized)
    {
        initialized = true;
        if (!pSettings->section_exist(script_section) || !pSettings->line_exist(script_section, current_version))
            script_version = 0;
        script_version = pSettings->r_u16(script_section, current_version);
    }
    return (script_version);
}

////////////////////////////////////////////////////////////////////////////
// CPureServerObject
////////////////////////////////////////////////////////////////////////////
void CPureServerObject::save(IWriter& tMemoryStream) {}

void CPureServerObject::load(IReader& tFileStream) {}

void CPureServerObject::load(NET_Packet& tNetPacket) {}

void CPureServerObject::save(NET_Packet& tNetPacket) {}

////////////////////////////////////////////////////////////////////////////
// CSE_Abstract
////////////////////////////////////////////////////////////////////////////
CSE_Abstract::CSE_Abstract(LPCSTR caSection)
{
    m_editor_flags.zero();
    RespawnTime = 0;
    net_Ready   = FALSE;
    ID          = 0xffff;
    ID_Parent   = 0xffff;
    ID_Phantom  = 0xffff;
    owner       = 0;
    m_gameType.SetDefaults();
    // .s_gameid = 0;
    s_RP = 0xFE;   // Use supplied coords
    s_flags.assign(0);
    s_name         = caSection;
    s_name_replace = 0;   // xr_strdup("");
    o_Angle.set(0.f, 0.f, 0.f);
    o_Position.set(0.f, 0.f, 0.f);
    m_bALifeControl  = false;
    m_wVersion       = 0;
    m_script_version = 0;
    m_tClassID       = TEXT2CLSID(pSettings->r_string(caSection, "class"));

    // m_spawn_probability = 1.f;
    m_spawn_flags.zero();
    m_spawn_flags.set(flSpawnEnabled, TRUE);
    m_spawn_flags.set(flSpawnOnSurgeOnly, TRUE);
    m_spawn_flags.set(flSpawnSingleItemOnly, TRUE);
    m_spawn_flags.set(flSpawnIfDestroyedOnly, TRUE);
    m_spawn_flags.set(flSpawnInfiniteCount, TRUE);
    // m_max_spawn_count    = 1;
    // m_spawn_control	    = "";
    // m_spawn_count        = 0;
    // m_last_spawn_time    = 0;
    // m_next_spawn_time    = 0;
    // m_min_spawn_interval = 0;
    // m_max_spawn_interval = 0;
    m_ini_file = 0;

    if (pSettings->line_exist(caSection, "custom_data"))
    {
        pcstr const    raw_file_name = pSettings->r_string(caSection, "custom_data");
        IReader const* config        = 0;
#ifdef XRGAME_EXPORTS
        if (ai().get_alife())
            config = ai().alife().get_config(raw_file_name);
        else
#endif   // #ifdef XRGAME_EXPORTS
        {
            string_path file_name;
            FS.update_path(file_name, "$game_config$", raw_file_name);
            if (FS.exist(file_name))
                config = FS.r_open(file_name);
        }

        if (config)
        {
            int   size = config->length() * sizeof(char);
            LPSTR temp = (LPSTR)_alloca(size + 1);
            CopyMemory(temp, config->pointer(), size);
            temp[size]   = 0;
            m_ini_string = temp;

#ifdef XRGAME_EXPORTS
            if (NULL == ai().get_alife())
#endif   // #ifdef XRGAME_EXPORTS
            {
                IReader* _r = (IReader*)config;
                FS.r_close(_r);
            }
        }
        else
            Msg("! cannot open config file %s", raw_file_name);
    }

#ifndef AI_COMPILER
    m_script_clsid = object_factory().script_clsid(m_tClassID);
#endif
}

CSE_Abstract::~CSE_Abstract()
{
    xr_free(s_name_replace);
    xr_delete(m_ini_file);
}

ISE_Visual* CSE_Abstract::visual()
{
    return (0);
}

ISE_Shape* CSE_Abstract::shape()
{
    return (0);
}

ISE_Motion* CSE_Abstract::motion()
{
    return (0);
}

CInifile& CSE_Abstract::spawn_ini()
{
    if (!m_ini_file)
#pragma warning(push)
#pragma warning(disable:4238)
        m_ini_file = xr_new<CInifile>(&IReader((void*)(*(m_ini_string)), m_ini_string.size()), FS.get_path("$game_config$")->m_Path);
#pragma warning(pop)
    return (*m_ini_file);
}

void CSE_Abstract::Spawn_Write(NET_Packet& tNetPacket, BOOL bLocal)
{
    // generic
    tNetPacket.w_begin(M_SPAWN);
    tNetPacket.w_stringZ(s_name);
    tNetPacket.w_stringZ(s_name_replace ? s_name_replace : "");
    tNetPacket.w_u8(0);   // в тч тут режим был, s_gameid!
    tNetPacket.w_u8(s_RP);
    tNetPacket.w_vec3(o_Position);
    tNetPacket.w_vec3(o_Angle);
    tNetPacket.w_u16(RespawnTime);
    tNetPacket.w_u16(ID);
    tNetPacket.w_u16(ID_Parent);
    tNetPacket.w_u16(ID_Phantom);

    s_flags.set(M_SPAWN_VERSION, TRUE);
    if (bLocal)
        tNetPacket.w_u16(u16(s_flags.flags | M_SPAWN_OBJECT_LOCAL));
    else
        tNetPacket.w_u16(u16(s_flags.flags & ~(M_SPAWN_OBJECT_LOCAL | M_SPAWN_OBJECT_ASPLAYER)));

    tNetPacket.w_u16(SPAWN_VERSION);
    tNetPacket.w_u16(m_gameType.m_GameType.get());

    tNetPacket.w_u16(script_server_object_version());

    // client object custom data serialization SAVE
    u16 client_data_size = (u16)client_data.size();   // не может быть больше 256 байт
    tNetPacket.w_u16(client_data_size);
    // Msg("SERVER:saving:save:%d bytes:%d:%s",client_data_size,ID,s_name_replace ? s_name_replace : "");
    if (client_data_size > 0)
    {
        tNetPacket.w(&*client_data.begin(), client_data_size);
    }

    tNetPacket.w_u16(m_tSpawnID);
    // tNetPacket.w_float(m_spawn_probability);
    // tNetPacket.w_u32(m_spawn_flags.get());
    // tNetPacket.w_stringZ(m_spawn_control);
    // tNetPacket.w_u32(m_max_spawn_count);
    // tNetPacket.w_u64(m_min_spawn_interval);
    // tNetPacket.w_u64(m_max_spawn_interval);

#ifdef XRSEFACTORY_EXPORTS
    CScriptValueContainer::assign();
#endif

    u32 position;
    tNetPacket.w_chunk_open16(position);
    STATE_Write(tNetPacket);
    R_ASSERT3((m_tClassID == CLSID_SPECTATOR) || (u16(tNetPacket.w_tell() - position) > sizeof(u16)), "object isn't successfully saved, get your backup :(", name_replace());
    tNetPacket.w_chunk_close16(position);

    // write specific data
    /* u32 position = tNetPacket.w_tell();
    tNetPacket.w_u16(0);
    STATE_Write(tNetPacket);
    u16 size = u16(tNetPacket.w_tell() - position);
    // #ifdef XRSEFACTORY_EXPORTS
    R_ASSERT3((m_tClassID == CLSID_SPECTATOR) || (size > sizeof(size)), "object isn't successfully saved, get your backup :(", name_replace());
    // #endif
    tNetPacket.w_seek(position, &size, sizeof(u16));*/
}

static enum EGameTypes
{
    GAME_ANY                = 0,
    GAME_SINGLE             = 1,
    GAME_DEATHMATCH         = 2,
    //	GAME_CTF = 3,
    //	GAME_ASSAULT = 4,   // Team1 - assaulting, Team0 - Defending
    GAME_CS                 = 5,
    GAME_TEAMDEATHMATCH     = 6,
    GAME_ARTEFACTHUNT       = 7,
    GAME_CAPTURETHEARTEFACT = 8,

    // identifiers in range [100...254] are registered for script game type
    GAME_DUMMY              = 255   // temporary game type
};

BOOL CSE_Abstract::Spawn_Read(NET_Packet& tNetPacket)
{
    u16 dummy16;
    // generic
    Msg("\n");
    tNetPacket.r_begin(dummy16);
    R_ASSERT(M_SPAWN == dummy16);
    tNetPacket.r_stringZ(s_name);
    //Msg("CSE_Abstract::s_name [%s]", *s_name);

    string4096 temp;
    tNetPacket.r_stringZ(temp);
    //Msg("CSE_Abstract::s_name_replace [%s]", temp);
    set_name_replace(temp);
    u8 temp_gt;
    tNetPacket.r_u8(temp_gt /*s_gameid*/);
    //Msg("CSE_Abstract::s_gameid (temp_gt) [%d]", temp_gt);
    tNetPacket.r_u8(s_RP);
    //Msg("CSE_Abstract::s_RP [%d]", s_RP);
    tNetPacket.r_vec3(o_Position);
    //Msg("CSE_Abstract::o_Position [%f, %f, %f]", o_Position.x, o_Position.y, o_Position.z);
    tNetPacket.r_vec3(o_Angle);
    //Msg("CSE_Abstract::o_Angle [%f, %f, %f]", o_Angle.x, o_Angle.y, o_Angle.z);
    tNetPacket.r_u16(RespawnTime);
    //Msg("CSE_Abstract::RespawnTime [%d]", RespawnTime);
    tNetPacket.r_u16(ID);
    //Msg("CSE_Abstract::ID [%d]", ID);
    tNetPacket.r_u16(ID_Parent);
    //Msg("CSE_Abstract::ID_Parent [%d]", ID_Parent);
    tNetPacket.r_u16(ID_Phantom);
    //Msg("CSE_Abstract::ID_Phantom [%d]", ID_Phantom);

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)       ((byte) & 0x80 ? '1' : '0'), ((byte) & 0x40 ? '1' : '0'), ((byte) & 0x20 ? '1' : '0'), ((byte) & 0x10 ? '1' : '0'), ((byte) & 0x08 ? '1' : '0'), ((byte) & 0x04 ? '1' : '0'), ((byte) & 0x02 ? '1' : '0'), ((byte) & 0x01 ? '1' : '0')

    tNetPacket.r_u16(s_flags.flags);
    //Msg("CSE_Abstract::s_flags.flags [" BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN "]", BYTE_TO_BINARY(s_flags.flags >> 8), BYTE_TO_BINARY(s_flags.flags));

    // dangerous!!!!!!!!!
    if (s_flags.is(M_SPAWN_VERSION))
    {
        tNetPacket.r_u16(m_wVersion);
        //Msg("CSE_Abstract::m_wVersion [" BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN "]", BYTE_TO_BINARY(m_wVersion >> 8), BYTE_TO_BINARY(m_wVersion));
    }

    //Msg("check m_wVersion > [" BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN "]", BYTE_TO_BINARY(120 >> 8), BYTE_TO_BINARY(120));
    
    if (m_wVersion > 120)
    {
        //Msg("OK");
        u16 gt;
        tNetPacket.r_u16(gt);
        //Msg("CSE_Abstract::m_gameType.m_GameType [%d]", gt);
        m_gameType.m_GameType.assign(gt);
    }
    else
    {
        //Msg("Else");
        m_gameType.SetDefaults();
    }

    if (0 == m_wVersion)
    {
        tNetPacket.r_pos -= sizeof(u16);
        m_wVersion = 0;
        return FALSE;
    }

    if (m_wVersion > 69)
    {
        //Msg("m_wVersion > 69");
        m_script_version = tNetPacket.r_u16();
        //Msg("CSE_Abstract::m_script_version [%d]", m_script_version);
    }

    // read specific data

    // client object custom data serialization LOAD
    if (m_wVersion > 70)
    {
        u16 client_data_size;
        if (m_wVersion > 93)
        {
            //Msg("m_wVersion > 93");
            client_data_size = tNetPacket.r_u16();
        }
        else
        {
            //Msg("m_wVersion > 70");
            client_data_size = tNetPacket.r_u8();
        }
        //Msg("CSE_Abstract::client_data_size [%d]", client_data_size);
        //u16 client_data_size = (m_wVersion > 93) ? tNetPacket.r_u16() : tNetPacket.r_u8();   // не может быть больше 256 байт
        if (client_data_size > 0)
        {
            // Msg("SERVER:loading:load:%d bytes:%d:%s",client_data_size,ID,s_name_replace ? s_name_replace : "");
            client_data.resize(client_data_size);
            tNetPacket.r(&*client_data.begin(), client_data_size);
            //Msg("CSE_Abstract::client_data read [%d] bytes", client_data_size);
        }
        else
            client_data.clear();
    }
    else
        client_data.clear();

    if (m_wVersion > 79)
    {
        //Msg("m_wVersion > 79");
        tNetPacket.r_u16(m_tSpawnID);
        //Msg("CSE_Abstract::m_tSpawnID [%d]", m_tSpawnID);
    }

    if (m_wVersion < 112)
    {
        //Msg("m_wVersion < 112");
        if (m_wVersion > 82)
        {
            //Msg("m_wVersion > 82");
            tNetPacket.r_float();   // m_spawn_probability);
        }

        if (m_wVersion > 83)
        {
            //Msg("m_wVersion > 83");
            tNetPacket.r_u32();   // m_spawn_flags.assign(tNetPacket.r_u32());
            //Msg("read r_u32");
            xr_string temp;
            tNetPacket.r_stringZ(temp);   // tNetPacket.r_stringZ(m_spawn_control);
            //Msg("read r_stringZ");
            tNetPacket.r_u32();           // m_max_spawn_count);
            //Msg("read r_u32");
            // this stuff we do not need even in case of uncomment
            tNetPacket.r_u32();   // m_spawn_count);
            //Msg("read r_u32");
            tNetPacket.r_u64();   // m_last_spawn_time);
            //Msg("read r_u64");
        }

        if (m_wVersion > 84)
        {
            //Msg("m_wVersion > 84");
            tNetPacket.r_u64();   // m_min_spawn_interval);
            //Msg("read r_u64");
            tNetPacket.r_u64();   // m_max_spawn_interval);
            //Msg("read r_u64");
        }
    }

    if (tNetPacket.token_allowed())
    {
        //Msg("check token [token_chunk16]");
        tNetPacket.r_check_token(NET_Packet_token_type::token_chunk16);
    }
    u16 size;
    tNetPacket.r_u16(size);   // size 
    //Msg("CSE_Abstract::size [%d]", size);
    bool b1 = (m_tClassID == CLSID_SPECTATOR);
    //Msg("CSE_Abstract::m_tClassID [%d]", m_tClassID);
    bool b2 = (size > sizeof(size)) || (tNetPacket.inistream != NULL);
    //Msg("size > sizeof(size) [%d] [%d]", size, sizeof(size));
    R_ASSERT3((b1 || b2), "cannot read object, which is not successfully saved :(", name_replace());
    STATE_Read(tNetPacket, size);
    return TRUE;
}

#undef BYTE_TO_BINARY_PATTERN
#undef BYTE_TO_BINARY(byte)

void CSE_Abstract::load(NET_Packet& tNetPacket)
{
    CPureServerObject::load(tNetPacket);
    u16 client_data_size = (m_wVersion > 93) ? tNetPacket.r_u16() : tNetPacket.r_u8();   // не может быть больше 256 байт
    if (client_data_size > 0)
    {
#ifdef DEBUG
        // Msg("SERVER:loading:load:%d bytes:%d:%s",client_data_size,ID,s_name_replace ? s_name_replace : "");
#endif   // DEBUG
        client_data.resize(client_data_size);
        tNetPacket.r(&*client_data.begin(), client_data_size);
    }
    else
    {
#ifdef DEBUG
        if (!client_data.empty())
            Msg("CSE_Abstract::load: client_data is cleared for [%d][%s]", ID, name_replace());
#endif   // DEBUG
        client_data.clear();
    }
}

CSE_Abstract* CSE_Abstract::base()
{
    return (this);
}

const CSE_Abstract* CSE_Abstract::base() const
{
    return (this);
}

CSE_Abstract* CSE_Abstract::init()
{
    return (this);
}

LPCSTR CSE_Abstract::name() const
{
    return (*s_name);
}

LPCSTR CSE_Abstract::name_replace() const
{
    return (s_name_replace);
}

Fvector& CSE_Abstract::position()
{
    return (o_Position);
}

Fvector& CSE_Abstract::angle()
{
    return (o_Angle);
}

Flags16& CSE_Abstract::flags()
{
    return (s_flags);
}

xr_token game_types[] = {{"any_game", eGameIDNoGame}, {"single", eGameIDSingle}, {"deathmatch", eGameIDDeathmatch}, {"team_deathmatch", eGameIDTeamDeathmatch}, {"artefacthunt", eGameIDArtefactHunt}, {"capture_the_artefact", eGameIDCaptureTheArtefact},
    // eGameIDDominationZone
    // eGameIDTeamDominationZone
    {0, 0}};

#ifndef XRGAME_EXPORTS
void CSE_Abstract::FillProps(LPCSTR pref, PropItemVec& items)
{
#ifdef XRSEFACTORY_EXPORTS
    CreatePropsForGameTypeChooser(&m_gameType, pref, items);
    PHelper().CreateU16(items, PrepareKey(pref, "Respawn Time (s)"), &RespawnTime, 0, 43200);
#endif   // XRSEFACTORY_EXPORTS
}

void CSE_Abstract::FillProp(LPCSTR pref, PropItemVec& items)
{
    CScriptValueContainer::assign();
    CScriptValueContainer::clear();
    FillProps(pref, items);
}
#endif   // #ifndef XRGAME_EXPORTS

bool CSE_Abstract::validate()
{
    return (true);
}

/**
void CSE_Abstract::save_update(NET_Packet &tNetPacket)
{
    tNetPacket.w(&m_spawn_count,sizeof(m_spawn_count));
    tNetPacket.w(&m_last_spawn_time,sizeof(m_last_spawn_time));
    tNetPacket.w(&m_next_spawn_time,sizeof(m_next_spawn_time));
}

void CSE_Abstract::load_update(NET_Packet &tNetPacket)
{
    tNetPacket.r(&m_spawn_count,sizeof(m_spawn_count));
    tNetPacket.r(&m_last_spawn_time,sizeof(m_last_spawn_time));
    tNetPacket.r(&m_next_spawn_time,sizeof(m_next_spawn_time));
}
/**/

void CSE_Abstract::UPDATE_Read(NET_Packet& tNetPacket) {}

void CSE_Abstract::UPDATE_Write(NET_Packet& tNetPacket) {}
