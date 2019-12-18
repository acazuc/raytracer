#include "Main.h"
#include "Parser/FileParser.h"
#include "Math/Quaternion.h"
#include "Utils/System.h"
#include "Raytracer.h"
#include "Verbose.h"
#include "PNG.h"
#include <GLFW/glfw3.h>
#include <cstring>
#include <cmath>

static Raytracer *raytracer;
static GLFWwindow *window = NULL;
static GLuint texture;
static float zoom = 1;
static int cursorState = 1;
static int windowWidth;
static int windowHeight;
static double prevCursorX;
static double prevCursorY;
static double cursorX;
static double cursorY;
static double prevOffsetX = 0;
static double prevOffsetY = 0;
static double offsetX = 0;
static double offsetY = 0;

static bool createWindow(size_t width, size_t height)
{
	if (!(window = glfwCreateWindow(width, height, "Raytracer", NULL, NULL)))
	{
		VERBOSE_ERROR("Window: can't create window");
		return false;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	return true;
}

static void updateTexture()
{
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, raytracer->getWidth(), raytracer->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, raytracer->getImgData().data());
}

static void draw()
{
	static int i = 0;
	if ((++i) & 1)
		updateTexture();
	glViewport(0, 0, windowWidth, windowHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, windowWidth, windowHeight, 0, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glColor3f(1, 1, 1);
	float displayWidth = raytracer->getWidth() * zoom;
	float displayHeight = raytracer->getHeight() * zoom;
	float x0 = (float(windowWidth) - displayWidth) / 2 + offsetX * zoom;
	float x1 = (float(windowWidth) + displayWidth) / 2 + offsetX * zoom;
	float y0 = (float(windowHeight) - displayHeight) / 2 + offsetY * zoom;
	float y1 = (float(windowHeight) + displayHeight) / 2 + offsetY * zoom;
	float coords[4 * 2];
	float vertex[4 * 2];
	coords[0] = 0;
	coords[1] = 0;
	coords[2] = 1;
	coords[3] = 0;
	coords[4] = 1;
	coords[5] = 1;
	coords[6] = 0;
	coords[7] = 1;
	vertex[0] = x0;
	vertex[1] = y0;
	vertex[2] = x1;
	vertex[3] = y0;
	vertex[4] = x1;
	vertex[5] = y1;
	vertex[6] = x0;
	vertex[7] = y1;
	glVertexPointer(2, GL_FLOAT, 0, vertex);
	glTexCoordPointer(2, GL_FLOAT, 0, coords);
	glBindTexture(GL_TEXTURE_2D, texture);
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
				if (raytracer->getBatches()[y][x] != BATCH_CALCULATING && raytracer->getBatches()[y][x] != BATCH_FILTERING)
					continue;
				ssize_t startX = x0 + std::min(displayWidth - 1, float(x * BATCH_SIZE) * zoom);
				ssize_t startY = y0 + std::min(displayHeight - 1, float(y * BATCH_SIZE) * zoom);
				ssize_t endX = x0 + std::min(displayWidth - 1, float((x + 1) * BATCH_SIZE) * zoom);
				ssize_t endY = y0 + std::min(displayHeight - 1, float((y + 1) * BATCH_SIZE) * zoom);
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
		glColor3f(1, 1, 1);
		glVertex2f(x0 - 1.5, y0 - 1.5);
		glVertex2f(x1 + 1.5, y0 - 1.5);
		glVertex2f(x1 + 1.5, y0 - 1.5);
		glVertex2f(x1 + 1.5, y1 + 1.5);
		glVertex2f(x1 + 1.5, y1 + 1.5);
		glVertex2f(x0 - 1.5, y1 + 1.5);
		glVertex2f(x0 - 1.5, y1 + 1.5);
		glVertex2f(x0 - 1.5, y0 - 1.5);
	}
	glEnd();
}

static void run()
{
	raytracer->render();
}

static void setZoom(float z)
{
	z = std::min(20.f, std::max(1.f / 20, z));
	if (zoom == z)
		return;
	float displayWidth = raytracer->getWidth() * zoom;
	float displayHeight = raytracer->getHeight() * zoom;
	float x0 = (float(windowWidth) - displayWidth) / 2 + offsetX * zoom;
	float y0 = (float(windowHeight) - displayHeight) / 2 + offsetY * zoom;
	double dx = (cursorX - x0) / zoom;
	double mx = cursorX - windowWidth / 2;
	double dy = (cursorY - y0) / zoom;
	double my = cursorY - windowHeight / 2;
	zoom = z;
	offsetX = -dx + mx / zoom + raytracer->getWidth() / 2;
	offsetY = -dy + my / zoom + raytracer->getHeight() / 2;
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (yoffset > 0)
		setZoom(zoom * 1.5);
	else if (yoffset < 0)
		setZoom(zoom / 1.5);
	(void)window;
	(void)xoffset;
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (action == GLFW_PRESS)
		{
			cursorState = 0;
			//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			glfwGetCursorPos(window, &prevCursorX, &prevCursorY);
			prevOffsetX = offsetX;
			prevOffsetY = offsetY;
		}
		else if (action == GLFW_RELEASE)
		{
			cursorState = 1;
			//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			//glfwSetCursorPos(window, prevCursorX, prevCursorY);
		}
	}
	(void)mods;
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	cursorX = xpos;
	cursorY = ypos;
	if (cursorState == 0)
	{
		offsetX = prevOffsetX + (xpos - prevCursorX) / zoom;
		offsetY = prevOffsetY + (ypos - prevCursorY) / zoom;
	}
	(void)window;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_0 && action == GLFW_PRESS)
	{
		setZoom(1);
		offsetX = 0;
		offsetY = 0;
	}
	(void)mods;
	(void)window;
	(void)scancode;
}

int main(int ac, char **av)
{
	Verbose::init();
	srand(time(nullptr));
	if (!glfwInit())
	{
		VERBOSE_ERROR("Can't init glfw");
		return EXIT_FAILURE;
	}
	if (ac < 2)
	{
		VERBOSE_ERROR("raytracer <filename>");
		return EXIT_FAILURE;
	}
	FileParser *parser = new FileParser(av[1]);
	{
		int64_t started = System::nanotime();
		if (!parser->parse())
			return EXIT_FAILURE;
		int64_t ended = System::nanotime();
		VERBOSE_INFO("Parsed scene in " << (ended - started) / 1000000 << " ms");
	}
	{
		int64_t started = System::nanotime();
		raytracer = parser->createRaytracer();
		int64_t ended = System::nanotime();
		VERBOSE_INFO("Created raytracer in " << (ended - started) / 1000000 << " ms");
	}
	if (!createWindow(raytracer->getWidth(), raytracer->getHeight()))
		return EXIT_FAILURE;
	std::thread *thread = new std::thread(run);
	(void)thread;
	glClearColor(.1, .1, .1, 1);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSwapInterval(-1);
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
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
