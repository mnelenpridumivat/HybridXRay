#ifndef EToolsH
#define EToolsH

class IKinematics;
extern "C" {
	// fast functions
	namespace ETOOLS{
		bool 					TestRayTriA			(const Fvector& C, const Fvector& D, Fvector** p, float& u, float& v, float& range, bool bCull);
		bool 					TestRayTriB			(const Fvector& C, const Fvector& D, Fvector* p, float& u, float& v, float& range, bool bCull);
		bool 					TestRayTri2			(const Fvector& C, const Fvector& D, Fvector* p, float& range);
		
		typedef  void						pb_callback			(void* user_data, float& val);
		void						SimplifyCubeMap		(u32* src_data, u32 src_width, u32 src_height, u32* dst_data, u32 dst_width, u32 dst_height, float sample_factor=1.f, pb_callback cb=0, void* pb_data=0);

		int						r_count				();
		void						ray_options			(u32 flags);
		void						box_options			(u32 flags);

		int						ogg_enc				(const char* in_fn, const char* out_fn, float quality, void* comment, int comment_size);
		
		
		bool						intersect			(const Fmatrix &object_transform, const IKinematics& K, const Fvector& origin, const Fvector &direction, u16 &bone_id,  float &dist, Fvector &norm);
	};
};

#endif // EToolsH