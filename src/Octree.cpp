#include "Octree.h"
#include "Objects/Object.h"

#define MAX_OBJECTS 10

OctreeNode::OctreeNode(Vec3 pos, Vec3 size)
: size(size)
, pos(pos)
{
	this->childs[0] = nullptr;
}

OctreeNode::~OctreeNode()
{
	if (this->childs[0])
	{
		for (size_t i = 0; i < 8; ++i)
			delete (this->childs[i]);
	}
}

void OctreeNode::addObject(Object *object)
{
	if (!this->childs[0] && this->objects.size() < MAX_OBJECTS)
	{
		this->objects.push_back(object);
		return;
	}
	if (!this->childs[0])
	{
		Vec3 size2(this->size / 2.f);
		Vec3 pos2(this->pos + size2);
		this->childs[0] = new OctreeNode(Vec3(pos.x , pos.y , pos.z ), size2);
		this->childs[1] = new OctreeNode(Vec3(pos2.x, pos.y , pos.z ), size2);
		this->childs[2] = new OctreeNode(Vec3(pos.x , pos2.y, pos.z ), size2);
		this->childs[3] = new OctreeNode(Vec3(pos2.x, pos2.y, pos.z ), size2);
		this->childs[4] = new OctreeNode(Vec3(pos.x , pos.y , pos2.z), size2);
		this->childs[5] = new OctreeNode(Vec3(pos2.x, pos.y , pos2.z), size2);
		this->childs[6] = new OctreeNode(Vec3(pos.x , pos2.y, pos2.z), size2);
		this->childs[7] = new OctreeNode(Vec3(pos2.x, pos2.y, pos2.z), size2);
	}
}

Octree::Octree()
: node(nullptr)
{
}

Octree::~Octree()
{
	delete (this->node);
}

void Octree::addObject(Object *object)
{
	if (!this->node)
		this->node = new OctreeNode(Vec3(-10000), Vec3(20000));
	this->node->addObject(object);
}
