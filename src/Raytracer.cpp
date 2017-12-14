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
#include "Debug.h"
#include <cstring>
#include <cmath>

Raytracer::Raytracer(uint32_t width, uint32_t height)
: ambient(0, 0, 0)
, height(height)
, width(width)
{
	this->colorBuffer = new Vec4[width * height];
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
		float length = Vec3(*vec - ray.pos).length();
		if (nearestDistance != -1 && length > nearestDistance)
			continue;
		object = this->objects[i];
		position = *vec;
		nearestDistance = length;
		delete (vec);
	}
	return (nearestDistance != -1);
}

Vec4 Raytracer::getDiffuseSpecularTransparencyLight(Light *light, Object *object, Ray &ray, Vec3 &pos)
{
	Vec4 result(object->Kd);
	if (object->texture)
		result *= object->texture->getDataAt(object->getUVAt(ray, pos));
	if (result.a >= 1)
		return (result);
	Ray newRay(pos, Vec3(0));
	newRay.dir = light->getDirectionFrom(pos);
	Object *newObject = nullptr;
	Vec3 newPos(0);
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
	Object *collObject = nullptr;
	Vec3 collPosition(0);
	Ray diffuseRay(pos, Vec3(0));
	Vec3 reflect = ray.dir.reflect(norm);
	float opacity;
	for (uint64_t i = 0; i < this->lights.size(); ++i)
	{
		diffuseRay.dir = this->lights[i]->getDirectionFrom(pos);
		Vec3 tmp = diffuseRay.dir;
		Vec4 lightColorIntensity = Vec4(this->lights[i]->color * this->lights[i]->intensity, 1);
		if (trace(diffuseRay, collObject, collPosition, object))
		{
			if ((collPosition - pos).length() > tmp.length())
				goto next;
			opacity = collObject->Kd.a;
			if (collObject->texture)
				opacity *= collObject->texture->getDataAt(collObject->getUVAt(diffuseRay, collPosition)).a;
			if (opacity >= 1)
				continue;
			Vec4 result = getDiffuseSpecularTransparencyLight(this->lights[i], collObject, diffuseRay, collPosition);
			lightColorIntensity = lightColorIntensity * (1 - result.a) * Vec4(result.rgb(), 1);
		}
next:
		diffuseRay.dir.normalize();
		diffuse += lightColorIntensity * std::max(0.f, diffuseRay.dir.dot(norm));
		specular += lightColorIntensity * pow(std::max(0.f, reflect.dot(diffuseRay.dir)), object->Ns) * object->Ks;
	}
}

Vec4 Raytracer::getReflectionColor(Ray &ray, Object *object, Vec3 &pos, Vec3 &norm, int recursion)
{
	if (recursion > 100)
		return (Vec4(0, 0, 0));
	Ray newRay;
	newRay.pos = pos;
	newRay.dir = ray.dir.reflect(norm);
	return (getRayColor(newRay, object, recursion));
}

Vec4 Raytracer::getTransparencyColor(Ray &ray, Object *object, Vec3 &pos, Vec3 &norm, int recursion)
{
	Ray newRay;
	newRay.pos = pos;
	if (object->Ni == 1)
	{
		newRay.dir = ray.dir;
	}
	else
	{
		float refFactor = 1 / object->Ni;
		float costhetai = cos(ray.dir.angle(norm));
		float sin2thetat = refFactor * refFactor * (costhetai * costhetai - 1);
		newRay.dir = ray.dir * refFactor + norm * (refFactor * costhetai + sqrt(1 + sin2thetat));
	}
	return (getRayColor(newRay, object, recursion));
}

Vec4 Raytracer::getRayColor(Ray &ray, Object *avoid, int recursion, float *zIndex)
{

	Object *object = nullptr;
	Vec3 pos(0);
	if (!trace(ray, object, pos, avoid))
		return (Vec4(0));
	if (zIndex)
		*zIndex = (pos - ray.pos).length();
	Vec3 norm = object->getNormAt(ray, pos);
	Vec4 diffuse(0);
	Vec4 specular(0);
	getDiffuseSpecularLight(ray, object, pos, norm, diffuse, specular);
	Vec4 texColor(1);
	if (object->texture)
		texColor = object->texture->getDataAt(object->getUVAt(ray, pos));
	Vec4 col = diffuse + Vec4(this->ambient, 0);
	float transparency = object->Kd.a * texColor.a;
	if (transparency < 1)
		col = col * transparency + getTransparencyColor(ray, object, pos, norm, recursion) * (1 - transparency);
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
	float angleY = (x - this->width / 2.) / this->width * (this->fov / 180. * M_PI);
	float angleX = (y - this->height / 2.) / this->height * (this->fov / 180. * M_PI) / this->width * this->height;
	ray.dir.rotate(Vec3(angleX, angleY, 0) + this->rot);
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
	}
	ended = System::nanotime();
	LOG("Draw: " << (ended - started) / 1000000. << " ms");
	/*Vec3 *cel = Cel::cel(this->colorBuffer, 20, this->width, this->height);
	delete[] (this->colorBuffer);
	this->colorBuffer = cel;*/
	/*Vec3 *sobel = Sobel::sobel(this->colorBuffer, this->zBuffer, this->width, this->height);
	delete[] (this->colorBuffer);
	this->colorBuffer = sobel;*/
	/*Vec3 *fisheye = Fisheye::fisheye(this->colorBuffer, this->width, this->height);
	delete[] (this->colorBuffer);
	this->colorBuffer = fisheye;*/
	/*Vec3 *fxaa = Fxaa::fxaa(this->colorBuffer, this->width, this->height);
	delete[] (this->colorBuffer);
	this->colorBuffer = fxaa;*/
	/*Vec3 *blur = Blur::blur(this->colorBuffer, 0, this->width, this->height);
	delete[] (this->colorBuffer);
	this->colorBuffer = blur;*/
	/*Vec3 *greyShade = GreyShade::greyShade(this->colorBuffer, this->width, this->height);
	delete[] (this->colorBuffer);
	this->colorBuffer = greyShade;*/
	/*Vec3 *negative = Negative::negative(this->colorBuffer, this->width, this->height);
	delete[] (this->colorBuffer);
	this->colorBuffer = negative;*/
	/*Vec3 *sepia = Sepia::sepia(this->colorBuffer, this->width, this->height);
	delete[] (this->colorBuffer);
	this->colorBuffer = sepia;*/
	/*Vec3 *gamma = Gamma::gamma(this->colorBuffer, this->width, this->height, 1 / 2.2);
	delete[] (this->colorBuffer);
	this->colorBuffer = gamma;*/
	for (uint64_t i = 0; i < this->width * this->height; ++i)
	{
		this->imgData[i * 4 + 0] = std::min((int64_t)0xff, std::max((int64_t)0, (int64_t)(this->colorBuffer[i].r * 0xff)));
		this->imgData[i * 4 + 1] = std::min((int64_t)0xff, std::max((int64_t)0, (int64_t)(this->colorBuffer[i].g * 0xff)));
		this->imgData[i * 4 + 2] = std::min((int64_t)0xff, std::max((int64_t)0, (int64_t)(this->colorBuffer[i].b * 0xff)));
		this->imgData[i * 4 + 3] = std::min((int64_t)0xff, std::max((int64_t)0, (int64_t)(this->colorBuffer[i].a * 0xff)));
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
