#include <stdlib.h>
#define JMP_MATH_IMPLEMENTATION
#include "jmp_math.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "sre.h"

#define ZNEAR 0.1f
#define ZFAR 100.0f

SRE_API int32_t	sre_init	(uint32_t width, uint32_t height, sre_t **sre_out)
{
	sre_t *sre = (sre_t *) malloc (sizeof (sre_t));
	assert (sre != NULL);

	sre->screen.width = width;
	sre->screen.height = height;
	sre->screen.pixels = (uint8_t *) malloc (sre->screen.width * sre->screen.height * 4);
	assert (sre->screen.pixels != NULL);
	memset (sre->screen.pixels, 0x00, sre->screen.width * sre->screen.height * 4);

	sre->screen.depth_buffer = (float *) malloc (sizeof (float) * sre->screen.width * sre->screen.height);
	assert (sre->screen.depth_buffer != NULL);
	memset (sre->screen.depth_buffer, 0xFF, sizeof (float) * sre->screen.width * sre->screen.height);

	sre->projection = jmp_perspective_projection (45.0f, ((float)sre->screen.width / (float)sre->screen.height), ZNEAR, ZFAR);
	sre->view = jmp_float4x4_diagonal (1.0f);
	sre->model = jmp_float4x4_diagonal (1.0f);

	sre->active_texture.data = NULL;
	sre->active_texture.width = 0;
	sre->active_texture.height = 0;

	*sre_out = sre;
	return (1);
}

SRE_API	void	sre_quit	(sre_t *sre)
{
	
}

SRE_API void	sre_set_texture	(sre_t *sre, uint8_t *img, uint32_t w, uint32_t h)
{
	sre->active_texture.data = img;
	sre->active_texture.width = w;
	sre->active_texture.height = h;
}

SRE_API	void	sre_clear		(sre_t *sre)
{
	memset (sre->screen.pixels, 0x00, sre->screen.width * sre->screen.height * 4);
	memset (sre->screen.depth_buffer, 0xFF, sizeof (float) * sre->screen.width * sre->screen.height);
}

SRE_API void	sre_set_camera	(sre_t *sre, jmp_float3_t position, jmp_float3_t rotation)
{
	position.x *= -1;
	position.y *= -1;
	position.z *= -1;

	sre->view = jmp_float4x4_diagonal (1.0f);
	sre->view = jmp_float4x4_mul (jmp_float4x4_mul (jmp_float4x4_mul (jmp_rotate_x (rotation.x), jmp_rotate_y (rotation.y)), jmp_rotate_z (rotation.z)), jmp_translate (position));
}

SRE_API void	sre_set_model	(sre_t *sre, jmp_float3_t position, jmp_float3_t rotation)
{
	sre->model = jmp_float4x4_diagonal (1.0f);
	sre->model = jmp_float4x4_mul (jmp_translate (position), jmp_float4x4_mul (jmp_float4x4_mul (jmp_rotate_x (rotation.x), jmp_rotate_y (rotation.y)), jmp_rotate_z (rotation.z)));
}

