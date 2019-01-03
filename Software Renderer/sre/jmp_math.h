#ifndef JMP_MATH_H
#define JMP_MATH_H

#include <immintrin.h>

typedef union
{
	struct
	{
		float x, y;
	};
} jmp_float2_t;

typedef union
{
	struct
	{
		float x, y, z;
	};

	struct
	{
		jmp_float2_t xy;
	};
} jmp_float3_t;

typedef union
{
	struct
	{
		float x, y, z, w;
	};

	struct
	{
		jmp_float2_t xy;
	};

	struct
	{
		jmp_float3_t xyz;
	};
} jmp_float4_t;

typedef union
{
	struct
	{
		float a, b;
		float c, d;		
	};

	float			elements [4];
	jmp_float2_t	rows [2];
} jmp_float2x2_t;

typedef union
{
	struct
	{
		float a, b, c;
		float d, e, f;
		float g, h, i;
	};

	float			elements [9];
	jmp_float3_t	rows [3];
} jmp_float3x3_t;

typedef union
{
	struct
	{
		float a, b, c, d;
		float e, f, g, h;
		float i, j, k, l;
		float m, n, o, p;
	};

	float			elements [16];
	jmp_float4_t	rows [4];
} jmp_float4x4_t;

#ifdef __cplusplus
extern "C" {
#endif

	float			jmp_radians				(float deg);
	float			jmp_degrees				(float rad);

	jmp_float2_t	jmp_float2				(float x, float y);
	jmp_float3_t	jmp_float3				(float x, float y, float z);
	jmp_float4_t	jmp_float4				(float x, float y, float z, float w);

	jmp_float2_t	jmp_float2_add			(jmp_float2_t a, jmp_float2_t b);
	jmp_float3_t	jmp_float3_add			(jmp_float3_t a, jmp_float3_t b);
	jmp_float4_t	jmp_float4_add			(jmp_float4_t a, jmp_float4_t b);

	jmp_float2_t	jmp_float2_addf			(jmp_float2_t a, float b);
	jmp_float3_t	jmp_float3_addf			(jmp_float3_t a, float b);
	jmp_float4_t	jmp_float4_addf			(jmp_float4_t a, float b);

	jmp_float2_t	jmp_float2_sub			(jmp_float2_t a, jmp_float2_t b);
	jmp_float3_t	jmp_float3_sub			(jmp_float3_t a, jmp_float3_t b);
	jmp_float4_t	jmp_float4_sub			(jmp_float4_t a, jmp_float4_t b);

	jmp_float2_t	jmp_float2_scale		(jmp_float2_t a, float scalar);
	jmp_float3_t	jmp_float3_scale		(jmp_float3_t a, float scalar);
	jmp_float4_t	jmp_float4_scale		(jmp_float4_t a, float scalar);

	float			jmp_float2_dot			(jmp_float2_t a, jmp_float2_t b);
	float			jmp_float3_dot			(jmp_float3_t a, jmp_float3_t b);
	float			jmp_float4_dot			(jmp_float4_t a, jmp_float4_t b);

	float			jmp_float2_length_sqr	(jmp_float2_t a);
	float			jmp_float3_length_sqr	(jmp_float3_t a);
	float			jmp_float4_length_sqr	(jmp_float4_t a);

	float			jmp_float2_length		(jmp_float2_t a);
	float			jmp_float3_length		(jmp_float3_t a);
	float			jmp_float4_length		(jmp_float4_t a);

	jmp_float2_t	jmp_float2_normalize	(jmp_float2_t a);
	jmp_float3_t	jmp_float3_normalize	(jmp_float3_t a);
	jmp_float4_t	jmp_float4_normalize	(jmp_float4_t a);

	jmp_float2_t	jmp_float2_perp			(jmp_float2_t a);
	jmp_float3_t	jmp_float3_cross		(jmp_float3_t a, jmp_float3_t b);


	jmp_float2x2_t	jmp_float2x2			(jmp_float2_t a, jmp_float2_t b);
	jmp_float2x2_t	jmp_float2x2_diagonal	(float d);

	jmp_float2x2_t	jmp_float2x2_transpose	(jmp_float2x2_t a);
	jmp_float2x2_t	jmp_float2x2_mul		(jmp_float2x2_t a, jmp_float2x2_t b);

	jmp_float3x3_t	jmp_float3x3			(jmp_float3_t a, jmp_float3_t b, jmp_float3_t c);
	jmp_float3x3_t	jmp_float3x3_diagonal	(float d);

	jmp_float3x3_t	jmp_float3x3_transpose	(jmp_float3x3_t a);
	jmp_float3x3_t	jmp_float3x3_mul		(jmp_float3x3_t a, jmp_float3x3_t b);

	jmp_float4x4_t	jmp_float4x4			(jmp_float4_t a, jmp_float4_t b, jmp_float4_t c, jmp_float4_t d);
	jmp_float4x4_t	jmp_float4x4_diagonal	(float d);

	jmp_float4x4_t	jmp_float4x4_transpose	(jmp_float4x4_t a);
	jmp_float4x4_t	jmp_float4x4_mul		(jmp_float4x4_t a, jmp_float4x4_t b);
	jmp_float4_t	jmp_float4x4_mul_float4 (jmp_float4x4_t matrix, jmp_float4_t v);


	jmp_float4x4_t	jmp_perspective_projection	(float fov, float aspect, float znear, float zfar);
	jmp_float4x4_t	jmp_translate				(jmp_float3_t t);
	jmp_float4x4_t	jmp_scale					(jmp_float3_t t);
	jmp_float4x4_t	jmp_rotate_x				(float angle);
	jmp_float4x4_t	jmp_rotate_y				(float angle);
	jmp_float4x4_t	jmp_rotate_z				(float angle);
#ifdef __cplusplus
}
#endif


