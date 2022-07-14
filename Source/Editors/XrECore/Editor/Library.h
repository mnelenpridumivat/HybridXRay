//----------------------------------------------------
// file: Library.h
//----------------------------------------------------

#ifndef LibraryH
#define LibraryH
   
//----------------------------------------------------
//----------------------------------------------------
class ECORE_API ELibrary//:	public pureDeviceCreate, public pureDeviceDestroy
{
	bool				m_bReady;
	friend class TfrmChoseObject;

    void				UnloadEditObject	(LPCSTR full_name);
public:
						ELibrary			();
	virtual 			~ELibrary			();

	void 				OnCreate			();
	void 				OnDestroy			();
	void 				Save				(FS_FileSet* modif_map=0);

    void 				ReloadObjects		();
    void 				CleanLibrary		();
    void 				ReloadObject		(LPCSTR name);

    int					GetObjects			(FS_FileSet& files);

    void				EvictObjects		();

	virtual		void	OnDeviceCreate		();
	virtual		void	OnDeviceDestroy		();
};

extern ECORE_API ELibrary Lib;
//----------------------------------------------------
#endif /*_INCDEF_Library_H_*/

