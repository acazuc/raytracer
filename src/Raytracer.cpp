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
: height(height)
, width(width)
{
	this->batches.resize((height + BATCH_SIZE - 1) / BATCH_SIZE, std::vector<enum BatchState>((width + BATCH_SIZE - 1) / BATCH_SIZE, BATCH_NEED_CALCULATION));
	this->samples = 1;
	this->threadsCount = 1;
	this->colorBuffer.resize(width * height);
	this->zBuffer.resize(width * height);
	this->imgData.resize(width * height);
	this->globalIlluminationSamples = 50;
	this->globalIlluminationDistance = 1000;
	this->globalIlluminationFactor = 1;
	this->globalIllumination = false;
	this->ambientOcclusionSamples = 50;
	this->ambientOcclusionDistance = 10;
	this->ambientOcclusionFactor = 1;
	this->ambientOcclusion = false;
}

Raytracer::~Raytracer()
{
}

bool Raytracer::trace(Ray &ray, CollisionContext &collision, Object *avoid)
{
	float nearestDistance = INFINITY;
	float t;
	Object *obj = nullptr;
	for (Object *object : this->objects)
	{
		if (!object->collide(ray, t))
			continue;
		if (t >= nearestDistance)
			continue;
		/*if (this->objects[i] == avoid && t < EPSILON) //Useless, already checked in Object::collide
			continue;*/
		obj = object;
		nearestDistance = t;
	}
	if (nearestDistance == INFINITY)
		return false;
	collision.object = obj;
	collision.pos = ray.pos + ray.dir * nearestDistance;
	collision.t = nearestDistance;
	return true;
}

Vec3 Raytracer::getGlobalIllumination(FragmentContext &context, CollisionContext &collision)
{
	if (!this->globalIllumination)
		return Vec3(0);
	if (context.globalIlluminationRecursion >= 1)
		return Vec3(0);
	Vec3 result(0);
	context.globalIlluminationRecursion++;
	float max = context.rnd.max();
	for (size_t i = 0; i < this->globalIlluminationSamples; ++i)
	{
		Vec3 dir(normalize(Vec3(context.rnd() / max, context.rnd() / max, context.rnd() / max) - .5f));
		float d = dot(dir, collision.norm);
		if (d <= EPSILON)
		{
			i--;
			continue;
		}
		Ray newRay(collision.pos, dir);
		CollisionContext newCollision;
		if (!trace(newRay, newCollision, collision.object))
			continue;
		float len = newCollision.t / this->globalIlluminationDistance;
		Vec4 color(getRayColor(context, newRay, collision.object));
		result += color.rgb() * color.a * d / (1 + len);
	}
	context.globalIlluminationRecursion--;
	return result / float(this->globalIlluminationSamples) * this->globalIlluminationFactor;
}

float Raytracer::getAmbientOcclusion(FragmentContext &context, CollisionContext &collision)
{
	if (!this->ambientOcclusion)
		return 1;
	if (context.globalIlluminationRecursion >= 1)
		return 1;
	float result = 0;
	float max = context.rnd.max();
	for (size_t i = 0; i < this->ambientOcclusionSamples; ++i)
	{
		Vec3 dir(normalize(Vec3(context.rnd() / max, context.rnd() / max, context.rnd() / max) - .5f));
		float d = dot(dir, collision.norm);
		if (d <= EPSILON)
		{
			i--;
			continue;
		}
		Ray newRay(collision.pos, dir);
		CollisionContext newCollision;
		if (!trace(newRay, newCollision, collision.object))
			continue;
		float len = newCollision.t / this->ambientOcclusionDistance;
		result += 1. / (1 + len) * newCollision.object->material->opacity;// * (1 - newCollision.object->Ir);
	}
	return 1 - result / float(this->ambientOcclusionSamples) * this->ambientOcclusionFactor;
}

