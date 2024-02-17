﻿#include "stdafx.h"
#include "game_sv_deathmatch.h"
#include "HUDmanager.h"
#include "xrserver_objects_alife_monsters.h"
#include "level.h"
#include "xrserver.h"
#include "Inventory.h"
#include "CustomZone.h"
#include "../xrEngine/igame_persistent.h"
#include "clsid_game.h"
#include "Actor.h"
#include "game_cl_base.h"
#include "xr_level_controller.h"
#include "hudItem.h"
#include "weapon.h"
#include "eatable_item_object.h"
#include "Missile.h"
#include "game_cl_base_weapon_usage_statistic.h"
#include "UIGameDM.h"
//#define DELAYED_ROUND_TIME	7000
#include "ui/UIBuyWndShared.h"
#include "../xrEngine/xr_ioconsole.h"

#define UNBUYABLESLOT 20

//-----------------------------------------------------------------
u32  g_sv_dm_dwForceRespawn         = 0;
s32  g_sv_dm_dwFragLimit            = 10;
s32  g_sv_dm_dwTimeLimit            = 0;
BOOL g_sv_dm_bDamageBlockIndicators = TRUE;
u32  g_sv_dm_dwDamageBlockTime      = 0;
BOOL g_sv_dm_bAnomaliesEnabled      = TRUE;
u32  g_sv_dm_dwAnomalySetLengthTime = 3;
BOOL g_sv_dm_bPDAHunt               = TRUE;
u32  g_sv_dm_dwWarmUp_MaxTime       = 0;
BOOL g_sv_dm_bDMIgnore_Money_OnBuy  = FALSE;
//-----------------------------------------------------------------
BOOL game_sv_Deathmatch::IsDamageBlockIndEnabled()
{
    return g_sv_dm_bDamageBlockIndicators;
};
s32 game_sv_Deathmatch::GetTimeLimit()
{
    return g_sv_dm_dwTimeLimit;
};
s32 game_sv_Deathmatch::GetFragLimit()
{
    return g_sv_dm_dwFragLimit;
};
u32 game_sv_Deathmatch::GetDMBLimit()
{
    return g_sv_dm_dwDamageBlockTime;
};
u32 game_sv_Deathmatch::GetForceRespawn()
{
    return g_sv_dm_dwForceRespawn;
};
u32 game_sv_Deathmatch::GetWarmUpTime()
{
    return g_sv_dm_dwWarmUp_MaxTime;
};
BOOL game_sv_Deathmatch::IsAnomaliesEnabled()
{
    return g_sv_dm_bAnomaliesEnabled;
};
u32 game_sv_Deathmatch::GetAnomaliesTime()
{
    return g_sv_dm_dwAnomalySetLengthTime;
};
//-----------------------------------------------------------------

game_sv_Deathmatch::game_sv_Deathmatch(): pure_relcase(&game_sv_Deathmatch::net_Relcase)
{
    m_type                   = GAME_DEATHMATCH;

    m_dwLastAnomalySetID     = 1001;
    m_dwLastAnomalyStartTime = 0;

    m_delayedRoundEnd        = false;
    m_delayedTeamEliminated  = false;
    m_dwLastAnomalyStartTime = 0;

    m_bSpectatorMode         = false;
    m_dwSM_CurViewEntity     = 0;
    m_pSM_CurViewEntity      = NULL;
    m_dwSM_LastSwitchTime    = 0;

    //-------------------------------
    m_vFreeRPoints.clear();
    m_dwLastRPoint     = u32(-1);
    //-------------------------------
    m_dwWarmUp_CurTime = 0;
    m_bInWarmUp        = false;
    //-------------------------------
};

game_sv_Deathmatch::~game_sv_Deathmatch()
{
    if (!m_AnomalySetsList.empty())
    {
        for (u32 i = 0; i < m_AnomalySetsList.size(); i++)
        {
            m_AnomalySetsList[i].clear();
        };
        m_AnomalySetsList.clear();
    };

    if (!m_AnomalyIDSetsList.empty())
    {
        for (u32 i = 0; i < m_AnomalyIDSetsList.size(); i++)
        {
            m_AnomalyIDSetsList[i].clear();
        };
        m_AnomalyIDSetsList.clear();
    };

    m_AnomalySetID.clear();
    m_vFreeRPoints.clear();
};

void game_sv_Deathmatch::Create(shared_str& options)
{
    inherited::Create(options);
    LoadTeams();
    switch_Phase(GAME_PHASE_PENDING);

    ::Random.seed(GetTickCount());
    m_CorpseList.clear();

    m_AnomaliesPermanent.clear();
    m_AnomalySetsList.clear();
    m_AnomalySetID.clear();
    LoadAnomalySets();
}

void game_sv_Deathmatch::OnRoundStart()
{
    LoadAnomalySets();
    m_delayedRoundEnd       = false;
    m_delayedTeamEliminated = false;
    pWinnigPlayerName       = "";
    m_dwLastAnomalySetID    = 1001;

    for (u32 i = 0; i < teams.size(); ++i)
    {
        teams[i].score       = 0;
        teams[i].num_targets = 0;
    };

    m_dwWarmUp_CurTime = 0;
    m_bInWarmUp        = false;
    if (!m_bFastRestart)
    {
        if (GetWarmUpTime() != 0)
        {
            m_dwWarmUp_CurTime = Level().timeServer() + GetWarmUpTime() * 1000;
            m_bInWarmUp        = true;
        }
    }
    inherited::OnRoundStart();

    if (IsAnomaliesEnabled())
        StartAnomalies();
    //-------------------------------------
    m_vFreeRPoints.clear();
    m_dwLastRPoint = u32(-1);
    m_server->ForEachClientDo(
        [&](IClient* client)
        {
            xrClientData* l_pC = static_cast<xrClientData*>(client);

            if (!l_pC || !l_pC->net_Ready || !l_pC->ps)
                return;

            game_PlayerState* ps = l_pC->ps;

            ps->clear();
            ps->pItemList.clear();
            ps->DeathTime = Device->dwTimeGlobal - 1001;

            SetPlayersDefItems(ps);
            Money_SetStart(client->ID);
            SpawnPlayer(client->ID, "spectator");
        }

    );
    //-------------------------------------

    // Respawn all players and some info
}

void game_sv_Deathmatch::OnRoundEnd()
{
    switch (Phase())
    {
        case GAME_PHASE_INPROGRESS:
        {
            m_server->ForEachClientDo(
                [&](IClient* client)
                {
                    xrClientData*     l_pC = static_cast<xrClientData*>(client);
                    game_PlayerState* ps   = l_pC->ps;
                    if (!ps)
                        return;
                    if (ps->IsSkip())
                        return;
                    this->SpawnPlayer(client->ID, "spectator");
                });
        }
        break;
    }
    inherited::OnRoundEnd();
};

void game_sv_Deathmatch::OnPlayerKillPlayer(game_PlayerState* ps_killer, game_PlayerState* ps_killed, KILL_TYPE KillType, SPECIAL_KILL_TYPE SpecialKillType, CSE_Abstract* pWeaponA)
{
    Processing_Victim(ps_killed, ps_killer);

    signal_Syncronize();

    if (!ps_killed || !ps_killer)
        return;

    KILL_RES KillRes      = GetKillResult(ps_killer, ps_killed);
    bool     CanGiveBonus = OnKillResult(KillRes, ps_killer, ps_killed);

    Game().m_WeaponUsageStatistic->OnPlayerKillPlayer(ps_killer, KillType, SpecialKillType);

    if (CanGiveBonus)
        OnGiveBonus(KillRes, ps_killer, ps_killed, KillType, SpecialKillType, pWeaponA);
}

void game_sv_Deathmatch::Processing_Victim(game_PlayerState* pVictim, game_PlayerState* pKiller)
{
    if (!pVictim)
        return;

    pVictim->setFlag(GAME_PLAYER_FLAG_VERY_VERY_DEAD);
    pVictim->m_iDeaths++;
    pVictim->m_iKillsInRowCurr = 0;
    pVictim->DeathTime         = Device->dwTimeGlobal;

    if (!pKiller)
    {
        pVictim->m_iSelfKills++;
    }

    SetPlayersDefItems(pVictim);

    Victim_Exp(pVictim);
    Game().m_WeaponUsageStatistic->OnPlayerKilled(pVictim);
};

void game_sv_Deathmatch::Victim_Exp(game_PlayerState* pVictim)
{
    Set_RankUp_Allowed(true);
    Player_AddExperience(pVictim, 0.0f);
    Set_RankUp_Allowed(false);
};

KILL_RES game_sv_Deathmatch::GetKillResult(game_PlayerState* pKiller, game_PlayerState* pVictim)
{
    if (!pKiller || !pVictim)
        return KR_NONE;

    if (pKiller == pVictim)
        return KR_SELF;
    return KR_RIVAL;
};

bool game_sv_Deathmatch::OnKillResult(KILL_RES KillResult, game_PlayerState* pKiller, game_PlayerState* pVictim)
{
    if (!pKiller || !pVictim)
        return false;
    bool        res   = true;
    TeamStruct* pTeam = GetTeamData(u8(pKiller->team));
    switch (KillResult)
    {
        case KR_NONE:
        {
            res = false;
        }
        break;
        case KR_SELF:
        {
            //.			pKiller->kills -= 1;
            pKiller->m_iSelfKills++;

            if (pTeam)
                Player_AddMoney(pKiller, pTeam->m_iM_KillSelf);

            res = false;
        }
        break;
        case KR_RIVAL:
        {
            //.			pKiller->kills += 1;
            pKiller->m_iRivalKills++;
            pKiller->m_iKillsInRowCurr++;
            pKiller->m_iKillsInRowMax = _max(pKiller->m_iKillsInRowCurr, pKiller->m_iKillsInRowMax);
            if (pTeam)
            {
                s32 ResMoney = pTeam->m_iM_KillRival;

                if (pKiller->testFlag(GAME_PLAYER_FLAG_INVINCIBLE))
                    ResMoney = s32(ResMoney * pTeam->m_fInvinsibleKillModifier);

                Player_AddMoney(pKiller, ResMoney);
            };
            res = true;
        }
        break;
        default:
        {
        }
        break;
    }
    return res;
}

