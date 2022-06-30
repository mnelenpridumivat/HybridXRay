#ifndef	_PHYSICS_SHELL_HOLDER_EDITOR_BASE_
#define	_PHYSICS_SHELL_HOLDER_EDITOR_BASE_

class ECORE_API CPhysicsShellHolderEditorBase
{
public:
	void								CreatePhysicsShell	( Fmatrix*	obj_xform );
    void								DeletePhysicsShell	();
    void								UpdateObjectXform	( Fmatrix &obj_xform );
    void								ApplyDragForce		( const Fvector &force );
protected:
	CPhysicsShellHolderEditorBase(): m_object_xform(Fidentity){}
	~CPhysicsShellHolderEditorBase()	{ /*DeletePhysicsShell	();*/ }
protected:
    Fmatrix								m_object_xform;
private:
  	virtual	LPCSTR						_BCL	ObjectName							()		const	 { return "EditorActor"; }
	virtual	LPCSTR						_BCL	ObjectNameVisual					()		const	 { return "unknown"; }
	virtual	LPCSTR						_BCL	ObjectNameSect						()		const	 { return "unknown"; }
	virtual	bool						_BCL	ObjectGetDestroy					()		const	 { return false; };
	virtual	u16							_BCL	ObjectID							()		const	 { return u16(-1);}
//	virtual	IRenderVisual*				_BCL	ObjectVisual						()				 { return m_pVisual;}
	virtual	void						_BCL	ObjectProcessingDeactivate			()				 {;}
	virtual	void						_BCL	ObjectProcessingActivate			()				 {}
	virtual	void						_BCL	ObjectSpatialMove					()				 {}
	virtual	void						_BCL	enable_notificate					()				 {}
	virtual bool						_BCL	has_parent_object					()				 { return false; }
	virtual	void						_BCL	on_physics_disable					()				 {}
	virtual	bool						_BCL	IsInventoryItem						()				 { return false; }
	virtual	bool						_BCL	IsActor								()				 { return false; }
 	virtual bool						_BCL	IsStalker							()				 { return false; }
	//virtual	void						SetWeaponHideState					( u16 State, bool bSet )=0;
	virtual	void						_BCL	HideAllWeapons						( bool v )		 {}//(SetWeaponHideState(INV_STATE_BLOCK_ALL,true))
	virtual	void						_BCL	MovementCollisionEnable				( bool enable )	 {}
	virtual	void					 	_BCL	BonceDamagerCallback			( float &damage_factor ){}
public:
    virtual	Fmatrix&					_BCL	ObjectXFORM				()														{  return m_object_xform;}
private:
    virtual	Fvector&					_BCL	ObjectPosition			()														{  return m_object_xform.c;}
};



#endif
 