Vec4 Raytracer::getDiffuseSpecularTransparencyLight(Light *light, CollisionContext &collision)
{
	Vec4 result(collision.object->material->diffuseColor, 1 - collision.object->material->opacity);
	{
		if (collision.object->material->diffuseTexture)
		{
			Vec4 texCol(collision.object->material->diffuseTexture->getDataAt(collision.object->getUVAt(collision)));
			result *= Vec4(texCol.rgb(), 1 - texCol.a);
		}
	}
	if (result.a <= 0)
		return result;
	Ray newRay(collision.pos, light->getDirectionFrom(collision.pos));
	Vec3 tmp(newRay.dir);
	newRay.dir = normalize(newRay.dir);
	{
		CollisionContext newCollision;
		if (!trace(newRay, newCollision, collision.object))
			goto end;
		if (newCollision.t > length(tmp))
			goto end;
		Vec4 nextCol(getDiffuseSpecularTransparencyLight(light, newCollision));
		result *= Vec4(nextCol.rgb(), nextCol.a);
	}
end:
	return result;
}

void Raytracer::getDiffuseSpecularLight(Ray &ray, CollisionContext &collision, Vec3 &diffuse, Vec3 &specular)
{
	diffuse = Vec3(0);
	specular = Vec3(0);
	Ray newRay(collision.pos, Vec3());
	Vec3 refl(reflect(ray.dir, collision.norm));
	float opacity;
	for (Light *light : this->lights)
	{
		newRay.dir = light->getDirectionFrom(collision.pos);
		Vec3 tmp(newRay.dir);
		float tmpLength = length(tmp);
		newRay.dir = normalize(newRay.dir);
		Vec3 lightColorIntensity = light->color * light->intensity;
		CollisionContext newCollision;
		if (trace(newRay, newCollision, collision.object))
		{
			if (newCollision.t > tmpLength)
				goto next;
			opacity = newCollision.object->material->opacity;
			if (newCollision.object->material->diffuseTexture)
				opacity *= newCollision.object->material->diffuseTexture->getDataAt(newCollision.object->getUVAt(newCollision)).a;
			if (opacity >= 1)
				continue;
			Vec4 result(getDiffuseSpecularTransparencyLight(light, newCollision));
			lightColorIntensity = lightColorIntensity * result.a * result.rgb();
		}
next:
		diffuse += lightColorIntensity * std::max(0.f, dot(newRay.dir, collision.norm));
		specular += lightColorIntensity * float(pow(std::max(0.f, dot(refl, newRay.dir)), collision.object->material->specularFactor));
	}
	specular *= collision.object->material->specularColor;
}

Vec4 Raytracer::getReflectionColor(FragmentContext &context, Ray &ray, CollisionContext &collision)
{
	if (context.reflectionRecursion > 100)
		return Vec4(0);
	Ray newRay;
	newRay.pos = collision.pos;
	newRay.dir = reflect(ray.dir, collision.norm);
	newRay.Ni = ray.Ni;
	context.reflectionRecursion++;
	Vec4 ret(getRayColor(context, newRay, collision.object));
	context.reflectionRecursion--;
	return ret;
}

Vec4 Raytracer::getTransparencyColor(FragmentContext &context, Ray &ray, CollisionContext &collision, bool normRev)
{
	Ray newRay;
	newRay.pos = collision.pos;
	float nextNi = collision.object->material->refraction;
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
		float costhetai = -dot(ray.dir, collision.norm);
		float sin2thetat = factor * factor * (1 - costhetai * costhetai);
		sin2thetat = std::min(1.f, sin2thetat);
		newRay.dir = ray.dir * factor + collision.norm * float(factor * costhetai - sqrt(1 - sin2thetat));
	}
	newRay.dir = normalize(newRay.dir);
	return getRayColor(context, newRay, collision.object);
}

