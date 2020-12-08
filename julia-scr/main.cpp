
#include <SFML/Graphics.hpp>
#include "JuliaRenderer.h"
#include "ComplexNumber.h"
#include <cmath>
#include <cstring>

const double targetFPS = 5.0;
const double animStep = 0.003;

void setupWindow();
void cleanupWindow();
void runScreensaver();
void handleInput();
void runInteractive();
void updateParameter(ComplexNumber* c, ComplexNumber* target);
inline double interpolate(double a, double b, double alpha);
inline double randZeroOne();

sf::RenderWindow* window;
sf::Uint8* pixels;
sf::Texture texture;
sf::Sprite* sprite;
JuliaRenderer* juliaRenderer;
ComplexFunctor* funcs[] = { new QuadraticPolynomial(), new CubicPolynomial(), new QuarticPolynomial(), 
							new CubicInverseScaled(), new QuadraticPolynomialScaled(),
							new QuadraticPolynomialSpecial(), new CubicPolynomialSpecial, new QuarticPolynomialSpecial()};
const int numFuncs = sizeof(funcs) / sizeof(ComplexFunctor*);

int main(int argc, char* argv[])
{
	srand(time(NULL));

	// parse command line
	for (int i = 0; i < argc; i++)
	{
		if (strcmp(argv[i], "/s") == 0) {
			setupWindow();
			runScreensaver();
		}
		else if (strcmp(argv[i], "/i") == 0) {
			setupWindow();
			runInteractive();
		}
	}	

	return 0;
}

void setupWindow() {
	sf::VideoMode videoMode = sf::VideoMode::getDesktopMode();
	window = new sf::RenderWindow(videoMode, "julia-scr", sf::Style::None);
	window->setMouseCursorVisible(false);

	int sz = videoMode.width > videoMode.height ? videoMode.height : videoMode.width;
	pixels = new sf::Uint8[sz * sz * 4];
	texture.create(sz, sz);
	sprite = new sf::Sprite(texture);
	sprite->setPosition(videoMode.width / 2 - sz / 2, videoMode.height / 2 - sz / 2);
}

void cleanupWindow() {
	delete juliaRenderer;
	delete sprite;
	delete[] pixels;
	delete window;
}

void runInteractive() 
{
	ComplexNumber c(0, 0);
	juliaRenderer = new JuliaRenderer(pixels, texture.getSize().x, texture.getSize().y, 2.0, 100, &c, funcs[0], (JuliaRenderer::ColorScheme)(rand() % 3 + 3));

	sf::Thread inputThread(&handleInput);
	inputThread.launch();

	bool showHUD = true;
	sf::Font font;
	if (!font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf"))
		printf("Arial.ttf could not be found...\n");
	sf::Text infoText;
	infoText.setFillColor(sf::Color::White);
	infoText.setCharacterSize(30);
	infoText.setFont(font);

	while (window->isOpen())
	{
		sf::Event event;
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
			{
				inputThread.terminate();
				window->close();
			}
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::H)
				showHUD = !showHUD;
		}

		window->clear();

		if (showHUD) {
			infoText.setString(juliaRenderer->complexFunction->toString() 
				+ "\nc = " + c.toString() 
				+ "\nIterations: " + std::to_string(juliaRenderer->maxIters) 
				+ "\nFocal point: " + juliaRenderer->focalPoint.toString() 
				+ "\nView size: " + std::to_string(juliaRenderer->viewSize));
		}

		juliaRenderer->drawResponsive();

		texture.update(pixels);

		window->draw(*sprite);

		if (showHUD) {
			window->draw(infoText);
		}

		window->display();
	}

	cleanupWindow();
}

