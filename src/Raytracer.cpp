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
#include <random>
#include <cmath>

#define THREAD_NUMBER 8

Raytracer::Raytracer(uint32_t width, uint32_t height)
: ambient(0, 0, 0)
, height(height)
, width(width)
{
	this->fsaa = 1;
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
	for (size_t i = 0; i < this->objects.size(); ++i)
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
	if (nearestDistance == INFINITY)
		return false;
	pos = ray.pos + ray.dir * nearestDistance;
	return true;
}

#define GI_SAMPLES 5
#define GI_FACTOR 1
#define GI_ATTENUATION 5
#define GI_ENABLED 1

Vec3 Raytracer::getGlobalIllumination(Vec3 &pos, Vec3 &norm, Object *object, int recursion)
{
	Vec3 result(0);
	if (!GI_ENABLED)
		return result;
	if (recursion > 1)
		return result;
	std::minstd_rand rnd(rand());
	float max = rnd.max();
	for (size_t i = 0; i < GI_SAMPLES; ++i)
	{
		Vec3 dir(normalize(Vec3(rnd() / max, rnd() / max, rnd() / max) - .5f));
		float d = dot(dir, norm);
		if (d <= EPSILON)
		{
			--i;
			continue;
		}
		Ray newRay(pos, dir);
		Object *collide = nullptr;
		Vec3 collidePos;
		if (!trace(newRay, collide, collidePos, object))
			continue;
		float len = length(pos - collidePos) / GI_ATTENUATION;
		Vec3 color(getRayColor(newRay, object, 100 + recursion));
		result += color * (1 - collide->Ir) * d / (1 + len);
		//result += collide->Kd.rgb() * collide->Kd.a * (1 - collide->Ir) * d / (1 + len);
	}
	return result / static_cast<float>(GI_SAMPLES) * static_cast<float>(GI_FACTOR);
}

#define AO_SAMPLES 5
#define AO_FACTOR 1
#define AO_ATTENUATION 5
#define AO_ENABLED 1

float Raytracer::getAmbientOcclusion(Vec3 &pos, Vec3 &norm, Object *object)
{
	if (!AO_ENABLED)
		return 1;
	float result = 0;
	std::minstd_rand rnd(rand());
	float max = rnd.max();
	for (size_t i = 0; i < AO_SAMPLES; ++i)
	{
		Vec3 dir(normalize(Vec3(rnd() / max, rnd() / max, rnd() / max) - .5f));
		float d = dot(dir, norm);
		if (d <= EPSILON)
		{
			--i;
			continue;
		}
		Ray newRay(pos, dir);
		Object *collide = nullptr;
		Vec3 collidePos;
		if (!trace(newRay, collide, collidePos, object))
			continue;
		float len = length(pos - collidePos) / AO_ATTENUATION;
		result += 1. / (1 + len) * collide->Kd.a * (1 - collide->Ir);
	}
	return 1 - result / static_cast<float>(AO_SAMPLES) * AO_FACTOR;
}

Vec4 Raytracer::getDiffuseSpecularTransparencyLight(Light *light, Object *object, Ray &ray, Vec3 &pos)
{
	Vec4 result(object->Kd);
	if (object->Kd_map)
		result *= object->Kd_map->getDataAt(object->getUVAt(ray, pos));
	if (result.a >= 1)
		return result;
	Ray newRay(pos, normalize(light->getDirectionFrom(pos)));
	Object *newObject = nullptr;
	Vec3 newPos;
	if (!trace(newRay, newObject, newPos, object))
		goto end;
	if (length(newPos - pos) > length(newRay.dir))
		goto end;
	result *= getDiffuseSpecularTransparencyLight(light, newObject, newRay, newPos);
end:
	return result;
}

