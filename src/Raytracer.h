#ifndef RAYTRACER_H
# define RAYTRACER_H

# define BATCH_SIZE 32

# include "Math/Vec4.h"
# include "Math/Vec3.h"
# include "Math/Mat3.h"
# include <condition_variable>
# include <cstdint>
# include <vector>
# include <thread>
# include <random>
# include <atomic>
# include <mutex>

class Material;
class Filter;
class Object;
class Light;
class Ray;

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
	size_t reflectionRecursion;
	size_t globalIlluminationRecursion;
};

struct CollisionContext
{
	Object *object;
	float t;
	Vec3 pos;
	Vec3 norm;
	Vec2 UV;
	float tmpData[4];
};

class Raytracer
{

	private:
		std::vector<std::vector<enum BatchState>> batches;
		std::vector<TVec4<uint8_t>> imgData;
		std::vector<std::thread*> threads;
		std::vector<Material*> materials;
		std::atomic<uint8_t> threadsFinished;
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
		uint8_t threadsCount;
		uint8_t samples;
		size_t maxGlobalIlluminationRecursion;
		size_t maxReflection;
		size_t height;
		size_t width;
		float fov;
		size_t globalIlluminationSamples;
		float globalIlluminationFactor;
		bool globalIllumination;
		bool threadsStopped;
		bool shading;
		bool paused;
		static void runThread(Raytracer *raytracer);
		bool findBatch(enum BatchState state, enum BatchState setState, size_t *batchX, size_t *batchY);
		void runThreadCalculation();
		void runThreadFiltering();
		Vec4 calculatePixel(size_t x, size_t y);
		Vec4 getRayColor(FragmentContext &context, Ray &ray, Object *avoid, float *zIndex = nullptr);
		bool trace(Ray &ray, CollisionContext &collision, Object *avoid);
		void getDiffuseSpecularLight(Ray &ray, CollisionContext &collision, Vec3 &diffuse, Vec3 &specular);
		Vec4 getReflectionColor(FragmentContext &context, Ray &ray, CollisionContext &collision);
		Vec4 getTransparencyColor(FragmentContext &context, Ray &ray, CollisionContext &collision, bool normRev);
		Vec4 getDiffuseSpecularTransparencyLight(Light *light, CollisionContext &collision);
		Vec3 getGlobalIllumination(FragmentContext &context, CollisionContext &collision);

	public:
		Raytracer(size_t width, size_t height);
		~Raytracer();
		void render();
		void addMaterial(Material *material);
		void addFilter(Filter *filter);
		void addObject(Object *object);
		void addLight(Light *light);
		void setCameraPosition(Vec3 position);
		void setCameraRotation(Vec3 rotation);
		void setCameraFov(float fov);
		void setSamples(uint8_t samples);
		void setThreads(uint8_t threads);
		void setShading(bool shading);
		void setGlobalIlluminationSamples(size_t samples);
		void setGlobalIlluminationFactor(float factor);
		void setGlobalIllumination(bool globalIllumination);
		void setMaxReflection(size_t maxReflection);
		inline void pause() {this->paused = true;};
		inline void resume() {this->paused = false;};
		inline bool isPaused() {return this->paused;};
		inline std::vector<TVec4<uint8_t>> &getImgData() {return this->imgData;};
		inline std::vector<std::vector<enum BatchState>> &getBatches() {return this->batches;};
		inline std::mutex &getBatchesMutex() {return this->batchesMutex;};
		inline size_t getWidth() {return this->width;};
		inline size_t getHeight() {return this->height;};

};

#endif
