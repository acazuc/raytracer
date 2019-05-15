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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, raytracer->getWidth(), raytracer->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, raytracer->getImgData());
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

static void run()
{
	raytracer->render();
}

int main()
{
	if (!glfwInit())
		ERROR("Can't init glfw");
	createWindow();
	raytracer = new Raytracer(WINDOW_WIDTH, WINDOW_HEIGHT);
	raytracer->setFSAA(4);
	raytracer->setFov(60);
	raytracer->setAmbient(Vec3(.2));
	raytracer->setPos(Vec3(0, 0, -15));
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

	Sphere *sphere = new Sphere(2);
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
	light->pos = Vec3(-1, 0, 0);
	light->intensity = .8;
	light->color = Vec3(1, 1, 1);
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
	{
		//Right
		Plane *p1 = new Plane();
		p1->pos = Vec3(5, 0, 0);
		p1->setRot(Vec3(M_PI / 2, M_PI / 2, 0));
		p1->Kd = Vec4(0, 1, 0, 1);
		raytracer->addObject(p1);
		//Left
		Plane *p2 = new Plane();
		p2->pos = Vec3(-5, 0, 0);
		p2->setRot(Vec3(-M_PI / 2, -M_PI / 2, 0));
		p2->Kd = Vec4(1, 0, 0, 1);
		raytracer->addObject(p2);
		//Front
		Plane *p3 = new Plane();
		p3->pos = Vec3(0, 0, 5);
		p3->setRot(Vec3(M_PI / 2, 0, 0));
		p3->Kd = Vec4(1, 1, 1, 1);
		//p3->Ir = 1;
		raytracer->addObject(p3);
		//Top
		Plane *p4 = new Plane();
		p4->pos = Vec3(0, 5, 0);
		p4->setRot(Vec3(0, 0, 0));
		p4->Kd = Vec4(1, 1, 1, 1);
		raytracer->addObject(p4);
		//Bot
		Plane *p5 = new Plane();
		p5->pos = Vec3(0, -5, 0);
		p5->setRot(Vec3(0, 0, 0));
		p5->Kd = Vec4(1, 1, 1, 1);
		raytracer->addObject(p5);
		//Behind
		Plane *p6 = new Plane();
		p6->pos = Vec3(0, 0, -20);
		p6->setRot(Vec3(M_PI / 2, 0, 0));
		p6->Kd = Vec4(1, 1, 1, 1);
		//p6->Ir = 1;
		raytracer->addObject(p6);
		Sphere *s1 = new Sphere(1.5);
		s1->pos = Vec3(3, -3.5, 1);
		s1->Kd = Vec4(1, 1, 1, 1);
		//s1->Ir = 1;
		raytracer->addObject(s1);
		Sphere *s2 = new Sphere(1.5);
		s2->pos = Vec3(-2, -3.5, -2);
		s2->Kd = Vec4(.2, .2, 1, 1);
		raytracer->addObject(s2);
		/*PonctualLight *l1 = new PonctualLight();
		l1->pos = Vec3(0, 4.5, -20 + 4.166 * 1);
		l1->intensity = 1;
		l1->color = Vec3(1, 0, 0);
		raytracer->addLight(l1);
		PonctualLight *l2 = new PonctualLight();
		l2->pos = Vec3(0, 4.5, -20 + 4.166 * 3);
		l2->intensity = 1;
		l2->color = Vec3(0, 1, 0);
		raytracer->addLight(l2);
		PonctualLight *l3 = new PonctualLight();
		l3->pos = Vec3(0, 4.5, -20 + 4.166 * 5);
		l3->intensity = 1;
		l3->color = Vec3(0, 0, 1);
		raytracer->addLight(l3);*/
		PonctualLight *l0 = new PonctualLight();
		l0->pos = Vec3(0, 4.5, -7.5);
		l0->intensity = 1;
		l0->color = Vec3(1, 1, 1);
		raytracer->addLight(l0);
	}
	std::thread *thread = new std::thread(run);
	(void)thread;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//static float a = 0;
		//light->pos = Vec3(cos(a) * 10, sin(a) * 10, 4);
		//sphere->setRot(Vec3(0, a, 0));
		//sphere->Ni = 2 + 1 * cos(a);
		//light->pos.z = cos(a * 3) * 10;
		//raytracer->setRot(Vec3(a, 0, 0));
		//a += M_PI / 2 / 20;
		//a++;
		//raytracer->render();
		//PNG::write(std::to_string(a) + ".png", (uint8_t*)raytracer->getImgData(), WINDOW_WIDTH, WINDOW_HEIGHT);
		draw();
		glfwPollEvents();
		glfwSwapBuffers(window);
		usleep(100000);
	}
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDeleteTextures(1, &texture);
	glfwTerminate();
	return EXIT_SUCCESS;
}
