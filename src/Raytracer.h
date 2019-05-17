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

class Filter;
class Object;
class Light;
class Ray;

class Raytracer
{

	private:
		std::vector<std::vector<enum BatchState>> batches;
		std::vector<std::thread*> threads;
		std::vector<Filter*> filters;
		std::vector<Object*> objects;
		std::vector<Light*> lights;
		std::condition_variable finishedCondition;
		std::condition_variable runCondition;
		std::mutex conditionMutex;
		std::mutex batchesMutex;
		Filter *currentFilter;
		Vec4 *filterSrc;
		Vec4 *filterDst;
		Vec4 *colorBuffer;
		Mat3 unrotMat;
		Vec3 ambient;
		Mat3 rotMat;
		Vec3 pos;
		enum ThreadAction threadsAction;
		uint8_t threadsFinished;
		uint8_t threadsCount;
		uint8_t samples;
		size_t height;
		size_t width;
		float *zBuffer;
		float fov;
		bool globalIllumination;
		bool ambientOcclusion;
		bool threadsStopped;
		bool shading;
		TVec4<uint8_t> *imgData;
		static void runThread(Raytracer *raytracer);
		bool findBatch(enum BatchState state, enum BatchState setState, size_t *batchX, size_t *batchY);
		void runThreadCalculation();
		void runThreadFiltering();
		void calculatePixel(size_t x, size_t y);
		Vec3 getRayColor(Ray &ray, Object *avoid, int recursion, float *zIndex = nullptr);
		bool trace(Ray &ray, Object *&object, Vec3 &pos, Object *avoid);
		void getDiffuseSpecularLight(Ray &ray, Object *object, Vec3 &pos, Vec3 &norm, Vec3 &diffuse, Vec3 &specular);
		Vec3 getReflectionColor(Ray &ray, Object *object, Vec3 &pos, Vec3 &norm, int recursion);
		Vec3 getTransparencyColor(Ray &ray, Object *object, Vec3 &pos, Vec3 &norm, bool normRev, int recursion);
		Vec4 getDiffuseSpecularTransparencyLight(Light *light, Object *object, Ray &ray, Vec3 &pos);
		float getAmbientOcclusion(Vec3 &pos, Vec3 &norm, Object *object);
		Vec3 getGlobalIllumination(Vec3 &pos, Vec3 &norm, Object *object, int recursion);

	public:
		Raytracer(size_t width, size_t height);
		~Raytracer();
		void render();
		void addFilter(Filter *filter);
		void addObject(Object *object);
		void addLight(Light *light);
		void setAmbient(Vec3 ambient) {this->ambient = ambient;};
		void setPos(Vec3 pos) {this->pos = pos;};
		void setRot(Vec3 rot);
		void setFov(float fov) {this->fov = fov;};
		void setSamples(uint8_t samples);
		void setThreads(uint8_t threads);
		void setShading(bool shading);
		void setAmbientOcclusion(bool ambientOcclusion);
		void setGlobalIllumination(bool globalIllumination);
		inline TVec4<uint8_t> *getImgData() {return this->imgData;};
		inline std::vector<std::vector<enum BatchState>> &getBatches() {return this->batches;};
		inline std::mutex &getBatchesMutex() {return this->batchesMutex;};
		inline size_t getWidth() {return this->width;};
		inline size_t getHeight() {return this->height;};

};

#endif
