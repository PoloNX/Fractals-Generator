#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <iostream>
#include <array>

#include "utils.hpp"

int main()
{
    //resolution of the window
    int width = sf::VideoMode::getDesktopMode().width;
    int height = sf::VideoMode::getDesktopMode().height;

	//Create the window
    sf::RenderWindow window(sf::VideoMode(width, height), "Fractals Generator SFML + GLSL");
    window.setVerticalSyncEnabled(true);
    
	//Init ImGu::SFML
    ImGui::SFML::Init(window);
	
	//Create a rect (we will give it to the shader after)
    sf::RectangleShape rect;
    rect.setSize(sf::Vector2f(window.getSize()));

    //Create a vector named center which can change in the code to move the fractal
    sf::Vector2f center = sf::Vector2f(window.getSize());
	center.x /= 4;
	center.y = 0;

	//Some variables
    int precision = 400;
    float r = 1.0f;
    float n = 2.0f;
    float scale = 2.2f;
    float targetScale = scale;

	//Textures of the palette
    sf::Texture blue, orange, rainbow, blue_orange, custom;
    utils::loadTexture(blue, orange, rainbow, blue_orange, custom);
	
    //Load the shader and setup with basic parameters
    sf::Shader shader;
    shader.loadFromFile("shaders/shader.glsl", sf::Shader::Fragment);
    shader.setParameter("max_iteration", precision);
    shader.setParameter("center", center);
    shader.setParameter("resolution", sf::Vector2f(window.getSize()));
    shader.setParameter("scale", scale);
    shader.setParameter("fractal", 1);
    shader.setParameter("palette", blue);
    shader.setParameter("R", r);
    shader.setParameter("n", n);
	
	//We can use one enum but I'm lazy
	int fractal = 0; //0 = mandelbrot, 1 = burning ship, 2 = julia

	//A vector with palette's texture
	std::array<sf::Texture, 5> color = { blue, orange, rainbow,blue_orange, custom };
    int whichColor = 0;

    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

    sf::Clock updateClock;
    sf::Clock deltaClock;

    while (window.isOpen())
    {
        window.clear();

        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(window, event);
		
           switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::Resized:
                    width = window.getSize().x;
                    height = window.getSize().y;
                    shader.setUniform("resolution", sf::Vector2f(window.getSize()));
                    break;
                case sf::Event::MouseWheelScrolled:
                    targetScale = std::clamp(targetScale - targetScale * 0.1f * event.mouseWheelScroll.delta, 0.000001f, 4.f);
                    shader.setUniform("scale", scale);
                    break;

                case sf::Event::MouseMoved: {
                    sf::Vector2i newPos(event.mouseMove.x, event.mouseMove.y);
                    sf::Vector2i delta = newPos - mousePos;
                    ImGuiIO& io = ImGui::GetIO();
                    mousePos = newPos;
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && fractal != 2) {
                        if (!io.WantCaptureMouse) { 
							//We change the center
                            center += scale * sf::Vector2f(sf::Vector2i(delta.x, -delta.y));
                            shader.setUniform("center", center);
                        }
                        break;
                    }
                }
            }
            
		    //If the fractal is the Julia the code act like this
            if (fractal == 2) {
                ImGuiIO& io = ImGui::GetIO();
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !io.WantCaptureMouse  && 
                    event.mouseMove.x < window.getPosition().x + window.getSize().x && event.mouseButton.x > window.getPosition().x &&
                    event.mouseMove.y < window.getPosition().y + window.getSize().y && event.mouseButton.y > window.getPosition().y)
                {
                    utils::screenshot(window, fractal, rect, shader);
                }
                
                scale = 2.2f;
				//Edit variable with the cursor pos
                r = (event.mouseMove.x - width / 2) / float(width / 3) - 0.5;
                n = (event.mouseMove.y - height / 2) / float(height / 3) - 0.5;
                shader.setUniform("scale", scale);
                shader.setUniform("R", r);
                shader.setUniform("n", n);
            }
        }
			
    
        //Don't zoom with Julia
        if (targetScale != scale && fractal != 2)
        {
            float diff = targetScale - scale;
            if (targetScale < scale)
                scale = std::max(scale + diff * 0.02f * updateClock.getElapsedTime().asSeconds(), targetScale);
            else if (targetScale > scale)
                scale = std::min(scale + diff * 0.02f * updateClock.getElapsedTime().asSeconds(), targetScale);

            shader.setUniform("scale", scale);
        }

        else {
            updateClock.restart();
        }

		//Update imgui sfml window
        ImGui::SFML::Update(window, deltaClock.restart());

		//Create ImGui windows
        utils::manageImGui(fractal, whichColor, r, n, window, rect, shader, precision, width, height);
        shader.setParameter("palette", color[whichColor]);

		//Draw the rect with the shader
        window.draw(rect, &shader);
        ImGui::SFML::Render(window);
        window.display();
    }
	
    ImGui::SFML::Shutdown();
    return 0;
}
