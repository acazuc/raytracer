#include "Raytracer.h"
#include "Fxaa.h"
#include <cmath>

Raytracer::Raytracer(uint32_t width, uint32_t height)
: ambient(0, 0, 0)
, height(height)
, width(width)
{
	this->colorBuffer = new Vec3[width * height];
	this->zBuffer = new float[width * height];
	this->imgData = new char[width * height * 4];
}

Raytracer::~Raytracer()
{
	delete[] (this->colorBuffer);
	delete[] (this->zBuffer);
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
		delete (vec);
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
		diffuseRay.dir.normalize();
		if (trace(diffuseRay, collObject, collPosition, object))
			continue;
		Vec3 lightColorIntensity = this->lights[i]->color * this->lights[i]->intensity;
		diffuse += lightColorIntensity * std::max(0.f, diffuseRay.dir.dot(norm));
		specular += lightColorIntensity * pow(std::max(0.f, reflect.dot(diffuseRay.dir)), object->specularFactor) * object->specular;
	}
}

Vec3 Raytracer::getReflectionColor(Ray &ray, Object *object, Vec3 &pos, Vec3 &norm, int recursion)
{
	if (recursion > 100)
		return (Vec3(0, 0, 0));
	Ray newRay;
	newRay.pos = pos;
	newRay.dir = ray.dir.reflect(norm);
	return (getRayColor(newRay, object, recursion));
}

Vec3 Raytracer::getRayColor(Ray &ray, Object *avoid, int recursion)
{

	Object *object = nullptr;
	Vec3 pos;
	if (!trace(ray, object, pos, avoid))
		return (Vec3(0, 0, 0));
	Vec3 norm = object->getNormAt(ray, pos);
	Vec3 diffuse(0);
	Vec3 specular(0);
	getDiffuseSpecularLight(ray, object, pos, norm, diffuse, specular);
	Vec3 col = Vec3(object->color) * (diffuse + this->ambient) + specular;
	if (object->reflection > 0)
		col = col * (1 - object->reflection) + getReflectionColor(ray, object, pos, norm, recursion + 1) * object->color * object->reflection;
	return (col);
}

void Raytracer::calculatePixel(uint64_t x, uint64_t y)
{
	Ray ray(Vec3(0), Vec3(0, 0, 1));
	float angleY = (x - this->width / 2.) / this->width * (this->fov / 180. * M_PI);
	float angleX = (y - this->height / 2.) / this->height * (this->fov / 180. * M_PI) / this->width * this->height;
	ray.dir.rotate(Vec3(angleX, angleY, 0));
	ray.dir.normalize();
	Vec3 col = getRayColor(ray, nullptr, 0);
	float gamma = 1. / 2.2;
	col.x = pow(col.x, gamma);
	col.y = pow(col.y, gamma);
	col.z = pow(col.z, gamma);
	this->colorBuffer[(x + y * this->width)].r = std::min(1.f, std::max(0.f, col.r));
	this->colorBuffer[(x + y * this->width)].g = std::min(1.f, std::max(0.f, col.g));
	this->colorBuffer[(x + y * this->width)].b = std::min(1.f, std::max(0.f, col.b));
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
	Vec3 *fxaa = Fxaa::fxaa(this->colorBuffer, this->width, this->height);
	delete[] (this->colorBuffer);
	this->colorBuffer = fxaa;
	for (uint64_t i = 0; i < this->width * this->height; ++i)
	{
		this->imgData[i * 4 + 0] = std::min((int64_t)0xff, std::max((int64_t)0, (int64_t)(this->colorBuffer[i].r * 0xff)));
		this->imgData[i * 4 + 1] = std::min((int64_t)0xff, std::max((int64_t)0, (int64_t)(this->colorBuffer[i].g * 0xff)));
		this->imgData[i * 4 + 2] = std::min((int64_t)0xff, std::max((int64_t)0, (int64_t)(this->colorBuffer[i].b * 0xff)));
		this->imgData[i * 4 + 3] = 0xff;
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