Vec4 Raytracer::getRayColor(FragmentContext &context, Ray &ray, Object *avoid, float *zIndex)
{
	CollisionContext collision;
	if (!trace(ray, collision, avoid))
		return Vec4(0);
	if (zIndex)
		*zIndex = collision.t;
	collision.UV = collision.object->getUVAt(collision);
	//return Vec4(std::abs(collision.UV.x), std::abs(collision.UV.y), 1, 1);
	collision.norm = collision.object->getNormAt(collision);
	bool normRev;
	if (dot(collision.norm, ray.dir) > 0)
	{
		collision.norm = -collision.norm;
		normRev = true;
	}
	else
	{
		normRev = false;
	}
	//return Vec4(collision.norm, 1);
	Vec3 diffuse;
	Vec3 specular;
	if (this->shading)
	{
		getDiffuseSpecularLight(ray, collision, diffuse, specular);
	}
	else
	{
		diffuse = Vec3(1);
		specular = Vec3(0);
	}
	Vec4 texColor;
	if (collision.object->material->diffuseTexture)
		texColor = collision.object->material->diffuseTexture->getDataAt(collision.UV);
	else
		texColor = Vec4(1);
	Vec3 emi(collision.object->material->emissiveColor);
	if (collision.object->material->emissiveTexture)
	{
		Vec4 emiTex(collision.object->material->emissiveTexture->getDataAt(collision.UV));
		emi *= emiTex.rgb() * emiTex.a;
	}
	Vec3 col = diffuse;
	col *= getAmbientOcclusion(context, collision);
	float transparency;
	if (this->shading)
	{
		transparency = collision.object->material->opacity * texColor.a;
		if (transparency < 1)
		{
			Vec4 transparencyColor(getTransparencyColor(context, ray, collision, normRev));
			float transFactor = transparencyColor.a * (1 - transparency);
			col = col * transparency + transparencyColor.rgb() * transFactor;
			transparency += transFactor;
		}
	}
	else
	{
		transparency = 1;
	}
	col *= collision.object->material->diffuseColor;
	col *= texColor.rgb();
	Vec3 spec(collision.object->material->specularColor);
	if (collision.object->material->specularTexture)
	{
		Vec4 texSpec(collision.object->material->specularTexture->getDataAt(collision.UV));
		spec *= texSpec.rgb() * texSpec.a;
	}
	col += spec * (specular + getGlobalIllumination(context, collision));
	if (this->shading && collision.object->material->reflection > 0)
	{
		Vec4 reflectionColor(getReflectionColor(context, ray, collision));
		col = col * (1 - collision.object->material->reflection) + reflectionColor.rgb() * reflectionColor.a * collision.object->material->diffuseColor * texColor.rgb() * collision.object->material->reflection;
	}
	return Vec4(col, transparency);
}

Vec4 Raytracer::calculatePixel(size_t x, size_t y)
{
	Ray ray(this->position, Vec3(0, 0, 1));
	ray.Ni = 1;
	float ratio = this->width / float(this->height);
	if (this->width < this->height)
		ratio = 1 / ratio;
	float lowestZ = INFINITY;
	Vec4 colorSum(0);
	for (size_t yy = 0; yy < this->samples; ++yy)
	{
		for (size_t xx = 0; xx < this->samples; ++xx)
		{
			float rx = (2 * (x + xx / float(this->samples)) / this->width - 1) * tan(this->fov / 2 * M_PI / 180) * ratio;
			float ry = (1 - 2 * (y + yy / float(this->samples)) / this->height) * tan(this->fov / 2 * M_PI / 180);
			ray.dir = normalize(this->mat * Vec3(rx, ry, 1));
			float zIndex = 0;
			FragmentContext ctx;
			ctx.reflectionRecursion = 0;
			ctx.globalIlluminationRecursion = 0;
			ctx.rnd = std::minstd_rand(rand());
			Vec4 col(getRayColor(ctx, ray, nullptr, &zIndex));
			col = clamp(col, 0.f, 1.f);
			if (zIndex < lowestZ)
				lowestZ = zIndex;
			colorSum += col;
		}
	}
	size_t idx = x + y * this->width;
	this->zBuffer[idx] = lowestZ;
	Vec4 col(colorSum / float(this->samples * this->samples));
	col = clamp(col, 0.f, 1.f);
	return col;
}

bool Raytracer::findBatch(enum BatchState state, enum BatchState setState, size_t *batchX, size_t *batchY)
{
	std::lock_guard<std::mutex> guard(this->batchesMutex);
	ssize_t batchesY = (this->batches.size() + 1) / 2;
	ssize_t batchesX = (this->batches[0].size() + 1) / 2;
	ssize_t batchesSize = std::max(batchesY, batchesX) + 1;
	for (ssize_t i = 0; i < batchesSize; ++i)
	{
		for (ssize_t yy = batchesY - i; yy < batchesY + i; ++yy)
		{
			for (ssize_t xx = batchesX - i; xx < batchesX + i; ++xx)
			{
				if (yy < 0 || size_t(yy) >= this->batches.size())
					continue;
				if (xx < 0 || size_t(xx) >= this->batches[0].size())
					continue;
				if (this->batches[yy][xx] == state)
				{
					*batchY = yy;
					*batchX = xx;
					this->batches[yy][xx] = setState;
					return true;
				}
			}
		}
	}
	return false;
}

