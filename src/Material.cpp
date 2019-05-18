#include "Material.h"

Material::Material()
: specularTexture(nullptr)
, emissiveTexture(nullptr)
, diffuseTexture(nullptr)
, normalTexture(nullptr)
, emissiveColor(0)
, specularColor(0)
, diffuseColor(.5)
, specularFactor(100)
, reflection(0)
, refraction(1)
, opacity(1)
{
}
