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

bool Raytracer::trace(Ray &ray, Object *&object, Vec3 &position, Object *avoid)
{
	float nearestDistance = -1;
	for (uint64_t i = 0; i < this->objects.size(); ++i)
	{
		if (this->objects[i] == avoid)
			continue;
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

void Raytracer::getDiffuseSpecularLight(Ray &ray, Object *object, Vec3 &pos, Vec3 &norm, Vec3 &diffuse, Vec3 &specular)
{
	Object *collObject = nullptr;
	Vec3 collPosition;
	Ray diffuseRay(pos, Vec3(0));
	Vec3 reflect = ray.dir.reflect(norm);
	for (uint64_t i = 0; i < this->lights.size(); ++i)
	{
		diffuseRay.dir = this->lights[i]->getDirectionFrom(pos);
		if (trace(diffuseRay, collObject, collPosition, object))
			continue;
		Vec3 lightColorIntensity = this->lights[i]->color * this->lights[i]->intensity;
		diffuse += lightColorIntensity * std::max(0., cos(diffuseRay.dir.angle(norm)));
		specular += lightColorIntensity * pow(std::max(0.f, reflect.dot(diffuseRay.dir)), object->specularFactor) * object->specular;
	}
}

void Raytracer::calculatePixel(uint64_t x, uint64_t y)
{
	Ray ray(Vec3(0), Vec3(0));
	float angleY = (x - this->width / 2.) / this->width * (this->fov / 180. * M_PI);
	float angleX = (y - this->height / 2.) / this->height * (this->fov / 180. * M_PI) / this->width * this->height;
	ray.dir.x = sin(angleY);
	ray.dir.y = sin(angleX);
	ray.dir.z = cos(angleX) * cos(angleY);
	ray.dir.normalize();
	Object *object = nullptr;
	Vec3 position;
	if (trace(ray, object, position, nullptr))
	{
		Vec3 norm = object->getNormAt(position);
		Vec3 diffuse(0);
		Vec3 specular(0);
		getDiffuseSpecularLight(ray, object, position, norm, diffuse, specular);
		this->imgData[(x + y * this->width) * 4 + 0] = std::min(0xff, std::max(0, (int32_t)(0xff * (object->color.r * diffuse.r + specular.r))));
		this->imgData[(x + y * this->width) * 4 + 1] = std::min(0xff, std::max(0, (int32_t)(0xff * (object->color.g * diffuse.g + specular.g))));
		this->imgData[(x + y * this->width) * 4 + 2] = std::min(0xff, std::max(0, (int32_t)(0xff * (object->color.b * diffuse.b + specular.b))));
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