void Raytracer::runThreadFiltering()
{
	while (true)
	{
		size_t batchX;
		size_t batchY;
		if (!findBatch(BATCH_NEED_FILTERING, BATCH_FILTERING, &batchX, &batchY))
			return;
		uint8_t calc[BATCH_SIZE * BATCH_SIZE];
		std::fill(std::begin(calc), std::end(calc), 0);
		size_t startX = batchX * BATCH_SIZE;
		size_t startY = batchY * BATCH_SIZE;
		size_t endX = std::min(this->width, startX + BATCH_SIZE);
		size_t endY = std::min(this->height, startY + BATCH_SIZE);
		for (size_t i = BATCH_SIZE / 2; i; i /= 2)
		{
			for (size_t y = startY; y < endY; y += i)
			{
				for (size_t x = startX; x < endX; x += i)
				{
					if (calc[(y % BATCH_SIZE) * BATCH_SIZE + (x % BATCH_SIZE)])
						continue;
					calc[(y % BATCH_SIZE) * BATCH_SIZE + (x % BATCH_SIZE)] = 1;
					Vec4 col((*this->currentFilter)(this->filterSrc->data(), this->zBuffer.data(), x, y, this->width, this->height));
					size_t idx = x + y * this->width;
					(*this->filterDst)[idx] = col;
					this->imgData[idx].r = col.r * 0xff;
					this->imgData[idx].g = col.g * 0xff;
					this->imgData[idx].b = col.b * 0xff;
					this->imgData[idx].a = col.a * 0xff;
				}
			}
		}
		{
			std::lock_guard<std::mutex> guard(this->batchesMutex);
			this->batches[batchY][batchX] = BATCH_DONE;
		}
	}
}

void Raytracer::runThreadCalculation()
{
	while (true)
	{
		size_t batchX;
		size_t batchY;
		if (!findBatch(BATCH_NEED_CALCULATION, BATCH_CALCULATING, &batchX, &batchY))
			return;
		uint8_t calc[BATCH_SIZE * BATCH_SIZE];
		std::fill(std::begin(calc), std::end(calc), 0);
		size_t startX = batchX * BATCH_SIZE;
		size_t startY = batchY * BATCH_SIZE;
		size_t endX = std::min(this->width, startX + BATCH_SIZE);
		size_t endY = std::min(this->height, startY + BATCH_SIZE);
		for (size_t i = BATCH_SIZE / 2; i; i /= 2)
		{
			for (size_t y = startY; y < endY; y += i)
			{
				for (size_t x = startX; x < endX; x += i)
				{
					if (!calc[(y % BATCH_SIZE) * BATCH_SIZE + (x % BATCH_SIZE)])
					{
						calc[(y % BATCH_SIZE) * BATCH_SIZE + (x % BATCH_SIZE)] = 1;
						Vec4 col(this->calculatePixel(x, y));
						size_t idx = x + y * this->width;
						this->colorBuffer[idx] = col;
						this->imgData[idx].r = col.r * 0xff;
						this->imgData[idx].g = col.g * 0xff;
						this->imgData[idx].b = col.b * 0xff;
						this->imgData[idx].a = col.a * 0xff;
					}
					size_t base = x + y * this->width;
					for (size_t yy = 0; yy < i; ++yy)
					{
						for (size_t xx = 0; xx < i; ++xx)
						{
							size_t idx = x + xx + (y + yy) * this->width;
							if (!idx)
								continue;
							this->imgData[idx] = this->imgData[base];
						}
					}
				}
			}
		}
		{
			std::lock_guard<std::mutex> guard(this->batchesMutex);
			if (this->filters.size())
				this->batches[batchY][batchX] = BATCH_NEED_FILTERING;
			else
				this->batches[batchY][batchX] = BATCH_DONE;
		}
	}
}

void Raytracer::runThread(Raytracer *raytracer)
{
	std::unique_lock<std::mutex> lock(raytracer->conditionMutex, std::defer_lock);
	while (true)
	{
		if (raytracer->threadsStopped)
			return;
		if (raytracer->threadsAction == THREAD_CALCULATION)
			raytracer->runThreadCalculation();
		else if (raytracer->threadsAction == THREAD_FILTERING)
			raytracer->runThreadFiltering();
		raytracer->threadsFinished++;
		lock.lock();
		raytracer->finishedCondition.notify_one();
		raytracer->runCondition.wait(lock);
		lock.unlock();
	}
}

