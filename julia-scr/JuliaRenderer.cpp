#include "JuliaRenderer.h"

JuliaRenderer::JuliaRenderer(sf::Uint8 pixels[], int width, int height, double escapeRadius, int iterations, ComplexNumber* initialComplexParameter, ComplexFunctor* complexFunction, ColorScheme colorScheme)
	: pixels(pixels), width(width), height(height), escapeRadius(escapeRadius), maxIters(iterations), complexParameter(initialComplexParameter), complexFunction(complexFunction), colorScheme(colorScheme)
{
	x = 0; 
	y = 0;
	viewSize = 2 * escapeRadius;
}

JuliaRenderer::~JuliaRenderer()
{

}

ComplexNumber JuliaRenderer::getClosestFatouPoint(ComplexNumber& c)
{
	int mostIts = 0;
	ComplexNumber closestFatou(0, 0);
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			double r = focalPoint.real + x * (viewSize / (double)width) - (viewSize / 2);
			double i = focalPoint.imag + y * (viewSize / (double)height) - (viewSize / 2);

			ComplexNumber z(r, i);
			int iters = 0;
			while ((z.real * z.real + z.imag * z.imag < escapeRadius * escapeRadius) && (iters < maxIters))
			{
				z = (*complexFunction)(z, c);
				iters++;
			}

			if (iters < maxIters)
			{
				if (iters > mostIts) 
				{
					mostIts = iters;
					closestFatou.real = r;
					closestFatou.imag = i;
				}
			}
		}
	}
	return closestFatou;
}

void JuliaRenderer::drawInner() {
	double r = focalPoint.real + x * (viewSize / (double)width) - (viewSize / 2);
	double i = focalPoint.imag + y * (viewSize / (double)height) - (viewSize / 2);

	ComplexNumber z(r, i);
	int iters = 0;
	while ((z.real * z.real + z.imag * z.imag < escapeRadius * escapeRadius) && (iters < maxIters))
	{
		z = (*complexFunction)(z, *complexParameter);
		iters++;
	}

	if (iters >= maxIters)
	{
		setPixel(pixels, width, height, x, y, 0, 0, 0, 255);
	}
	else
	{
		switch (colorScheme)
		{
		case RedBasedRGB:
			setPixel(pixels, width, height, x, y, iters << 2, iters, iters << 1, 255);
			break;
		case GreenBasedRGB:
			setPixel(pixels, width, height, x, y, iters << 1, iters << 2, iters, 255);
			break;
		case BlueBasedRGB:
			setPixel(pixels, width, height, x, y, iters, iters << 1, iters << 2, 255);
			break;
		case HSVGradient:
			sf::Color c = getGradientColor((float)iters / (float)maxIters);
			setPixel(pixels, width, height, x, y, c.r, c.g, c.b, c.a);
			break;
		}
	}
}

void JuliaRenderer::draw() {
	for (x = 0; x < width; x++)
	{
		for (y = 0; y < height; y++)
		{
			drawInner();
		}
	}
}

void JuliaRenderer::drawResponsive()
{
	sf::Clock clock;
	clock.restart();
	for (; x < width; x++)
	{
		for (; y < height; y++)
		{
			if (clock.getElapsedTime().asMilliseconds() > 250)
				return;

			drawInner();
		}
		y = 0;
	}
	x = 0;
}

void JuliaRenderer::setPixel(sf::Uint8 pixels[], int width, int height, int x, int y, sf::Uint8 r, sf::Uint8 g, sf::Uint8 b, sf::Uint8 a)
{
	pixels[4 * (x + y * width)] = r;
	pixels[4 * (x + y * width) + 1] = g;
	pixels[4 * (x + y * width) + 2] = b;
	pixels[4 * (x + y * width) + 3] = a;
}

sf::Color JuliaRenderer::getGradientColor(float a) {
	sf::Color c;
	c.r = a * 255;
	c.g = 255;
	c.b = a * 10 * 255;
	return HsvToRgb(c);
}

sf::Color JuliaRenderer::HsvToRgb(sf::Color hsv)
{
	sf::Color rgb;
	unsigned char region, remainder, p, q, t;

	if (hsv.g == 0)
	{
		rgb.r = hsv.b;
		rgb.g = hsv.b;
		rgb.b = hsv.b;
		return rgb;
	}

	region = hsv.r / 43;
	remainder = (hsv.r - (region * 43)) * 6;

	p = (hsv.b * (255 - hsv.g)) >> 8;
	q = (hsv.b * (255 - ((hsv.g * remainder) >> 8))) >> 8;
	t = (hsv.b * (255 - ((hsv.g * (255 - remainder)) >> 8))) >> 8;

	switch (region)
	{
	case 0:
		rgb.r = hsv.b; rgb.g = t; rgb.b = p;
		break;
	case 1:
		rgb.r = q; rgb.g = hsv.b; rgb.b = p;
		break;
	case 2:
		rgb.r = p; rgb.g = hsv.b; rgb.b = t;
		break;
	case 3:
		rgb.r = p; rgb.g = q; rgb.b = hsv.b;
		break;
	case 4:
		rgb.r = t; rgb.g = p; rgb.b = hsv.b;
		break;
	default:
		rgb.r = hsv.b; rgb.g = p; rgb.b = q;
		break;
	}
	rgb.a = hsv.a;
	return rgb;
}

sf::Color JuliaRenderer::RgbToHsv(sf::Color rgb)
{
	sf::Color hsv;
	unsigned char rgbMin, rgbMax;

	rgbMin = rgb.r < rgb.g ? (rgb.r < rgb.b ? rgb.r : rgb.b) : (rgb.g < rgb.b ? rgb.g : rgb.b);
	rgbMax = rgb.r > rgb.g ? (rgb.r > rgb.b ? rgb.r : rgb.b) : (rgb.g > rgb.b ? rgb.g : rgb.b);

	hsv.b = rgbMax;
	if (hsv.b == 0)
	{
		hsv.r = 0;
		hsv.g = 0;
		return hsv;
	}

	hsv.g = 255 * long(rgbMax - rgbMin) / hsv.b;
	if (hsv.g == 0)
	{
		hsv.r = 0;
		return hsv;
	}

	if (rgbMax == rgb.r)
		hsv.r = 0 + 43 * (rgb.g - rgb.b) / (rgbMax - rgbMin);
	else if (rgbMax == rgb.g)
		hsv.r = 85 + 43 * (rgb.b - rgb.r) / (rgbMax - rgbMin);
	else
		hsv.r = 171 + 43 * (rgb.r - rgb.g) / (rgbMax - rgbMin);

	return hsv;
}