void Raytracer::getDiffuseSpecularLight(Ray &ray, Object *object, Vec3 &pos, Vec3 &norm, Vec3 &diffuse, Vec3 &specular)
{
	diffuse = Vec3(0);
	specular = Vec3(0);
	Object *newObject = nullptr;
	Ray newRay(pos, Vec3());
	Vec3 refl(reflect(ray.dir, norm));
	float opacity;
	diffuse += object->Ka.rgb() * object->Ka.a;
	Vec3 newPos;
	for (uint64_t i = 0; i < this->lights.size(); ++i)
	{
		newRay.dir = this->lights[i]->getDirectionFrom(pos);
		Vec3 tmp(newRay.dir);
		newRay.dir = normalize(newRay.dir);
		float tmpLength = length(tmp);
		Vec3 lightColorIntensity = this->lights[i]->color * this->lights[i]->intensity;
		if (trace(newRay, newObject, newPos, object))
		{
			if (length(newPos - pos) > tmpLength)
				goto next;
			opacity = newObject->Kd.a;
			if (newObject->Kd_map)
				opacity *= newObject->Kd_map->getDataAt(newObject->getUVAt(newRay, newPos)).a;
			if (opacity >= 1)
				continue;
			Vec4 result = getDiffuseSpecularTransparencyLight(this->lights[i], newObject, newRay, newPos);
			lightColorIntensity = lightColorIntensity * (1 - result.a) * result.rgb();
		}
next:
		diffuse += lightColorIntensity * std::max(0.f, dot(newRay.dir, norm));
		specular += lightColorIntensity * float(pow(std::max(0.f, dot(refl, newRay.dir)), object->Ns));
	}
	specular *= object->Ks.rgb();
}

Vec3 Raytracer::getReflectionColor(Ray &ray, Object *object, Vec3 &pos, Vec3 &norm, int recursion)
{
	Ray newRay;
	newRay.pos = pos;
	newRay.dir = normalize(reflect(ray.dir, norm));
	newRay.Ni = ray.Ni;
	return getRayColor(newRay, object, recursion + 1);
}

Vec3 Raytracer::getTransparencyColor(Ray &ray, Object *object, Vec3 &pos, Vec3 &norm, bool normRev, int recursion)
{
	Ray newRay;
	newRay.pos = pos;
	float nextNi = object->Ni;
	if (normRev)
		nextNi = 1;
	newRay.Ni = nextNi;
	if (ray.Ni == nextNi)
	{
		newRay.dir = ray.dir;
	}
	else
	{
		float factor = ray.Ni / nextNi;
		float costhetai = -dot(ray.dir, norm);
		float sin2thetat = factor * factor * (1 - costhetai * costhetai);
		sin2thetat = std::min(1.f, sin2thetat);
		newRay.dir = ray.dir * factor + norm * float(factor * costhetai - sqrt(1 - sin2thetat));
	}
	newRay.dir = normalize(newRay.dir);;
	return getRayColor(newRay, object, recursion);
}

Vec3 Raytracer::getRayColor(Ray &ray, Object *avoid, int recursion, float *zIndex)
{
	if (recursion > 100)
		return Vec3(0);
	Object *object = nullptr;
	Vec3 pos;
	if (!trace(ray, object, pos, avoid))
		return Vec3(0);
	if (zIndex)
		*zIndex = length(pos - ray.pos);
	bool normRev = false;
	Vec3 norm = normalize(object->getNormAt(ray, pos));
	if (dot(norm, ray.dir) > 0)
	{
		norm = -norm;
		normRev = true;
	}
	Vec3 diffuse;
	Vec3 specular;
	getDiffuseSpecularLight(ray, object, pos, norm, diffuse, specular);
	Vec4 texColor;
	if (object->Kd_map)
		texColor = object->Kd_map->getDataAt(object->getUVAt(ray, pos));
	else
		texColor = Vec4(1);
	Vec3 col = diffuse + this->ambient;
	col *= getAmbientOcclusion(pos, norm, object);
	float transparency = object->Kd.a * texColor.a;
	if (transparency < 1)
	{
		Vec3 transparencyColor(getTransparencyColor(ray, object, pos, norm, normRev, recursion));
		col = col * transparency + transparencyColor * (1 - transparency);
		//col = transparencyColor.rgb() * (1 - transparency);
	}
	col *= object->Kd.rgb();
	col *= texColor.rgb();
	col += specular;
	col += getGlobalIllumination(pos, norm, object, recursion);
	if (object->Ir > 0)
		col = col * (1 - object->Ir) + getReflectionColor(ray, object, pos, norm, recursion + 1) * object->Kd.rgb() * texColor.rgb() * object->Ir;
	return col;
}

