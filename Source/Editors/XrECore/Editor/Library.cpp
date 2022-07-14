//----------------------------------------------------
// file: Library.cpp
//----------------------------------------------------

#include "stdafx.h"
#pragma hdrstop

#include "Library.h"
#include "EditObject.h"
#include "ui_main.h"

//----------------------------------------------------
ELibrary Lib;
//----------------------------------------------------
ELibrary::ELibrary()
{
    m_bReady  = false;
}
//----------------------------------------------------

ELibrary::~ELibrary()
{
}
//----------------------------------------------------

void ELibrary::OnCreate()
{
	//EDevice->seqDevCreate.Add	(this,REG_PRIORITY_NORMAL);
	//EDevice->seqDevDestroy.Add(this,REG_PRIORITY_NORMAL);
    m_bReady = true;
}
//----------------------------------------------------

void ELibrary::OnDestroy()
{
    m_bReady = false;
//	EDevice->seqDevCreate.Remove(this);
//	EDevice->seqDevDestroy.Remove(this);

    // remove all instance CEditableObject
	EditObjPairIt O = m_EditObjects.begin();
	EditObjPairIt E = m_EditObjects.end();
    for(; O!=E; O++){
    	if (0!=O->second->m_RefCount){
//.        	ELog.DlgMsg(mtError,"Object '%s' still referenced.",O->first.c_str());
//.	    	R_ASSERT(0==O->second->m_RefCount);
        }
    	xr_delete(O->second);
    }
	m_EditObjects.clear();
}
//----------------------------------------------------

void ELibrary::CleanLibrary()
{
    // remove all unused instance CEditableObject
    for(EditObjPairIt O = m_EditObjects.begin(); O!=m_EditObjects.end(); ){
    	if (0==O->second->m_RefCount){ 
        	EditObjPairIt D		= O; O++;
        	xr_delete			(D->second);
            m_EditObjects.erase	(D);
        }else					O++;
    }
}
//----------------------------------------------------
void ELibrary::ReloadObject(LPCSTR nm)
{
	R_ASSERT(nm&&nm[0]);
    string512 name; strcpy(name,nm); strlwr(name);
	EditObjPairIt it = m_EditObjects.find(name);
    if (it!=m_EditObjects.end())
    	it->second->Reload();
}
//---------------------------------------------------------------------------
void ELibrary::ReloadObjects(){
	EditObjPairIt O = m_EditObjects.begin();
	EditObjPairIt E = m_EditObjects.end();
    for(; O!=E; O++)
    	O->second->Reload();
}
//----------------------------------------------------

void ELibrary::OnDeviceCreate(){
	EditObjPairIt O = m_EditObjects.begin();
	EditObjPairIt E = m_EditObjects.end();
    for(; O!=E; O++)
    	O->second->OnDeviceCreate();
}
//---------------------------------------------------------------------------

void ELibrary::OnDeviceDestroy(){
	EditObjPairIt O = m_EditObjects.begin();
	EditObjPairIt E = m_EditObjects.end();
    for(; O!=E; O++)
    	O->second->OnDeviceDestroy();
}
//---------------------------------------------------------------------------

void ELibrary::EvictObjects()
{
	EditObjPairIt O = m_EditObjects.begin();
	EditObjPairIt E = m_EditObjects.end();
    for(; O!=E; O++)
    	O->second->EvictObject();
}
//----------------------------------------------------

CEditableObject* ELibrary::LoadEditObject(LPCSTR name)
{
    CEditableObject* m_EditObject = xr_new<CEditableObject>(name);
    //FS.update_path(fn,_objects_,EFS.ChangeFileExt(name,".object").c_str());
    if (FS.exist(name))
    {
        if (m_EditObject->Load(name))	return m_EditObject;
    }else{
		ELog.Msg(mtError,"Can't find file '%s'",name);
    }
    xr_delete(m_EditObject);
	return 0;
}
//---------------------------------------------------------------------------

CEditableObject* ELibrary::CreateEditObject(LPCSTR nm)
{
    R_ASSERT(nm&&nm[0]);
//.    UI->ProgressInfo		(nm);
    xr_string name		= xr_string(nm);
    xr_strlwr(name);
    // file exist - find in already loaded
    CEditableObject* m_EditObject = 0;
	EditObjPairIt it 	= m_EditObjects.find(name);
    if (it!=m_EditObjects.end())	m_EditObject = it->second;
    else if (0!=(m_EditObject=LoadEditObject(name.c_str())))
		m_EditObjects[name]	= m_EditObject;
    if (m_EditObject)	m_EditObject->m_RefCount++;
	return m_EditObject;
}
//---------------------------------------------------------------------------

void ELibrary::RemoveEditObject(CEditableObject*& object)
{
	if (object){
	    object->m_RefCount--;
    	R_ASSERT(object->m_RefCount>=0);
		if ((object->m_RefCount==0))
			if (!object->IsModified()) UnloadEditObject(object->GetName());
        object=0;
	}
}
//---------------------------------------------------------------------------

void ELibrary::Save(FS_FileSet* modif_map)
{
	EditObjPairIt O = m_EditObjects.begin();
	EditObjPairIt E = m_EditObjects.end();
    if (modif_map)
    {
        for(; O!=E; O++)
        	if (modif_map->end()!=modif_map->find(FS_File(O->second->GetName())))
            {
                string_path 			nm;
                FS.update_path	(nm,_objects_,O->second->GetName());
                strcpy(nm, EFS.ChangeFileExt(nm,".object").c_str());

                if (!O->second->Save(nm))
                    Log			("!Can't save object:",nm);
            }
    }else
    {
        for(; O!=E; O++)
            if (O->second->IsModified())
            {
                string_path		nm;
                FS.update_path	(nm,_objects_,O->second->GetName());
                strcpy			(nm, EFS.ChangeFileExt(nm,".object").c_str());

                if (!O->second->Save(nm))
                    Log			("!Can't save object:",nm);
            }
    }
}
//---------------------------------------------------------------------------

int ELibrary::GetObjects(FS_FileSet& files)
{
    return FS.file_list(files,_objects_,FS_ListFiles|FS_ClampExt,"*.object");
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void ELibrary::UnloadEditObject(LPCSTR full_name)
{
    EditObjPairIt it 	= m_EditObjects.find(full_name);
    if (it!=m_EditObjects.end()){
    	if (0!=it->second->m_RefCount){
        	ELog.DlgMsg(mtError,"Object '%s' still referenced.",it->first.c_str());
            THROW;
        }
    	m_EditObjects.erase(it);
    	xr_delete		(it->second);
    }
}
//---------------------------------------------------------------------------

