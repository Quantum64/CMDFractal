#include "engine.h"
#include "ttmath/ttmath.h"

#define bits_exp 1
#define bits_base 3

using namespace ttmath;

constexpr int width = 220;
constexpr int height = 140;
constexpr int max = height > width ? height : width;

const int colors[] = { COLOUR::FG_RED, COLOUR::FG_YELLOW, COLOUR::FG_GREEN, COLOUR::FG_CYAN, COLOUR::FG_DARK_CYAN, COLOUR::FG_BLUE, COLOUR::FG_DARK_BLUE, COLOUR::FG_MAGENTA, COLOUR::FG_DARK_MAGENTA };

double cxs = 0;
double cys = 0;

inline int calculate(double x, double y, double scale, int maxIterations) {
	double scaledX = (x / static_cast<double>(max)) * 4 - 2.8;
	double scaledY = (y / static_cast<double>(max)) * 3.5 - 1;

	scaledX = (scaledX / scale);
	scaledY = (scaledY / scale);

	double cx = cxs;
	double cy = cys;
	int iteration = 0;
	while (cx * cx + cy * cy <= 4 && iteration < maxIterations) {
		double xtemp = cx * cx - cy * cy + scaledX;
		cy = 2 * cx * cy + scaledY;
		cx = xtemp;
		iteration = iteration + 1;
	}
	return iteration;
}

inline int calculate(Big<bits_exp, bits_base> x, Big<bits_exp, bits_base> y, Big<bits_exp, bits_base> scale, int maxIterations) {
	Big<bits_exp, bits_base> scaledX = (x / static_cast<double>(width)) * 4 - 2.8;
	Big<bits_exp, bits_base> scaledY = (y / static_cast<double>(height)) * 2.2 - 1;

	scaledX = (scaledX / scale);
	scaledY = (scaledY / scale);

	Big<bits_exp, bits_base> cx = 0.0;
	Big<bits_exp, bits_base> cy = 0.0;
	int iteration = 0;
	while (cx * cx + cy * cy <= 4 && iteration < maxIterations) {
		Big<bits_exp, bits_base> xtemp = cx * cx - cy * cy + scaledX;
		cy = cx * cy * 2 + scaledY;
		cx = xtemp;
		iteration = iteration + 1;
	}
	return iteration;
}

class Fractal : public olcConsoleGameEngine {
private:
	Big<bits_exp, bits_base> one = "1";
	Big<bits_exp, bits_base> xOffset = 0, yOffset = 0;
	Big<bits_exp, bits_base> scale = 1;

public:
	bool OnUserCreate() {
		return true;
	}

	bool OnUserUpdate(float delta) {
		Fill(0, 0, width, height, COLOUR::FG_BLACK);
		if (m_keys[87].bHeld) {
			yOffset -= (one / scale) * 150 * delta;
		}
		if (m_keys[83].bHeld) {
			yOffset += (one / scale) * 150 * delta;
		}
		if (m_keys[68].bHeld) {
			xOffset += (one / scale) * 150 * delta;
		}
		if (m_keys[65].bHeld) {
			xOffset -= (one / scale) * 150 * delta;
		}
		if (m_keys[69].bHeld) {
			scale += scale * 4 * delta;
		}
		if (m_keys[81].bHeld) {
			scale -= scale * 4 * delta;
		}
		if (m_keys[69].bHeld) {
			scale += scale * 4 * delta;
		}
		if (m_keys[81].bHeld) {
			scale -= scale * 4 * delta;
		}

		if (m_keys[90].bHeld) {
			cys += delta / 2;
		}
		if (m_keys[88].bHeld) {
			cys -= delta / 2;
		}
		if (m_keys[67].bHeld) {
			cxs += delta / 2;
		}
		if (m_keys[86].bHeld) {
			cxs -= delta / 2;
		}

		Big<bits_exp, bits_base> tmp = scale * scale;
		tmp.Ln(tmp);
		int maxIterations = 200 + 10 * static_cast<int>(tmp.ToDouble());
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				int iterations = 0;
				//if (scale < 80e13) {
					iterations = calculate((scale * xOffset + x).ToDouble(), (scale * yOffset + y).ToDouble(), scale.ToDouble(), maxIterations);
				//}
				//else {
					//iterations = calculate(scale * xOffset + x, scale * yOffset + y, scale, maxIterations);
				//}
				int color = colors[iterations / 10 % 9];
				if (iterations == maxIterations) {
					color = COLOUR::FG_DARK_RED;
				}
				if (iterations < 5) {
					color = COLOUR::FG_BLACK;
				}
				Draw(x, y, PIXEL_TYPE::PIXEL_SOLID, color);
				/*
				if (iterations == maxIterations) {
					Draw(x, y, PIXEL_TYPE::PIXEL_SOLID, COLOUR::FG_DARK_RED);
				}
				else if (iterations > (maxIterations / 2)) {
					Draw(x, y, PIXEL_TYPE::PIXEL_SOLID, COLOUR::FG_RED);
				}
				else if (iterations > (maxIterations / 3)) {
					Draw(x, y, PIXEL_TYPE::PIXEL_SOLID, COLOUR::FG_YELLOW);
				}
				else if (iterations > (maxIterations / 5)) {
					Draw(x, y, PIXEL_TYPE::PIXEL_SOLID, COLOUR::FG_GREEN);
				}
				else if (iterations > (maxIterations / 10)) {
					Draw(x, y, PIXEL_TYPE::PIXEL_SOLID, COLOUR::FG_BLUE);
				}
				else if (iterations > (maxIterations / 30)) {
					Draw(x, y, PIXEL_TYPE::PIXEL_SOLID, COLOUR::FG_DARK_BLUE);
				}
				else if (iterations > (maxIterations / 50)) {
					Draw(x, y, PIXEL_TYPE::PIXEL_SOLID, COLOUR::FG_MAGENTA);
				}
				else if (iterations > (maxIterations / 75)) {
					Draw(x, y, PIXEL_TYPE::PIXEL_SOLID, COLOUR::FG_DARK_MAGENTA);
				}
				*/
			}
		}
		return true;
	}
};

int main()
{
	Fractal game;
	game.ConstructConsole(width, height, 8, 8);
	game.Start();
	return 0;
}