void Raytracer::render()
{
	uint64_t started;
	uint64_t ended;
	std::unique_lock<std::mutex> lock(this->conditionMutex, std::defer_lock);
	started = System::nanotime();
	this->threadsStopped = false;
	this->threadsAction = THREAD_CALCULATION;
	this->threadsFinished = 0;
	this->threads.resize(this->threadsCount);
	for (uint8_t i = 0; i < this->threadsCount; ++i)
	{
		this->threads[i] = new std::thread(&Raytracer::runThread, this);
	}
	lock.lock();
	this->finishedCondition.wait(lock, [this]{return this->threadsFinished == this->threadsCount;});
	lock.unlock();
	ended = System::nanotime();
	LOG("Draw: " << (ended - started) / 1000000 << " ms");
	if (this->filters.size())
	{
		this->threadsAction = THREAD_FILTERING;
		started = System::nanotime();
		std::vector<Vec4> tmp(this->width * this->height);
		this->filterDst = &tmp;
		this->filterSrc = &this->colorBuffer;
		for (size_t i = 0; i < this->filters.size(); ++i)
		{
			for (size_t y = 0; y < this->batches.size(); ++y)
			{
				for (size_t x = 0; x < this->batches[y].size(); ++x)
					this->batches[y][x] = BATCH_NEED_FILTERING;
			}
			this->threadsFinished = 0;
			this->currentFilter = this->filters[i];
			lock.lock();
			this->runCondition.notify_all();
			this->finishedCondition.wait(lock, [this]{return this->threadsFinished == this->threadsCount;});
			lock.unlock();
			std::swap(this->filterSrc, this->filterDst);
		}
		if (this->filterSrc != &this->colorBuffer)
			std::copy(std::begin(*this->filterSrc), std::end(*this->filterSrc), std::begin(this->colorBuffer));
		ended = System::nanotime();
		LOG("Filters: " << (ended - started) / 1000000 << " ms");
	}
	this->threadsStopped = true;
	lock.lock();
	this->runCondition.notify_all();
	lock.unlock();
	for (uint8_t i = 0; i < this->threadsCount; ++i)
	{
		this->threads[i]->join();
		delete (this->threads[i]);
	}
	this->threads.clear();
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

void Raytracer::setCameraPosition(Vec3 position)
{
	this->position = position;
}

void Raytracer::setCameraRotation(Vec3 rotation)
{
	this->mat = Mat3(1);
	this->mat = Mat3::rotateZ(this->mat, rotation.z);
	this->mat = Mat3::rotateY(this->mat, rotation.y);
	this->mat = Mat3::rotateX(this->mat, rotation.x);
	this->invMat = Mat3(1);
	this->invMat = Mat3::rotateX(this->invMat, -rotation.x);
	this->invMat = Mat3::rotateY(this->invMat, -rotation.y);
	this->invMat = Mat3::rotateZ(this->invMat, -rotation.z);
}

void Raytracer::setCameraFov(float fov)
{
	this->fov = fov;
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

void Raytracer::setGlobalIlluminationDistance(float distance)
{
	this->globalIlluminationDistance = distance;
}

void Raytracer::setGlobalIlluminationSamples(size_t samples)
{
	this->globalIlluminationSamples = samples;
}

void Raytracer::setGlobalIlluminationFactor(float factor)
{
	this->globalIlluminationFactor = factor;
}

void Raytracer::setGlobalIllumination(bool globalIllumination)
{
	this->globalIllumination = globalIllumination;
}

void Raytracer::setAmbientOcclusionDistance(float distance)
{
	this->ambientOcclusionDistance = distance;
}

void Raytracer::setAmbientOcclusionSamples(size_t samples)
{
	this->ambientOcclusionSamples = samples;
}

void Raytracer::setAmbientOcclusionFactor(float factor)
{
	this->ambientOcclusionFactor = factor;
}

void Raytracer::setAmbientOcclusion(bool ambientOcclusion)
{
	this->ambientOcclusion = ambientOcclusion;
}