void Raytracer::calculatePixel(uint64_t x, uint64_t y)
{
	Ray ray(this->pos, Vec3(0, 0, 1));
	ray.Ni = 1;
	float ratio = this->width / (float)this->height; // assuming width > height
	if (this->width < this->height)
		ratio = 1 / ratio;
	float lowestZ = INFINITY;
	Vec3 colorSum(0);
	for (size_t yy = 0; yy < this->fsaa; ++yy)
	{
		for (size_t xx = 0; xx < this->fsaa; ++xx)
		{
			float rx = (2 * (x + xx / static_cast<float>(this->fsaa)) / this->width - 1) * tan(this->fov / 2 * M_PI / 180) * ratio;
			float ry = (1 - 2 * (y + yy / static_cast<float>(this->fsaa)) / this->height) * tan(this->fov / 2 * M_PI / 180);
			ray.dir = normalize(this->rotMat * Vec3(rx, ry, 1));
			float zIndex = 0;
			Vec3 col = getRayColor(ray, nullptr, 0, &zIndex);
			if (zIndex < lowestZ)
				lowestZ = zIndex;
			colorSum += col;
		}
	}
	size_t idx = x + y * this->width;
	this->zBuffer[idx] = lowestZ;
	Vec4 &dst = this->colorBuffer[idx];
	Vec3 col = colorSum / static_cast<float>(this->fsaa * this->fsaa);
	dst.r = std::min(1.f, std::max(0.f, col.r));
	dst.g = std::min(1.f, std::max(0.f, col.g));
	dst.b = std::min(1.f, std::max(0.f, col.b));
	dst.a = 1;//std::min(1.f, std::max(0.f, col.a));
	this->imgData[idx * 4 + 0] = std::min((int64_t)0xff, std::max((int64_t)0, (int64_t)(dst.r * 0xff)));
	this->imgData[idx * 4 + 1] = std::min((int64_t)0xff, std::max((int64_t)0, (int64_t)(dst.g * 0xff)));
	this->imgData[idx * 4 + 2] = std::min((int64_t)0xff, std::max((int64_t)0, (int64_t)(dst.b * 0xff)));
	this->imgData[idx * 4 + 3] = std::min((int64_t)0xff, std::max((int64_t)0, (int64_t)(dst.a * 0xff)));
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
	/*Vec4 *blur = Blur::blur(this->colorBuffer, 10, this->width, this->height);
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
	/*Vec4 *fog = Fog::fog(this->colorBuffer, this->zBuffer, this->width, this->height, Vec4(1, 0, 0, 1), 0, 20, FOG_LINEAR);
	delete[] (this->colorBuffer);
	this->colorBuffer = fog;*/
	/*for (uint64_t i = 0; i < this->width * this->height; ++i)
	{
		Vec4 &org = this->colorBuffer[i];
		this->imgData[i * 4 + 0] = std::min((int64_t)0xff, std::max((int64_t)0, (int64_t)(org.r * 0xff)));
		this->imgData[i * 4 + 1] = std::min((int64_t)0xff, std::max((int64_t)0, (int64_t)(org.g * 0xff)));
		this->imgData[i * 4 + 2] = std::min((int64_t)0xff, std::max((int64_t)0, (int64_t)(org.b * 0xff)));
		this->imgData[i * 4 + 3] = std::min((int64_t)0xff, std::max((int64_t)0, (int64_t)(org.a * 0xff)));
	}*/
}

void Raytracer::addObject(Object *object)
{
	this->objects.push_back(object);
}

void Raytracer::addLight(Light *light)
{
	this->lights.push_back(light);
}

void Raytracer::setRot(Vec3 vec)
{
	//this->rot = vec;
	Mat3 rot(1);
	rot = Mat3::rotateZ(rot, vec.z);
	rot = Mat3::rotateY(rot, vec.y);
	rot = Mat3::rotateX(rot, vec.x);
	this->rotMat = rot;
	Mat3 unrot(1);
	unrot = Mat3::rotateX(unrot, -vec.x);
	unrot = Mat3::rotateY(unrot, -vec.y);
	unrot = Mat3::rotateZ(unrot, -vec.z);
	this->unrotMat = unrot;

}
