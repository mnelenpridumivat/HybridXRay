using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Windows.Forms;

namespace Object_tool
{
	public class Object
	{
		// Data
		public List<Bone> bones = new List<Bone>();
		public List<Surface> surfaces = new List<Surface>();
		public List<Motion> motions = new List<Motion>();
		public List<Mesh> meshes = new List<Mesh>();
		public float scale = 1.0f;
		public bool scale_center_of_mass = true;
		public string userdata = "";
		public string lod = "";
		public List<string> motion_refs = new List<string>();
		public bool cop_refs = false;
		public bool has_normals = false;

		// File System
		public string FILE_NAME = "";
		public string TEMP_FILE_NAME = "";
		public string NAME = "";

		public class Bone
		{
			public ushort bone_id;
			public ushort shape_type;
			public ushort shape_flags;
			public string bone_name;
			public string material;
			public float mass;
			public ushort joint_type;
			public Fvector position;
			public Fvector rotation;
			public Fvector center_of_mass;
			public bool breakable;
			public float spring;
			public float damping;
			public float friction;

			public Bone()
			{
				bone_id = 0;
				shape_type = 0;
				shape_flags = 0;
				joint_type = 0;
				bone_name = "Error! Not loaded!";
				material = "Error! Not loaded!";
				mass = 10.0f;
				position = new Fvector();
				rotation = new Fvector();
				center_of_mass = new Fvector();
				breakable = false;
				spring = 0.0f;
				damping = 0.0f;
				friction = 0.0f;
			}
		};

		public class Surface
		{
			public uint flags;
			public string texture;
			public string shader;
			public string name;

			public Surface()
			{
				flags = 0;
				texture = "Error! Not loaded!";
				shader = "Error! Not loaded!";
				name = "Error! Not loaded!";
			}
		};

		public class Motion
		{
			public string name;

			public Motion()
			{
				name = "Error! Not loaded!";
			}
		}

		public class Mesh
        {
			public class SurfFace
            {
				public long faces;
				public string name;

				public SurfFace()
                {
					faces = 0;
					name = "Error! Not loaded!";
				}
			};

			public List<SurfFace> surfFaces = new List<SurfFace>();
			public long vertex_count;
			public long face_count;

			public Mesh()
			{
				vertex_count = 0;
				face_count = 0;
			}
		};

		public void Init(string filename, string temp_folder)
        {
			if (Directory.Exists(temp_folder))
				Directory.Delete(temp_folder, true);

			if (!Directory.Exists(temp_folder))
				Directory.CreateDirectory(temp_folder);

			NAME = filename.Substring(filename.LastIndexOf('\\') + 1);
			FILE_NAME = filename;
			TEMP_FILE_NAME = temp_folder + $"\\{NAME}";

			File.Copy(FILE_NAME, TEMP_FILE_NAME, true);
		}

