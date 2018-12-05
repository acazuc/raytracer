#include "Main.h"
#include "Lights/DirectionalLight.h"
#include "Lights/PonctualLight.h"
#include "Objects/Cylinder.h"
#include "Objects/Triangle.h"
#include "Objects/Sphere.h"
#include "Objects/Plane.h"
#include "Objects/Cone.h"
#include "Raytracer.h"
#include "Debug.h"
#include "PNG.h"
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
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
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
	raytracer->setFov(30);
	raytracer->setAmbient(Vec3(0, 0, 0));
	raytracer->setPos(Vec3(0, 0, 0));
	raytracer->setRot(Vec3(0, 0, 0));
	/*for (uint64_t i = 0; i < 11 * 11; ++i)
	{
		Sphere *sphere = new Sphere(1);
		sphere->pos = Vec3((i % 11) - 5.5, (i / 11) - 5.5, 15);
		sphere->Kd = Vec4(1, 1, 0, 1);
		sphere->setRot(Vec3(0, 0, 0));
		raytracer->addObject(sphere);
	}*/
	Image earth;
	earth.filtering = IMAGE_FILTERING_NEAREST;
	uint8_t *data;
	uint32_t width;
	uint32_t height;
	if (!PNG::read(std::string("earth.png"), data, width, height))
		ERROR("failed to read texture");
	earth.setData(width, height, data);
	delete[] (data);
	Image bump;
	bump.filtering = IMAGE_FILTERING_CUBIC;
	if (!PNG::read(std::string("normal_7.png"), data, width, height))
		ERROR("failed to read bump map");
	bump.setData(width, height, data);
	delete[] (data);

	Sphere *sphere = new Sphere(1);
	//sphere->Kd_map = &earth;
	//sphere->N_map = &bump;
	sphere->pos = Vec3(0, 0, 10);
	sphere->setRot(Vec3(0, 0, 0));
	sphere->Kd = Vec4(1, 1, 1, 0);
	sphere->Ir = 0;
	sphere->Ni = 1;
	raytracer->addObject(sphere);

	Cylinder *cylinder = new Cylinder(1);
	//cylinder->Kd_map = &earth;
	//cylinder->N_map = &bump;
	cylinder->pos = Vec3(0, 0, 5);
	cylinder->Kd = Vec4(1, 1, 0, 1);
	//raytracer->addObject(cylinder);

	Cone *cone = new Cone(0);
	cone->pos = Vec3(0, 0, 13);
	cone->setRot(Vec3(0, 0, 0));
	cone->Kd = Vec4(0, 1, 1, 1);
	//raytracer->addObject(cone);

	Plane *plane = new Plane();
	plane->pos = Vec3(0, 0, 10);
	plane->setRot(Vec3(-M_PI / 2, 0, 0));
	plane->Kd = Vec4(1, 1, 1, 1);
	plane->Kd_map = &earth;
	//plane->N_map = &bump;
	raytracer->addObject(plane);

	plane = new Plane();
	plane->pos = Vec3(0, 0, 10);
	plane->setRot(Vec3(-M_PI / 2, 0, 0));
	plane->Kd = Vec4(1, 1, 1, 1);
	//raytracer->addObject(plane);
	/*Triangle *triangle = new Triangle();
	triangle->pos = Vec3(-1, 1, 6);
	triangle->rot = Vec3(0, 0, 0);
	triangle->pos2 = Vec3(0, -1, 6);
	triangle->pos3 = Vec3(1, 1, 6);
	triangle->color = Vec4(1, 1, 0, 1);
	triangle->specular = .5;
	raytracer->addObject(triangle);*/
	PonctualLight *light = new PonctualLight();
	light->pos = Vec3(0, 0, 0);
	light->intensity = 1;
	light->color = Vec3(1, 1, 1);
	raytracer->addLight(light);
	/*float b = 5;
	plane = new Plane();
	plane->pos = Vec3(0, 0, b);
	plane->setRot(Vec3(-M_PI / 2, 0, 0));
	plane->Kd = Vec4(1);
	raytracer->addObject(plane);
	light = new PonctualLight();
	light->pos = Vec3(-1.10, 0, b - .5);
	light->intensity = 1.5;
	light->color = Vec3(1, 0, 0);
	raytracer->addLight(light);
	light = new PonctualLight();
	light->pos = Vec3(0, 0, b - .5);
	light->intensity = 1.5;
	light->color = Vec3(0, 1, 0);
	raytracer->addLight(light);
	light = new PonctualLight();
	light->pos = Vec3(1.10, 0, b - .5);
	light->intensity = 1.5;
	light->color = Vec3(0, 0, 1);
	raytracer->addLight(light);
	sphere = new Sphere(10000);
	sphere->pos = Vec3(0, 70.8, 1);
	sphere->Kd = Vec4(1, 1, 1, 1);
	sphere->Ir = .5;
	raytracer->addObject(sphere);
	sphere = new Sphere(10000);
	sphere->pos = Vec3(0, -70.8, 1);
	sphere->Kd = Vec4(1, 1, 1, 1);
	sphere->Ir = .5;
	raytracer->addObject(sphere);*/
	/*DirectionalLight *dLight = new DirectionalLight();
	dLight->dir = Vec3(-1, -1, -1);
	dLight->intensity = .1;
	dLight->color = Vec3(1, 1, 1);
	raytracer->addLight(dLight);*/
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		static float a = 0;
		//light->pos = Vec3(cos(a) * 10, sin(a) * 10, 4);
		sphere->setRot(Vec3(0, a, 0));
		sphere->Ni = 2 + 1 * cos(a);
		//light->pos.z = cos(a * 3) * 10;
		//raytracer->setRot(Vec3(a, 0, 0));
		a += M_PI / 2 / 20;
		//a++;
		raytracer->render();
		//PNG::write(std::to_string(a) + ".png", (uint8_t*)raytracer->getImgData(), WINDOW_WIDTH, WINDOW_HEIGHT);
		draw();
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDeleteTextures(1, &texture);
	glfwTerminate();
	return EXIT_SUCCESS;
}
