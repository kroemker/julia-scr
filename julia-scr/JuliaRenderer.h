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
		HSVGradient
	};

	JuliaRenderer(sf::Uint8 pixels[], int width, int height, double escapeRadius, int iterations, ComplexNumber* initialComplexParameter, ComplexFunctor* complexFunction, ColorScheme colorScheme = RedBasedRGB);
	~JuliaRenderer();

	void setFocalPoint(double x, double y);
	void setViewSize(double sz);
	double getViewSize();

	void draw();

	ComplexNumber getClosestFatouPoint(ComplexNumber& c);

	int maxIters;
	ColorScheme colorScheme;
	ComplexFunctor* complexFunction;
	ComplexNumber* complexParameter;
private:
	inline void setPixel(sf::Uint8 pixels[], int width, int height, int x, int y, sf::Uint8 r, sf::Uint8 g, sf::Uint8 b, sf::Uint8 a);
	sf::Color getGradientColor(float a);
	sf::Color HsvToRgb(sf::Color hsv);
	sf::Color RgbToHsv(sf::Color rgb);

	double focalX, focalY, viewSize;

	sf::Uint8* pixels;
	int width, height;
	double escapeRadius;
};

