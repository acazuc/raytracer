#include "Main.h"
#include "Lights/DirectionalLight.h"
#include "Lights/PunctualLight.h"
#include "Objects/Cylinder.h"
#include "Objects/Triangle.h"
#include "Objects/Sphere.h"
#include "Objects/Plane.h"
#include "Objects/Cone.h"
#include "Raytracer.h"
#include "Parser.h"
#include "Debug.h"
#include "PNG.h"
#include <GLFW/glfw3.h>
#include <cstring>
#include <cmath>

static Raytracer *raytracer;
static GLFWwindow *window = NULL;
static GLuint texture;

static uint64_t nanotime()
{
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (ts.tv_sec * 1000000000 + ts.tv_nsec);
}

static void createWindow(size_t width, size_t height)
{
	if (!(window = glfwCreateWindow(width, height, "Raytracer", NULL, NULL)))
		ERROR("Window: can't create window");
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

static void draw()
{
	int width;
	int height;
	glfwGetFramebufferSize(window, &width, &height);
	float coords[4 * 2];
	float vertex[4 * 2];
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, height, 0, 0, 1);
	glColor3f(1, 1, 1);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, raytracer->getWidth(), raytracer->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, raytracer->getImgData());
	coords[0] = 0;
	coords[1] = 0;
	coords[2] = 1;
	coords[3] = 0;
	coords[4] = 1;
	coords[5] = 1;
	coords[6] = 0;
	coords[7] = 1;
	vertex[0] = (width - raytracer->getWidth()) / 2;
	vertex[1] = (height - raytracer->getHeight()) / 2;
	vertex[2] = (width + raytracer->getWidth()) / 2;
	vertex[3] = (height - raytracer->getHeight()) / 2;
	vertex[4] = (width + raytracer->getWidth()) / 2;
	vertex[5] = (height + raytracer->getHeight()) / 2;
	vertex[6] = (width - raytracer->getWidth()) / 2;
	vertex[7] = (height + raytracer->getHeight()) / 2;
	glVertexPointer(2, GL_FLOAT, 0, vertex);
	glTexCoordPointer(2, GL_FLOAT, 0, coords);
	glDrawArrays(GL_QUADS, 0, 4);
	glLineWidth(1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glColor3f(1, .5, 0);
	glBegin(GL_LINES);
	{
		std::lock_guard<std::mutex> guard(raytracer->getBatchesMutex());
		for (size_t y = 0; y < raytracer->getBatches().size(); ++y)
		{
			for (size_t x = 0; x < raytracer->getBatches()[y].size(); ++x)
			{
				if (raytracer->getBatches()[y][x] != BATCH_CALCULATING)
					continue;
				size_t startX = std::min(size_t(raytracer->getWidth() - 1), x * BATCH_SIZE);
				size_t startY = std::min(size_t(raytracer->getHeight() - 1), y * BATCH_SIZE);
				size_t endX = std::min(size_t(raytracer->getWidth() - 1), (x + 1) * BATCH_SIZE);
				size_t endY = std::min(size_t(raytracer->getHeight() - 1), (y + 1) * BATCH_SIZE);
				glVertex2f(startX + .5f, startY + .5f);
				glVertex2f(endX + .5f, startY + .5f);
				glVertex2f(endX + .5f, startY + .5f);
				glVertex2f(endX + .5f, endY + .5f);
				glVertex2f(endX + .5f, endY + .5f);
				glVertex2f(startX + .5f, endY + .5f);
				glVertex2f(startX + .5f, endY + .5f);
				glVertex2f(startX + .5f, startY + .5f);
			}
		}
	}
	glEnd();
}

static void run()
{
	raytracer->render();
}

int main(int ac, char **av)
{
	if (!glfwInit())
		ERROR("Can't init glfw");
	if (ac < 2)
		ERROR("raytracer <filename>");
	Parser *parser = new Parser(av[1]);
	if (!parser->parse())
		return EXIT_FAILURE;
	raytracer = parser->createRaytracer();
	createWindow(raytracer->getWidth(), raytracer->getHeight());
	/*raytracer = new Raytracer(WINDOW_WIDTH, WINDOW_HEIGHT);
	raytracer->setSamples(1);
	raytracer->setFov(60);
	raytracer->setAmbient(Vec3(.2));
	raytracer->setPos(Vec3(0, 0, -15));
	raytracer->setRot(Vec3(0, 0, 0));*/
	/*for (uint64_t i = 0; i < 11 * 11; ++i)
	{
		Sphere *sphere = new Sphere(1);
		sphere->pos = Vec3((i % 11) - 5.5, (i / 11) - 5.5, 15);
		sphere->Kd = Vec4(1, 1, 0, 1);
		sphere->setRot(Vec3(0, 0, 0));
		raytracer->addObject(sphere);
	}*/
	/*Image earth;
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
	delete[] (data);*/

	/*Sphere *sphere = new Sphere(2);
	//sphere->Kd_map = &earth;
	//sphere->N_map = &bump;
	sphere->pos = Vec3(1.5, 3, 8);
	sphere->setRot(Vec3(0, 0, 0));
	sphere->Kd = Vec4(1, 0, 0, 1);
	//sphere->Ir = 0;
	//sphere->Ni = 1;
	//raytracer->addObject(sphere);

	Cylinder *cylinder = new Cylinder(2);
	//cylinder->Kd_map = &earth;
	//cylinder->N_map = &bump;
	cylinder->pos = Vec3(10, -1, 7);
	cylinder->Kd = Vec4(1, 1, 0, 1);
	//raytracer->addObject(cylinder);

	Cone *cone = new Cone(0);
	cone->pos = Vec3(-5, 0, 13);
	cone->setRot(Vec3(0, 0, 0));
	cone->Kd = Vec4(0, 1, 1, 1);
	//raytracer->addObject(cone);

	Plane *plane = new Plane();
	plane->pos = Vec3(0, 0, 50);
	plane->setRot(Vec3(-M_PI / 2, 0, 0));
	plane->Kd = Vec4(1, 1, 1, 1);
	//plane->Kd_map = &earth;
	//plane->N_map = &bump;
	//raytracer->addObject(plane);

	plane = new Plane();
	plane->pos = Vec3(0, 0, 10);
	plane->setRot(Vec3(-M_PI / 2, 0, 0));
	plane->Kd = Vec4(1, 1, 1, 1);*/
	//raytracer->addObject(plane);
	/*Triangle *triangle = new Triangle();
	triangle->pos = Vec3(-1, 1, 6);
	triangle->rot = Vec3(0, 0, 0);
	triangle->pos2 = Vec3(0, -1, 6);
	triangle->pos3 = Vec3(1, 1, 6);
	triangle->color = Vec4(1, 1, 0, 1);
	triangle->specular = .5;
	raytracer->addObject(triangle);*/
	/*PonctualLight *light = new PonctualLight();
	light->pos = Vec3(-1, 0, 0);
	light->intensity = .8;
	light->color = Vec3(1, 1, 1);*/
	//raytracer->addLight(light);
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
	std::thread *thread = new std::thread(run);
	(void)thread;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	uint64_t interval = 1000000000 / 30;
	while (!glfwWindowShouldClose(window))
	{
		uint64_t started = nanotime();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		draw();
		glfwPollEvents();
		glfwSwapBuffers(window);
		uint64_t ended = nanotime();
		uint64_t elapsed = ended - started;
		if (elapsed > interval)
			continue;
		usleep((interval - elapsed) / 1000);
	}
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDeleteTextures(1, &texture);
	glfwTerminate();
	return EXIT_SUCCESS;
}
