#include "Raytracer.h"
#include "Objects/Object.h"
#include "Filters/Filter.h"
#include "Utils/System.h"
#include "Lights/Light.h"
#include "Material.h"
#include "Consts.h"
#include "Image.h"
#include "Debug.h"
#include "Ray.h"
#include <cstring>
#include <random>
#include <cmath>

Raytracer::Raytracer(size_t width, size_t height)
: ambient(0, 0, 0)
, height(height)
, width(width)
{
	this->batches.resize((height + BATCH_SIZE - 1) / BATCH_SIZE, std::vector<enum BatchState>((width + BATCH_SIZE - 1) / BATCH_SIZE, BATCH_NOT_DONE));
	this->samples = 1;
	this->colorBuffer = new Vec4[width * height];
	this->zBuffer = new float[width * height];
	this->imgData = new TVec4<uint8_t>[width * height];
	this->globalIllumination = false;
	this->ambientOcclusion = false;
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

#define GI_SAMPLES 25
#define GI_FACTOR 1
#define GI_ATTENUATION 2

Vec3 Raytracer::getGlobalIllumination(Vec3 &pos, Vec3 &norm, Object *object, int recursion)
{
	Vec3 result(0);
	if (!this->globalIllumination)
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
		Vec3 color(getRayColor(newRay, object, 99 + recursion));
		result += color * d / (1 + len);
	}
	return result / static_cast<float>(GI_SAMPLES) * static_cast<float>(GI_FACTOR);
}

#define AO_SAMPLES 25
#define AO_FACTOR 1
#define AO_ATTENUATION 2

float Raytracer::getAmbientOcclusion(Vec3 &pos, Vec3 &norm, Object *object)
{
	if (!this->ambientOcclusion)
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
		result += 1. / (1 + len) * collide->material->opacity;// * (1 - collide->Ir);
	}
	return 1 - result / static_cast<float>(AO_SAMPLES) * AO_FACTOR;
}

Vec4 Raytracer::getDiffuseSpecularTransparencyLight(Light *light, Object *object, Ray &ray, Vec3 &pos)
{
	Vec4 result(object->material->diffuseColor, object->material->opacity);
	if (object->material->diffuseTexture)
		result *= object->material->diffuseTexture->getDataAt(object->getUVAt(ray, pos));
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
	diffuse += object->material->ambientColor;
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
			opacity = newObject->material->opacity;
			if (newObject->material->diffuseTexture)
				opacity *= newObject->material->diffuseTexture->getDataAt(newObject->getUVAt(newRay, newPos)).a;
			if (opacity >= 1)
				continue;
			Vec4 result = getDiffuseSpecularTransparencyLight(this->lights[i], newObject, newRay, newPos);
			lightColorIntensity = lightColorIntensity * (1 - result.a) * result.rgb();
		}
