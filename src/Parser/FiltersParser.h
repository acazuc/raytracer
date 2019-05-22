#ifndef FILTERS_PARSER_H
# define FILTERS_PARSER_H

# include "./Parser.h"

class Raytracer;
class Filter;

class FilterDepthOfFieldParser : public Parser
{

	private:
		float aperture;
		float focal;
		void parseAperture(xmlAttr *attr);
		void parseFocal(xmlAttr *attr);

	public:
		FilterDepthOfFieldParser();
		Filter *toFilter();

};

class FilterCelShadingParser : public Parser
{

	private:
		uint8_t cels;
		void parseCels(xmlAttr *attr);

	public:
		FilterCelShadingParser();
		Filter *toFilter();

};

class FilterSaturationParser : public Parser
{

	private:
		float value;
		void parseValue(xmlAttr *attr);

	public:
		FilterSaturationParser();
		Filter *toFilter();

};

class FilterBrightnessParser : public Parser
{

	private:
		float value;
		void parseValue(xmlAttr *attr);

	public:
		FilterBrightnessParser();
		Filter *toFilter();

};

class FilterGreyShadeParser : public Parser
{

	public:
		Filter *toFilter();

};

class FilterDenoisingParser : public Parser
{

	private:
		float threshold;
		float radius;
		void parseThreshold(xmlAttr *attr);
		void parseRadius(xmlAttr *attr);

	public:
		FilterDenoisingParser();
		Filter *toFilter();

};

class FilterContrastParser : public Parser
{

	private:
		float value;
		void parseValue(xmlAttr *attr);

	public:
		FilterContrastParser();
		Filter *toFilter();

};

class FilterNegativeParser : public Parser
{

	public:
		Filter *toFilter();

};

class FilterFisheyeParser : public Parser
{

	private:
		float aperture;
		void parseAperture(xmlAttr *attr);

	public:
		FilterFisheyeParser();
		Filter *toFilter();

};

class FilterSobelParser : public Parser
{

	private:
		float threshold;
		void parseThreshold(xmlAttr *attr);

	public:
		FilterSobelParser();
		Filter *toFilter();

};

class FilterGammaParser : public Parser
{

	private:
		float value;
		void parseValue(xmlAttr *attr);

	public:
		FilterGammaParser();
		Filter *toFilter();

};

class FilterSepiaParser : public Parser
{

	public:
		Filter *toFilter();

};

class FilterFxaaParser : public Parser
{

	public:
		Filter *toFilter();

};

class FilterBlurParser : public Parser
{

	private:
		float radius;
		void parseRadius(xmlAttr *attr);

	public:
		FilterBlurParser();
		Filter *toFilter();

};

class FilterGlowParser : public Parser
{

	private:
		float threshold;
		float intensity;
		float radius;
		void parseThreshold(xmlAttr *attr);
		void parseIntensity(xmlAttr *attr);
		void parseRadius(xmlAttr *attr);

	public:
		FilterGlowParser();
		Filter *toFilter();

};

class FiltersParser : public Parser
{

	private:
		FilterDepthOfFieldParser *depthOfField;
		FilterCelShadingParser *celShading;
		FilterSaturationParser *saturation;
		FilterBrightnessParser *brightness;
		FilterGreyShadeParser *greyShade;
		FilterDenoisingParser *denoising;
		FilterContrastParser *contrast;
		FilterNegativeParser *negative;
		FilterFisheyeParser *fisheye;
		FilterSobelParser *sobel;
		FilterGammaParser *gamma;
		FilterSepiaParser *sepia;
		FilterFxaaParser *fxaa;
		FilterBlurParser *blur;
		FilterGlowParser *glow;
		void parseDepthOfField(xmlNode *node);
		void parseCelShading(xmlNode *node);
		void parseSaturation(xmlNode *node);
		void parseBrightness(xmlNode *node);
		void parseGreyShade(xmlNode *node);
		void parseDenoising(xmlNode *node);
		void parseContrast(xmlNode *node);
		void parseNegative(xmlNode *node);
		void parseFisheye(xmlNode *node);
		void parseSobel(xmlNode *node);
		void parseGamma(xmlNode *node);
		void parseSepia(xmlNode *node);
		void parseFxaa(xmlNode *node);
		void parseBlur(xmlNode *node);
		void parseGlow(xmlNode *node);

	public:
		FiltersParser();
		~FiltersParser();
		void addFilters(Raytracer *raytracer);

};

#endif
