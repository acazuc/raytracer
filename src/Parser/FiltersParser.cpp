#include "FiltersParser.h"
#include "Filters/Color/Saturation.h"
#include "Filters/Color/Brightness.h"
#include "Filters/Color/GreyShade.h"
#include "Filters/Color/Contrast.h"
#include "Filters/Color/Negative.h"
#include "Filters/DepthOfField.h"
#include "Filters/Color/Gamma.h"
#include "Filters/Color/Sepia.h"
#include "Filters/Denoising.h"
#include "Filters/Fisheye.h"
#include "Filters/Sobel.h"
#include "Filters/Fxaa.h"
#include "Filters/Blur.h"
#include "Filters/Glow.h"
#include "Filters/Cel.h"
#include "Raytracer.h"

FilterDepthOfFieldParser::FilterDepthOfFieldParser()
: aperture(178)
, focal(1)
{
	registerAttributeCallback("aperture", ParserAttributeCallback(&FilterDepthOfFieldParser::parseAperture));
	registerAttributeCallback("focal", ParserAttributeCallback(&FilterDepthOfFieldParser::parseFocal));
}

void FilterDepthOfFieldParser::parseAperture(xmlAttr *attr)
{
	parseAttrFloat(attr, &this->aperture);
}

void FilterDepthOfFieldParser::parseFocal(xmlAttr *attr)
{
	parseAttrFloat(attr, &this->focal);
}

Filter *FilterDepthOfFieldParser::toFilter()
{
	return new DepthOfField(this->focal, this->aperture);
}

FilterCelShadingParser::FilterCelShadingParser()
: cels(10)
{
	registerAttributeCallback("cels", ParserAttributeCallback(&FilterCelShadingParser::parseCels));
}

void FilterCelShadingParser::parseCels(xmlAttr *attr)
{
	size_t cels;
	parseAttrUInt(attr, &cels);
	this->cels = cels;
}

Filter *FilterCelShadingParser::toFilter()
{
	return new Cel(this->cels);
}

FilterSaturationParser::FilterSaturationParser()
: value(1)
{
	registerAttributeCallback("value", ParserAttributeCallback(&FilterSaturationParser::parseValue));
}

void FilterSaturationParser::parseValue(xmlAttr *attr)
{
	parseAttrFloat(attr, &this->value);
}

Filter *FilterSaturationParser::toFilter()
{
	return new Saturation(this->value);
}

FilterBrightnessParser::FilterBrightnessParser()
: value(1)
{
	registerAttributeCallback("value", ParserAttributeCallback(&FilterBrightnessParser::parseValue));
}

void FilterBrightnessParser::parseValue(xmlAttr *attr)
{
	parseAttrFloat(attr, &this->value);
}

Filter *FilterBrightnessParser::toFilter()
{
	return new Brightness(this->value);
}

Filter *FilterGreyShadeParser::toFilter()
{
	return new GreyShade();
}

FilterDenoisingParser::FilterDenoisingParser()
: threshold(0.999)
, radius(2)
{
	registerAttributeCallback("threshold", ParserAttributeCallback(&FilterDenoisingParser::parseThreshold));
	registerAttributeCallback("radius", ParserAttributeCallback(&FilterDenoisingParser::parseRadius));
}

void FilterDenoisingParser::parseThreshold(xmlAttr *attr)
{
	parseAttrFloat(attr, &this->threshold);
}

void FilterDenoisingParser::parseRadius(xmlAttr *attr)
{
	parseAttrFloat(attr, &this->radius);
}

Filter *FilterDenoisingParser::toFilter()
{
	return new Denoising(this->radius, this->threshold);
}

FilterContrastParser::FilterContrastParser()
: value(1)
{
	registerAttributeCallback("value", ParserAttributeCallback(&FilterContrastParser::parseValue));
}

void FilterContrastParser::parseValue(xmlAttr *attr)
{
	parseAttrFloat(attr, &this->value);
}

Filter *FilterContrastParser::toFilter()
{
	return new Contrast(this->value);
}

Filter *FilterNegativeParser::toFilter()
{
	return new Negative();
}

FilterFisheyeParser::FilterFisheyeParser()
: aperture(178)
{
	registerAttributeCallback("aperture", ParserAttributeCallback(&FilterFisheyeParser::parseAperture));
}

void FilterFisheyeParser::parseAperture(xmlAttr *attr)
{
	parseAttrFloat(attr, &this->aperture);
}