void handleInput() 
{
	float stepSize = 0.00001;
	float k = 0;
	float kStep = 0.01;
	while (true)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::PageUp))
			if (k > kStep) {
				juliaRenderer->maxIters++;
				k -= kStep;
			}
			else
			{
				k += stepSize;
			}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::PageDown))
			if (k > kStep) {
				juliaRenderer->maxIters--;
				k -= kStep;
			}
			else
			{
				k += stepSize;
			}

		for (size_t i = 0; i < numFuncs && i < 12; i++)
		{
			if (sf::Keyboard::isKeyPressed((sf::Keyboard::Key)((size_t)sf::Keyboard::F1 + i)))
			{
				juliaRenderer->complexFunction = funcs[i];
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			juliaRenderer->complexParameter->imag += stepSize;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			juliaRenderer->complexParameter->imag -= stepSize;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			juliaRenderer->complexParameter->real += stepSize;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			juliaRenderer->complexParameter->real -= stepSize;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
			*juliaRenderer->complexParameter = ComplexNumber(0, 0);
			juliaRenderer->viewSize = 2.0 * juliaRenderer->escapeRadius;
		}

		// zooming
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
			juliaRenderer->viewSize = juliaRenderer->viewSize * 0.99999;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
			juliaRenderer->viewSize = juliaRenderer->viewSize * 1.00001;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			juliaRenderer->focalPoint.imag += stepSize;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			juliaRenderer->focalPoint.imag -= stepSize;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			juliaRenderer->focalPoint.real -= stepSize;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			juliaRenderer->focalPoint.real += stepSize;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab)) {
			if (k > kStep) {
				juliaRenderer->colorScheme = (JuliaRenderer::ColorScheme)((juliaRenderer->colorScheme + 1) % JuliaRenderer::ColorScheme::NumColorSchemes);
				k -= kStep;
			}
			else
			{
				k += stepSize;
			}
		}
	}
}

void runScreensaver()
{
	bool zoomMode = rand() < RAND_MAX / 5;

	// setup julia rendering
	ComplexNumber c(-1 + 2 * randZeroOne(), -1 + 2 * randZeroOne());
	ComplexNumber target(-1 + 2 * randZeroOne(), -1 + 2 * randZeroOne());
	juliaRenderer = new JuliaRenderer(pixels, texture.getSize().x, texture.getSize().y, 2.0, 100, &c, funcs[rand() % numFuncs], 
									  (JuliaRenderer::ColorScheme)(rand() % JuliaRenderer::ColorScheme::NumColorSchemes));

	if (zoomMode)
	{
		sprite->setScale((float)window->getSize().x / (float)texture.getSize().x, (float)window->getSize().y / (float)texture.getSize().y);
		sprite->setPosition(0,0);
		juliaRenderer->focalPoint = juliaRenderer->getClosestFatouPoint(c);
	}

	double iterc = 0;
	double fps = 0;
	sf::Clock clock;
	while (window->isOpen())
	{
		// handle window
		sf::Event event;
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed || event.type == sf::Event::KeyPressed || event.type == sf::Event::MouseButtonPressed) {
				window->close();
			}
		}

		// draw julia set
		window->clear();

		juliaRenderer->draw();

		texture.update(pixels);

		window->draw(*sprite);

		window->display();

		if (zoomMode) {
			juliaRenderer->viewSize *= 0.99;
			juliaRenderer->maxIters += 20;
		}
		else
		{
			updateParameter(&c, &target);

			// update fps
			sf::Time elapsed = clock.restart();
			fps = 1.0 / (double)elapsed.asSeconds();
			if (fps < targetFPS)
			{
				iterc += targetFPS / fps;
				juliaRenderer->maxIters = juliaRenderer->maxIters > 100 ? juliaRenderer->maxIters - iterc : 100;
			}
			else
			{
				iterc = iterc > 0 ? iterc - fps / targetFPS : 0;
				juliaRenderer->maxIters += juliaRenderer->maxIters < 1000 ? 5 : 0;
			}
		}
	}

	cleanupWindow();
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

inline double interpolate(double a, double b, double alpha)
{
	return (1 - alpha) * a + alpha * b;
}

inline double randZeroOne()
{
	return (double)rand() / (double)RAND_MAX;
}