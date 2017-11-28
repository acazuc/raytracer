#ifndef RAYTRACER_H
# define RAYTRACER_H

# define THREAD_NUMBER 1

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
		uint32_t height;
		uint32_t width;
		float fov;
		char *imgData;
		static void runThread(Raytracer *raytracer, uint64_t start, uint64_t end);
		void calculatePixel(uint64_t x, uint64_t y);
		bool trace(Ray &ray, Object *&object, Vec3 &position);

	public:
		Raytracer(uint32_t width, uint32_t height);
		~Raytracer();
		void render();
		void addObject(Object *object);
		void addLight(Light *light);
		void setFov(float fov) {this->fov = fov;};
		inline char *getImgData() {return (this->imgData);};

};

#endif
