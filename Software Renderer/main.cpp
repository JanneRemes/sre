#include <SDL.h>
#include "sre/sre.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

int main (int argc, char **argv)
{
	SDL_Init (SDL_INIT_EVERYTHING);
	SDL_Window *window = SDL_CreateWindow ("Danger Noodle", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);
	SDL_Renderer *renderer = SDL_CreateRenderer (window, -1, SDL_RENDERER_ACCELERATED);

	int32_t channels = 0;
	int32_t image_w = 0;
	int32_t image_h = 0;
	uint8_t *image_pixels = (uint8_t *) stbi_load ("data/test.tga", &image_w, &image_h, &channels, STBI_rgb_alpha);

	sre_t *sre = NULL;
	sre_init (1280 / 2, 720 / 2, &sre);

	SDL_Texture *target = SDL_CreateTexture (renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, sre->screen.width, sre->screen.height);

	const aiScene *scene = aiImportFile ("data/test.obj", aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_Triangulate | aiProcess_GenSmoothNormals);
	assert (scene != NULL);

	uint16_t vertex_count = 0;
	sre_vertex_t *vertices = NULL;

	for (uint32_t i = 0; i < scene->mNumMeshes; ++ i)
	{
		if (i > 0)
		{
			break;
		}

		aiMesh *mesh = scene->mMeshes [i];
		assert (mesh != NULL);

		vertices = (sre_vertex_t *) malloc (sizeof (sre_vertex_t) * (size_t) (mesh->mNumFaces * 3));
		if (vertices == NULL)
		{
			exit (-1);
		}

		const aiVector3D zero_vector (0.0f, 0.0f, 0.0f);
		for (uint32_t j = 0; j < mesh->mNumFaces; ++ j)
		{
			const aiFace *face = &mesh->mFaces [j];
			assert (face->mNumIndices == 3);

			vertices [vertex_count + 0].position = jmp_float3 (mesh->mVertices [face->mIndices [2]].x, mesh->mVertices [face->mIndices [2]].y, mesh->mVertices [face->mIndices [2]].z);
			vertices [vertex_count + 0].color = jmp_float3 (1.0f, 1.0f, 1.0f);
			vertices [vertex_count + 0].uv = jmp_float2 (0.0f, 0.0f);

			vertices [vertex_count + 1].position = jmp_float3 (mesh->mVertices [face->mIndices [1]].x, mesh->mVertices [face->mIndices [1]].y, mesh->mVertices [face->mIndices [1]].z);
			vertices [vertex_count + 1].color = jmp_float3 (1.0f, 1.0f, 1.0f);
			vertices [vertex_count + 1].uv = jmp_float2 (0.0f, 0.0f);

			vertices [vertex_count + 2].position = jmp_float3 (mesh->mVertices [face->mIndices [0]].x, mesh->mVertices [face->mIndices [0]].y, mesh->mVertices [face->mIndices [0]].z);
			vertices [vertex_count + 2].color = jmp_float3 (1.0f, 1.0f, 1.0f);
			vertices [vertex_count + 2].uv = jmp_float2 (0.0f, 0.0f);

			if (mesh->HasTextureCoords (0))
			{
				vertices [vertex_count + 0].uv = jmp_float2 (mesh->mTextureCoords[0] [face->mIndices [2]].x, 1.0f - mesh->mTextureCoords[0] [face->mIndices [2]].y);
				vertices [vertex_count + 1].uv = jmp_float2 (mesh->mTextureCoords[0] [face->mIndices [1]].x, 1.0f - mesh->mTextureCoords[0] [face->mIndices [1]].y);
				vertices [vertex_count + 2].uv = jmp_float2 (mesh->mTextureCoords[0] [face->mIndices [0]].x, 1.0f - mesh->mTextureCoords[0] [face->mIndices [0]].y);
			}

			if (mesh->HasVertexColors (0))
			{
				vertices [vertex_count + 0].color = jmp_float3 (mesh->mColors [0][face->mIndices [2]].r, mesh->mColors [0][face->mIndices [2]].g, mesh->mColors [0][face->mIndices [2]].b);
				vertices [vertex_count + 1].color = jmp_float3 (mesh->mColors [0][face->mIndices [1]].r, mesh->mColors [0][face->mIndices [1]].g, mesh->mColors [0][face->mIndices [1]].b);
				vertices [vertex_count + 2].color = jmp_float3 (mesh->mColors [0][face->mIndices [0]].r, mesh->mColors [0][face->mIndices [0]].g, mesh->mColors [0][face->mIndices [0]].b);
			}

			vertex_count += 3;
		}
	}

	int32_t last_x = 0;
	int32_t last_y = 0;
	int32_t capture = 0;

	jmp_float3_t camera_position = jmp_float3 (0.0f, 0.0f, -10.0f);
	jmp_float3_t camera_rotation = jmp_float3 (0.0f, 0.0f, 0.0f);

	uint32_t open = (window != NULL);
	while (open)
	{
		SDL_Event event;
		while (SDL_PollEvent (&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
				{
					open = 0;
				} break;

				case SDL_KEYDOWN:
				{
					switch (event.key.keysym.sym)
					{
						case SDLK_SPACE:
						{
							capture = !capture;
							SDL_SetRelativeMouseMode ((SDL_bool) capture);
						} break;
					}
				} break;

				case SDL_MOUSEMOTION:
				{
					if (capture)
					{
						camera_rotation.x -= (10.0f * event.motion.yrel * 0.016f) * 3.1415926f / 180.0f;
						camera_rotation.y -= (10.0f * event.motion.xrel * 0.016f) * 3.1415926f / 180.0f;

						if (jmp_degrees (camera_rotation.x) > 90.0f)
						{
							camera_rotation.x = jmp_radians (90.0f);
						}

						else if (jmp_degrees (camera_rotation.x) < -90.0f)
						{
							camera_rotation.x = jmp_radians (-90.0f);
						}
					}
				} break;
			}
		}
		
		uint8_t *keyboard = (uint8_t *) SDL_GetKeyboardState (NULL);
		if (keyboard [SDL_SCANCODE_W])
		{
			if (jmp_degrees (camera_rotation.x) < 90.0f && jmp_degrees (camera_rotation.x) > -90.0f)
			{
				camera_position.z += cosf (camera_rotation.y + jmp_radians (0.0f)) * 10.0f * 0.016f;
				camera_position.x -= sinf (camera_rotation.y + jmp_radians (0.0f)) * 10.0f * 0.016f;
			}

			camera_position.y += sinf (camera_rotation.x + jmp_radians (0.0f)) * 10.0f * 0.016f;
		}

		if (keyboard [SDL_SCANCODE_S])
		{
			if (jmp_degrees (camera_rotation.x) < 90.0f && jmp_degrees (camera_rotation.x) > -90.0f)
			{
				camera_position.z += cosf (camera_rotation.y + jmp_radians (180.0f)) * 10.0f * 0.016f;
				camera_position.x -= sinf (camera_rotation.y + jmp_radians (180.0f)) * 10.0f * 0.016f;
			}

			camera_position.y -= sinf (camera_rotation.x + jmp_radians (0.0f)) * 10.0f * 0.016f;
		}

		if (keyboard [SDL_SCANCODE_A])
		{
			camera_position.z -= cosf (camera_rotation.y + jmp_radians (270.0f)) * 10.0f * 0.016f;
			camera_position.x += sinf (camera_rotation.y + jmp_radians (270.0f)) * 10.0f * 0.016f;
		}

		if (keyboard [SDL_SCANCODE_D])
		{
			camera_position.z -= cosf (camera_rotation.y + jmp_radians (90.0f)) * 10.0f * 0.016f;
			camera_position.x += sinf (camera_rotation.y + jmp_radians (90.0f)) * 10.0f * 0.016f;
		}

		SDL_SetRenderDrawColor (renderer, 0, 0, 0, 0xFF);
		SDL_RenderClear (renderer);

		sre_clear (sre);
		sre_set_camera (sre, camera_position, camera_rotation);
		sre_set_texture (sre, image_pixels, image_w, image_h);
		static float r = 0.0f;
		r += 0.016f;
		sre_set_model (sre, jmp_float3 (0.0f, 0.0f, 10.0f), jmp_float3 (0.0f, 0.0f, 0.0f));
		sre_draw (sre, vertex_count, vertices);

		uint32_t index = (sre->screen.width / 2) * 4 + (sre->screen.height / 2 * sre->screen.width * 4);
		sre->screen.pixels [index] = 0xFF;
		sre->screen.pixels [index + 1] = 0x00;
		sre->screen.pixels [index + 2] = 0x00;
		sre->screen.pixels [index + 3] = 0xFF;

		SDL_UpdateTexture (target, NULL, sre->screen.pixels, sre->screen.width * SRE_BYTES_PER_PIXEL);
		SDL_RenderCopy (renderer, target, NULL, NULL);

		SDL_RenderPresent (renderer);
		SDL_Delay (16);
	}

	sre_quit (sre);
	SDL_DestroyTexture (target);
	SDL_DestroyRenderer (renderer);
	SDL_DestroyWindow (window);
	SDL_Quit ();
	return (0);
}
