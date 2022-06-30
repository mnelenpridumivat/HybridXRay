//----------------------------------------------------
// file: SceneClassList.h
//----------------------------------------------------

#ifndef pick_defsH
#define pick_defsH
//----------------------------------------------------

class CEditableObject;
class CEditableMesh;
class CCustomObject;

#if 1
	class SPickQuery{
    	Flags32				m_Flags;
    public:
        Fvector				m_Start;
        Fvector				m_Direction;
        float				m_Dist;
        Fbox 				m_BB; 
    public:
    	class SResult{
		public:            
        	Fvector			verts[3];
            float			u,v;      
            float			range;
            CEditableObject*e_obj;
            CEditableMesh*	e_mesh;
//.            u32				tag;
            union	{
                u32			tag;				// 4b
                struct {
                    u32		material:14;		//
                    u32		suppress_shadows:1;	//
                    u32		suppress_wm:1;		//
                    u32		sector:16;			//
                };
            };

            bool operator <	(const SResult& F)const{return range<F.range;}
            SResult			(const SResult& F)
            {
                verts[0]	= F.verts[0];
                verts[1]	= F.verts[1];
                verts[2]	= F.verts[2];
                u			= F.u;
                v			= F.v;
                range		= F.range;
                tag			= F.tag;
                e_obj		= F.e_obj;
                e_mesh		= F.e_mesh;
            }
        };
		DEFINE_VECTOR(SResult,ResultVec,ResultIt);
    protected:
    	ResultVec			results;
    public:
        IC void	prepare_rq	(const Fvector& start, const Fvector& dir, float dist, u32 flags)
        {
            m_Start.set		(start);
            m_Direction.set	(dir);
            m_Dist			= dist;
            m_Flags.assign	(flags);
        	results.clear	();          
        }
        IC void	prepare_bq	(const Fbox& bbox, u32 flags)
        {
        	m_BB.set		(bbox);
            m_Flags.assign	(flags);
        	results.clear	();
        }
        IC int r_count		()
        {
        	return results.size();
        }
        IC SResult* r_begin	()
        {
        	return results.data();
        }
        IC SResult* r_end	()
        {
        	return results.data() +results.size();
        }
        IC void r_clear		()
        {
        	results.clear	();
        }
        IC void r_sort		()
        {
        	std::sort		(results.begin(),results.end());
        }
    };
    struct SRayPickObjVisualInfo
    {
    	IKinematics   	*K;
        u16				bone_id;
        Fvector			normal;
    };
	struct SRayPickInfo{
        SRayPickObjVisualInfo   visual_inf;
		CCustomObject*			s_obj;
		CEditableObject*		e_obj;
		CEditableMesh*			e_mesh;
		Fvector     			pt;
		SRayPickInfo			(){ Reset(); visual_inf.bone_id = u16(-1); }
		IC void Reset			(){ ZeroMemory(this,sizeof(SRayPickInfo));}
	};
#endif
//----------------------------------------------------
#endif // pick_definitionH