		public void LoadData()
		{
			var xr_loader = new XRayLoader();
			surfaces.Clear();
			bones.Clear();
			motion_refs.Clear();
			meshes.Clear();
			scale = 1.0f;
			scale_center_of_mass = true;
			userdata = "";
			lod = "";
			cop_refs = false;
			has_normals = false;

			using (var r = new BinaryReader(new FileStream(TEMP_FILE_NAME, FileMode.Open)))
			{
				xr_loader.SetStream(r.BaseStream);
				bool FindBody = xr_loader.SetData(xr_loader.find_and_return_chunk_in_chunk((int)OBJECT.EOBJ_CHUNK_OBJECT_BODY, false, true));

				if (xr_loader.find_chunk((int)OBJECT.EOBJ_CHUNK_LODS, !FindBody, true))
				{
					lod = xr_loader.read_stringData();
				}

				if (xr_loader.find_chunk((int)OBJECT.EOBJ_CHUNK_CLASSSCRIPT, !FindBody, true))
				{
					userdata = xr_loader.read_stringZ();
				}

				if (xr_loader.find_chunkSize((int)OBJECT.EOBJ_CHUNK_ACTORTRANSFORM, !FindBody, true) == 32)
				{
					xr_loader.ReadBytes(12);
					xr_loader.ReadBytes(12);
					scale = xr_loader.ReadFloat();
					uint chk = xr_loader.ReadUInt32();
					scale_center_of_mass = Convert.ToBoolean(chk);
				}

				if (xr_loader.find_chunkSize((int)OBJECT.EOBJ_CHUNK_SCALE, !FindBody, true) == 5)
				{
					scale = xr_loader.ReadFloat();
					uint chk = xr_loader.ReadByte();
					scale_center_of_mass = Convert.ToBoolean(chk);
				}

				if (xr_loader.find_chunk((int)OBJECT.EOBJ_CHUNK_SMOTIONS3, !FindBody, true))
				{
					uint count = xr_loader.ReadUInt32();
					for (int i = 0; i < count; i++)
						motion_refs.Add(xr_loader.read_stringZ());

					cop_refs = true;

				}
				else if (xr_loader.find_chunk((int)OBJECT.EOBJ_CHUNK_SMOTIONS2, !FindBody, true))
				{
					string motions = xr_loader.read_stringZ();
					string motion = "";

					for (int i = 0; i < motions.Length; i++)
					{
						if (motions[i] != ',')
							motion += motions[i];
						else
						{
							motion_refs.Add(motion);
							motion = "";
						}
					}

					if (motion != "")
						motion_refs.Add(motion);

					cop_refs = false;
				}

				if (xr_loader.SetData(xr_loader.find_and_return_chunk_in_chunk((int)OBJECT.EOBJ_CHUNK_EDITMESHES, !FindBody, true)))
				{
					int id = 0;
					uint size;

					while (true)
					{
						if (!xr_loader.find_chunk(id)) break;

						Mesh mesh = new Mesh();
						Stream temp = xr_loader.reader.BaseStream;

						if (!xr_loader.SetData(xr_loader.find_and_return_chunk_in_chunk(id, false, true))) break;

						size = xr_loader.find_chunkSize((int)MESH.EMESH_CHUNK_VERTS);
						if (size == 0) break;
						mesh.vertex_count = xr_loader.ReadUInt32();

						size = xr_loader.find_chunkSize((int)MESH.EMESH_CHUNK_FACES);
						if (size == 0) break;
						mesh.face_count = xr_loader.ReadUInt32();

						if (xr_loader.find_chunk((int)MESH.EMESH_CHUNK_NORMALS, false, true))
						{
							has_normals = true;
						}

						if (xr_loader.find_chunk((int)MESH.EMESH_CHUNK_SFACE, false, true))
						{
							uint count = xr_loader.ReadUInt16();

							for (int i = 0; i < count; i++)
                            {
								Mesh.SurfFace surfFace = new Mesh.SurfFace();

								surfFace.name = xr_loader.read_stringZ();
								surfFace.faces = xr_loader.ReadUInt32();
								xr_loader.ReadBytes((int)surfFace.faces * 4);

								mesh.surfFaces.Add(surfFace);
							}
						}

						meshes.Add(mesh);

						id++;
						xr_loader.SetStream(temp);
					}
				}

				xr_loader.SetStream(r.BaseStream);
				FindBody = xr_loader.SetData(xr_loader.find_and_return_chunk_in_chunk((int)OBJECT.EOBJ_CHUNK_OBJECT_BODY, false, true));

				if (xr_loader.find_chunk((int)OBJECT.EOBJ_CHUNK_SURFACES3, !FindBody, true))
				{
					uint surface_count = xr_loader.ReadUInt32();
					for (int i = 0; i < surface_count; i++)
					{
						Surface surface = new Surface();
						surface.name = xr_loader.read_stringZ(); // Name
						surface.shader = xr_loader.read_stringZ(); // Shader
						xr_loader.read_stringZ(); // Shader XRLC
						xr_loader.read_stringZ(); // GameMtl
						surface.texture = xr_loader.read_stringZ(); // Texture
						xr_loader.read_stringZ(); // VMap
						surface.flags = xr_loader.ReadUInt32();   // Flags
						xr_loader.ReadUInt32();   // FVF
						xr_loader.ReadUInt32();   // TC count

						surfaces.Add(surface);
					}
				}
				else if (xr_loader.find_chunk((int)OBJECT.EOBJ_CHUNK_SURFACES2, !FindBody, true))
				{
					uint surface_count = xr_loader.ReadUInt32();
					for (int i = 0; i < surface_count; i++)
					{
						Surface surface = new Surface();
						surface.name = xr_loader.read_stringZ(); // Name
						surface.shader = xr_loader.read_stringZ(); // Shader
						xr_loader.read_stringZ(); // Shader XRLC
						surface.texture = xr_loader.read_stringZ(); // Texture
						xr_loader.read_stringZ(); // VMap
						surface.flags = xr_loader.ReadUInt32();   // Flags
						xr_loader.ReadUInt32();   // FVF
						xr_loader.ReadUInt32();   // TC count

						surfaces.Add(surface);
					}
				}
				else if (xr_loader.find_chunk((int)OBJECT.EOBJ_CHUNK_SURFACES, !FindBody, true))
				{
					uint surface_count = xr_loader.ReadUInt32();
					for (int i = 0; i < surface_count; i++)
					{
						Surface surface = new Surface();
						surface.name = xr_loader.read_stringZ(); // Name
						surface.shader = xr_loader.read_stringZ(); // Shader
						surface.flags = xr_loader.ReadByte();     // Flags
						xr_loader.ReadUInt32();   // FVF
						xr_loader.ReadUInt32();   // TC count
						surface.texture = xr_loader.read_stringZ(); // Texture
						xr_loader.read_stringZ(); // VMap

						surfaces.Add(surface);
					}
				}

				xr_loader.SetStream(r.BaseStream);
				FindBody = xr_loader.SetData(xr_loader.find_and_return_chunk_in_chunk((int)OBJECT.EOBJ_CHUNK_OBJECT_BODY, false, true));

				bool B_CHUNK = xr_loader.SetData(xr_loader.find_and_return_chunk_in_chunk((int)OBJECT.EOBJ_CHUNK_BONES2, !FindBody, true));
				if (B_CHUNK)
				{
					int chunk = 0;
					uint size;

					while (true)
					{
						Stream temp = xr_loader.reader.BaseStream;

						if (!xr_loader.SetData(xr_loader.find_and_return_chunk_in_chunk(chunk, false, true))) break;

						Bone bone = new Bone();
						bone.bone_id = (ushort)chunk;

						size = xr_loader.find_chunkSize((int)BONE.BONE_CHUNK_DEF, false, true);
						if (size == 0) break;
						bone.bone_name = xr_loader.read_stringZ();

						size = xr_loader.find_chunkSize((int)BONE.BONE_CHUNK_MATERIAL, false, true);
						if (size == 0) break;
						bone.material = xr_loader.read_stringZ();

						size = xr_loader.find_chunkSize((int)BONE.BONE_CHUNK_SHAPE, false, true);
						if (size == 0) break;
						bone.shape_type = (ushort)xr_loader.ReadUInt16();
						bone.shape_flags = (ushort)xr_loader.ReadUInt16();

						if (xr_loader.find_chunk((int)BONE.BONE_CHUNK_MASS, false, true))
						{
							bone.mass = xr_loader.ReadFloat();
							bone.center_of_mass.set(xr_loader.ReadVector());
						}

						size = xr_loader.find_chunkSize((int)BONE.BONE_CHUNK_BIND_POSE, false, true);
						if (size == 0) break;
						bone.position.set(xr_loader.ReadVector());
						bone.rotation.set(xr_loader.ReadVector());

						if (xr_loader.find_chunk((int)BONE.BONE_CHUNK_FLAGS, false, true))
							bone.breakable = xr_loader.ReadUInt32() == 1;

						size = xr_loader.find_chunkSize((int)BONE.BONE_CHUNK_IK_JOINT, false, true);
						if (size == 0) break;
						bone.joint_type = (ushort)xr_loader.ReadUInt32();
						xr_loader.ReadBytes(16*3); // Лимиты, добавлю позже
						bone.spring = xr_loader.ReadFloat();
						bone.damping = xr_loader.ReadFloat();

						if (xr_loader.find_chunk((int)BONE.BONE_CHUNK_IK_JOINT_FRICTION, false, true))
							bone.friction = xr_loader.ReadFloat();

						bones.Add(bone);

						chunk++;
						xr_loader.SetStream(temp);
					}
				}
				else if (xr_loader.find_chunk((int)OBJECT.EOBJ_CHUNK_BONES, !FindBody, true))
				{
					uint size = xr_loader.ReadUInt32();
					for (int i = 0; i < size; i++)
					{
						Bone bone = new Bone();
						bone.shape_flags = 0;
						bone.shape_type = 0;
						bone.bone_name = xr_loader.read_stringZ();
						bone.bone_id = (ushort)i;
						bone.material = "default_object";
						bone.mass = 10.0f;
						xr_loader.read_stringZ();
						xr_loader.read_stringZ();
						xr_loader.ReadBytes(12);
						xr_loader.ReadBytes(12);
						xr_loader.ReadFloat();

						bones.Add(bone);
					}
				}
			}
		}

