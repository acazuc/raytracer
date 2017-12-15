#include "Raytracer.h"
#include "Filters/Color/GreyShade.h"
#include "Filters/Color/Negative.h"
#include "Filters/Color/Sepia.h"
#include "Filters/Fisheye.h"
#include "Filters/Sobel.h"
#include "Filters/Gamma.h"
#include "Filters/Fxaa.h"
#include "Filters/Blur.h"
#include "Utils/System.h"
#include "Filters/Cel.h"
#include "Filters/Fog.h"
#include "Consts.h"
#include "Debug.h"
#include <cstring>
#include <cmath>

#define THREAD_NUMBER 8

Raytracer::Raytracer(uint32_t width, uint32_t height)
: ambient(0, 0, 0)
, height(height)
, width(width)
{
	this->colorBuffer = new Vec4[width * height];
	this->zBuffer = new float[width * height];
	this->imgData = new char[width * height * 4];
	this->threads = new std::thread*[THREAD_NUMBER];
}

Raytracer::~Raytracer()
{
	delete[] (this->colorBuffer);
	delete[] (this->zBuffer);
	delete[] (this->imgData);
}

bool Raytracer::trace(Ray &ray, Object *&object, Vec3 &pos, Object *avoid)
{
	float nearestDistance = INFINITY;
	float t;
	for (uint64_t i = 0; i < this->objects.size(); ++i)
	{
		if (!this->objects[i]->collide(ray, t))
			continue;
		if (t >= nearestDistance)
			continue;
		if (this->objects[i] == avoid && t < EPSILON)
			continue;
		object = this->objects[i];
		nearestDistance = t;
	}
	if (nearestDistance == -1)
		return (false);
	pos = ray.pos + ray.dir * nearestDistance;
	return (true);
}

Vec4 Raytracer::getDiffuseSpecularTransparencyLight(Light *light, Object *object, Ray &ray, Vec3 &pos)
{
	Vec4 result(object->Kd);
	if (object->Kd_map)
		result *= object->Kd_map->getDataAt(object->getUVAt(ray, pos));
	if (result.a >= 1)
		return (result);
	Ray newRay(pos, light->getDirectionFrom(pos));
	newRay.dir.normalize();
	Object *newObject = nullptr;
	Vec3 newPos;
	if (!trace(newRay, newObject, newPos, object))
		goto end;
	if ((newPos - pos).length() > newRay.dir.length())
		goto end;
	result *= getDiffuseSpecularTransparencyLight(light, newObject, newRay, newPos);
end:
	return (result);
}

void Raytracer::getDiffuseSpecularLight(Ray &ray, Object *object, Vec3 &pos, Vec3 &norm, Vec4 &diffuse, Vec4 &specular)
{
	Object *newObject = nullptr;
	Ray newRay(pos, Vec3());
	Vec3 reflect = ray.dir.reflect(norm);
	float opacity;
	diffuse += object->Ka;
	Vec3 newPos;
	for (uint64_t i = 0; i < this->lights.size(); ++i)
	{
		newRay.dir = this->lights[i]->getDirectionFrom(pos);
		Vec3 tmp(newRay.dir);
		newRay.dir.normalize();
		Vec4 lightColorIntensity = Vec4(this->lights[i]->color * this->lights[i]->intensity, 1);
		if (trace(newRay, newObject, newPos, object))
		{
			if ((newPos - pos).length() > tmp.length())
				goto next;
			opacity = newObject->Kd.a;
			if (newObject->Kd_map)
				opacity *= newObject->Kd_map->getDataAt(newObject->getUVAt(newRay, newPos)).a;
			if (opacity >= 1)
				continue;
			Vec4 result = getDiffuseSpecularTransparencyLight(this->lights[i], newObject, newRay, newPos);
			lightColorIntensity = lightColorIntensity * (1 - result.a) * Vec4(result.rgb(), 1);
		}
next:
		diffuse += lightColorIntensity * std::max(0.f, newRay.dir.dot(norm));
		specular += lightColorIntensity * pow(std::max(0.f, reflect.dot(newRay.dir)), object->Ns) * object->Ks;
	}
}

Vec4 Raytracer::getReflectionColor(Ray &ray, Object *object, Vec3 &pos, Vec3 &norm, int recursion)
{
	if (recursion > 1)
		return (Vec4(0, 0, 0));
	Ray newRay;
	newRay.pos = pos;
	newRay.dir = ray.dir.reflect(norm);
	newRay.dir.normalize();
	newRay.Ni = ray.Ni;
	return (getRayColor(newRay, object, recursion + 1));
}

Vec4 Raytracer::getTransparencyColor(Ray &ray, Object *object, Vec3 &pos, Vec3 &norm, bool normRev, int recursion)
{
	Ray newRay;
	newRay.pos = pos;
	float nextNi = object->Ni;
	if (normRev)
		nextNi = 1;//object->Ni;
	newRay.Ni = nextNi;
	if (ray.Ni == nextNi)
	{
		newRay.dir = ray.dir;
	}
	else
	{
		float factor = ray.Ni / nextNi;
		float costhetai = -ray.dir.dot(norm);
		float sin2thetat = factor * factor * (1 - costhetai * costhetai);
		sin2thetat = std::min(1.f, sin2thetat);
		newRay.dir = ray.dir * factor + norm * (factor * costhetai - sqrt(1 - sin2thetat));
	}
	newRay.dir.normalize();
	return (getRayColor(newRay, object, recursion));
}

