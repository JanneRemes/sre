#ifndef SRE_BSP_H
#define SRE_BSP_H

#include "begin.h"

#define SRE_BSP_HEADER	(('P' << 24) | ('S' << 16) | ('B' << 8) | 'I')
#define SRE_BSP_VERSION 38

typedef enum
{
	SRE_BSP_LUMP_ENTITIES = 0,
	SRE_BSP_LUMP_PLANES,
	SRE_BSP_LUMP_VERTICES,
	SRE_BSP_LUMP_VISIBILITY,
	SRE_BSP_LUMP_NODES,
	SRE_BSP_LUMP_TEXINFO,
	SRE_BSP_LUMP_FACES,
	SRE_BSP_LUMP_LIGHTING,
	SRE_BSP_LUMP_LEAFS,
	SRE_BSP_LUMP_LEAF_FACES,
	SRE_BSP_LUMP_LEAF_BRUSHES,
	SRE_BSP_LUMP_EDGES,
	SRE_BSP_LUMP_SURFACE_EDGES,
	SRE_BSP_LUMP_MODELS,
	SRE_BSP_LUMP_BURSHES,
	SRE_BSP_LUMP_BURSH_SIDES,
	SRE_BSP_LUMP_POP,
	SRE_BSP_LUMP_AREAS,
	SRE_BSP_LUMP_AREA_PORTALS,
	SRE_BSP_NUMBER_OF_LUMBS
} SRE_BSP_LUMP;

typedef struct
{
	int32_t offset;
	int32_t length;
} sre_bsp_lump_t;

typedef struct
{
	int32_t	identifier;
	int32_t	version;
	sre_bsp_lump_t lumps [SRE_BSP_NUMBER_OF_LUMBS];
} sre_bsp_header_t;

typedef struct
{
	jmp_float3_t	min;
	jmp_float3_t	max;
	jmp_float3_t	origin;

	int32_t			head_node;
	int32_t			first_face;
	int32_t			number_of_faces;
} sre_bsp_model_t;

typedef enum
{
	SRE_BSP_PLANE_X = 0,
	SRE_BSP_PLANE_Y,
	SRE_BSP_PLANE_Z,
	SRE_BSP_PLANE_ANY_X,
	SRE_BSP_PLANE_ANY_Y,
	SRE_BSP_PLANE_ANY_Z
} SRE_BSP_PLANE;

typedef struct
{
	jmp_float3_t	normal;
	float			distance;
	int32_t			type;
} sre_bsp_plane_t;


typedef enum
{
	SRE_BSP_CONTENTS_PLAYER_CLIP	= 0x1000,
	SRE_BSP_CONTENTS_MONSTER_CLIP	= 0x2000,
	SRE_BSP_CONTENTS_AREA_PORTAL	= 0x8000,

	SRE_BSP_CONTENTS_CURRENT_0		= 0x40000,
	SRE_BSP_CONTENTS_CURRENT_90		= 0x80000,

	SRE_BSP_CONTENTS_CURRENT_180	= 0x100000,
	SRE_BSP_CONTENTS_CURRENT_270	= 0x200000,
	SRE_BSP_CONTENTS_CURRENT_UP		= 0x400000,
	SRE_BSP_CONTENTS_CURRENT_DOWN	= 0x800000,

	SRE_BSP_CONTENTS_CURRENT_ORIGIN	= 0x1000000,
	SRE_BSP_CONTENTS_MONSTER		= 0x2000000,
	SRE_BSP_CONTENTS_DEAD_MONSTER	= 0x4000000,
	SRE_BSP_CONTENTS_DETAIL			= 0x8000000,
	SRE_BSP_CONTENTS_TRANSLUCENT	= 0x10000000,
	SRE_BSP_CONTENTS_LADDER			= 0x20000000,
} SRE_BSP_CONTENTS;

typedef enum
{
	SRE_BSP_SURFACE_LIGHT			= 0x01,
	SRE_BSP_SURFACE_SLICK			= 0x02,
	SRE_BSP_SURFACE_SKY				= 0x04,
	SRE_BSP_SURFACE_WARP			= 0x08,
	SRE_BSP_SURFACE_TRANSPARENT33	= 0x10,
	SRE_BSP_SURFACE_TRANSPARENT66	= 0x20,
	SRE_BSP_SURFACE_FLOWING			= 0x40,
	SRE_BSP_SURFACE_NO_DRAW			= 0x80
} SRE_BSP_SURFACE;

typedef struct
{
	int32_t		plane_number;
	int32_t		children [2];
	int16_t		mins [3];
	int16_t		maxs [3];
	uint16_t	first_face;
	uint16_t	number_of_faces;
} sre_bsp_node_t;

typedef struct
{
	float	vectors [2][4];
	int32_t	flags;
	int32_t value;
	char	texture [32];
	int32_t	next_texture_info;
} sre_bsp_texture_info_t;

typedef struct
{
	uint16_t v [2];
} sre_bsp_edge_t;

SRE_EXTERN_C_BEGIN



SRE_EXTERN_C_END
#endif // SRE_BSP_H
