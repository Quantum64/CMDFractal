#include "engine.h"

constexpr int width = 160;
constexpr int height = 100;
constexpr int maxIterations = 500;

int calculate(int x, int y, double scale) {
	double scaledX = (x / static_cast<double>(width)) * 4 - 2.8;
	double scaledY = (y / static_cast<double>(height)) * 2.2 - 1;

	scaledX = (scaledX / scale);
	scaledY = (scaledY / scale);

	double cx = 0.0;
	double cy = 0.0;
	int iteration = 0;
	while (cx * cx + cy * cy <= 4 && iteration < maxIterations) {
		double xtemp = cx * cx - cy * cy + scaledX;
		cy = 2 * cx * cy + scaledY;
		cx = xtemp;
		iteration = iteration + 1;
	}
	return iteration;
}

class Fractal : public olcConsoleGameEngine {
private:
	double xOffset = 0, yOffset = 0;
	double scale = 1;

public:
	bool OnUserCreate() {
		return true;
	}

	bool OnUserUpdate(float delta) {
		Fill(0, 0, width, height, COLOUR::FG_BLACK);
		if (m_keys[87].bHeld) {
			yOffset -= 150 * delta / scale;
		}
		if (m_keys[83].bHeld) {
			yOffset += 150 * delta / scale;
		}
		if (m_keys[68].bHeld) {
			xOffset += 150 * delta / scale;
		}
		if (m_keys[65].bHeld) {
			xOffset -= 150 * delta / scale;
		}
		if (m_keys[69].bHeld) {
			scale += 4 * delta * scale;
		}
		if (m_keys[81].bHeld) {
			if (scale > 0) {
				scale -= 4 * delta * scale;
			}
		}
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				int iterations = calculate(x + static_cast<int>(xOffset * scale), y + static_cast<int>(yOffset * scale), scale);
				if (iterations == maxIterations) {
					Draw(x, y, PIXEL_TYPE::PIXEL_SOLID, COLOUR::FG_DARK_RED);
				}
				else if (iterations > 100) {
					Draw(x, y, PIXEL_TYPE::PIXEL_SOLID, COLOUR::FG_RED);
				}
				else if (iterations > 75) {
					Draw(x, y, PIXEL_TYPE::PIXEL_SOLID, COLOUR::FG_YELLOW);
				}
				else if (iterations > 50) {
					Draw(x, y, PIXEL_TYPE::PIXEL_SOLID, COLOUR::FG_GREEN);
				}
				else if (iterations > 25) {
					Draw(x, y, PIXEL_TYPE::PIXEL_SOLID, COLOUR::FG_BLUE);
				}
				else if (iterations > 12) {
					Draw(x, y, PIXEL_TYPE::PIXEL_SOLID, COLOUR::FG_DARK_BLUE);
				}
				else if (iterations > 5) {
					Draw(x, y, PIXEL_TYPE::PIXEL_SOLID, COLOUR::FG_MAGENTA);
				}
				else if (iterations > 3) {
					Draw(x, y, PIXEL_TYPE::PIXEL_SOLID, COLOUR::FG_DARK_MAGENTA);
				}
			}
		}
		return true;
	}
};

int main()
{
	Fractal game;
	game.ConstructConsole(160, 100, 8, 8);
	game.Start();
	return 0;
}