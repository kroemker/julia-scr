#pragma once

#include <SFML/Graphics.hpp>
#include "ComplexNumber.h"
#include "ComplexFunctor.h"

class JuliaRenderer
{
public:
	enum ColorScheme
	{
		RedBasedRGB,
		GreenBasedRGB,
		BlueBasedRGB,
		RedBasedGradientHSV,
		GreenBasedGradientHSV,
		BlueBasedGradientHSV,
		NumColorSchemes
	};

	JuliaRenderer(sf::Uint8 pixels[], int width, int height, double escapeRadius, int iterations, ComplexNumber* initialComplexParameter, ComplexFunctor* complexFunction, ColorScheme colorScheme = RedBasedRGB);
	~JuliaRenderer();

	void draw();
	void drawResponsive();

	ComplexNumber getClosestFatouPoint(ComplexNumber& c);

	int maxIters;
	ColorScheme colorScheme;
	ComplexFunctor* complexFunction;
	ComplexNumber* complexParameter;
	double viewSize;
	ComplexNumber focalPoint;
	double escapeRadius;
private:
	void drawInner();
	inline void setPixel(sf::Uint8 pixels[], int width, int height, int x, int y, sf::Uint8 r, sf::Uint8 g, sf::Uint8 b, sf::Uint8 a);
	sf::Color getGradientColor(float a);
	sf::Color HsvToRgb(sf::Color hsv);
	sf::Color RgbToHsv(sf::Color rgb);

	sf::Uint8* pixels;
	int width, height;
	int x, y;
};

