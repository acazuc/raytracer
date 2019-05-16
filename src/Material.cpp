#include "Material.h"

Material::Material()
: diffuseTexture(nullptr)
, normalTexture(nullptr)
, specularColor(0)
, diffuseColor(.5)
, ambientColor(0)
, specularFactor(100)
, reflection(0)
, refraction(1)
, opacity(1)
{
}
