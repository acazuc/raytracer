#include "Main.h"
#include "Lights/DirectionalLight.h"
#include "Lights/PonctualLight.h"
#include "Objects/Cylinder.h"
#include "Objects/Sphere.h"
#include "Objects/Plane.h"
#include "Objects/Cone.h"
#include "Raytracer.h"
#include "Debug.h"
#include <GLFW/glfw3.h>
#include <cstring>
#include <cmath>

static Raytracer *raytracer;
static GLFWwindow *window = NULL;
static GLuint texture;

static void createWindow()
{
	if (!(window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Raytracer", NULL, NULL)))
		ERROR("Window: can't create window");
	glfwSwapInterval(1);
	glfwMakeContextCurrent(window);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

static void draw()
{
	int coords[4 * 2];
	int vertex[4 * 2];
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, raytracer->getImgData());
	std::memset(coords, 0, sizeof(coords));
	coords[2] = 1;
	coords[4] = 1;
	coords[5] = 1;
	coords[7] = 1;
	std::memset(vertex, 0, sizeof(vertex));
	vertex[2] = WINDOW_WIDTH;
	vertex[4] = WINDOW_WIDTH;
	vertex[5] = WINDOW_HEIGHT;
	vertex[7] = WINDOW_HEIGHT;
	glVertexPointer(2, GL_INT, 0, vertex);
	glTexCoordPointer(2, GL_INT, 0, coords);
	glDrawArrays(GL_QUADS, 0, 4);
}

int main()
{
	if (!glfwInit())
		ERROR("Can't init glfw");
	createWindow();
	raytracer = new Raytracer(WINDOW_WIDTH, WINDOW_HEIGHT);
	raytracer->setFov(60);
	raytracer->setAmbient(Vec3(.01, .01, .01));
	raytracer->setPos(Vec3(0, 0, 0));
	raytracer->setRot(Vec3(0, 0, 0));
	/*for (uint64_t i = 0; i < 11 * 11; ++i)
	{
		Sphere *sphere = new Sphere(1);
		sphere->pos = Vec3((i % 11) * 3, 0, 10 + (i / 11) * 3);
		sphere->color = Vec4(1, 1, 1, 1);
		sphere->reflection = 0;
		raytracer->addObject(sphere);
	}*/
	Sphere *sphere = new Sphere(1);
	sphere->pos = Vec3(0, -1.5, 8);
	sphere->color = Vec4(1, 1, 1, 1);
	sphere->reflection = 0;
	sphere->refraction = 0;
	raytracer->addObject(sphere);
	Cylinder *cylinder = new Cylinder(1);
	cylinder->pos = Vec3(3, 0, 13);
	cylinder->color = Vec4(1, 1, 0, 1);
	raytracer->addObject(cylinder);
	Cone *cone = new Cone(0);
	cone->pos = Vec3(-3, 0, 13);
	cone->rot = Vec3(0, 0, 0);
	cone->color = Vec4(1, 1, 1, 1);
	raytracer->addObject(cone);
	Plane *plane = new Plane();
	plane->pos = Vec3(0, -2, 0);
	plane->color = Vec4(1, 1, 1, 1);
	plane->specular = 0;
	raytracer->addObject(plane);
	plane = new Plane();
	plane->pos = Vec3(0, 0, 20);
	plane->rot = Vec3(-90, 0, 0);
	plane->color = Vec4(1, 1, 1, 1);
	plane->specular = 0;
	raytracer->addObject(plane);
	PonctualLight *light = new PonctualLight();
	light->pos = Vec3(10, -1, -10);
	light->intensity = 1;
	light->color = Vec3(1, 1, 1);
	raytracer->addLight(light);
	/*DirectionalLight *dLight = new DirectionalLight();
	dLight->dir = Vec3(-1, -1, -1);
	dLight->intensity = .5;
	dLight->color = Vec3(1, 1, 1);
	raytracer->addLight(dLight);*/
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	raytracer->render();
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		draw();
		glfwSwapBuffers(window);
	}
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glfwTerminate();
	return (EXIT_SUCCESS);
}
