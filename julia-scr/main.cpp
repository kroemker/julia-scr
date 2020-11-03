
#include <SFML/Graphics.hpp>
#include "JuliaRenderer.h"
#include "ComplexNumber.h"
#include <cmath>
#include <cstring>

const double targetFPS = 5.0;
const double animStep = 0.003;

void runFullscreen();
void zoomIn(JuliaRenderer* juliaRenderer);
void updateParameter(ComplexNumber* c, ComplexNumber* target);
ComplexFunctor* getRandomComplexFunction();
inline double interpolate(double a, double b, double alpha);
inline double randZeroOne();

int main(int argc, char* argv[])
{
	srand(time(NULL));

	// parse command line
	for (int i = 0; i < argc; i++)
	{
		if (strcmp(argv[i], "/s") == 0)
			runFullscreen();
	}	

	return 0;
}

void runFullscreen()
{
	bool zoomMode = rand() < RAND_MAX / 4;

	// setup window
	sf::VideoMode videoMode = sf::VideoMode::getDesktopMode();
	sf::RenderWindow window(videoMode, "julia-scr", sf::Style::Fullscreen);
	window.setMouseCursorVisible(false);

	// setup texture
	int sz = videoMode.width > videoMode.height ? videoMode.height : videoMode.width;
	sf::Uint8* pixels = new sf::Uint8[sz * sz * 4];
	sf::Texture texture;
	texture.create(sz, sz);
	sf::Sprite sprite(texture);
	sprite.setPosition(videoMode.width / 2 - sz / 2, videoMode.height / 2 - sz / 2);

	// setup julia rendering
	ComplexFunctor* f = getRandomComplexFunction();
	JuliaRenderer juliaRenderer(pixels, sz, sz, 2.0, 100, f, (JuliaRenderer::ColorScheme)(rand() % 4));

	ComplexNumber c(-1 + 2 * randZeroOne(), -1 + 2 * randZeroOne());
	ComplexNumber target(-1 + 2 * randZeroOne(), -1 + 2 * randZeroOne());

	if (zoomMode)
	{
		sprite.setScale((float)videoMode.width / sz, (float)videoMode.height / sz);
		sprite.setPosition(0,0);
		ComplexNumber fatou = juliaRenderer.getClosestFatouPoint(c);
		juliaRenderer.setFocalPoint(fatou.real, fatou.imag);
	}

	double iterc = 0;
	double fps = 0;
	sf::Clock clock;
	while (window.isOpen())
	{
		// handle window
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || event.type == sf::Event::KeyPressed || event.type == sf::Event::MouseButtonPressed) {
				window.close();
			}
		}

		// draw julia set
		window.clear();

		juliaRenderer.draw(c);

		texture.update(pixels);

		window.draw(sprite);

		window.display();

		if (zoomMode)
			zoomIn(&juliaRenderer);
		else
		{
			updateParameter(&c, &target);

			// update fps
			sf::Time elapsed = clock.restart();
			fps = 1 / elapsed.asSeconds();
			if (fps < targetFPS)
			{
				iterc += targetFPS / fps;
				juliaRenderer.maxIters -= juliaRenderer.maxIters > 50 ? iterc : 0;
			}
			else
			{
				iterc -= iterc > 0 ? fps / targetFPS : 0;
				juliaRenderer.maxIters += juliaRenderer.maxIters < 1000 ? 5 : 0;
			}
		}
	}
}

void zoomIn(JuliaRenderer* juliaRenderer)
{
	double v = juliaRenderer->getViewSize();
	juliaRenderer->setViewSize(v * 0.99);
}

void updateParameter(ComplexNumber* c, ComplexNumber* target)
{
	if (fabs(c->real - target->real) <= 0.05 || fabs(c->imag - target->imag) <= 0.05)
	{
		if (rand() % 2 == 0)
		{
			target->real = 2.6 * randZeroOne() - 1.3;
		}
		else
		{
			target->imag = 2.6 * randZeroOne() - 1.3;
		}
	}
	else
	{
		c->real += c->real > target->real ? -animStep : +animStep;
		c->imag += c->imag > target->imag ? -animStep : +animStep;
	}
}

ComplexFunctor* getRandomComplexFunction() 
{
	switch (rand() % 4)
	{
	case 0:
		return new QuadraticPolynomial();
	case 1:
		return new CubicPolynomial();
	case 2:
		return new QuarticPolynomial();
	case 3:
		return new CubicInverseScaled();
	default:
		return new ComplexFunctor();
	}
}

inline double interpolate(double a, double b, double alpha)
{
	return (1 - alpha) * a + alpha * b;
}

inline double randZeroOne()
{
	return (double)rand() / (double)RAND_MAX;
}