MESH pCone1 {
	VERTEX_COUNT 17
	FACE_COUNT 30
	const Fvector vertices[VERTEX_COUNT] = {
		{ 1.414214,	-1.000000,	 1.414212},
		{ 2.000000,	-1.000000,	-0.000000},
		{ 1.847760,	-1.000000,	 0.765365},
		{ 1.847759,	-1.000000,	-0.765367},
		{ 1.414214,	-1.000000,	-1.414214},
		{ 0.765367,	-1.000000,	-1.847760},
		{-0.000000,	-1.000000,	-2.000000},
		{-0.765367,	-1.000000,	-1.847759},
		{-1.414214,	-1.000000,	-1.414214},
		{-1.847759,	-1.000000,	-0.765367},
		{-2.000000,	-1.000000,	 0.000000},
		{-1.847758,	-1.000000,	 0.765367},
		{-1.414213,	-1.000000,	 1.414214},
		{-0.765366,	-1.000000,	 1.847759},
		{ 0.000001,	-1.000000,	 2.000000},
		{ 0.765368,	-1.000000,	 1.847758},
		{ 0.000000,	 1.000000,	-0.000000},
	}
	const u16 faces[FACE_COUNT*3] = {
		0,		1,		2,
		0,		3,		1,
		0,		4,		3,
		0,		5,		4,
		0,		6,		5,
		0,		7,		6,
		0,		8,		7,
		0,		9,		8,
		0,		10,		9,
		0,		11,		10,
		0,		12,		11,
		0,		13,		12,
		0,		14,		13,
		0,		15,		14,
		16,		0,		2,
		16,		15,		0,
		16,		14,		15,
		16,		13,		14,
		16,		12,		13,
		16,		11,		12,
		16,		10,		11,
		16,		9,		10,
		16,		8,		9,
		16,		7,		8,
		16,		6,		7,
		16,		5,		6,
		16,		4,		5,
		16,		3,		4,
		16,		1,		3,
		16,		2,		1,
	}
}