		public void LoadMotions()
		{
			var xr_loader = new XRayLoader();
			motions.Clear();

			using (var r = new BinaryReader(new FileStream(TEMP_FILE_NAME, FileMode.Open)))
			{
				xr_loader.SetStream(r.BaseStream);
				bool FindBody = xr_loader.SetData(xr_loader.find_and_return_chunk_in_chunk((int)OBJECT.EOBJ_CHUNK_OBJECT_BODY, false, true));

				if (xr_loader.find_chunk((int)OBJECT.EOBJ_CHUNK_SMOTIONS, !FindBody, true))
				{
					uint count = xr_loader.ReadUInt32();

					for (int i = 0; i < count; i++)
					{
						Motion motion = new Motion();
						motion.name = xr_loader.read_stringZ();
						xr_loader.ReadBytes(12);
						uint vers = xr_loader.ReadUInt16();

						switch (vers)
						{
							case 4:
								{
									xr_loader.ReadBytes(22);
									uint sz = xr_loader.ReadUInt32();
									for (int j = 0; j < sz; j++)
									{
										xr_loader.ReadBytes(4);
										for (int m = 0; m < 6; m++)
										{
											xr_loader.ReadBytes(8);
											uint sz1 = xr_loader.ReadUInt32();
											for (int p = 0; p < sz1; p++)
											{
												xr_loader.ReadBytes(40);
											}
										}
									}
								}
								break;
							case 5:
								{
									xr_loader.ReadBytes(24);
									uint sz = xr_loader.ReadUInt32();
									for (int j = 0; j < sz; j++)
									{
										xr_loader.read_stringZ();
										xr_loader.ReadBytes(4);
										for (int m = 0; m < 6; m++)
										{
											xr_loader.ReadBytes(8);
											uint sz1 = xr_loader.ReadUInt32();
											for (int p = 0; p < sz1; p++)
											{
												xr_loader.ReadBytes(40);
											}
										}
									}
								}
								break;
							default:
								{
									if (vers >= 6)
									{
										xr_loader.ReadBytes(19);
										uint sz = xr_loader.ReadUInt16();
										for (int j = 0; j < sz; j++)
										{
											xr_loader.read_stringZ();
											xr_loader.ReadBytes(1);
											for (int m = 0; m < 6; m++)
											{
												xr_loader.ReadBytes(2);
												uint sz1 = xr_loader.ReadUInt16();
												for (int p = 0; p < sz1; p++)
												{
													xr_loader.ReadBytes(8);
													byte shape = xr_loader.ReadByte();
													if (shape != 4)
													{
														xr_loader.ReadBytes(14);
													}
												}
											}
										}
									}
									else
										break;
								}
								break;
						}

						if (vers >= 7)
						{
							uint sz = xr_loader.ReadUInt32();
							if (sz > 0)
							{
								for (int j = 0; j < sz; ++j)
								{
									xr_loader.read_motion_mark_string();
									uint sz1 = xr_loader.ReadUInt32();

									for (int p = 0; p < sz1; ++p)
									{
										xr_loader.ReadBytes(8);
									}
								}
							}
						}

						motions.Add(motion);
					}
				}
			}
		}

