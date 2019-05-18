#ifndef RAYTRACER_H
# define RAYTRACER_H

# define BATCH_SIZE 32

# include "Vec/Vec4.h"
# include "Vec/Vec3.h"
# include "Mat/Mat3.h"
# include <condition_variable>
# include <cstdint>
# include <vector>
# include <thread>
# include <random>
# include <mutex>

enum BatchState
{
	BATCH_NEED_CALCULATION,
	BATCH_CALCULATING,
	BATCH_NEED_FILTERING,
	BATCH_FILTERING,
	BATCH_DONE
};

enum ThreadAction
{
	THREAD_CALCULATION,
	THREAD_FILTERING
};

struct FragmentContext
{
	std::minstd_rand rnd;
	int reflectionRecursion;
	int globalIlluminationRecursion;
};

class Filter;
class Object;
class Light;
class Ray;

class Raytracer
{

	private:
		std::vector<std::vector<enum BatchState>> batches;
		std::vector<TVec4<uint8_t>> imgData;
		std::vector<std::thread*> threads;
		std::vector<Filter*> filters;
		std::vector<Object*> objects;
		std::vector<Light*> lights;
		std::vector<float> zBuffer;
		std::vector<Vec4> colorBuffer;
		std::vector<Vec4> *filterSrc;
		std::vector<Vec4> *filterDst;
		std::condition_variable finishedCondition;
		std::condition_variable runCondition;
		std::mutex conditionMutex;
		std::mutex batchesMutex;
		Filter *currentFilter;
		Vec3 position;
		Mat3 invMat;
		Mat3 mat;
		enum ThreadAction threadsAction;
		uint8_t threadsFinished;
		uint8_t threadsCount;
		uint8_t samples;
		size_t height;
		size_t width;
		float fov;
		size_t globalIlluminationSamples;
		float globalIlluminationDistance;
		float globalIlluminationFactor;
		size_t ambientOcclusionSamples;
		float ambientOcclusionDistance;
		float ambientOcclusionFactor;
		bool globalIllumination;
		bool ambientOcclusion;
		bool threadsStopped;
		bool shading;
		static void runThread(Raytracer *raytracer);
		bool findBatch(enum BatchState state, enum BatchState setState, size_t *batchX, size_t *batchY);
		void runThreadCalculation();
		void runThreadFiltering();
		Vec4 calculatePixel(size_t x, size_t y);
		Vec4 getRayColor(FragmentContext &context, Ray &ray, Object *avoid, float *zIndex = nullptr);
		bool trace(Ray &ray, Object *&object, Vec3 &pos, Object *avoid);
		void getDiffuseSpecularLight(Ray &ray, Object *object, Vec3 &pos, Vec3 &norm, Vec3 &diffuse, Vec3 &specular);
		Vec4 getReflectionColor(FragmentContext &context, Ray &ray, Object *object, Vec3 &pos, Vec3 &norm);
		Vec4 getTransparencyColor(FragmentContext &context, Ray &ray, Object *object, Vec3 &pos, Vec3 &norm, bool normRev);
		Vec4 getDiffuseSpecularTransparencyLight(Light *light, Object *object, Ray &ray, Vec3 &pos);
		float getAmbientOcclusion(FragmentContext &context, Vec3 &pos, Vec3 &norm, Object *object);
		Vec3 getGlobalIllumination(FragmentContext &context, Vec3 &pos, Vec3 &norm, Object *object);

	public:
		Raytracer(size_t width, size_t height);
		~Raytracer();
		void render();
		void addFilter(Filter *filter);
		void addObject(Object *object);
		void addLight(Light *light);
		void setCameraPosition(Vec3 position);
		void setCameraRotation(Vec3 rotation);
		void setCameraFov(float fov);
		void setSamples(uint8_t samples);
		void setThreads(uint8_t threads);
		void setShading(bool shading);
		void setAmbientOcclusionDistance(float distance);
		void setAmbientOcclusionSamples(size_t samples);
		void setAmbientOcclusionFactor(float factor);
		void setAmbientOcclusion(bool ambientOcclusion);
		void setGlobalIlluminationDistance(float distance);
		void setGlobalIlluminationSamples(size_t samples);
		void setGlobalIlluminationFactor(float factor);
		void setGlobalIllumination(bool globalIllumination);
		inline std::vector<TVec4<uint8_t>> &getImgData() {return this->imgData;};
		inline std::vector<std::vector<enum BatchState>> &getBatches() {return this->batches;};
		inline std::mutex &getBatchesMutex() {return this->batchesMutex;};
		inline size_t getWidth() {return this->width;};
		inline size_t getHeight() {return this->height;};

};

#endif
