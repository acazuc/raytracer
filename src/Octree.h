#ifndef OCTREE_H
# define OCTREE_H

# include "Math/Vec3.h"
# include <vector>

class Object;

class OctreeNode
{

	private:
		std::vector<Object*> objects;
		Vec3 size;
		Vec3 pos;
		OctreeNode *childs[8];

	public:
		OctreeNode(Vec3 pos, Vec3 size);
		~OctreeNode();
		void addObject(Object *object);
		inline std::vector<Object*> &getObjects() {return this->objects;};

};

class Octree
{

	private:
		OctreeNode *node;

	public:
		Octree();
		~Octree();
		void addObject(Object *object);

};

#endif