next:
		diffuse += lightColorIntensity * std::max(0.f, dot(newRay.dir, norm));
		specular += lightColorIntensity * float(pow(std::max(0.f, dot(refl, newRay.dir)), object->material->specularFactor));
	}
	specular *= object->material->specularColor;
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
	float nextNi = object->material->refraction;
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
	Vec3 diffuse;
	Vec3 specular;
	bool normRev = false;
	Vec3 norm;
	if (this->shading)
	{
		norm = normalize(object->getNormAt(ray, pos));
		if (dot(norm, ray.dir) > 0)
		{
			norm = -norm;
			normRev = true;
		}
		getDiffuseSpecularLight(ray, object, pos, norm, diffuse, specular);
	}
	else
	{
		diffuse = Vec3(1);
		specular = Vec3(0);
	}
	Vec4 texColor;
	if (object->material->diffuseTexture)
		texColor = object->material->diffuseTexture->getDataAt(object->getUVAt(ray, pos));
	else
		texColor = Vec4(1);
	Vec3 col = diffuse + this->ambient;
	col *= getAmbientOcclusion(pos, norm, object);
	if (this->shading)
	{
		float transparency = object->material->opacity * texColor.a;
		if (transparency < 1)
		{
			Vec3 transparencyColor(getTransparencyColor(ray, object, pos, norm, normRev, recursion));
			col = col * transparency + transparencyColor * (1 - transparency);
			//col = transparencyColor.rgb() * (1 - transparency);
		}
	}
	col *= object->material->diffuseColor;
	col *= texColor.rgb();
	col += specular;
	col += getGlobalIllumination(pos, norm, object, recursion);
	if (this->shading && object->material->reflection > 0)
		col = col * (1 - object->material->reflection) + getReflectionColor(ray, object, pos, norm, recursion + 1) * object->material->diffuseColor * texColor.rgb() * object->material->reflection;
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
	for (size_t yy = 0; yy < this->samples; ++yy)
	{
		for (size_t xx = 0; xx < this->samples; ++xx)
		{
			float rx = (2 * (x + xx / static_cast<float>(this->samples)) / this->width - 1) * tan(this->fov / 2 * M_PI / 180) * ratio;
			float ry = (1 - 2 * (y + yy / static_cast<float>(this->samples)) / this->height) * tan(this->fov / 2 * M_PI / 180);
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
	Vec3 col = colorSum / static_cast<float>(this->samples * this->samples);
	col = clamp(col, 0.f, 1.f);
	this->colorBuffer[idx] = Vec4(col, 1);
	this->imgData[idx].r = col.r * 0xff;
	this->imgData[idx].g = col.g * 0xff;
	this->imgData[idx].b = col.b * 0xff;
	this->imgData[idx].a = 1;
}

void Raytracer::runThread(Raytracer *raytracer)
{
	while (true)
	{
		size_t batchX;
		size_t batchY;
		{
			std::lock_guard<std::mutex> guard(raytracer->batchesMutex);
			ssize_t batchesY = (raytracer->batches.size() + 1) / 2;
			ssize_t batchesX = (raytracer->batches[0].size() + 1) / 2;
			ssize_t batchesSize = std::max(batchesY, batchesX) + 1;
			for (ssize_t i = 0; i < batchesSize; ++i)
			{
				for (ssize_t yy = batchesY - i; yy < batchesY + i; ++yy)
				{
					for (ssize_t xx = batchesX - i; xx < batchesX + i; ++xx)
					{
						if (yy < 0 || size_t(yy) >= raytracer->batches.size())
							continue;
						if (xx < 0 || size_t(xx) >= raytracer->batches[0].size())
							continue;
						if (raytracer->batches[yy][xx] == BATCH_NOT_DONE)
						{
							batchY = yy;
							batchX = xx;
							raytracer->batches[yy][xx] = BATCH_CALCULATING;
							goto calcBatch;
						}
					}
				}
			}
		}
		return;
calcBatch:
		uint8_t calc[BATCH_SIZE * BATCH_SIZE];
		std::fill(std::begin(calc), std::end(calc), 0);
		size_t startX = batchX * BATCH_SIZE;
		size_t startY = batchY * BATCH_SIZE;
		size_t endX = std::min(raytracer->width, startX + BATCH_SIZE);
		size_t endY = std::min(raytracer->height, startY + BATCH_SIZE);
		for (size_t i = BATCH_SIZE / 2; i; i /= 2)
		{
			for (size_t y = startY; y < endY; y += i)
			{
				for (size_t x = startX; x < endX; x += i)
				{
					if (!calc[(y % BATCH_SIZE) * BATCH_SIZE + (x % BATCH_SIZE)])
					{
						calc[(y % BATCH_SIZE) * BATCH_SIZE + (x % BATCH_SIZE)] = 1;
						raytracer->calculatePixel(x, y);
					}
					size_t base = x + y * raytracer->width;
					for (size_t yy = 0; yy < i; ++yy)
					{
						for (size_t xx = 0; xx < i; ++xx)
						{
							size_t idx = x + xx + (y + yy) * raytracer->width;
							if (!idx)
								continue;
							raytracer->imgData[idx] = raytracer->imgData[base];
						}
					}
				}
			}
		}
		std::lock_guard<std::mutex> guard(raytracer->batchesMutex);
		raytracer->batches[batchY][batchX] = BATCH_DONE;
	}
}

void Raytracer::render()
{
	uint64_t started;
	uint64_t ended;
	started = System::nanotime();
	this->threads.resize(this->threadsCount);
	for (uint8_t i = 0; i < this->threadsCount; ++i)
	{
		this->threads[i] = new std::thread(&Raytracer::runThread, this);
	}
	for (uint8_t i = 0; i < this->threadsCount; ++i)
	{
		this->threads[i]->join();
		delete (this->threads[i]);
	}
	this->threads.clear();
	ended = System::nanotime();
	LOG("Draw: " << (ended - started) / 1000000 << " ms");
	if (this->filters.size())
	{
		started = System::nanotime();
		Vec4 *tmp = new Vec4[this->width * this->height];
		Vec4 *dst = tmp;
		Vec4 *src = this->colorBuffer;
		for (size_t i = 0; i < this->filters.size(); ++i)
		{
			(*this->filters[i])(dst, src, this->zBuffer, this->width, this->height);
			std::swap(src, dst);
		}
		if (src != this->colorBuffer)
			std::memcpy(this->colorBuffer, src, sizeof(*src) * this->width * this->height);
		delete[] (tmp);
		ended = System::nanotime();
		LOG("Filters: " << (ended - started) / 1000000 << " ms");
	}
	for (size_t i = 0; i < this->width * this->height; ++i)
	{
		Vec4 org = this->colorBuffer[i];
		org = clamp(org, 0.f, 1.f);
		this->imgData[i].r = org.r * 0xff;
		this->imgData[i].g = org.g * 0xff;
		this->imgData[i].b = org.b * 0xff;
		this->imgData[i].a = org.a * 0xff;
	}
}

void Raytracer::addFilter(Filter *filter)
{
	this->filters.push_back(filter);
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

void Raytracer::setSamples(uint8_t samples)
{
	this->samples = samples;
}

void Raytracer::setThreads(uint8_t threads)
{
	this->threadsCount = threads;
}

void Raytracer::setShading(bool shading)
{
	this->shading = shading;
}

void Raytracer::setGlobalIllumination(bool globalIllumination)
{
	this->globalIllumination = globalIllumination;
}

void Raytracer::setAmbientOcclusion(bool ambientOcclusion)
{
	this->ambientOcclusion = ambientOcclusion;
}
