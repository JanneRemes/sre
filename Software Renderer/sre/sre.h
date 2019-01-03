#ifndef SRE_H
#define SRE_H

#include "begin.h"

typedef struct sre_t
{
	struct
	{
		uint32_t	width;
		uint32_t	height;
		uint8_t		*pixels;
		float		*depth_buffer;
	} screen;

	jmp_float4x4_t	view;
	jmp_float4x4_t	model;
	jmp_float4x4_t	projection;

	struct
	{
		uint8_t *data;
		uint32_t width;
		uint32_t height;
	} active_texture;
} sre_t;

typedef struct
{
	jmp_float3_t position;
	jmp_float3_t color;
	jmp_float2_t uv;
} sre_vertex_t;

SRE_EXTERN_C_BEGIN

SRE_API int32_t	sre_init	(uint32_t width, uint32_t height, sre_t **sre);
SRE_API	void	sre_quit	(sre_t *sre);

SRE_API void	sre_set_texture	(sre_t *sre, uint8_t *img, uint32_t w, uint32_t h);
SRE_API	void	sre_clear		(sre_t *sre);

SRE_API void	sre_set_camera	(sre_t *sre, jmp_float3_t position, jmp_float3_t rotation);
SRE_API void	sre_set_model	(sre_t *sre, jmp_float3_t position, jmp_float3_t rotation);
SRE_API	void	sre_draw		(sre_t *sre, uint16_t count, sre_vertex_t *vertices);

SRE_EXTERN_C_END
#endif // SRE_H