Filter *FilterFisheyeParser::toFilter()
{
	return new Fisheye(this->aperture);
}

FilterSobelParser::FilterSobelParser()
: threshold(0)
{
	registerAttributeCallback("threshold", ParserAttributeCallback(&FilterSobelParser::parseThreshold));
}

void FilterSobelParser::parseThreshold(xmlAttr *attr)
{
	parseAttrFloat(attr, &this->threshold);
}

Filter *FilterSobelParser::toFilter()
{
	return new Sobel(this->threshold);
}

FilterGammaParser::FilterGammaParser()
: value(1)
{
	registerAttributeCallback("value", ParserAttributeCallback(&FilterGammaParser::parseValue));
}

void FilterGammaParser::parseValue(xmlAttr *attr)
{
	parseAttrFloat(attr, &this->value);
}

Filter *FilterGammaParser::toFilter()
{
	return new Gamma(this->value);
}

Filter *FilterSepiaParser::toFilter()
{
	return new Sepia();
}

Filter *FilterFxaaParser::toFilter()
{
	return new Fxaa();
}

FilterBlurParser::FilterBlurParser()
: radius(0)
{
	registerAttributeCallback("radius", ParserAttributeCallback(&FilterBlurParser::parseRadius));
}

void FilterBlurParser::parseRadius(xmlAttr *attr)
{
	parseAttrFloat(attr, &this->radius);
}

Filter *FilterBlurParser::toFilter()
{
	return new Blur(this->radius);
}

FilterGlowParser::FilterGlowParser()
: threshold(0.9)
, intensity(1)
, radius(20)
{
	registerAttributeCallback("threshold", ParserAttributeCallback(&FilterGlowParser::parseThreshold));
	registerAttributeCallback("intensity", ParserAttributeCallback(&FilterGlowParser::parseIntensity));
	registerAttributeCallback("radius", ParserAttributeCallback(&FilterGlowParser::parseRadius));
}

void FilterGlowParser::parseThreshold(xmlAttr *attr)
{
	parseAttrFloat(attr, &this->threshold);
}

void FilterGlowParser::parseIntensity(xmlAttr *attr)
{
	parseAttrFloat(attr, &this->intensity);
}

void FilterGlowParser::parseRadius(xmlAttr *attr)
{
	parseAttrFloat(attr, &this->radius);
}

Filter *FilterGlowParser::toFilter()
{
	return new Glow(this->radius, this->threshold, this->intensity);
}

FiltersParser::FiltersParser()
: depthOfField(nullptr)
, celShading(nullptr)
, saturation(nullptr)
, brightness(nullptr)
, greyShade(nullptr)
, denoising(nullptr)
, contrast(nullptr)
, negative(nullptr)
, fisheye(nullptr)
, sobel(nullptr)
, gamma(nullptr)
, sepia(nullptr)
, fxaa(nullptr)
, blur(nullptr)
, glow(nullptr)
{
	registerNodeCallback("DepthOfField", ParserNodeCallback(&FiltersParser::parseDepthOfField));
	registerNodeCallback("CelShading", ParserNodeCallback(&FiltersParser::parseCelShading));
	registerNodeCallback("Saturation", ParserNodeCallback(&FiltersParser::parseSaturation));
	registerNodeCallback("Brightness", ParserNodeCallback(&FiltersParser::parseBrightness));
	registerNodeCallback("GreyShade", ParserNodeCallback(&FiltersParser::parseGreyShade));
	registerNodeCallback("Denoising", ParserNodeCallback(&FiltersParser::parseDenoising));
	registerNodeCallback("Contrast", ParserNodeCallback(&FiltersParser::parseContrast));
	registerNodeCallback("Negative", ParserNodeCallback(&FiltersParser::parseNegative));
	registerNodeCallback("Fisheye", ParserNodeCallback(&FiltersParser::parseFisheye));
	registerNodeCallback("Sobel", ParserNodeCallback(&FiltersParser::parseSobel));
	registerNodeCallback("Gamma", ParserNodeCallback(&FiltersParser::parseGamma));
	registerNodeCallback("Sepia", ParserNodeCallback(&FiltersParser::parseSepia));
	registerNodeCallback("Fxaa", ParserNodeCallback(&FiltersParser::parseFxaa));
	registerNodeCallback("Blur", ParserNodeCallback(&FiltersParser::parseBlur));
	registerNodeCallback("Glow", ParserNodeCallback(&FiltersParser::parseGlow));
}