#ifdef JMP_MATH_IMPLEMENTATION

#include <stdint.h>
#include <math.h>

float			jmp_radians				(float deg)
{
	return (deg * 3.1415926f / 180.0f);
}

float			jmp_degrees			(float rad)
{
	return (rad * 180.0f / 3.1415926f);
}

#ifndef JMP_MATH_NO_VECTORS
	jmp_float2_t jmp_float2 (float x, float y)
	{
		jmp_float2_t result = {x, y};
		return (result);
	}

	jmp_float3_t jmp_float3 (float x, float y, float z)
	{
		jmp_float3_t result = {x, y, z};
		return (result);
	}

	jmp_float4_t jmp_float4 (float x, float y, float z, float w)
	{
		jmp_float4_t result = {x, y, z, w};
		return (result);
	}

	jmp_float2_t jmp_float2_add (jmp_float2_t a, jmp_float2_t b)
	{
		a.x += b.x;
		a.y += b.y;
		return (a);
	}

	jmp_float3_t jmp_float3_add (jmp_float3_t a, jmp_float3_t b)
	{
		a.x += b.x;
		a.y += b.y;
		a.z += b.z;
		return (a);
	}

	jmp_float4_t jmp_float4_add (jmp_float4_t a, jmp_float4_t b)
	{
		a.x += b.x;
		a.y += b.y;
		a.z += b.z;
		a.w += b.w;
		return (a);
	}

	jmp_float2_t jmp_float2_addf (jmp_float2_t a, float b)
	{
		a.x += b;
		a.y += b;
		return (a);
	}

	jmp_float3_t jmp_float3_addf (jmp_float3_t a, float b)
	{
		a.x += b;
		a.y += b;
		a.z += b;
		return (a);
	}

	jmp_float4_t jmp_float4_addf (jmp_float4_t a, float b)
	{
		a.x += b;
		a.y += b;
		a.z += b;
		a.w += b;
		return (a);
	}


	jmp_float2_t jmp_float2_sub (jmp_float2_t a, jmp_float2_t b)
	{
		a.x -= b.x;
		a.y -= b.y;
		return (a);
	}

	jmp_float3_t jmp_float3_sub (jmp_float3_t a, jmp_float3_t b)
	{
		a.x -= b.x;
		a.y -= b.y;
		a.z -= b.z;
		return (a);
	}

	jmp_float4_t jmp_float4_sub (jmp_float4_t a, jmp_float4_t b)
	{
		a.x -= b.x;
		a.y -= b.y;
		a.z -= b.z;
		a.w -= b.w;
		return (a);
	}

	jmp_float2_t jmp_float2_scale (jmp_float2_t a, float scalar)
	{
		a.x *= scalar;
		a.y *= scalar;
		return (a);
	}

	jmp_float3_t jmp_float3_scale (jmp_float3_t a, float scalar)
	{
		a.x *= scalar;
		a.y *= scalar;
		a.z *= scalar;
		return (a);
	}

	jmp_float4_t jmp_float4_scale (jmp_float4_t a, float scalar)
	{
		a.x *= scalar;
		a.y *= scalar;
		a.z *= scalar;
		a.w *= scalar;
		return (a);
	}


	float jmp_float2_dot (jmp_float2_t a, jmp_float2_t b)
	{
		return (a.x * b.x + a.y * b.y);
	}

	float jmp_float3_dot (jmp_float3_t a, jmp_float3_t b)
	{
		return (a.x * b.x + a.y * b.y + a.z * b.z);
	}

	float jmp_float4_dot (jmp_float4_t a, jmp_float4_t b)
	{
		return (a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w);
	}


	float jmp_float2_length_sqr	(jmp_float2_t a)
	{
		return (a.x * a.x + a.y * a.y);
	}

	float jmp_float3_length_sqr	(jmp_float3_t a)
	{
		return (a.x * a.x + a.y * a.y + a.z * a.z);
	}

	float jmp_float4_length_sqr	(jmp_float4_t a)
	{
		return (a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w);
	}


	float jmp_float2_length (jmp_float2_t a)
	{
		return sqrtf (jmp_float2_length_sqr (a));
	}

	float jmp_float3_length (jmp_float3_t a)
	{
		return sqrtf (jmp_float3_length_sqr (a));
	}

	float jmp_float4_length (jmp_float4_t a)
	{
		return sqrtf (jmp_float4_length_sqr (a));
	}


	jmp_float2_t jmp_float2_normalize (jmp_float2_t a)
	{
		float length = jmp_float2_length (a);
		if (length > 0.0f)
		{
			length = 1.0f / length;
			a.x *= length;
			a.y *= length;
		}

		return (a);
	}

	jmp_float3_t jmp_float3_normalize (jmp_float3_t a)
	{
		float length = jmp_float3_length (a);
		if (length > 0.0f)
		{
			length = 1.0f / length;
			a.x *= length;
			a.y *= length;
			a.z *= length;
		}

		return (a);
	}

	jmp_float4_t jmp_float4_normalize (jmp_float4_t a)
	{
		float length = jmp_float4_length (a);
		if (length > 0.0f)
		{
			length = 1.0f / length;
			a.x *= length;
			a.y *= length;
			a.z *= length;
			a.w *= length;
		}

		return (a);
	}

	jmp_float2_t jmp_float2_perp (jmp_float2_t a)
	{
		jmp_float2_t result = {-a.y, a.x};
		return (result);
	}

	jmp_float3_t jmp_float3_cross (jmp_float3_t a, jmp_float3_t b)
	{
		jmp_float3_t result = {
			(a.y * b.z) - (a.z * b.y),
			(a.x * b.z) - (a.z * b.x),
			(a.x * b.y) - (a.y * b.x)
		};

		return (result);
	}
