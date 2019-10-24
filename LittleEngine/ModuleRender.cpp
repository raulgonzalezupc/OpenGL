#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "Application.h"
#include "SDL.h"
#include "glew-2.1.0/include/GL/glew.h"
#include "MathGeoLib/include/Math/float4.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/float4x4.h"
#include "MathGeoLib/include/Math/float3x3.h"
#include "MathGeoLib/include/Geometry/Frustum.h"

ModuleRender::ModuleRender()
{
}

// Destructor
ModuleRender::~ModuleRender()
{
}

// Called before render is available
bool ModuleRender::Init()
{	
	
	LOG("Creating Renderer context");
	
	LOG("Using Glew %s", glewGetString(GLEW_VERSION)); 
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY); 
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24); 
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8); 
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4); 
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	SDL_GLContext glcontext = SDL_GL_CreateContext(App->window->window);
	glewInit();
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); 
	glClearDepth(1.0f); 
	glClearColor(0.f, 0.f, 0.f, 1.f); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
	glEnable(GL_DEPTH_TEST); 
	glFrontFace(GL_CCW); 
	glEnable(GL_CULL_FACE); 
	glEnable(GL_TEXTURE_2D); 
	float buffer_data[] = { -1.0f, -1.0f, 0.0f,
							 1.0f, -1.0f, 0.0f,       
							 0.0f,  1.0f, 0.0f };


	float aspect = SCREEN_WIDTH / SCREEN_HEIGHT;
	Frustum frustum;
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = math::float3::zero; 
	frustum.front = -math::float3::unitZ;
	frustum.up = math::float3::unitY;
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 100.0f;
	frustum.verticalFov = PI / 4.0f;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * aspect);
	math::float4x4 proj = frustum.ProjectionMatrix();
	math::float4x4 model = math::float4x4::FromTRS(math::float3(0.0f, 0.0f, -4.0f), math::float3x3::RotateY(PI / 4.0f), math::float3(1.0f, 1.0f, 1.0f));
	float4x4 view = float4x4::LookAt(math::float3(0.0f,0.0f,-1.0f), math::float3(0.0f, 0.0f, -1.0f), math::float3(0.0f, 1.0f, 0.0f), math::float3(0.0f, 1.0f, 0.0f));
	math::float4x4 transform = proj * view * float4x4(model);

	//translation
	float4 vertex0(buffer_data[0], buffer_data[1], buffer_data[2], 1.0f);
	float4 tmp0 = transform * vertex0;
	float3 finalVertex0 (tmp0.x / tmp0.w, tmp0.y / tmp0.w, tmp0.z / tmp0.w);
	buffer_data[0] = finalVertex0.x;
	buffer_data[1] = finalVertex0.y;
	buffer_data[2] = finalVertex0.z;

	float4 vertex1(buffer_data[3], buffer_data[4], buffer_data[5], 1.0f);
	float4 tmp1 = transform * vertex1;
	float3 finalVertex1(tmp1.x / tmp1.w, tmp1.y / tmp1.w, tmp1.z / tmp1.w);
	buffer_data[3] = finalVertex1.x;
	buffer_data[4] = finalVertex1.y;
	buffer_data[5] = finalVertex1.z;

	float4 vertex2(buffer_data[6], buffer_data[7], buffer_data[8], 1.0f);
	float4 tmp2 = transform * vertex2;
	float3 finalVertex2(tmp2.x / tmp2.w, tmp2.y / tmp2.w, tmp2.z / tmp2.w);
	buffer_data[6] = finalVertex2.x;
	buffer_data[7] = finalVertex2.y;
	buffer_data[8] = finalVertex2.z;

	GLuint vbo; 
	glGenBuffers(1, &vbo); 
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(buffer_data), buffer_data, GL_STATIC_DRAW); 
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glEnableVertexAttribArray(0); // attribute 0         
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	



	return true;
}

update_status ModuleRender::PreUpdate()
{
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRender::Update()
{
	glDrawArrays(GL_TRIANGLES, 0, 3);
	

	
	
	return UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate()
{
	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::CleanUp()
{
	LOG("Destroying renderer");
	
	//Destroy window
	glDisableVertexAttribArray(0);   
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return true;
}

void ModuleRender::WindowResized(unsigned width, unsigned height)
{
}