void game_sv_Deathmatch::OnGiveBonus(KILL_RES KillResult, game_PlayerState* pKiller, game_PlayerState* pVictim, KILL_TYPE KillType, SPECIAL_KILL_TYPE SpecialKillType, CSE_Abstract* pWeaponA)
{
    if (!pKiller)
        return;
    switch (KillResult)
    {
        case KR_RIVAL:
        {
            switch (KillType)
            {
                case KT_HIT:
                {
                    switch (SpecialKillType)
                    {
                        case SKT_HEADSHOT:
                        {
                            Player_AddExperience(pKiller, READ_IF_EXISTS(pSettings, r_float, "mp_bonus_exp", "headshot", 0));
                            Player_AddBonusMoney(pKiller, READ_IF_EXISTS(pSettings, r_s32, "mp_bonus_money", "headshot", 0), SKT_HEADSHOT);
                        }
                        break;
                        case SKT_BACKSTAB:
                        {
                            Player_AddBonusMoney(pKiller, READ_IF_EXISTS(pSettings, r_s32, "mp_bonus_money", "backstab", 0), SKT_BACKSTAB);
                        }
                        break;
                        default:
                        {
                            if (pWeaponA)
                            {
                                switch (pWeaponA->m_tClassID)
                                {
                                    case CLSID_OBJECT_W_KNIFE:
                                    {
                                        Player_AddBonusMoney(pKiller, READ_IF_EXISTS(pSettings, r_s32, "mp_bonus_money", "knife_kill", 0), SKT_KNIFEKILL);
                                    }
                                    break;
                                };
                            };
                        }
                        break;
                    };
                }
                break;
                default:
                {
                }
                break;
            };

            if (pKiller->m_iKillsInRowCurr)
            {
                string64 tmpStr;
                sprintf_s(tmpStr, "%d_kill_in_row", pKiller->m_iKillsInRowCurr);
                Player_AddBonusMoney(pKiller, READ_IF_EXISTS(pSettings, r_s32, "mp_bonus_money", tmpStr, 0), SKT_KIR, u8(pKiller->m_iKillsInRowCurr & 0xff));
            };
        }
        break;
        default:
        {
        }
        break;
    }
}

game_PlayerState* game_sv_Deathmatch::GetWinningPlayer()
{
    game_PlayerState* res      = NULL;
    s16               MaxFrags = -10000;
    m_server->ForEachClientDo(
        [&](IClient* client)
        {
            xrClientData*     l_pC = static_cast<xrClientData*>(client);
            game_PlayerState* ps   = l_pC->ps;
            if (!ps)
                return;
            if (ps->frags() > MaxFrags)
            {
                MaxFrags = ps->frags();
                res      = ps;
            }
        });
    return res;
};

void game_sv_Deathmatch::OnPlayerScores()
{
    game_PlayerState* pWinner = GetWinningPlayer();
    if (pWinner)
    {
        pWinnigPlayerName = pWinner->getName();
        m_phase           = GAME_PHASE_PLAYER_SCORES;
        switch_Phase(m_phase);
        //OnDelayedRoundEn_d("Player Scores");
    }
};

void game_sv_Deathmatch::OnTimelimitExceed()
{
    OnDelayedRoundEnd(eRoundEnd_TimeLimit);
    OnPlayerScores();
    //OnRoundEnd();
}
void game_sv_Deathmatch::OnFraglimitExceed()
{
    OnDelayedRoundEnd(eRoundEnd_FragLimit);
    OnPlayerScores();
    //OnRoundEnd();
}

#include "UIGameDM.h"
void game_sv_Deathmatch::Update()
{
    inherited::Update();

    switch (Phase())
    {
        case GAME_PHASE_INPROGRESS:
        {
            check_for_WarmUp();

            checkForRoundEnd();

            check_InvinciblePlayers();

            check_ForceRespawn();

            check_for_Anomalies();

            if (m_bSpectatorMode)
            {
                if (!m_pSM_CurViewEntity || m_pSM_CurViewEntity->CLS_ID != CLSID_OBJECT_ACTOR || m_dwSM_LastSwitchTime < Level().timeServer())
                    SM_SwitchOnNextActivePlayer();

                CUIGameDM* GameDM = smart_cast<CUIGameDM*>(HUD().GetUI()->UIGame());
                if (GameDM)
                {
                    CObject* pObject = Level().CurrentViewEntity();
                    if (pObject && pObject->CLS_ID == CLSID_OBJECT_ACTOR)
                    {
                        string1024 Text;
                        sprintf_s(Text, "Following %s", pObject->cName().c_str());

                        GameDM->SetSpectrModeMsgCaption(Text);
                    }
                    else
                        GameDM->SetSpectrModeMsgCaption("Server works in spectator mode");
                }
            };
        }
        break;
        case GAME_PHASE_PENDING:
        {
            checkForRoundStart();
        }
        break;
        case GAME_PHASE_PLAYER_SCORES:
        {
            if (m_delayedRoundEnd && m_roundEndDelay < Device->TimerAsync())
            {
                OnRoundEnd();   //eRoundEnd_Finish
            }
        }
        break;
    }
}

INT  g_sv_Pending_Wait_Time      = 10000;
INT  g_sv_Wait_For_Players_Ready = 1;

bool game_sv_Deathmatch::checkForRoundStart()
{
    if (m_bFastRestart ||
        (AllPlayers_Ready() ||
            (
#ifdef DEBUG
                !g_sv_Wait_For_Players_Ready &&
#endif
                (((Level().timeServer() - StartTime())) > u32(g_sv_Pending_Wait_Time)))))
    {
        if (!HasMapRotation() || !SwitchToNextMap())
        {
            OnRoundStart();
        }
        else
        {
            if (!OnNextMap())
            {};
        };
        return true;
    };

    return false;
}

bool game_sv_Deathmatch::checkForTimeLimit()
{
    if (m_dwWarmUp_CurTime != 0 || m_bInWarmUp)
        return false;
    if (GetTimeLimit() && ((Level().timeServer() - StartTime())) > u32(GetTimeLimit() * 60000))
    {
        if (!HasChampion())
            return false;
        OnTimelimitExceed();
        return true;
    };
    return false;
}

bool game_sv_Deathmatch::checkForFragLimit()
{
    if (g_sv_dm_dwFragLimit)
    {
        m_server->ForEachClientDo(
            [&](IClient* client)
            {
                game_PlayerState* ps = static_cast<xrClientData*>(client)->ps;
                if (ps->frags() >= g_sv_dm_dwFragLimit)
                {
                    OnFraglimitExceed();
                    return true;
                }
            });
    }
    return false;
}

bool game_sv_Deathmatch::checkForRoundEnd()
{
    if (m_dwWarmUp_CurTime != 0 || m_bInWarmUp)
        return false;
    if (checkForTimeLimit())
        return true;

    if (checkForFragLimit())
        return true;

    return false;
}

void game_sv_Deathmatch::SM_SwitchOnNextActivePlayer()
{
    xrClientData* PossiblePlayers[32];
    u32           cnt           = get_players_count();
    u32           PPlayersCount = 0;
    int           it            = 0;
    m_server->ForEachClientDo(
        [&](IClient* client)
        {
            xrClientData*     l_pC = (xrClientData*)client;
            game_PlayerState* ps   = l_pC->ps;
            if (!l_pC->net_Ready)
                return;
            if (ps->IsSkip())
                return;

            if (ps->testFlag(GAME_PLAYER_FLAG_VERY_VERY_DEAD))
                return;
            PossiblePlayers[PPlayersCount++] = l_pC;
            it++;
        });

    CObject* pNewObject = NULL;
    if (!PPlayersCount)
    {
        xrClientData* C = (xrClientData*)m_server->GetServerClient();
        pNewObject      = Level().Objects.net_Find(C->ps->GameID);
    }
    else
    {
        xrClientData* C = NULL;
        C               = (xrClientData*)PossiblePlayers[::Random.randI((int)PPlayersCount)];
        pNewObject      = Level().Objects.net_Find(C->ps->GameID);
        CActor* pActor  = smart_cast<CActor*>(pNewObject);

        if (!pActor || !pActor->g_Alive() || !pActor->inventory().ActiveItem())
            return;
    };
    SM_SwitchOnPlayer(pNewObject);
};

#include "WeaponHUD.h"

void game_sv_Deathmatch::net_Relcase(CObject* O)
{
    if (m_pSM_CurViewEntity == O)
        m_pSM_CurViewEntity = NULL;
}

void game_sv_Deathmatch::SM_SwitchOnPlayer(CObject* pNewObject)
{
    if (!pNewObject)
        return;

    Level().SetEntity(pNewObject);

    if (pNewObject != m_pSM_CurViewEntity)
    {
        CActor* pActor = smart_cast<CActor*>(m_pSM_CurViewEntity);

        if (pActor)
            pActor->inventory().Items_SetCurrentEntityHud(false);
    }

    CActor* pActor = smart_cast<CActor*>(pNewObject);
    if (pActor)
    {
        pActor->inventory().Items_SetCurrentEntityHud(true);
        /*
		CHudItem* pHudItem = smart_cast<CHudItem*>(pActor->inventory().ActiveItem());
		if (pHudItem) 
		{
			pHudItem->OnStateSwitch(pHudItem->GetState());
		};
*/
    }

    m_pSM_CurViewEntity   = pNewObject;
    m_dwSM_CurViewEntity  = pNewObject->ID();
    m_dwSM_LastSwitchTime = Level().timeServer() + m_dwSM_SwitchDelta;
}