Vec4 Raytracer::getRayColor(Ray &ray, Object *avoid, int recursion, float *zIndex)
{
	Object *object = nullptr;
	Vec3 pos;
	if (!trace(ray, object, pos, avoid))
		return (Vec4(0));
	if (zIndex)
		*zIndex = (pos - ray.pos).length();
	Vec3 norm = object->getNormAt(ray, pos);
	bool normRev = false;
	if (norm.dot(ray.dir) > 0)
	{
		norm = -norm;
		normRev = true;
	}
	Vec4 diffuse(0);
	Vec4 specular(0);
	getDiffuseSpecularLight(ray, object, pos, norm, diffuse, specular);
	Vec4 texColor(1);
	if (object->Kd_map)
		texColor = object->Kd_map->getDataAt(object->getUVAt(ray, pos));
	Vec4 col = diffuse + Vec4(this->ambient, 0);
	float transparency = object->Kd.a * texColor.a;
	if (transparency < 1)
		col = col * transparency + getTransparencyColor(ray, object, pos, norm, normRev, recursion) * (1 - transparency);
	col *= object->Kd;
	col *= texColor;
	col += specular;
	if (object->Ir > 0)
		col = col * (1 - object->Ir) + getReflectionColor(ray, object, pos, norm, recursion + 1) * object->Kd * texColor * object->Ir;
	return (col);
}

void Raytracer::calculatePixel(uint64_t x, uint64_t y)
{
	Ray ray(this->pos, Vec3(0, 0, 1));
	ray.Ni = 1;
	float ratio = this->width / (float)this->height; // assuming width > height
	if (this->width < this->height)
		ratio = 1 / ratio;
	float rx = (2 * (x + 0.5) / this->width - 1) * tan(this->fov / 2 * M_PI / 180) * ratio;
	float ry = (1 - 2 * (y + 0.5) / this->height) * tan(this->fov / 2 * M_PI / 180);
	ray.dir = Vec3(rx, ry, 1);
	ray.dir.rotate(this->rot);
	ray.dir.normalize();
	float zIndex = 0;
	Vec4 col = getRayColor(ray, nullptr, 0, &zIndex);
	this->zBuffer[(x + y * this->width)] = zIndex;
	Vec4 &dst = this->colorBuffer[x + y * this->width];
	dst.r = std::min(1.f, std::max(0.f, col.r));
	dst.g = std::min(1.f, std::max(0.f, col.g));
	dst.b = std::min(1.f, std::max(0.f, col.b));
	dst.a = std::min(1.f, std::max(0.f, col.a));
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
	uint64_t started;
	uint64_t ended;
	uint64_t total = this->width * this->height;
	started = System::nanotime();
	for (uint8_t i = 0; i < THREAD_NUMBER; ++i)
	{
		uint64_t start = total * i / THREAD_NUMBER;
		uint64_t end = total * (i + 1) / THREAD_NUMBER;
		this->threads[i] = new std::thread(&Raytracer::runThread, this, start, end);
	}
	for (uint8_t i = 0; i < THREAD_NUMBER; ++i)
	{
		this->threads[i]->join();
		delete (this->threads[i]);
	}
	ended = System::nanotime();
	LOG("Draw: " << (ended - started) / 1000000. << " ms");
	/*Vec4 *cel = Cel::cel(this->colorBuffer, 25, this->width, this->height);
	delete[] (this->colorBuffer);
	this->colorBuffer = cel;*/
	/*Vec4 *sobel = Sobel::sobel(this->colorBuffer, this->zBuffer, this->width, this->height);
	delete[] (this->colorBuffer);
	this->colorBuffer = sobel;*/
	/*Vec4 *fisheye = Fisheye::fisheye(this->colorBuffer, this->width, this->height);
	delete[] (this->colorBuffer);
	this->colorBuffer = fisheye;*/
	/*Vec4 *fxaa = Fxaa::fxaa(this->colorBuffer, this->width, this->height);
	delete[] (this->colorBuffer);
	this->colorBuffer = fxaa;*/
	/*Vec4 *blur = Blur::blur(this->colorBuffer, 5, this->width, this->height);
	delete[] (this->colorBuffer);
	this->colorBuffer = blur;*/
	/*Vec4 *greyShade = GreyShade::greyShade(this->colorBuffer, this->width, this->height);
	delete[] (this->colorBuffer);
	this->colorBuffer = greyShade;*/
	/*Vec4 *negative = Negative::negative(this->colorBuffer, this->width, this->height);
	delete[] (this->colorBuffer);
	this->colorBuffer = negative;*/
	/*Vec4 *sepia = Sepia::sepia(this->colorBuffer, this->width, this->height);
	delete[] (this->colorBuffer);
	this->colorBuffer = sepia;*/
	/*Vec4 *gamma = Gamma::gamma(this->colorBuffer, this->width, this->height, 1 / 2.2);
	delete[] (this->colorBuffer);
	this->colorBuffer = gamma;*/
	/*Vec4 *fog = Fog::fog(this->colorBuffer, this->zBuffer, this->width, this->height, Vec4(1, 0, 0, 1), 0, 12, FOG_LINEAR);
	delete[] (this->colorBuffer);
	this->colorBuffer = fog;*/
	for (uint64_t i = 0; i < this->width * this->height; ++i)
	{
		Vec4 &org = this->colorBuffer[i];
		this->imgData[i * 4 + 0] = std::min((int64_t)0xff, std::max((int64_t)0, (int64_t)(org.r * 0xff)));
		this->imgData[i * 4 + 1] = std::min((int64_t)0xff, std::max((int64_t)0, (int64_t)(org.g * 0xff)));
		this->imgData[i * 4 + 2] = std::min((int64_t)0xff, std::max((int64_t)0, (int64_t)(org.b * 0xff)));
		this->imgData[i * 4 + 3] = std::min((int64_t)0xff, std::max((int64_t)0, (int64_t)(org.a * 0xff)));
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