SRE_API	void	sre_draw	(sre_t *sre, uint16_t count, sre_vertex_t *vertices)
{
	count -= (count % 3);
	jmp_float2_t texture_size = jmp_float2 (sre->active_texture.width, sre->active_texture.height);

	jmp_float4x4_t mvp	= sre->projection;
	mvp = jmp_float4x4_mul (mvp, jmp_float4x4_mul (sre->view, sre->model));
	
	uint32_t triangles = 0;
	uint16_t i;
	for (i = 0; i < count; i += 3)
	{
		jmp_float3_t a = vertices [i + 0].position;
		jmp_float3_t b = vertices [i + 1].position;
		jmp_float3_t c = vertices [i + 2].position;

		jmp_float3_t color_0 = vertices [i + 0].color;
		jmp_float3_t color_1 = vertices [i + 1].color;
		jmp_float3_t color_2 = vertices [i + 2].color;

		jmp_float2_t uv_coords [3] = {
			vertices [i + 0].uv,
			vertices [i + 1].uv,
			vertices [i + 2].uv,
		};

		jmp_float4_t pa = jmp_float4x4_mul_float4 (mvp, jmp_float4 (a.x, a.y, a.z, 1.0f));
		jmp_float4_t pb = jmp_float4x4_mul_float4 (mvp, jmp_float4 (b.x, b.y, b.z, 1.0f));
		jmp_float4_t pc = jmp_float4x4_mul_float4 (mvp, jmp_float4 (c.x, c.y, c.z, 1.0f));

		
		
		if (pa.z < ZNEAR || pa.z > ZFAR ||
			pb.z < ZNEAR || pb.z > ZFAR ||
			pc.z < ZNEAR || pc.z > ZFAR)
		{
			continue;
		}

		float az = pa.z;
		float bz = pb.z;
		float cz = pc.z;

		pa = jmp_float4_scale (pa, 1.0f / pa.w);
		pb = jmp_float4_scale (pb, 1.0f / pb.w);
		pc = jmp_float4_scale (pc, 1.0f / pc.w);

		if ((pa.x < -1.0f || pa.x > 1.0f || pa.y < -1.0f || pa.y > 1.0f) &&
			(pb.x < -1.0f || pb.x > 1.0f || pb.y < -1.0f || pb.y > 1.0f) &&
			(pc.x < -1.0f || pc.x > 1.0f || pc.y < -1.0f || pc.y > 1.0f))
		{
			continue;
		}

		jmp_float3_t U = jmp_float3_sub (pb.xyz, pa.xyz);
		jmp_float3_t V = jmp_float3_sub (pa.xyz, pc.xyz);
		jmp_float3_t normal = jmp_float3_normalize (jmp_float3_cross (U, V));
		float dot = jmp_float3_dot (normal, jmp_float3 (0, 0, -1.0f));
		if (dot >= 0.0f)
		{
			++ triangles;
			jmp_float2_t half = jmp_float2 (sre->screen.width / 2, sre->screen.height / 2);
			jmp_float2_t vertices [3] = {
				jmp_float2 (pa.x, pa.y),
				jmp_float2 (pb.x, pb.y),
				jmp_float2 (pc.x, pc.y),
			};

			

		

			vertices [0].x *= half.x;
			vertices [0].y *= half.y;
			vertices [0] = jmp_float2_add (vertices [0], half);

			vertices [1].x *= half.x;
			vertices [1].y *= half.y;
			vertices [1] = jmp_float2_add (vertices [1], half);

			vertices [2].x *= half.x;
			vertices [2].y *= half.y;
			vertices [2] = jmp_float2_add (vertices [2], half);
			
			jmp_float2_t ab = jmp_float2_sub (vertices [1], vertices [0]);
			jmp_float2_t bc = jmp_float2_sub (vertices [2], vertices [1]);
			jmp_float2_t ca = jmp_float2_sub (vertices [0], vertices [2]);

			jmp_float2_t min = vertices [0];
			jmp_float2_t max = vertices [0];

			if (min.x > vertices [1].x) { min.x = vertices [1].x; }
			if (min.x > vertices [2].x) { min.x = vertices [2].x; }
			if (min.y > vertices [1].y) { min.y = vertices [1].y; }
			if (min.y > vertices [2].y) { min.y = vertices [2].y; }
			if (max.x < vertices [1].x) { max.x = vertices [1].x; }
			if (max.x < vertices [2].x) { max.x = vertices [2].x; }
			if (max.y < vertices [1].y) { max.y = vertices [1].y; }
			if (max.y < vertices [2].y) { max.y = vertices [2].y; }

			if (min.x < 0.0f) { min.x = 0.0f; }
			if (min.y < 0.0f) { min.y = 0.0f; }
			if (max.x > (float) (sre->screen.width)) { max.x = (float) (sre->screen.width); }
			if (max.y > (float) (sre->screen.height)) { max.y = (float) (sre->screen.height); }
			
			int32_t min_x = min.x - 1;
			int32_t min_y = min.y - 1;
			int32_t max_x = max.x + 1;
			int32_t max_y = max.y + 1;

			int32_t x = 0;
			int32_t y = 0;
			for (y = min_y - 1; y < max_y; ++ y)
			{
				for (x = min_x - 1; x < max_x; ++ x)
				{
					jmp_float2_t ap = jmp_float2 (x - vertices [0].x, y - vertices [0].y);
					jmp_float2_t bp = jmp_float2 (x - vertices [1].x, y - vertices [1].y);
					jmp_float2_t cp = jmp_float2 (x - vertices [2].x, y - vertices [2].y);

					int32_t d1 = (ab.x * ap.y - ab.y * ap.x);
					int32_t d2 = (bc.x * bp.y - bc.y * bp.x);
					int32_t d3 = (ca.x * cp.y - ca.y * cp.x);

					if (x >= 0 && x < sre->screen.width &&
						y >= 0 && y < sre->screen.height)
					{
						if ((d1 | d2 | d3) >= 0)
						{
							size_t index = (x * 4) + ((sre->screen.height - 1 - y) * sre->screen.width * 4);
							size_t depth_index = x + ((sre->screen.height - 1 - y) * sre->screen.width);

							jmp_float2_t p = jmp_float2 (x, y);
						
							float w0 = ((vertices [1].y - vertices [2].y) * (p.x - vertices [2].x) + (vertices [2].x - vertices [1].x) * (p.y - vertices [2].y)) /
								((vertices [1].y - vertices [2].y) * (vertices [0].x - vertices [2].x) + (vertices [2].x - vertices [1].x) * (vertices [0].y - vertices [2].y));

							float w1 = ((vertices [2].y - vertices [0].y) * (p.x - vertices [2].x) + (vertices [0].x - vertices [2].x) * (p.y - vertices [2].y)) /
								((vertices [1].y - vertices [2].y) * (vertices [0].x - vertices [2].x) + (vertices [2].x - vertices [1].x) * (vertices [0].y - vertices [2].y));

							float w2 = 1.0f - w0 - w1;

							float depth = w0 * az + w1 * bz + w2 * cz;
							if (sre->screen.depth_buffer [depth_index] <= depth)
							{
								continue;
							}

							sre->screen.depth_buffer [depth_index] = depth;
						

							jmp_float3_t final_color = {(color_0.x * w0) + (color_1.x * w1) + (color_2.x * w2),
														(color_0.y * w0) + (color_1.y * w1) + (color_2.y * w2),
														(color_0.z * w0) + (color_1.z * w1) + (color_2.z * w2)};

							if (sre->active_texture.data != NULL && sre->active_texture.width != 0 && sre->active_texture.height != 0)
							{
								jmp_float2_t st = jmp_float2 ((uv_coords [0].x * w0) + (uv_coords [1].x * w1) + (uv_coords [2].x * w2), (uv_coords [0].y * w0) + (uv_coords [1].y * w1) + (uv_coords [2].y * w2));
								int32_t pixel_x = (int32_t) ((st.x * texture_size.x));
								int32_t pixel_y = (int32_t) ((st.y * texture_size.y));

								if (pixel_x < 0 || pixel_x >= sre->active_texture.width ||
									pixel_y < 0 || pixel_y >= sre->active_texture.height)
								{
									continue;
								}

								size_t pixel_index = (pixel_x * 4) + (pixel_y * sre->active_texture.width * 4);
								final_color.x *= ((float)sre->active_texture.data [pixel_index + 0]) / 255.0f;
								final_color.y *= ((float)sre->active_texture.data [pixel_index + 1]) / 255.0f;
								final_color.z *= ((float)sre->active_texture.data [pixel_index + 2]) / 255.0f;
							}


							uint8_t r = (uint8_t) (final_color.x * 255.0f);
							uint8_t g = (uint8_t) (final_color.y * 255.0f);
							uint8_t b = (uint8_t) (final_color.z * 255.0f);
							uint8_t a = 0xFF;

							sre->screen.pixels [index + 0] = r;
							sre->screen.pixels [index + 1] = g;
							sre->screen.pixels [index + 2] = b;
							sre->screen.pixels [index + 3] = a;
						}
					}
				}
			}
		}
	}

	printf ("%i                  \r", triangles);
}