BOOL game_sv_Deathmatch::AllPlayers_Ready()
{
    if (!m_server->GetServerClient())
        return FALSE;
    // Check if all players ready
    u32 cnt   = get_players_count();
    u32 ready = 0;

    m_server->ForEachClientDo(
        [&](IClient* client)
        {
            xrClientData*     l_pC = (xrClientData*)client;
            game_PlayerState* ps   = l_pC->ps;
            if (!l_pC->net_Ready)
            {
                if (l_pC->ID == m_server->GetServerClient()->ID)
                {
                    return;
                }
                ++ready;
            };
            if (ps->testFlag(GAME_PLAYER_FLAG_READY))
                ++ready;
            else if (ps->IsSkip())
                ++ready;
        });

    if (ready == cnt && ready != 0)
        return TRUE;
    return FALSE;
};

void game_sv_Deathmatch::OnPlayerReady(ClientID id)
{
    switch (m_phase)
    {
        case GAME_PHASE_PENDING:
        {
            game_PlayerState* ps = get_id(id);
            if (ps)
            {
                if (ps->testFlag(GAME_PLAYER_FLAG_READY))
                {
                    ps->resetFlag(GAME_PLAYER_FLAG_READY);
                }
                else
                {
                    ps->setFlag(GAME_PLAYER_FLAG_READY);
                };
            };
            signal_Syncronize();
        }
        break;
        case GAME_PHASE_INPROGRESS:
        {
            xrClientData*     xrCData = (xrClientData*)m_server->ID_to_client(id);
            game_PlayerState* ps      = get_id(id);
            if (ps->IsSkip())
                break;

            if (!(ps->testFlag(GAME_PLAYER_FLAG_VERY_VERY_DEAD)))
                break;
            if (ps->testFlag(GAME_PLAYER_FLAG_SPECTATOR))
                break;

            xrClientData* xrSCData = (xrClientData*)m_server->GetServerClient();

            if (xrSCData && xrSCData->ID == id && m_bSpectatorMode)
            {
                SM_SwitchOnNextActivePlayer();
                return;
            }
            //------------------------------------------------------------
            CSE_Abstract*  pOwner = xrCData->owner;
            CSE_Spectator* pS     = smart_cast<CSE_Spectator*>(pOwner);
            if (pS)
            {
                if (xrSCData->ps->DeathTime + 1000 > Device->dwTimeGlobal)
                {
                    //					return;
                }
            }
            //------------------------------------------------------------
            RespawnPlayer(id, false);
            pOwner                     = xrCData->owner;
            CSE_ALifeCreatureActor* pA = smart_cast<CSE_ALifeCreatureActor*>(pOwner);
            if (pA)
            {
                SpawnWeaponsForActor(pOwner, ps);
                //---------------------------------------
                Check_ForClearRun(ps);
            }
            //-------------------------------
        }
        break;
    };
}

void game_sv_Deathmatch::OnPlayerDisconnect(ClientID id_who, LPSTR Name, u16 GameID)
{
    inherited::OnPlayerDisconnect(id_who, Name, GameID);
};

u32 game_sv_Deathmatch::RP_2_Use(CSE_Abstract* E)
{
    return 0;
};

void game_sv_Deathmatch::assign_RP(CSE_Abstract* E, game_PlayerState* ps_who)
{
    VERIFY(E);
    u32 Team = RP_2_Use(E);
    VERIFY(rpoints[Team].size());

    CSE_Spectator* pSpectator = smart_cast<CSE_Spectator*>(E);
    if (pSpectator)
    {
        inherited::assign_RP(E, ps_who);
        return;
    };

    CSE_ALifeCreatureActor* pA = smart_cast<CSE_ALifeCreatureActor*>(E);
    if (!pA)
    {
        inherited::assign_RP(E, ps_who);
        return;
    };
    //-------------------------------------------------------------------------------
    xr_vector<RPoint>&       rp = rpoints[Team];

    xr_vector<xrClientData*> pEnemies;
    xr_vector<xrClientData*> pFriends;

    m_server->ForEachClientDo(
        [&](IClient* client)
        {
            // init
            game_PlayerState* ps = static_cast<xrClientData*>(client)->ps;
            if (ps->testFlag(GAME_PLAYER_FLAG_VERY_VERY_DEAD))
                return;
            if (ps->team == pA->s_team && !teams.empty())
                pFriends.push_back(static_cast<xrClientData*>(client));
            else
                pEnemies.push_back(static_cast<xrClientData*>(client));
        });
    //-------------------------------------------------------------------------------
    if (m_vFreeRPoints.empty())
    {
        for (u32 i = 0; i < rp.size(); i++)
        {
            if (i == m_dwLastRPoint && rp.size() > 1)
                continue;
            m_vFreeRPoints.push_back(i);
        }
    };
    R_ASSERT(m_vFreeRPoints.size());
    xr_vector<RPointData> tmpPoints;
    for (u32 i = 0; i < m_vFreeRPoints.size(); i++)
    {
        RPoint& r            = rp[m_vFreeRPoints[i]];
        float   MinEnemyDist = 10000.0f;
        for (u32 e = 0; e < pEnemies.size(); e++)
        {
            xrClientData* xrCData = pEnemies[e];
            if (!xrCData || !xrCData->owner)
                continue;

            CSE_Abstract* pOwner = xrCData->owner;
            float         Dist   = r.P.distance_to_sqr(pOwner->o_Position);

            if (MinEnemyDist > Dist)
                MinEnemyDist = Dist;
        };
        tmpPoints.push_back(RPointData(i, MinEnemyDist, false));
    }
    R_ASSERT(tmpPoints.size());
    std::sort(tmpPoints.begin(), tmpPoints.end());
    u32 HalfList   = tmpPoints.size() / (pEnemies.empty() ? 1 : 2);
    u32 NewPointID = (HalfList) ? (tmpPoints.size() - HalfList + ::Random.randI(HalfList)) : 0;

    m_dwLastRPoint = m_vFreeRPoints[tmpPoints[NewPointID].PointID];
    m_vFreeRPoints.erase(m_vFreeRPoints.begin() + tmpPoints[NewPointID].PointID);

    RPoint& r = rp[m_dwLastRPoint];
    E->o_Position.set(r.P);
    E->o_Angle.set(r.A);
};

bool game_sv_Deathmatch::IsBuyableItem(LPCSTR ItemName)
{
    if (m_strWeaponsData->GetItemIdx(ItemName) == u32(-1))
        return false;
    return true;
};

void game_sv_Deathmatch::CheckItem(game_PlayerState* ps, PIItem pItem, xr_vector<s16>* pItemsDesired, xr_vector<u16>* pItemsToDelete, bool ExactMatch = false)
{
    if (!pItem || !pItemsDesired || !pItemsToDelete)
        return;

    if (m_strWeaponsData->GetItemIdx(pItem->object().cNameSect()) == u32(-1))
        return;
    //-------------------------------------------
    bool found = false;
    for (u32 it = 0; it < pItemsDesired->size(); it++)
    {
        s16 ItemID = (*pItemsDesired)[it];
        if ((ItemID & 0x00ff) != u16(m_strWeaponsData->GetItemIdx(pItem->object().cNameSect())))
            continue;

        found              = true;
        CWeaponAmmo* pAmmo = smart_cast<CWeaponAmmo*>(pItem);
        if (pAmmo)
        {
            if (pAmmo->m_boxCurr != pAmmo->m_boxSize)
                break;
        };
        //----- Check for Addon Changes ---------------------
        CWeapon* pWeapon = smart_cast<CWeapon*>(pItem);
        if (pWeapon)
        {
            u8 OldAddons = pWeapon->GetAddonsState();
            u8 NewAddons = u8((ItemID & 0xff00) >> 0x08) /*u8(ItemID&0x00ff)>>0x05*/;
            if (ExactMatch)
            {
                if (OldAddons != NewAddons)
                {
                    found = false;
                    continue;
                }
            }
            if (OldAddons != NewAddons)
            {
                CSE_ALifeItemWeapon* pSWeapon = smart_cast<CSE_ALifeItemWeapon*>(get_entity_from_eid(pWeapon->ID()));
                if (pSWeapon)
                {
                    pSWeapon->m_addon_flags.zero();
                    pSWeapon->m_addon_flags.set(NewAddons, TRUE);
                }

                NET_Packet P;
                u_EventGen(P, GE_ADDON_CHANGE, pWeapon->ID());
                P.w_u8(NewAddons);
                u_EventSend(P);
            }
        };
        //---------------------------------------------------
        pItemsDesired->erase(pItemsDesired->begin() + it);
        break;
    };
    if (found)
        return;
    pItemsToDelete->push_back(pItem->object().ID());
};

