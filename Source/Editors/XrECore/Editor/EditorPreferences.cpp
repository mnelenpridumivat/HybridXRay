//---------------------------------------------------------------------------
#include "stdafx.h"
#pragma hdrstop

#include "ui_main.h"
#include "ui_toolscustom.h"
//---------------------------------------------------------------------------
CCustomPreferences* EPrefs=0;
//---------------------------------------------------------------------------

CCustomPreferences::CCustomPreferences()
{
	// view
    view_np				= 0.1f;
    view_fp				= 1500.f;
    view_fov			= deg2rad(60.f);
	// fog    
    fog_color			= 0x00555555;
    fog_fogness			= 0.9;
    // camera
    cam_fly_speed		= 5.0f;
    cam_fly_alt			= 1.8f;
    cam_sens_rot		= 0.6f;
    cam_sens_move		= 0.6f;
	// tools mouse
    tools_sens_move		= 0.3f;
    tools_sens_rot		= 0.3f;
    tools_sens_scale	= 0.3f;
	tools_show_move_axis =false;
    // box pick
    bp_lim_depth		= TRUE;
    bp_cull				= TRUE;
    bp_depth_tolerance	= 0.1f;
    // snap
    snap_angle			= deg2rad(5.f);
    snap_move			= 0.1f;
    snap_moveto			= 0.5f;
    // grid
    grid_cell_size		= 1.f;
    grid_cell_count		= 100;
    // scene
    scene_undo_level	= 125;
    scene_recent_count	= 10;
    scene_clear_color	= DEFAULT_CLEARCOLOR;
    // objects
    object_flags.zero	();
}
//---------------------------------------------------------------------------

CCustomPreferences::~CCustomPreferences()
{
}

void CCustomPreferences::ApplyValues()
{

    EDevice->m_Camera.SetViewport(view_np, view_fp, rad2deg(view_fov));
    Tools->SetFog	(fog_color,fog_fogness);

    UI->m_MouseSM	= 0.2f*tools_sens_move*tools_sens_move;
    UI->m_MouseSR	= 0.02f*tools_sens_rot*tools_sens_rot;
    UI->m_MouseSS	= 0.02f*tools_sens_scale*tools_sens_scale;

    EDevice->m_Camera.SetSensitivity	(cam_sens_move, cam_sens_rot);
    EDevice->m_Camera.SetFlyParams	(cam_fly_speed, cam_fly_alt);

    ExecCommand		(COMMAND_UPDATE_GRID);
}
//---------------------------------------------------------------------------

void  CCustomPreferences::OnClose	()
{
	ApplyValues	();	
}
//---------------------------------------------------------------------------

void CCustomPreferences::FillProp(PropItemVec& props)
{
}

void CCustomPreferences::Edit()
{
}
//---------------------------------------------------------------------------
extern bool bAllowLogCommands;
void CCustomPreferences::Load(CInifile* I)
{
    view_np				= R_FLOAT_SAFE	("editor_prefs","view_np"			,view_np		 	);
    view_fp				= R_FLOAT_SAFE	("editor_prefs","view_fp"			,view_fp		 	);
    view_fov			= R_FLOAT_SAFE	("editor_prefs","view_fov"			,view_fov			);

    fog_color			= R_U32_SAFE	("editor_prefs","fog_color"			,fog_color			);
    fog_fogness			= R_FLOAT_SAFE	("editor_prefs","fog_fogness"		,fog_fogness	 	);

    cam_fly_speed		= R_FLOAT_SAFE	("editor_prefs","cam_fly_speed"		,cam_fly_speed		);
    cam_fly_alt			= R_FLOAT_SAFE	("editor_prefs","cam_fly_alt"		,cam_fly_alt	 	);
    cam_sens_rot		= R_FLOAT_SAFE	("editor_prefs","cam_sens_rot"		,cam_sens_rot		);
    cam_sens_move		= R_FLOAT_SAFE	("editor_prefs","cam_sens_move"		,cam_sens_move		);

    tools_sens_move		= R_FLOAT_SAFE	("editor_prefs","tools_sens_move"	,tools_sens_move  	);
    tools_sens_rot		= R_FLOAT_SAFE	("editor_prefs","tools_sens_rot"	,tools_sens_rot		);
    tools_sens_scale	= R_FLOAT_SAFE	("editor_prefs","tools_sens_scale"	,tools_sens_scale	);
	tools_show_move_axis= R_BOOL_SAFE	("editor_prefs","tools_show_move_axis"	,tools_show_move_axis);
    
    bp_lim_depth		= R_BOOL_SAFE	("editor_prefs","bp_lim_depth"		,bp_lim_depth		);
    bp_cull				= R_BOOL_SAFE	("editor_prefs","bp_lim_depth"		,bp_cull		  	);
    bp_depth_tolerance	= R_FLOAT_SAFE	("editor_prefs","tools_sens_rot"	,bp_depth_tolerance	);

    snap_angle			= R_FLOAT_SAFE	("editor_prefs","snap_angle"		,snap_angle			);
    snap_move			= R_FLOAT_SAFE	("editor_prefs","snap_move"			,snap_move			);
    snap_moveto			= R_FLOAT_SAFE	("editor_prefs","snap_moveto"		,snap_moveto	   	);

    grid_cell_size		= R_FLOAT_SAFE	("editor_prefs","grid_cell_size"	,grid_cell_size		);
    grid_cell_count		= R_U32_SAFE	("editor_prefs","grid_cell_count"	,grid_cell_count   	);

    scene_undo_level	= R_U32_SAFE	("editor_prefs","scene_undo_level"	,scene_undo_level	);
    scene_recent_count	= R_U32_SAFE	("editor_prefs","scene_recent_count",scene_recent_count	);
    scene_clear_color	= R_U32_SAFE	("editor_prefs","scene_clear_color"	,scene_clear_color	);

    object_flags.flags	= R_U32_SAFE	("editor_prefs","object_flags"		,object_flags.flags );
    EDevice->RadiusRender = R_FLOAT_SAFE("render", "render_radius", EDevice->RadiusRender);


    start_w = R_U32_SAFE("render", "w", 1280);
    start_h = R_U32_SAFE("render", "h",800);
    start_maximized = R_BOOL_SAFE("render", "maximized", false);

    bAllowLogCommands = R_BOOL_SAFE("windows", "log", false);
	// read recent list    
    for (u32 i=0; i<scene_recent_count; i++){
    	shared_str fn  	= R_STRING_SAFE	("editor_prefs",xr_string().sprintf("recent_files_%d",i).c_str(),shared_str("") );
        if (fn.size())
        {
        	AStringIt it =   std::find(scene_recent_list.begin(), scene_recent_list.end(), fn.c_str() ) ;
            if (it==scene_recent_list.end())
	        	scene_recent_list.push_back(*fn);
        }
    }
    sWeather = R_STRING_SAFE	("editor_prefs", "weather", shared_str("") );
    // load shortcuts

    LoadShortcuts		(I);

    UI->LoadSettings	(I);
}

