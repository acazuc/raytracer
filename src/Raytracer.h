#ifndef RAYTRACER_H
# define RAYTRACER_H

# define THREAD_NUMBER 8

# include "Objects/Object.h"
# include "Lights/Light.h"
# include <cstdint>
# include <vector>
# include <thread>

class Raytracer
{

	private:
		std::vector<Object*> objects;
		std::vector<Light*> lights;
		std::thread *threads[THREAD_NUMBER];
		Vec3 ambient;
		uint32_t height;
		uint32_t width;
		float *colorBuffer;
		float *zBuffer;
		float fov;
		char *imgData;
		static void runThread(Raytracer *raytracer, uint64_t start, uint64_t end);
		void calculatePixel(uint64_t x, uint64_t y);
		Vec3 getRayColor(Ray &ray, Object *avoid, int recursion);
		bool trace(Ray &ray, Object *&object, Vec3 &position, Object *avoid);
		void getDiffuseSpecularLight(Ray &ray, Object *object, Vec3 &pos, Vec3 &norm, Vec3 &diffuse, Vec3 &specular);
		Vec3 getReflectionColor(Ray &ray, Object *object, Vec3 &pos, Vec3 &norm, int recursion);

	public:
		Raytracer(uint32_t width, uint32_t height);
		~Raytracer();
		void render();
		void addObject(Object *object);
		void addLight(Light *light);
		void setAmbient(Vec3 ambient) {this->ambient = ambient;};
		void setFov(float fov) {this->fov = fov;};
		inline char *getImgData() {return (this->imgData);};

};

#endif