#endif

#ifndef JMP_MATH_NO_MATRICES

	jmp_float2x2_t jmp_float2x2 (jmp_float2_t a, jmp_float2_t b)
	{
		jmp_float2x2_t result;
		result.rows [0] = a;
		result.rows [1] = b;
		return (result);
	}

	jmp_float2x2_t jmp_float2x2_diagonal (float d)
	{
		jmp_float2x2_t result = {
			d, 0.0f,
			0.0f, d
		};

		return (result);
	}

	jmp_float2x2_t jmp_float2x2_transpose (jmp_float2x2_t a)
	{
		jmp_float2x2_t result = {
			a.a, a.c,
			a.b, a.d
		};

		return (result);
	}

	jmp_float2x2_t jmp_float2x2_mul (jmp_float2x2_t a, jmp_float2x2_t b)
	{
		b = jmp_float2x2_transpose (b);
		jmp_float2x2_t result = {
			jmp_float2_dot (a.rows [0], b.rows [0]), jmp_float2_dot (a.rows [0], b.rows [1]),
			jmp_float2_dot (a.rows [1], b.rows [0]), jmp_float2_dot (a.rows [1], b.rows [1]),
		};

		return (result);
	}


	jmp_float3x3_t jmp_float3x3 (jmp_float3_t a, jmp_float3_t b, jmp_float3_t c)
	{
		jmp_float3x3_t result;
		result.rows [0] = a;
		result.rows [1] = b;
		result.rows [2] = c;
		return (result);
	}

	jmp_float3x3_t jmp_float3x3_diagonal (float d)
	{
		jmp_float3x3_t result = {
			d, 0.0f, 0.0f,
			0.0f, d, 0.0f,
			0.0f, 0.0f, d
		};

		return (result);
	}

	jmp_float3x3_t jmp_float3x3_transpose (jmp_float3x3_t a)
	{
		jmp_float3x3_t result = {
			a.a, a.d, a.g,
			a.b, a.e, a.h,
			a.c, a.f, a.i
		};

		return (result);
	}

	jmp_float3x3_t jmp_float3x3_mul (jmp_float3x3_t a, jmp_float3x3_t b)
	{
		b = jmp_float3x3_transpose (b);
		jmp_float3x3_t result = {
			jmp_float3_dot (a.rows [0], b.rows [0]), jmp_float3_dot (a.rows [0], b.rows [1]), jmp_float3_dot (a.rows [0], b.rows [2]),
			jmp_float3_dot (a.rows [1], b.rows [0]), jmp_float3_dot (a.rows [1], b.rows [1]), jmp_float3_dot (a.rows [1], b.rows [2]),
			jmp_float3_dot (a.rows [2], b.rows [0]), jmp_float3_dot (a.rows [2], b.rows [1]), jmp_float3_dot (a.rows [2], b.rows [2]),
		};

		return (result);
	}

	jmp_float4x4_t jmp_float4x4 (jmp_float4_t a, jmp_float4_t b, jmp_float4_t c, jmp_float4_t d)
	{
		jmp_float4x4_t result;
		result.rows [0] = a;
		result.rows [1] = b;
		result.rows [2] = c;
		result.rows [3] = d;
		return (result);
	}

	jmp_float4x4_t jmp_float4x4_diagonal (float d)
	{
		jmp_float4x4_t result = {
			d, 0.0f, 0.0f, 0.0f,
			0.0f, d, 0.0f, 0.0f,
			0.0f, 0.0f, d, 0.0f,
			0.0f, 0.0f, 0.0f, d
		};

		return (result);
	}

	jmp_float4x4_t jmp_float4x4_transpose (jmp_float4x4_t a)
	{
		jmp_float4x4_t result = {
			a.a, a.e, a.i, a.m,
			a.b, a.f, a.j, a.n,
			a.c, a.g, a.k, a.o,
			a.d, a.h, a.l, a.p
		};

		return (result);
	}

	jmp_float4x4_t jmp_float4x4_mul (jmp_float4x4_t a, jmp_float4x4_t b)
	{
		b = jmp_float4x4_transpose (b);
		jmp_float4x4_t result = {
			jmp_float4_dot (a.rows [0], b.rows [0]), jmp_float4_dot (a.rows [0], b.rows [1]), jmp_float4_dot (a.rows [0], b.rows [2]), jmp_float4_dot (a.rows [0], b.rows [3]),
			jmp_float4_dot (a.rows [1], b.rows [0]), jmp_float4_dot (a.rows [1], b.rows [1]), jmp_float4_dot (a.rows [1], b.rows [2]), jmp_float4_dot (a.rows [1], b.rows [3]),
			jmp_float4_dot (a.rows [2], b.rows [0]), jmp_float4_dot (a.rows [2], b.rows [1]), jmp_float4_dot (a.rows [2], b.rows [2]), jmp_float4_dot (a.rows [2], b.rows [3]),
			jmp_float4_dot (a.rows [3], b.rows [0]), jmp_float4_dot (a.rows [3], b.rows [1]), jmp_float4_dot (a.rows [3], b.rows [2]), jmp_float4_dot (a.rows [3], b.rows [3]),
		};

		return (result);
	}

	jmp_float4_t jmp_float4x4_mul_float4 (jmp_float4x4_t matrix, jmp_float4_t v)
	{
		//matrix = jmp_float4x4_transpose (matrix);
		jmp_float4_t result = {
			jmp_float4_dot (matrix.rows [0], v),
			jmp_float4_dot (matrix.rows [1], v),
			jmp_float4_dot (matrix.rows [2], v),
			jmp_float4_dot (matrix.rows [3], v),
		};

		return (result);
	}

	jmp_float4x4_t jmp_perspective_projection (float fov, float aspect, float znear, float zfar)
	{
		float c =  1.0f / (tanf (((fov * 0.5f) * 3.1415926f / 180.0f)));
		jmp_float4x4_t result = {
			c / aspect,	0.0f,			0.0f,									0.0f,
			0.0f,		c,				0.0f,									0.0f,
			0.0f,		0.0f,			(zfar) / (zfar - znear),		-(zfar * znear) / (zfar - znear),
			0.0f,		0.0f,			1.0f,	0.0f
		};

		//result = jmp_float4x4_diagonal (1.0f);

		return (result);
	}

	jmp_float4x4_t jmp_translate (jmp_float3_t t)
	{
		jmp_float4x4_t result = {
			1.0f, 0.0f, 0.0f, t.x,
			0.0f, 1.0f, 0.0f, t.y,
			0.0f, 0.0f, 1.0f, t.z,
			0.0f, 0.0f, 0.0f, 1.0f
		};

		return (result);
	}

	jmp_float4x4_t jmp_scale (jmp_float3_t t)
	{
		jmp_float4x4_t result = {
			t.x, 0.0f, 0.0f, 0.0f,
			0.0f, t.y, 0.0f, 0.0f,
			0.0f, 0.0f, t.z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};

		return (result);
	}

	jmp_float4x4_t jmp_rotate_x (float angle)
	{
		float sin_theta = sinf (angle);
		float cos_theta = cosf (angle);
		jmp_float4x4_t result = {
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, cos_theta, -sin_theta, 0.0f,
			0.0f, sin_theta, cos_theta, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};

		return (result);
	}

	jmp_float4x4_t jmp_rotate_y (float angle)
	{

		float sin_theta = sinf (angle);
		float cos_theta = cosf (angle);
		jmp_float4x4_t result = {
			cos_theta, 0.0f, sin_theta, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			-sin_theta, 0.0f, cos_theta, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};

		return (result);
	}

	jmp_float4x4_t jmp_rotate_z (float angle)
	{
		float sin_theta = sinf (angle);
		float cos_theta = cosf (angle);
		jmp_float4x4_t result = {
			cos_theta, -sin_theta, 0.0f, 0.0f,
			sin_theta, cos_theta, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};

		return (result);
	}
#endif

#endif
#endif // JMP_MATH_H