FiltersParser::~FiltersParser()
{
	delete (this->depthOfField);
	delete (this->celShading);
	delete (this->brightness);
	delete (this->greyShade);
	delete (this->denoising);
	delete (this->contrast);
	delete (this->negative);
	delete (this->fisheye);
	delete (this->sobel);
	delete (this->gamma);
	delete (this->sepia);
	delete (this->fxaa);
	delete (this->blur);
	delete (this->glow);
}

void FiltersParser::parseDepthOfField(xmlNode *node)
{
	delete (this->depthOfField);
	this->depthOfField = new FilterDepthOfFieldParser();
	this->depthOfField->parse(node);
}

void FiltersParser::parseCelShading(xmlNode *node)
{
	delete (this->celShading);
	this->celShading = new FilterCelShadingParser();
	this->celShading->parse(node);
}

void FiltersParser::parseSaturation(xmlNode *node)
{
	delete (this->saturation);
	this->saturation = new FilterSaturationParser();
	this->saturation->parse(node);
}

void FiltersParser::parseBrightness(xmlNode *node)
{
	delete (this->brightness);
	this->brightness = new FilterBrightnessParser();
	this->brightness->parse(node);
}

void FiltersParser::parseGreyShade(xmlNode *node)
{
	delete (this->greyShade);
	this->greyShade = new FilterGreyShadeParser();
	this->greyShade->parse(node);
}

void FiltersParser::parseDenoising(xmlNode *node)
{
	delete (this->denoising);
	this->denoising = new FilterDenoisingParser();
	this->denoising->parse(node);
}

void FiltersParser::parseContrast(xmlNode *node)
{
	delete (this->contrast);
	this->contrast = new FilterContrastParser();
	this->contrast->parse(node);
}

void FiltersParser::parseNegative(xmlNode *node)
{
	delete (this->negative);
	this->negative = new FilterNegativeParser();
	this->negative->parse(node);
}

void FiltersParser::parseFisheye(xmlNode *node)
{
	delete (this->fisheye);
	this->fisheye = new FilterFisheyeParser();
	this->fisheye->parse(node);
}

void FiltersParser::parseSobel(xmlNode *node)
{
	delete (this->sobel);
	this->sobel = new FilterSobelParser();
	this->sobel->parse(node);
}

void FiltersParser::parseGamma(xmlNode *node)
{
	delete (this->gamma);
	this->gamma = new FilterGammaParser();
	this->gamma->parse(node);
}

void FiltersParser::parseSepia(xmlNode *node)
{
	delete (this->sepia);
	this->sepia = new FilterSepiaParser();
	this->sepia->parse(node);
}

void FiltersParser::parseFxaa(xmlNode *node)
{
	delete (this->fxaa);
	this->fxaa = new FilterFxaaParser();
	this->fxaa->parse(node);
}

void FiltersParser::parseBlur(xmlNode *node)
{
	delete (this->blur);
	this->blur = new FilterBlurParser();
	this->blur->parse(node);
}

void FiltersParser::parseGlow(xmlNode *node)
{
	delete (this->glow);
	this->glow = new FilterGlowParser();
	this->glow->parse(node);
}

void FiltersParser::addFilters(Raytracer *raytracer)
{
	if (this->denoising)
		raytracer->addFilter(this->denoising->toFilter());
	if (this->gamma)
		raytracer->addFilter(this->gamma->toFilter());
	if (this->contrast)
		raytracer->addFilter(this->contrast->toFilter());
	if (this->brightness)
		raytracer->addFilter(this->brightness->toFilter());
	if (this->saturation)
		raytracer->addFilter(this->saturation->toFilter());
	if (this->negative)
		raytracer->addFilter(this->negative->toFilter());
	if (this->sepia)
		raytracer->addFilter(this->sepia->toFilter());
	if (this->depthOfField)
		raytracer->addFilter(this->depthOfField->toFilter());
	if (this->sobel)
		raytracer->addFilter(this->sobel->toFilter());
	if (this->greyShade)
		raytracer->addFilter(this->greyShade->toFilter());
	if (this->fisheye)
		raytracer->addFilter(this->fisheye->toFilter());
	if (this->celShading)
		raytracer->addFilter(this->celShading->toFilter());
	if (this->fxaa)
		raytracer->addFilter(this->fxaa->toFilter());
	if (this->blur)
		raytracer->addFilter(this->blur->toFilter());
	if (this->glow)
		raytracer->addFilter(this->glow->toFilter());
}