void game_sv_Deathmatch::OnPlayerBuyFinished(ClientID id_who, NET_Packet& P)
{
    game_PlayerState* ps = get_id(id_who);
    if (!ps || ps->IsSkip())
        return;

    P.r_s32(ps->LastBuyAcount);
    if (ps->LastBuyAcount != 0)
        ps->m_bClearRun = false;

    xr_vector<s16> ItemsDesired;

    u8             NumItems;
    P.r_u8(NumItems);
    for (u8 i = 0; i < NumItems; i++)
    {
        s16 ItemID;
        P.r_s16(ItemID);
        //		Msg("------------- Player wants %d", ItemID);
        ItemsDesired.push_back(ItemID);
    };

    CSE_ALifeCreatureActor* e_Actor = smart_cast<CSE_ALifeCreatureActor*>(get_entity_from_eid(ps->GameID));
    CActor*                 pActor  = smart_cast<CActor*>(Level().Objects.net_Find(ps->GameID));
    if (pActor)
    {
        PIItem                          pItem = NULL;
        xr_vector<u16>                  ItemsToDelete;

        bool                            ExactMatch = true;
        //��������� ����
        TIItemContainer::const_iterator IBelt      = pActor->inventory().m_belt.begin();
        TIItemContainer::const_iterator EBelt      = pActor->inventory().m_belt.end();

        for (; IBelt != EBelt; ++IBelt)
        {
            pItem = (*IBelt);
            CheckItem(ps, pItem, &ItemsDesired, &ItemsToDelete, ExactMatch);
        };

        //��������� ruck
        TIItemContainer::const_iterator IRuck = pActor->inventory().m_ruck.begin();
        TIItemContainer::const_iterator ERuck = pActor->inventory().m_ruck.end();

        for (; IRuck != ERuck; ++IRuck)
        {
            pItem = (*IRuck);
            if (!pItem)
                continue;
            CheckItem(ps, pItem, &ItemsDesired, &ItemsToDelete, ExactMatch);
        };

        //��������� �����
        TISlotArr::const_iterator ISlot = pActor->inventory().m_slots.begin();
        TISlotArr::const_iterator ESlot = pActor->inventory().m_slots.end();

        for (; ISlot != ESlot; ++ISlot)
        {
            pItem = (*ISlot).m_pIItem;
            CheckItem(ps, pItem, &ItemsDesired, &ItemsToDelete, ExactMatch);
        };

        xr_vector<u16>::iterator IDI = ItemsToDelete.begin();
        xr_vector<u16>::iterator EDI = ItemsToDelete.end();
        for (; IDI != EDI; ++IDI)
        {
            NET_Packet P;
            u_EventGen(P, GE_DESTROY, *IDI);
            Level().Send(P, net_flags(TRUE, TRUE));
        };
    };

    ps->pItemList.clear();
    for (u32 it = 0; it < ItemsDesired.size(); it++)
    {
        ps->pItemList.push_back(ItemsDesired[it]);
    };

    if (!pActor)
        return;

    SpawnWeaponsForActor(e_Actor, ps);
};

void game_sv_Deathmatch::SpawnWeaponsForActor(CSE_Abstract* pE, game_PlayerState* ps)
{
    CSE_ALifeCreatureActor* pA = smart_cast<CSE_ALifeCreatureActor*>(pE);
    R_ASSERT2(pA, "Owner not a Actor");
    if (!pA)
        return;

    if (!(ps->team < s16(TeamList.size())))
        return;

    for (u32 i = 0; i < ps->pItemList.size(); i++)
    {
        u16 ItemID = ps->pItemList[i];
        SpawnWeapon4Actor(pA->ID, *m_strWeaponsData->GetItemName(ItemID & 0x00FF), u8((ItemID & 0xFF00) >> 0x08));
        Game().m_WeaponUsageStatistic->OnWeaponBought(ps, *m_strWeaponsData->GetItemName(ItemID & 0x00FF));
    };
#ifndef NDEBUG
    if (!g_sv_dm_bDMIgnore_Money_OnBuy)
#endif
        Player_AddMoney(ps, ps->LastBuyAcount);
};

void game_sv_Deathmatch::LoadSkinsForTeam(const shared_str& caSection, TEAM_SKINS_NAMES* pTeamSkins)
{
    string256  SkinSingleName;
    string4096 Skins;

    // ���� strSectionName ������ ��������� ��� ������
    R_ASSERT(xr_strcmp(caSection, ""));

    pTeamSkins->clear();

    // ��� ����
    if (!pSettings->line_exist(caSection, "skins"))
        return;

    // ������ ������ ����� ����
    std::strcpy(Skins, pSettings->r_string(caSection, "skins"));
    u32 count = _GetItemCount(Skins);
    // ������ ��� ������ ��� ������, ����������� ��������, ������� � ������
    for (u32 i = 0; i < count; ++i)
    {
        _GetItem(Skins, i, SkinSingleName);
        pTeamSkins->push_back(SkinSingleName);
    };
};

void game_sv_Deathmatch::LoadDefItemsForTeam(const shared_str& caSection, DEF_ITEMS_LIST* pDefItems)
{
    string256  ItemName;
    string4096 DefItems;

    // ���� strSectionName ������ ��������� ��� ������
    R_ASSERT(xr_strcmp(caSection, ""));

    pDefItems->clear();

    // ��� ����
    if (!pSettings->line_exist(caSection, "default_items"))
        return;

    // ������ ������ ����� ����
    std::strcpy(DefItems, pSettings->r_string(caSection, "default_items"));
    u32 count = _GetItemCount(DefItems);
    // ������ ��� ������ ��� ������, ����������� ��������, ������� � ������
    for (u32 i = 0; i < count; ++i)
    {
        _GetItem(DefItems, i, ItemName);
        pDefItems->push_back(u16(m_strWeaponsData->GetItemIdx(ItemName) & 0xffff));
    };
};

void game_sv_Deathmatch::SetSkin(CSE_Abstract* E, u16 Team, u16 ID)
{
    if (!E)
        return;
    //-------------------------------------------
    CSE_Visual* pV = smart_cast<CSE_Visual*>(E);
    if (!pV)
        return;
    //-------------------------------------------
    string256 SkinName;
    std::strcpy(SkinName, pSettings->r_string("mp_skins_path", "skin_path"));
    //��������� �� ����� ��� ���� ��������
    //	if (SkinID != -1) ID = u16(SkinID);

    if (!TeamList.empty() && TeamList.size() > Team && !TeamList[Team].aSkins.empty())
    {
        //��������� �� ���������� ������ ��� ���� ��������
        if (TeamList[Team].aSkins.size() > ID)
        {
            std::strcat(SkinName, TeamList[Team].aSkins[ID].c_str());
        }
        else
            std::strcat(SkinName, TeamList[Team].aSkins[0].c_str());
    }
    else
    {
        //����� ��� ����� �������� �� ���������
        switch (Team)
        {
            case 0:
                std::strcat(SkinName, "stalker_hood_multiplayer");
                break;
            case 1:
                std::strcat(SkinName, "soldat_beret");
                break;
            case 2:
                std::strcat(SkinName, "stalker_black_mask");
                break;
            default:
                R_ASSERT2(0, "Unknown Team");
                break;
        };
    };
    std::strcat(SkinName, ".ogf");
    //.	Msg("* Skin - %s", SkinName);
    int len = xr_strlen(SkinName);
    R_ASSERT2(len < 64, "Skin Name is too LONG!!!");
    pV->set_visual(SkinName);
    //-------------------------------------------
};

void game_sv_Deathmatch::OnPlayerHitPlayer_Case(game_PlayerState* ps_hitter, game_PlayerState* ps_hitted, SHit* pHitS)
{
    if (ps_hitted->testFlag(GAME_PLAYER_FLAG_INVINCIBLE))
    {
        pHitS->power   = 0;
        pHitS->impulse = 0;
    }
};

void game_sv_Deathmatch::OnPlayerHitPlayer(u16 id_hitter, u16 id_hitted, NET_Packet& P)
{
    CSE_Abstract* e_hitter = get_entity_from_eid(id_hitter);
    CSE_Abstract* e_hitted = get_entity_from_eid(id_hitted);

    if (!e_hitter || !e_hitted)
        return;
    if (e_hitted->m_tClassID != CLSID_OBJECT_ACTOR)
        return;
    game_PlayerState* ps_hitter = get_eid(id_hitter);
    game_PlayerState* ps_hitted = get_eid(id_hitted);

    if (!ps_hitter || !ps_hitted)
        return;

    SHit HitS;
    HitS.Read_Packet(P);

    HitS.whoID = ps_hitter->GameID;

    OnPlayerHitPlayer_Case(ps_hitter, ps_hitted, &HitS);

    //---------------------------------------
    if (HitS.power > 0)
    {
        ps_hitted->lasthitter    = ps_hitter->GameID;
        ps_hitted->lasthitweapon = HitS.weaponID;
    };
    //---------------------------------------
    HitS.Write_Packet(P);
};

void game_sv_Deathmatch::LoadTeams()
{
    m_sBaseWeaponCostSection._set("deathmatch_base_cost");
    if (!pSettings->section_exist(m_sBaseWeaponCostSection))
    {
        R_ASSERT2(0, "No section for base weapon cost for this type of the Game!");
        return;
    };

    m_strWeaponsData->Load(m_sBaseWeaponCostSection);

    LoadTeamData("deathmatch_team0");
};

s32 game_sv_Deathmatch::GetMoneyAmount(const shared_str& caSection, char* caMoneyStr)
{
    if (pSettings->line_exist(caSection, caMoneyStr))
        return pSettings->r_s32(caSection, caMoneyStr);

    return 0;
};