void CCustomPreferences::Save(CInifile* I)
{
    I->w_u32("editor_prefs", "tools_settings", Tools->m_Settings.flags);

    I->w_float("editor_prefs", "view_np", view_np);
    I->w_float("editor_prefs", "view_fp", view_fp);
    I->w_float("editor_prefs", "view_fov", view_fov);

    I->w_u32("editor_prefs", "fog_color", fog_color);
    I->w_float("editor_prefs", "fog_fogness", fog_fogness);

    I->w_float("editor_prefs", "cam_fly_speed", cam_fly_speed);
    I->w_float("editor_prefs", "cam_fly_alt", cam_fly_alt);
    I->w_float("editor_prefs", "cam_sens_rot", cam_sens_rot);
    I->w_float("editor_prefs", "cam_sens_move", cam_sens_move);

    I->w_float("editor_prefs", "tools_sens_rot", tools_sens_rot);
    I->w_float("editor_prefs", "tools_sens_move", tools_sens_move);
    I->w_float("editor_prefs", "tools_sens_scale", tools_sens_scale);
    I->w_bool("editor_prefs", "tools_show_move_axis", tools_show_move_axis);

    I->w_bool("editor_prefs", "bp_lim_depth", bp_lim_depth);
    I->w_bool("editor_prefs", "bp_lim_depth", bp_cull);
    I->w_float("editor_prefs", "bp_depth_tolerance", bp_depth_tolerance);

    I->w_float("editor_prefs", "snap_angle", snap_angle);
    I->w_float("editor_prefs", "snap_move", snap_move);
    I->w_float("editor_prefs", "snap_moveto", snap_moveto);

    I->w_float("editor_prefs", "grid_cell_size", grid_cell_size);
    I->w_u32("editor_prefs", "grid_cell_count", grid_cell_count);

    I->w_u32("editor_prefs", "scene_undo_level", scene_undo_level);
    I->w_u32("editor_prefs", "scene_recent_count", scene_recent_count);
    I->w_u32("editor_prefs", "scene_clear_color", scene_clear_color);

    I->w_u32("editor_prefs", "object_flags", object_flags.flags);
    for (AStringIt it = scene_recent_list.begin(); it != scene_recent_list.end(); it++) {
        xr_string L; L.sprintf("recent_files_%d", it - scene_recent_list.begin());
        xr_string V; V.sprintf("\"%s\"", it->c_str());
        I->w_string("editor_prefs", L.c_str(), V.c_str());
    }
    I->w_string("editor_prefs", "weather", sWeather.c_str());
    I->w_u32("render", "w", EDevice->dwRealWidth);
    I->w_u32("render", "h", EDevice->dwRealHeight);
    I->w_bool("windows", "log", bAllowLogCommands);
    I->w_float("render", "render_radius", EDevice->RadiusRender);

    // load shortcuts
    SaveShortcuts(I);
    UI->SaveSettings(I);
}

void CCustomPreferences::Draw()
{
}

void CCustomPreferences::Load()
{
	string_path			fn;
	INI_NAME			(fn);
    CInifile* I			= xr_new<CInifile>(fn, TRUE, TRUE, TRUE);
    Load				(I);
    xr_delete			(I);
    ApplyValues			();
}
void CCustomPreferences::Save()
{
	string_path			fn;
	INI_NAME			(fn);
    CInifile* I 		= xr_new<CInifile>(fn, FALSE, TRUE, TRUE);
    I->set_override_names(TRUE);
	Save				(I);
	xr_delete			(I);
}

void CCustomPreferences::AppendRecentFile(LPCSTR name)
{
    for (AStringIt it=scene_recent_list.begin(); it!=scene_recent_list.end(); it++){
    	if (*it==name){
        	scene_recent_list.erase	(it);
            break;
        }
    }
	scene_recent_list.insert(scene_recent_list.begin(),name);
	while (scene_recent_list.size()>=EPrefs->scene_recent_count) 
    	scene_recent_list.pop_back();

    ExecCommand				(COMMAND_REFRESH_UI_BAR);
}
//---------------------------------------------------------------------------

void CCustomPreferences::OnCreate()
{
}
//---------------------------------------------------------------------------

void CCustomPreferences::OnDestroy()
{
}
//---------------------------------------------------------------------------