		public void CopyParams(string FileName)
		{
			var xr_loader = new XRayLoader();
			motion_refs.Clear();

			using (var r = new BinaryReader(new FileStream(FileName, FileMode.Open)))
			{
				xr_loader.SetStream(r.BaseStream);
				bool FindBody = xr_loader.SetData(xr_loader.find_and_return_chunk_in_chunk((int)OBJECT.EOBJ_CHUNK_OBJECT_BODY, false, true));

				if (xr_loader.find_chunk((int)OBJECT.EOBJ_CHUNK_LODS, !FindBody, true)) // Импортируем LOD
				{
					lod = xr_loader.read_stringData();
				}

				if (xr_loader.find_chunk((int)OBJECT.EOBJ_CHUNK_CLASSSCRIPT, !FindBody, true)) // Импортируем UserData
				{
					userdata = xr_loader.read_stringZ();
				}

				if (xr_loader.find_chunk((int)OBJECT.EOBJ_CHUNK_SMOTIONS3, !FindBody, true)) // Импортируем Моушн рефы
				{
					uint count = xr_loader.ReadUInt32();
					for (int i = 0; i < count; i++)
						motion_refs.Add(xr_loader.read_stringZ());
				}
				else if (xr_loader.find_chunk((int)OBJECT.EOBJ_CHUNK_SMOTIONS2, !FindBody, true))
				{
					string motions = xr_loader.read_stringZ();

					string motion = "";

					for (int i = 0; i < motions.Length; i++)
					{
						if (motions[i] != ',')
							motion += motions[i];
						else
						{
							motion_refs.Add(motion);
							motion = "";
						}
					}

					if (motion != "")
						motion_refs.Add(motion);
				}

				if (xr_loader.find_chunk((int)OBJECT.EOBJ_CHUNK_SURFACES3, !FindBody, true))
				{
					uint surf_count = xr_loader.ReadUInt32();
					if (surfaces.Count == surf_count && MessageBox.Show("Import textures and shaders path?\nThey may have different positions", "Object Editor", MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.Yes)
					{
						for (int i = 0; i < surf_count; i++)
						{
							xr_loader.read_stringZ(); // Name
							surfaces[i].shader = xr_loader.read_stringZ(); // Shader
							xr_loader.read_stringZ(); // Shader XRLC
							xr_loader.read_stringZ(); // GameMtl
							surfaces[i].texture = xr_loader.read_stringZ(); // Texture
							xr_loader.read_stringZ(); // VMap
							surfaces[i].flags = xr_loader.ReadUInt32();   // Flags
							xr_loader.ReadUInt32();   // FVF
							xr_loader.ReadUInt32();   // TC count
						}
					}
				}
				else if (xr_loader.find_chunk((int)OBJECT.EOBJ_CHUNK_SURFACES2, !FindBody, true))
				{
					uint surf_count = xr_loader.ReadUInt32();
					if (surfaces.Count == surf_count && MessageBox.Show("Import textures and shaders path?\nThey may have different positions", "Object Editor", MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.Yes)
					{
						for (int i = 0; i < surf_count; i++)
						{
							xr_loader.read_stringZ(); // Name
							surfaces[i].shader = xr_loader.read_stringZ(); // Shader
							xr_loader.read_stringZ(); // Shader XRLC
							surfaces[i].texture = xr_loader.read_stringZ(); // Texture
							xr_loader.read_stringZ(); // VMap
							surfaces[i].flags = xr_loader.ReadUInt32();   // Flags
							xr_loader.ReadUInt32();   // FVF
							xr_loader.ReadUInt32();   // TC count
						}
					}
				}
				else if (xr_loader.find_chunk((int)OBJECT.EOBJ_CHUNK_SURFACES, !FindBody, true))
				{
					uint surf_count = xr_loader.ReadUInt32();
					if (surfaces.Count == surf_count && MessageBox.Show("Import textures and shaders path?\nThey may have different positions", "Object Editor", MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.Yes)
					{
						for (int i = 0; i < surf_count; i++)
						{
							string name = xr_loader.read_stringZ(); // Name
							surfaces[i].shader = xr_loader.read_stringZ(); // Shader
							surfaces[i].flags = xr_loader.ReadByte();     // Flags
							xr_loader.ReadUInt32();   // FVF
							xr_loader.ReadUInt32();   // TC count
							surfaces[i].texture = xr_loader.read_stringZ(); // Texture
							xr_loader.read_stringZ(); // VMap
						}
					}
				}

				xr_loader.SetStream(r.BaseStream);
				FindBody = xr_loader.SetData(xr_loader.find_and_return_chunk_in_chunk((int)OBJECT.EOBJ_CHUNK_OBJECT_BODY, false, true));

				bool B_CHUNK = xr_loader.SetData(xr_loader.find_and_return_chunk_in_chunk((int)OBJECT.EOBJ_CHUNK_BONES2, !FindBody, true));
				if (B_CHUNK)
				{
					int chunk = 0;
					uint size;

					while (true)
					{
						Stream temp = xr_loader.reader.BaseStream;

						if (!xr_loader.SetData(xr_loader.find_and_return_chunk_in_chunk(chunk, false, true))) break;

						size = xr_loader.find_chunkSize((int)BONE.BONE_CHUNK_SHAPE, false, true);
						if (size == 0) break;
						bones[chunk].shape_type = (ushort)xr_loader.ReadUInt16();
						bones[chunk].shape_flags = (ushort)xr_loader.ReadUInt16();

						size = xr_loader.find_chunkSize((int)BONE.BONE_CHUNK_MATERIAL, false, true);
						if (size == 0) break;
						bones[chunk].material = xr_loader.read_stringZ();

						size = xr_loader.find_chunkSize((int)BONE.BONE_CHUNK_MASS, false, true);
						if (size == 0) break;
						bones[chunk].mass = xr_loader.ReadFloat();

						chunk++;
						xr_loader.SetStream(temp);

						if (bones.Count <= chunk) break;
					}
				}
				else if (xr_loader.find_chunk((int)OBJECT.EOBJ_CHUNK_BONES, !FindBody, true))
				{
					uint size = xr_loader.ReadUInt32();

					if (size == bones.Count)
					{
						for (int i = 0; i < size; i++)
						{
							bones[i].shape_flags = 0;
							bones[i].shape_type = 0;
							bones[i].bone_name = xr_loader.read_stringZ();
							bones[i].bone_id = (ushort)i;
							bones[i].material = "default_object";
							bones[i].mass = 10.0f;
							xr_loader.read_stringZ();
							xr_loader.read_stringZ();
							xr_loader.ReadBytes(12);
							xr_loader.ReadBytes(12);
							xr_loader.ReadFloat();
						}
					}
				}
			}
		}

		public long TotalFaces()
        {
			long faces = 0;
			for (int i = 0; i < surfaces.Count; i++)
            {
				Mesh.SurfFace surfFace = GetSurfFaceByName(surfaces[i].name);
				if (surfFace != null)
					faces += (surfFace.faces * ((surfaces[i].flags == 1) ? 2 : 1));
			}
			return faces;
        }

		public long TotalRawVerts()
		{
			long verts = 0;
			for (int i = 0; i < meshes.Count; i++)
				verts += meshes[i].vertex_count;
			return verts;
		}

		public Surface GetSurfaceByName(string name)
        {
			for (int i = 0; i < surfaces.Count; i++)
            {
				if (surfaces[i].name == name)
					return surfaces[i];
			}
			return null;
        }

		public Mesh.SurfFace GetSurfFaceByName(string name)
		{
			for (int i = 0; i < meshes.Count; i++)
			{
				for (int j = 0; j < meshes[i].surfFaces.Count; j++)
                {
					if (meshes[i].surfFaces[j].name == name)
						return meshes[i].surfFaces[j];
				}
			}
			return null;
		}
	};
}