void game_sv_Deathmatch::LoadTeamData(const shared_str& caSection)
{
    TeamStruct NewTeam;

    NewTeam.caSection = caSection;

    //	LoadWeaponsForTeam	(caSection, &NewTeam.aWeapons);
    LoadSkinsForTeam(caSection, &NewTeam.aSkins);
    LoadDefItemsForTeam(caSection, /*&NewTeam.aWeapons, */ &NewTeam.aDefaultItems);
    //-------------------------------------------------------------
    if (pSettings->section_exist(caSection))   //money
    {
        NewTeam.m_iM_Start            = GetMoneyAmount(caSection, "money_start");
        NewTeam.m_iM_OnRespawn        = GetMoneyAmount(caSection, "money_respawn");
        NewTeam.m_iM_Min              = GetMoneyAmount(caSection, "money_min");

        NewTeam.m_iM_KillRival        = GetMoneyAmount(caSection, "kill_rival");
        NewTeam.m_iM_KillSelf         = GetMoneyAmount(caSection, "kill_self");
        NewTeam.m_iM_KillTeam         = GetMoneyAmount(caSection, "kill_team");

        NewTeam.m_iM_TargetRival      = GetMoneyAmount(caSection, "target_rival");
        NewTeam.m_iM_TargetTeam       = GetMoneyAmount(caSection, "target_team");
        NewTeam.m_iM_TargetSucceed    = GetMoneyAmount(caSection, "target_succeed");
        NewTeam.m_iM_TargetSucceedAll = GetMoneyAmount(caSection, "target_succeed_all");
        NewTeam.m_iM_TargetFailed     = GetMoneyAmount(caSection, "target_failed");

        NewTeam.m_iM_RoundWin         = GetMoneyAmount(caSection, "round_win");
        NewTeam.m_iM_RoundLoose       = GetMoneyAmount(caSection, "round_loose");
        NewTeam.m_iM_RoundDraw        = GetMoneyAmount(caSection, "round_draw");

        NewTeam.m_iM_RoundWin_Minor   = GetMoneyAmount(caSection, "round_win_minor");
        NewTeam.m_iM_RoundLoose_Minor = GetMoneyAmount(caSection, "round_loose_minor");

        NewTeam.m_iM_RivalsWipedOut   = GetMoneyAmount(caSection, "rivals_wiped_out");

        NewTeam.m_iM_ClearRunBonus    = GetMoneyAmount(caSection, "clear_run_bonus");
        //---------------------------------------------------------------------------
        if (pSettings->line_exist(caSection, "kill_while_invincible"))
            NewTeam.m_fInvinsibleKillModifier = pSettings->r_float(caSection, "kill_while_invincible");
        else
            NewTeam.m_fInvinsibleKillModifier = 0.5f;
    };
    //-------------------------------------------------------------
    TeamList.push_back(NewTeam);
};

void game_sv_Deathmatch::OnDestroyObject(u16 eid_who)
{
    if (eid_who == m_dwSM_CurViewEntity && m_bSpectatorMode)
    {
        SM_SwitchOnNextActivePlayer();
    };

    for (u32 i = 0; i < m_CorpseList.size();)
    {
        if (m_CorpseList[i] == eid_who)
        {
            m_CorpseList.erase(m_CorpseList.begin() + i);
        }
        else
            i++;
    };

    CSE_Abstract* entity = get_entity_from_eid(eid_who);
    if (entity && entity->owner->ps->GameID == eid_who)
    {
        xrClientData* xrCData = entity->owner;
        //		game_PlayerState* ps = entity->owner->ps;
        if (Phase() == GAME_PHASE_INPROGRESS)
        {
            CSE_ALifeCreatureActor* A = smart_cast<CSE_ALifeCreatureActor*>(entity);
            if (A)
            {
                SpawnPlayer(xrCData->ID, "spectator");
            }
        };
    }
};

void game_sv_Deathmatch::Money_SetStart(ClientID id_who)
{
    xrClientData* C = (xrClientData*)m_server->ID_to_client(id_who);
    if (!C || (C->ID != id_who))
        return;
    game_PlayerState* ps_who = (game_PlayerState*)C->ps;
    if (!ps_who)
        return;
    ps_who->money_for_round = 0;
    if (ps_who->team < 0)
        return;
    TeamStruct* pTeamData = GetTeamData(u8(ps_who->team));
    if (!pTeamData)
        return;
    ps_who->money_for_round = pTeamData->m_iM_Start;
}

void game_sv_Deathmatch::OnTeamScore(u32 Team, bool Minor)
{
    TeamStruct* pTeam = GetTeamData(u8(Team));
    if (!pTeam)
        return;

    m_server->ForEachClientDo(
        [&](IClient* client)
        {
            // init
            xrClientData*     l_pC = (xrClientData*)client;
            game_PlayerState* ps   = l_pC->ps;
            if (!l_pC->net_Ready)
                return;
            if (ps->IsSkip())
                return;

            if (ps->team == s16(Team))
                Player_AddMoney(ps, ((Minor) ? pTeam->m_iM_RoundWin_Minor : pTeam->m_iM_RoundWin));
            else
                Player_AddMoney(ps, ((Minor) ? pTeam->m_iM_RoundLoose_Minor : pTeam->m_iM_RoundLoose));
        });
}

void game_sv_Deathmatch::net_Export_State(NET_Packet& P, ClientID id_to)
{
    inherited::net_Export_State(P, id_to);

    P.w_s32(g_sv_dm_dwFragLimit);
    P.w_s32(GetTimeLimit());
    //	P.w_u32			(GetDMBLimit());
    P.w_u32(GetForceRespawn());
    P.w_u32(m_dwWarmUp_CurTime);
    P.w_u8(u8(g_sv_dm_bDamageBlockIndicators));
    // Teams
    P.w_u16(u16(teams.size()));
    for (u32 t_it = 0; t_it < teams.size(); ++t_it)
    {
        P.w(&teams[t_it], sizeof(game_TeamState));
    }
    switch (Phase())
    {
        case GAME_PHASE_PLAYER_SCORES:
        {
            P.w_stringZ(pWinnigPlayerName);
        }
        break;
    }
}

int game_sv_Deathmatch::GetTeamScore(u32 idx)
{
    VERIFY((idx >= 0) && (idx < teams.size()));
    return teams[idx].score;
}

void game_sv_Deathmatch::OnPlayerSelectSkin(NET_Packet& P, ClientID sender)
{
    xrClientData* l_pC = m_server->ID_to_client(sender);
    s8            l_skin;
    P.r_s8(l_skin);
    OnPlayerChangeSkin(l_pC->ID, l_skin);
    //---------------------------------------
    signal_Syncronize();
    //---------------------------------------
    NET_Packet Px;
    GenerateGameMessage(Px);
    Px.w_u32(GAME_EVENT_PLAYER_GAME_MENU_RESPOND);
    Px.w_u8(PLAYER_CHANGE_SKIN);
    Px.w_s8(l_pC->ps->skin);
    m_server->SendTo(sender, Px, net_flags(TRUE, TRUE));
};

void game_sv_Deathmatch::OnPlayerChangeSkin(ClientID id_who, s8 skin)
{
    game_PlayerState* ps_who = get_id(id_who);
    if (!ps_who)
        return;
    ps_who->skin = skin;
    ps_who->resetFlag(GAME_PLAYER_FLAG_SPECTATOR);
    if (skin == ps_who->skin)
        return;
    if (skin == -1)
    {
        ps_who->skin = u8(::Random.randI((int)TeamList[ps_who->team].aSkins.size()));
    }

    KillPlayer(id_who, ps_who->GameID);
}

void game_sv_Deathmatch::SetTeamScore(u32 idx, int val)
{
    VERIFY((idx >= 0) && (idx < teams.size()));
    teams[idx].score = val;
}

void game_sv_Deathmatch::LoadAnomalySets()
{
    //-----------------------------------------------------------
    m_AnomalySetsList.clear();
    m_AnomaliesPermanent.clear();
    //-----------------------------------------------------------
    if (!m_AnomalyIDSetsList.empty())
    {
        for (u32 i = 0; i < m_AnomalyIDSetsList.size(); i++)
        {
            m_AnomalyIDSetsList[i].clear();
        };
        m_AnomalyIDSetsList.clear();
    };
    //-----------------------------------------------------------
    if (!g_pGameLevel || !Level().pLevel)
        return;

    char*        ASetBaseName = GetAnomalySetBaseName();

    string1024   SetName, AnomaliesNames, AnomalyName;
    ANOMALIES    AnomalySingleSet;
    ANOMALIES_ID AnomalyIDSingleSet;
    for (u32 i = 0; i < 20; i++)
    {
        AnomalySingleSet.clear();
        AnomalyIDSingleSet.clear();

        sprintf_s(SetName, "set%i", i);
        if (!Level().pLevel->line_exist(ASetBaseName, SetName))
            continue;

        std::strcpy(AnomaliesNames, Level().pLevel->r_string(ASetBaseName, SetName));
        u32 count = _GetItemCount(AnomaliesNames);
        if (!count)
            continue;

        for (u32 j = 0; j < count; j++)
        {
            _GetItem(AnomaliesNames, j, AnomalyName);
            AnomalySingleSet.push_back(AnomalyName);
        };

        if (AnomalySingleSet.empty())
            continue;
        m_AnomalySetsList.push_back(AnomalySingleSet);
        m_AnomalyIDSetsList.push_back(AnomalyIDSingleSet);
    };
    //---------------------------------------------------------
    if (Level().pLevel->line_exist(ASetBaseName, "permanent"))
    {
        std::strcpy(AnomaliesNames, Level().pLevel->r_string(ASetBaseName, "permanent"));
        u32 count = _GetItemCount(AnomaliesNames);
        for (u32 j = 0; j < count; j++)
        {
            _GetItem(AnomaliesNames, j, AnomalyName);
            m_AnomaliesPermanent.push_back(AnomalyName);
        };
    }
};

