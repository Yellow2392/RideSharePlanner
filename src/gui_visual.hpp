#ifndef GUI_VISUAL_HPP
#define GUI_VISUAL_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include <vector>
#include <string>
#include <optional>

#include "request.hpp"
#include "vehicle.hpp"
#include "json_parsing.hpp"
#include "utils.hpp"

#include "planner_gas.hpp"
#include "planner_gaso1.hpp"
#include "planner_gaso2.hpp"

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;
const unsigned int MAP_HEIGHT = 550;
const unsigned int MENU_HEIGHT = WINDOW_HEIGHT - MAP_HEIGHT;

const float MAX_SIM_COORD = 50.0f;
const float MAP_PADDING = 20.0f;

enum GASVariant { GAS, GAS_O1, GAS_O2 };

class Visualizer {
private:
    sf::RenderWindow m_window;
    sf::Font m_font;
    bool font_loaded = false;

    std::vector<Request> m_initial_requests;
    std::vector<Vehicle> m_initial_vehicles;
    
    std::vector<Request> m_current_requests;
    std::vector<Vehicle> m_current_vehicles;

    // Elementos del GUI
    sf::RectangleShape m_menuBar;
    sf::RectangleShape m_uploadButton;
    sf::Text m_uploadButtonText;
    sf::RectangleShape m_gasButton;
    sf::Text m_gasButtonText;
    sf::RectangleShape m_gasO1Button;
    sf::Text m_gasO1ButtonText;
    sf::RectangleShape m_gasO2Button;
    sf::Text m_gasO2ButtonText;

    // Input para el json
    sf::RectangleShape m_filePathInputRect;
    sf::Text m_filePathInputText;
    std::string m_filePathInputString;
    bool m_isInputActive = false;

    // Si hay errores
    sf::RectangleShape m_messageBoxRect;
    sf::Text m_messageBoxText;
    sf::Clock m_messageBoxClock;
    bool m_showMessageBox = false;
    float m_messageBoxDisplayTime = 3.0f;

    // Main metodos (numericos)
    void handleEvents();
    void update();
    void render();

    // Dibujando en mapita
    void drawRequests();
    void drawVehicles();
    void drawRoutes();

    // Dibujando en menu
    void drawMenuBar();
    void drawButtons();
    void drawFilePathInput();
    void drawMessageBox();

    sf::Vector2f simToScreen(double x_sim, double y_sim); // Convertir coordenadas

    // Logica
    void loadData(const std::string& filePath);
    void executeAlgorithm(GASVariant variant);
    void showMessage(const std::string& message, float duration = 3.0f);

public:
    Visualizer(const std::string& jsonPath = "./data.json");
    ~Visualizer();

    void run();
};

#endif //GUI_VISUAL_HPP