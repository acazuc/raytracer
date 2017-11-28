#include "Raytracer.h"
#include <cmath>

Raytracer::Raytracer(uint32_t width, uint32_t height)
: height(height)
, width(width)
{
	this->imgData = new char[width * height * 4];
}

Raytracer::~Raytracer()
{
	delete[] (this->imgData);
}

bool Raytracer::trace(Ray &ray, Object *&object, Vec3 &position)
{
	float nearestDistance = -1;
	for (uint64_t i = 0; i < this->objects.size(); ++i)
	{
		Vec3 *vec = this->objects[i]->collide(ray);
		if (!vec)
			continue;
		float length = vec->length();
		if (nearestDistance != -1 && length > nearestDistance)
			continue;
		object = this->objects[i];
		position = *vec;
		nearestDistance = length;
	}
	return (nearestDistance != -1);
}

void Raytracer::calculatePixel(uint64_t x, uint64_t y)
{
	Ray ray(Vec3(0), Vec3(0));
	float angleX = (x - this->width / 2.) / this->width * (this->fov / 180. * M_PI);
	float angleY = (y - this->height / 2.) / this->height * (this->fov / 180. * M_PI) / this->width * this->height;
	ray.dir.x = sin(angleY);
	ray.dir.y = sin(angleX);
	ray.dir.z = cos(angleX) * cos(angleY);
	Object *object = nullptr;
	Vec3 position;
	if (trace(ray, object, position))
	{
		this->imgData[(x + y * this->width) * 4 + 0] = object->color.r * 0xff;
		this->imgData[(x + y * this->width) * 4 + 1] = object->color.g * 0xff;
		this->imgData[(x + y * this->width) * 4 + 2] = object->color.b * 0xff;
		this->imgData[(x + y * this->width) * 4 + 3] = 0xff;
		return;
	}
	this->imgData[(x + y * this->width) * 4 + 0] = 0;
	this->imgData[(x + y * this->width) * 4 + 1] = 0;
	this->imgData[(x + y * this->width) * 4 + 2] = 0;
	this->imgData[(x + y * this->width) * 4 + 3] = 0xff;
}

void Raytracer::runThread(Raytracer *raytracer, uint64_t start, uint64_t end)
{
	for (uint64_t i = start; i < end; ++i)
	{
		raytracer->calculatePixel(i % raytracer->width, i / raytracer->width);
	}
}

void Raytracer::render()
{
	uint64_t total = this->width * this->height;
	for (uint8_t i = 0; i < THREAD_NUMBER; ++i)
	{
		uint64_t start = total * i / THREAD_NUMBER;
		uint64_t end = total * (i + 1) / THREAD_NUMBER;
		this->threads[i] = new std::thread(&Raytracer::runThread, this, start, end);
	}
	for (uint8_t i = 0; i < THREAD_NUMBER; ++i)
	{
		this->threads[i]->join();
	}
}

void Raytracer::addObject(Object *object)
{
	this->objects.push_back(object);
}

void Raytracer::addLight(Light *light)
{
	this->lights.push_back(light);
}