void game_sv_Deathmatch::Send_EventPack_for_AnomalySet(u32 AnomalySet, u8 Event)
{
    if (m_AnomalyIDSetsList.size() <= AnomalySet)
        return;
    //-----------------------------------
    NET_Packet EventPack;
    EventPack.w_begin(M_EVENT_PACK);
    //-----------------------------------
    ANOMALIES_ID* Anomalies = &(m_AnomalyIDSetsList[AnomalySet]);
    if (Anomalies->empty())
        return;
    for (u32 i = 0; i < Anomalies->size(); i++)
    {
        u16        ID = (*Anomalies)[i];
        //-----------------------------------
        NET_Packet P;
        u_EventGen(P, GE_ZONE_STATE_CHANGE, ID);
        P.w_u8(u8(Event));   //eZoneStateDisabled
        //-----------------------------------
        EventPack.w_u8(u8(P.B.count));
        EventPack.w(&P.B.data, P.B.count);
    };
    u_EventSend(EventPack);
};

void game_sv_Deathmatch::StartAnomalies(int AnomalySet)
{
    if (m_AnomalySetsList.empty())
        return;

    if (AnomalySet != -1 && u32(AnomalySet) >= m_AnomalySetsList.size())
        return;

    xr_vector<u8>& ASetID = m_AnomalySetID;
    if (ASetID.empty())
    {
        u8 Size = (u8)m_AnomalySetsList.size();
        for (u8 i = 0; i < Size; i++)
        {
            if (m_dwLastAnomalySetID == i && Size > 1)
                continue;
            ASetID.push_back(i);
        };
    };

    u8  ID              = u8(::Random.randI((int)ASetID.size()));
    u32 NewAnomalySetID = ASetID[ID];
    ASetID.erase(ASetID.begin() + ID);
    ///////////////////////////////////////////////////
    if (m_dwLastAnomalySetID < m_AnomalySetsList.size())
    {
        Send_EventPack_for_AnomalySet(m_dwLastAnomalySetID, CCustomZone::eZoneStateDisabled);   //Disable
    };
    ///////////////////////////////////////////////////
    if (AnomalySet != -1 && AnomalySet < (int)m_AnomalySetsList.size())
    {
        m_dwLastAnomalySetID = u32(AnomalySet);
        m_AnomalySetID.clear();
    }
    else
        m_dwLastAnomalySetID = NewAnomalySetID;

    if (IsAnomaliesEnabled())
        Send_EventPack_for_AnomalySet(m_dwLastAnomalySetID, CCustomZone::eZoneStateIdle);   //Idle
    m_dwLastAnomalyStartTime = Level().timeServer();
#ifdef DEBUG
    Msg("Anomaly Set %d Activated", m_dwLastAnomalySetID);
#endif
};

BOOL game_sv_Deathmatch::OnTouch(u16 eid_who, u16 eid_what, BOOL bForced)
{
    CSE_Abstract* e_who = m_server->ID_to_entity(eid_who);
    VERIFY(e_who);
    CSE_Abstract* e_what = m_server->ID_to_entity(eid_what);
    VERIFY(e_what);

    CSE_ALifeCreatureActor* A = smart_cast<CSE_ALifeCreatureActor*>(e_who);
    if (A)
    {
        // Actor touches something
        CSE_ALifeItemWeapon* W = smart_cast<CSE_ALifeItemWeapon*>(e_what);
        if (W)
        {
            // Weapon
            xr_vector<u16>& C    = A->children;
            u8              slot = W->get_slot();
            for (u32 it = 0; it < C.size(); ++it)
            {
                CSE_Abstract* Et = m_server->ID_to_entity(C[it]);
                if (0 == Et)
                    continue;
                CSE_ALifeItemWeapon* T = smart_cast<CSE_ALifeItemWeapon*>(Et);
                if (0 == T)
                    continue;
                if (slot == T->get_slot())
                {
                    if (bForced)
                    {
                        // We've found same slot occupied - disallow ownership
                        //-----------------------------------------------------
                        NET_Packet P;
                        u_EventGen(P, GE_OWNERSHIP_REJECT, eid_who);
                        P.w_u16(T->ID);
                        Level().Send(P, net_flags(TRUE, TRUE));
                        //-----------------------------------------------------
                        u_EventGen(P, GE_OWNERSHIP_TAKE, eid_who);
                        P.w_u16(eid_what);
                        Level().Send(P, net_flags(TRUE, TRUE));
                        //-----------------------------------------------------
                    }
                    return FALSE;
                }
            }

            // Weapon slot empty - ownership OK
            return TRUE;
        }

        CSE_ALifeItemAmmo* pIAmmo = smart_cast<CSE_ALifeItemAmmo*>(e_what);
        if (pIAmmo)
        {
            //Ammo
            return TRUE;
        };

        CSE_ALifeItemGrenade* pIGrenade = smart_cast<CSE_ALifeItemGrenade*>(e_what);
        if (pIGrenade)
        {
            //Grenade
            return TRUE;
        };

        CSE_ALifeItemCustomOutfit* pOutfit = smart_cast<CSE_ALifeItemCustomOutfit*>(e_what);
        if (pOutfit)
        {
            //Possibly Addons and/or Outfits
            return TRUE;
        };

        //---------------------------------------------------------------
        if (e_what->m_tClassID == CLSID_OBJECT_PLAYERS_BAG)
        {
            if (e_what->ID_Parent == 0xffff)
            {
                //-------------------------------
                //move all items from rukzak to player
                if (!e_what->children.empty())
                {
                    NET_Packet EventPack, PacketReject, PacketTake;
                    EventPack.w_begin(M_EVENT_PACK);

                    while (!e_what->children.empty())
                    {
                        CSE_Abstract* e_child_item = get_entity_from_eid(e_what->children.back());
                        R_ASSERT(e_child_item);

                        if (!OnTouch(eid_who, e_child_item->ID, FALSE))
                        {
                            NET_Packet P;
                            u_EventGen(P, GE_OWNERSHIP_REJECT, e_what->ID);
                            P.w_u16(e_child_item->ID);

                            m_server->Process_event_reject(P, m_server->GetServerClient()->ID, 0, e_what->ID, e_child_item->ID);
                            continue;
                        }

                        m_server->Perform_transfer(PacketReject, PacketTake, e_child_item, e_what, e_who);

                        EventPack.w_u8(u8(PacketReject.B.count));
                        EventPack.w(&PacketReject.B.data, PacketReject.B.count);
                        EventPack.w_u8(u8(PacketTake.B.count));
                        EventPack.w(&PacketTake.B.data, PacketTake.B.count);
                    }
                    if (EventPack.B.count > 2)
                        u_EventSend(EventPack);
                }
                //-------------------------------
                //destroy the BAG
                NET_Packet P;
                u_EventGen(P, GE_DESTROY, e_what->ID);
                m_server->OnMessage(P, m_server->GetServerClient()->ID);
                //-------------------------------

                game_PlayerState* pKiller = get_eid(eid_who);
                if (pKiller)
                {
                    if (g_sv_dm_bPDAHunt)
                    {
                        Player_AddBonusMoney(pKiller, READ_IF_EXISTS(pSettings, r_s32, "mp_bonus_money", "pda_taken", 0), SKT_PDA);
                    };
                };
                //-------------------------------
                return FALSE;
            }
        };
        //---------------------------------------------------------------
        if (IsBuyableItem(*e_what->s_name))
            return TRUE;
        //---------------------------------------------------------------
    };
    // We don't know what the hell is it, so disallow ownership just for safety
    return FALSE;
}

void game_sv_Deathmatch::OnDetach(u16 eid_who, u16 eid_what)
{
    CSE_Abstract* e_parent = get_entity_from_eid(eid_who);
    CSE_Abstract* e_entity = get_entity_from_eid(eid_what);

    if (e_entity->m_tClassID == CLSID_OBJECT_PLAYERS_BAG && e_parent->m_tClassID == CLSID_OBJECT_ACTOR)
    {
        //move all items from player to rukzak
        xr_vector<u16>::const_iterator it   = e_parent->children.begin();
        xr_vector<u16>::const_iterator it_e = e_parent->children.end();
        xr_vector<CSE_Abstract*>       to_transfer;
        xr_vector<CSE_Abstract*>       to_destroy;
        for (; it != it_e; ++it)
        {
            u16           ItemID = *it;
            CSE_Abstract* e_item = get_entity_from_eid(ItemID);

            R_ASSERT(e_item->ID_Parent == e_parent->ID);

            switch (e_item->m_tClassID)
            {
                case CLSID_OBJECT_AMMO:
                    //---------------------------------------
                case CLSID_OBJECT_A_VOG25:
                case CLSID_OBJECT_A_OG7B:
                case CLSID_OBJECT_A_M209:
                    //---------------------------------------
                    // Weapons Add-ons
                case CLSID_OBJECT_W_SCOPE:
                case CLSID_OBJECT_W_SILENCER:
                case CLSID_OBJECT_W_GLAUNCHER:
                    // Detectors
                case CLSID_DETECTOR_SIMPLE:
                    // PDA
                case CLSID_DEVICE_PDA:

                case CLSID_DEVICE_TORCH:
                case CLSID_IITEM_MEDKIT:
                case CLSID_IITEM_ANTIRAD:
                    // Grenades
                case CLSID_GRENADE_F1:
                case CLSID_OBJECT_G_RPG7:
                case CLSID_GRENADE_RGD5:
                    // Weapons
                case CLSID_OBJECT_W_M134:
                case CLSID_OBJECT_W_FN2000:
                case CLSID_OBJECT_W_AK74:
                case CLSID_OBJECT_W_LR300:
                case CLSID_OBJECT_W_HPSA:
                case CLSID_OBJECT_W_PM:
                case CLSID_OBJECT_W_FORT:
                case CLSID_OBJECT_W_BINOCULAR:
                case CLSID_OBJECT_W_SHOTGUN:
                case CLSID_OBJECT_W_SVD:
                case CLSID_OBJECT_W_SVU:
                case CLSID_OBJECT_W_RPG7:
                case CLSID_OBJECT_W_VAL:
                case CLSID_OBJECT_W_VINTOREZ:
                case CLSID_OBJECT_W_WALTHER:
                case CLSID_OBJECT_W_USP45:
                case CLSID_OBJECT_W_GROZA:
                case CLSID_OBJECT_W_BM16:
                case CLSID_OBJECT_W_RG6:
                {
                    to_transfer.push_back(e_item);
                }
                break;
                case CLSID_OBJECT_W_KNIFE:
                {
                    to_destroy.push_back(e_item);
                }
                break;
            };   //case
        }

        xr_vector<CSE_Abstract*>::const_iterator tr_it   = to_transfer.begin();
        xr_vector<CSE_Abstract*>::const_iterator tr_it_e = to_transfer.end();

        NET_Packet                               EventPack;
        NET_Packet                               PacketReject;
        NET_Packet                               PacketTake;
        EventPack.w_begin(M_EVENT_PACK);

        for (; tr_it != tr_it_e; ++tr_it)
        {
            m_server->Perform_transfer(PacketReject, PacketTake, *tr_it, e_parent, e_entity);
            EventPack.w_u8(u8(PacketReject.B.count));
            EventPack.w(&PacketReject.B.data, PacketReject.B.count);
            EventPack.w_u8(u8(PacketTake.B.count));
            EventPack.w(&PacketTake.B.data, PacketTake.B.count);
        }

        if (EventPack.B.count > 2)
            u_EventSend(EventPack);
    };
}

