#include "utils.hpp"

namespace utils {
	void screenshot(sf::RenderWindow& window, int fractal, sf::RectangleShape& rect, sf::Shader& shader) {
        std::string name = ".png";
		std::string path = "screenshots";
        sf::Texture texture;
        window.clear();
        window.draw(rect, &shader);
        texture.create(window.getSize().x, window.getSize().y);
        texture.update(window);
		
		if (fractal == 0) {
			path += "/mandelbrot/";
		}
		else if (fractal == 1) {
			path += "/ship/";
		}
		else if (fractal == 2) {
			path += "/julia/";
		}
		
		if (!std::filesystem::exists(path))
			std::filesystem::create_directories(path);
		
		int i = 0;
	
		while (std::filesystem::exists(path + std::to_string(i) + name))
			i++;
		name = std::to_string(i) + name;
        texture.copyToImage().saveToFile(path + name);
	}

	void loadTexture(sf::Texture& blue, sf::Texture& orange, sf::Texture& rainbow, sf::Texture& blue_orange, sf::Texture& custom) {
		if (!blue.loadFromFile("palette/blue.png")) {
			std::cout << "failed to load blue palette" << std::endl;
		}
		if (!orange.loadFromFile("palette/orange.png")) {
			std::cout << "failed to load orange palette" << std::endl;
		}
		if (!rainbow.loadFromFile("palette/rainbow.png")) {
			std::cout << "failed to load rainbow palette" << std::endl;
		}
		if (!blue_orange.loadFromFile("palette/blue_orange.png")) {
			std::cout << "failed to load blue_orange palette" << std::endl;
		}
		if (!custom.loadFromFile("palette/custom.png")) {
			std::cout << "failed to load custom palette" << std::endl;
		}
	}

	void manageImGui(int& fractal, int& whichColor, float& r, float& n, sf::RenderWindow& window, sf::RectangleShape& rect, sf::Shader& shader, int& precision) {
		ImGui::Begin("Fractals Generator by PoloNX");
		ImGui::SliderInt("iteration", &precision, 1, 3000);
		if (fractal == 2) {
			ImGui::SliderFloat("Radius", &r, -2.f, 2.f);
			ImGui::SliderFloat("Number", &n, -4.f, 4.f);
		}
		ImGui::NewLine();
		ImGui::Text("The type of fractal:");
		ImGui::Combo("Fractal", &fractal, "Mandelbrot\0Burning Ship\0Julia\0");
		ImGui::Combo("Color", &whichColor, "Blue\0Orange\0Rainbow\0Blue_Orange\0Custom\0");
		ImGui::NewLine();
		if (ImGui::Button("Exit")) {
			window.close();
		}
		if (fractal == 2)
			ImGui::Text("Screeshot -> left click");
		else
			if (ImGui::Button("Screenshot")) 
				utils::screenshot(window, fractal, rect, shader);
		ImGui::End();

		shader.setParameter("max_iteration", precision);
		shader.setParameter("R", r);
		shader.setParameter("n", n);
		shader.setUniform("fractal", fractal);
	}
}