void game_sv_Deathmatch::OnPlayerConnect(ClientID id_who)
{
    inherited::OnPlayerConnect(id_who);

    xrClientData*     xrCData = m_server->ID_to_client(id_who);
    game_PlayerState* ps_who  = get_id(id_who);

    if (!xrCData->flags.bReconnect)
    {
        ps_who->clear();
        ps_who->team = 0;
        ps_who->skin = -1;
    };
    ps_who->setFlag(GAME_PLAYER_FLAG_SPECTATOR);

    ps_who->resetFlag(GAME_PLAYER_FLAG_SKIP);

    if ((g_dedicated_server || m_bSpectatorMode) && (xrCData == m_server->GetServerClient()))
    {
        ps_who->setFlag(GAME_PLAYER_FLAG_SKIP);
        return;
    }

    if (!xrCData->flags.bReconnect)
        Money_SetStart(id_who);

    SetPlayersDefItems(ps_who);
}

void game_sv_Deathmatch::OnPlayerConnectFinished(ClientID id_who)
{
    xrClientData* xrCData = m_server->ID_to_client(id_who);
    SpawnPlayer(id_who, "spectator");
    // Send Message About Client Connected
    if (xrCData)
    {
        NET_Packet P;
        GenerateGameMessage(P);
        P.w_u32(GAME_EVENT_PLAYER_CONNECTED);
        P.w_stringZ(xrCData->name.c_str());
        u_EventSend(P);
    };
    Send_Anomaly_States(id_who);
};

void game_sv_Deathmatch::check_Player_for_Invincibility(game_PlayerState* ps)
{
    if (!ps)
        return;
    u32 CurTime = Device->dwTimeGlobal;

    if ((ps->RespawnTime + GetDMBLimit() * 1000 < CurTime) && ps->testFlag(GAME_PLAYER_FLAG_INVINCIBLE))
    {
        ps->resetFlag(GAME_PLAYER_FLAG_INVINCIBLE);
    }
};

void game_sv_Deathmatch::check_InvinciblePlayers()
{
    m_server->ForEachClientDo(
        [&](IClient* client)
        {
            xrClientData*     l_pC = (xrClientData*)client;
            game_PlayerState* ps   = l_pC->ps;
            if (ps->testFlag(GAME_PLAYER_FLAG_VERY_VERY_DEAD))
                return;
            u16 OldFlags = ps->flags__;
            check_Player_for_Invincibility(ps);
            if (ps->flags__ != OldFlags)
                signal_Syncronize();
        });
};

void game_sv_Deathmatch::RespawnPlayer(ClientID id_who, bool NoSpectator)
{
    inherited::RespawnPlayer(id_who, NoSpectator);

    xrClientData*           xrCData = (xrClientData*)m_server->ID_to_client(id_who);
    game_PlayerState*       ps      = xrCData->ps;
    CSE_Abstract*           pOwner  = xrCData->owner;
    CSE_ALifeCreatureActor* pA      = smart_cast<CSE_ALifeCreatureActor*>(pOwner);
    if (!pA)
        return;

    TeamStruct* pTeamData = GetTeamData(u8(ps->team));
    if (pTeamData)
        Player_AddMoney(ps, pTeamData->m_iM_OnRespawn);

    if (GetDMBLimit() * 1000 > 0)
        ps->setFlag(GAME_PLAYER_FLAG_INVINCIBLE);

    SpawnWeapon4Actor(pA->ID, "mp_players_rukzak", 0);
}

INT  G_DELAYED_ROUND_TIME = 7;
void game_sv_Deathmatch::OnDelayedRoundEnd(ERoundEnd_Result reason)
{
    DumpRoundStatistics();
    round_end_reason  = reason;

    m_delayedRoundEnd = true;
    m_roundEndDelay   = Device->TimerAsync() + G_DELAYED_ROUND_TIME * 1000;
}

void game_sv_Deathmatch::OnDelayedTeamEliminated()
{
    m_delayedTeamEliminated = true;
    m_TeamEliminatedDelay   = Device->TimerAsync() + G_DELAYED_ROUND_TIME * 1000;
}

void game_sv_Deathmatch::check_ForceRespawn()
{
    if (!GetForceRespawn())
        return;
    u32 cnt = get_players_count();
    m_server->ForEachClientDo(
        [&](IClient* client)
        {
            xrClientData*     l_pC = (xrClientData*)client;
            game_PlayerState* ps   = l_pC->ps;
            if (!l_pC->net_Ready || ps->IsSkip())
                return;
            if (!ps->testFlag(GAME_PLAYER_FLAG_VERY_VERY_DEAD))
                return;
            if (ps->testFlag(GAME_PLAYER_FLAG_SPECTATOR))
                return;
            u32 CurTime = Device->dwTimeGlobal;
            if (ps->DeathTime + GetForceRespawn() * 1000 < CurTime)
            {
                SetPlayersDefItems(ps);
                RespawnPlayer(l_pC->ID, true);
                SpawnWeaponsForActor(l_pC->owner, ps);
                Check_ForClearRun(ps);
            }
        });
};

INT  g_sv_Skip_Winner_Waiting = 0;
bool game_sv_Deathmatch::HasChampion()
{
    game_PlayerState* res          = NULL;
    s16               MaxFragsMin  = -100;
    s16               MaxFragsCurr = MaxFragsMin;

    m_server->ForEachClientDo(
        [&](IClient* client)
        {
            xrClientData*     l_pC = (xrClientData*)client;
            game_PlayerState* ps   = l_pC->ps;
            if (!ps)
                return;
            if (ps->frags() > MaxFragsCurr)
            {
                MaxFragsCurr = ps->frags();
                res          = ps;
            }
        });
    bool Result = true;
    m_server->ForEachClientDo(
        [&](IClient* client)
        {
            xrClientData*     l_pC = (xrClientData*)client;
            game_PlayerState* ps   = l_pC->ps;
            if (!ps)
                return;
            if ((ps->frags() == MaxFragsCurr) && (ps != res))
            {
                Result = false;
            }
        });

    return Result && (MaxFragsCurr > MaxFragsMin || g_sv_Skip_Winner_Waiting);
};

bool game_sv_Deathmatch::check_for_Anomalies()
{
    if (!IsAnomaliesEnabled())
        return false;

    if (m_dwLastAnomalySetID < 1000)
    {
        if (GetAnomaliesTime() == 0)
            return false;
        if (m_dwLastAnomalyStartTime + GetAnomaliesTime() * 60000 > Level().timeServer())
            return false;
    };
    StartAnomalies();
    return true;
}

BOOL game_sv_Deathmatch::Is_Anomaly_InLists(CSE_Abstract* E)
{
    if (!E)
        return FALSE;
    CSE_ALifeCustomZone* pCustomZone = smart_cast<CSE_ALifeCustomZone*>(E);
    if (pCustomZone)
    {
        if (pCustomZone->m_owner_id != 0xffffffff)
            return TRUE;
    }

    ANOMALIES_it It = std::find(m_AnomaliesPermanent.begin(), m_AnomaliesPermanent.end(), E->name_replace());
    if (It != m_AnomaliesPermanent.end())
    {
        return TRUE;
    };

    for (u32 j = 0; j < m_AnomalySetsList.size(); j++)
    {
        ANOMALIES*   Anomalies = &(m_AnomalySetsList[j]);
        ANOMALIES_it It        = std::find(Anomalies->begin(), Anomalies->end(), E->name_replace());
        if (It != Anomalies->end())
        {
            return TRUE;
        };
    };
    return FALSE;
}

BOOL game_sv_Deathmatch::OnPreCreate(CSE_Abstract* E)
{
    BOOL res = inherited::OnPreCreate(E);
    if (!res)
        return res;

    CSE_ALifeCustomZone* pCustomZone = smart_cast<CSE_ALifeCustomZone*>(E);
    if (pCustomZone)
    {
        return Is_Anomaly_InLists(pCustomZone);
    }

    return TRUE;
};

void game_sv_Deathmatch::OnCreate(u16 eid_who)
{
    inherited::OnCreate(eid_who);

    CSE_Abstract* pEntity = get_entity_from_eid(eid_who);
    if (!pEntity)
        return;
    CSE_ALifeCustomZone* pCustomZone = smart_cast<CSE_ALifeCustomZone*>(pEntity);
    if (pCustomZone)
    {
        pCustomZone->m_maxPower = pSettings->r_float(*pCustomZone->s_name, "max_start_power");
    }
};

void game_sv_Deathmatch::OnPostCreate(u16 eid_who)
{
    inherited::OnPostCreate(eid_who);

    CSE_Abstract* pEntity = get_entity_from_eid(eid_who);
    if (!pEntity)
        return;
    CSE_ALifeCustomZone* pCustomZone = smart_cast<CSE_ALifeCustomZone*>(pEntity);
    if (!pCustomZone || pCustomZone->m_owner_id != u32(-1))
        return;

    for (u32 j = 0; j < m_AnomalySetsList.size(); j++)
    {
        ANOMALIES*   Anomalies = &(m_AnomalySetsList[j]);
        ANOMALIES_it It        = std::find(Anomalies->begin(), Anomalies->end(), pCustomZone->name_replace());
        if (It != Anomalies->end())
        {
            m_AnomalyIDSetsList[j].push_back(eid_who);

            //-----------------------------------------------------------------------------
            NET_Packet P;
            u_EventGen(P, GE_ZONE_STATE_CHANGE, eid_who);
            P.w_u8(u8(CCustomZone::eZoneStateDisabled));   //eZoneStateDisabled
            u_EventSend(P);
            //-----------------------------------------------------------------------------
            return;
        };
    };

    ANOMALIES_it It = std::find(m_AnomaliesPermanent.begin(), m_AnomaliesPermanent.end(), pCustomZone->name_replace());
    if (It == m_AnomaliesPermanent.end())
    {
        Msg("! Anomaly Not Found in any Set : %s", pCustomZone->name_replace());

        NET_Packet P;
        u_EventGen(P, GE_ZONE_STATE_CHANGE, eid_who);
        P.w_u8(u8(CCustomZone::eZoneStateDisabled));   //eZoneStateDisabled
        u_EventSend(P);
    };
};

void game_sv_Deathmatch::Send_Anomaly_States(ClientID id_who)
{
    if (m_AnomalyIDSetsList.empty())
        return;
    //-----------------------------------
    NET_Packet EventPack;
    EventPack.w_begin(M_EVENT_PACK);
    //-----------------------------------
    for (u32 j = 0; j < m_AnomalyIDSetsList.size(); j++)
    {
        u8            AnomalyState = u8((m_dwLastAnomalySetID == j) ? CCustomZone::eZoneStateIdle : CCustomZone::eZoneStateDisabled);

        ANOMALIES_ID* Anomalies    = &(m_AnomalyIDSetsList[j]);
        if (Anomalies->empty())
            return;
        for (u32 i = 0; i < Anomalies->size(); i++)
        {
            u16        ID = (*Anomalies)[i];
            //-----------------------------------
            NET_Packet P;
            u_EventGen(P, GE_ZONE_STATE_CHANGE, ID);

            P.w_u8(u8(AnomalyState));
            //-----------------------------------
            EventPack.w_u8(u8(P.B.count));
            EventPack.w(&P.B.data, P.B.count);
        };
    };

    m_server->SendTo(id_who, EventPack, net_flags(TRUE, TRUE));
};

void game_sv_Deathmatch::Check_ForClearRun(game_PlayerState* ps)
{
    if (!ps)
        return;
    if (m_bInWarmUp)
        return;
    if (ps->LastBuyAcount != 0)
        return;
    TeamStruct* pTeam = GetTeamData(u8(ps->team));
    if (!pTeam)
        return;

    Player_AddMoney(ps, pTeam->m_iM_ClearRunBonus);
};

void game_sv_Deathmatch::ReadOptions(shared_str& options)
{
    inherited::ReadOptions(options);
    //-------------------------------
    g_sv_dm_dwForceRespawn         = get_option_i(*options, "frcrspwn", g_sv_dm_dwForceRespawn);
    g_sv_dm_dwFragLimit            = get_option_i(*options, "fraglimit", g_sv_dm_dwFragLimit);
    g_sv_dm_dwTimeLimit            = get_option_i(*options, "timelimit", g_sv_dm_dwTimeLimit);        // in (min)
    g_sv_dm_dwDamageBlockTime      = get_option_i(*options, "dmgblock", g_sv_dm_dwDamageBlockTime);   // in (sec)
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    g_sv_dm_bDamageBlockIndicators = (get_option_i(*options, "dmbi", (g_sv_dm_bDamageBlockIndicators ? 1 : 0)) != 0);
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    g_sv_dm_bAnomaliesEnabled      = (get_option_i(*options, "ans", (IsAnomaliesEnabled() ? 1 : 0)) != 0);
    g_sv_dm_dwAnomalySetLengthTime = get_option_i(*options, "anslen", g_sv_dm_dwAnomalySetLengthTime);   //in (min)
    //-----------------------------------------------------------------------
    m_bSpectatorMode               = false;
    if (!g_dedicated_server && (get_option_i(*options, "spectr", -1) != -1))
    {
        m_bSpectatorMode   = true;
        m_dwSM_SwitchDelta = get_option_i(*options, "spectr", 0) * 1000;
        if (m_dwSM_SwitchDelta < 1000)
            m_dwSM_SwitchDelta = 1000;
    };
    //-------------------------------------------------------------------------
    g_sv_dm_dwWarmUp_MaxTime = get_option_i(*options, "warmup", g_sv_dm_dwWarmUp_MaxTime);

    g_sv_dm_bPDAHunt         = (get_option_i(*options, "pdahunt", (g_sv_dm_bPDAHunt ? 1 : 0)) != 0);
};

static bool g_bConsoleCommandsCreated_DM = false;
void        game_sv_Deathmatch::ConsoleCommands_Create(){};

void        game_sv_Deathmatch::ConsoleCommands_Clear(){};

void        game_sv_Deathmatch::OnPlayerFire(ClientID id_who, NET_Packet& P)
{
    u16               PlayerID = P.r_u16();
    game_PlayerState* ps       = get_eid(PlayerID);
    if (!ps || ps->IsSkip())
        return;
    if (ps->testFlag(GAME_PLAYER_FLAG_INVINCIBLE))
    {
        ps->resetFlag(GAME_PLAYER_FLAG_INVINCIBLE);
        signal_Syncronize();
    };
};

#ifdef DEBUG
xr_vector<u32> xPath;
void           game_sv_Deathmatch::OnRender()
{
    inherited::OnRender();
    /*
	for (int i0 = 0; i0<int(rpoints[0].size())-1; i0++)
	{
		Fvector v0 = rpoints[0][i0].P;
		for (int i1=i0+1; i1<int(rpoints[0].size()); i1++)
		{
			Fvector v1 = rpoints[0][i1].P;
			bool			failed = !m_graph_engine->search(*m_level_graph,m_level_graph->vertex(u32(-1),v0),m_level_graph->vertex(u32(-1),v1),&xPath,GraphEngineSpace::CBaseParameters());
			if (failed) continue;

			xr_vector<u32>::const_iterator I = xPath.begin();
			xr_vector<u32>::const_iterator E = xPath.end();
			for ( ; I != E; ++I) {
				Level().debug_renderer().draw_aabb(
					Fvector().set(
					m_level_graph->vertex_position(*I)
					).add(
					Fvector().set(0.f,.025f,0.f)
					),
					.05f,
					.05f,
					.05f,
					0xff00ff00
					);
				if (I!=E-1)
				{
					Fvector p0 = m_level_graph->vertex_position(*I);
					Fvector p1 = m_level_graph->vertex_position(*(I+1));
					Level().debug_renderer().draw_line(Fidentity, 
						p0,
						p1,
						0xff00ff00);
				}

			}
		}
	}
	*/
};
#endif

void game_sv_Deathmatch::check_for_WarmUp()
{
    if (m_dwWarmUp_CurTime == 0 && !m_bInWarmUp)
        return;
    if (m_dwWarmUp_CurTime < Level().timeServer())
    {
        m_dwWarmUp_CurTime = 0;
        Console->Execute("g_restart_fast");
    };
};

void game_sv_Deathmatch::on_death(CSE_Abstract* e_dest, CSE_Abstract* e_src)
{
    CSE_ALifeCreatureActor* pVictim = smart_cast<CSE_ALifeCreatureActor*>(e_dest);
    if (!pVictim)
        return;
    pVictim->on_death(e_src);
}

void game_sv_Deathmatch::OnPlayer_Sell_Item(ClientID id_who, NET_Packet& P){};

void game_sv_Deathmatch::WriteGameState(CInifile& ini, LPCSTR sect, bool bRoundResult)
{
    inherited::WriteGameState(ini, sect, bRoundResult);

    if (!bRoundResult)
        ini.w_string(sect, "in_warmup", m_bInWarmUp ? "true" : "false");
    ini.w_string(sect, "anomalies", IsAnomaliesEnabled() ? "true" : "false");

    if (!bRoundResult)
    {
        game_PlayerState* ps = GetWinningPlayer();
        if (ps)
        {
            ini.w_string(sect, "best_killer", ps->getName());
        }
    }
    ini.w_s32(sect, "timelimit_mins", GetTimeLimit());
    ini.w_s32(sect, "fraglimit", GetFragLimit());

    if (!bRoundResult)
    {
        u32 game_time = (Level().timeServer() - m_round_start_time);
        ini.w_u32(sect, "round_time_sec", game_time / 1000);
    }
}
