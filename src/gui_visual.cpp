#include "gui_visual.hpp"
#include <iostream>
#include <string>

Visualizer::Visualizer(const std::string& jsonPath) : m_filePathInputString(jsonPath),
    m_window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}, 32), "Shared Route Planning Visualizer"),
    m_uploadButtonText(m_font, "UPLOAD", 14u),
    m_gasButtonText(m_font, "Run GAS", 14u),
    m_gasO1ButtonText(m_font, "Run GAS-O1", 14u),
    m_gasO2ButtonText(m_font, "Run GAS-O2", 14u),
    m_filePathInputText(m_font, "", 14u),
    m_messageBoxText(m_font, "", 16u)
{
    m_window.setFramerateLimit(60);
    
    if (!m_font.openFromFile("arial.ttf")) {
        std::cerr << "Error loading font! Make sure 'arial.ttf' is in the executable's directory.\n";
        font_loaded = false;
    }
    else {
        font_loaded = true;
    }

    // UI Elements
    // Menu
    m_menuBar.setSize(sf::Vector2f(WINDOW_WIDTH, MENU_HEIGHT));
    m_menuBar.setPosition(sf::Vector2f(0, MAP_HEIGHT));
    m_menuBar.setFillColor(sf::Color(50, 50, 50));

    // JSON Input
    m_filePathInputRect.setSize(sf::Vector2f(250, 30));
    m_filePathInputRect.setPosition(sf::Vector2f(10, MAP_HEIGHT + 10));
    m_filePathInputRect.setFillColor(sf::Color::White);
    m_filePathInputRect.setOutlineColor(sf::Color::Black);
    m_filePathInputRect.setOutlineThickness(1);

    if (font_loaded) {
        m_filePathInputText.setFont(m_font);
        m_filePathInputText.setCharacterSize(14);
        m_filePathInputText.setFillColor(sf::Color::Black);
        m_filePathInputText.setPosition(sf::Vector2f(m_filePathInputRect.getPosition().x + 5, m_filePathInputRect.getPosition().y + 5));
        m_filePathInputText.setString(m_filePathInputString);
    }

    float paddingX = 10.0f;

    // UPLOAD
    m_uploadButton.setSize(sf::Vector2f(80, 30));
    m_uploadButton.setPosition(sf::Vector2f(m_filePathInputRect.getPosition().x + m_filePathInputRect.getSize().x + paddingX, MAP_HEIGHT + 10));
    m_uploadButton.setFillColor(sf::Color(70, 130, 180));
    m_uploadButton.setOutlineColor(sf::Color::Black);
    m_uploadButton.setOutlineThickness(1);

    if (font_loaded) {
        m_uploadButtonText.setFont(m_font);
        m_uploadButtonText.setCharacterSize(14);
        m_uploadButtonText.setFillColor(sf::Color::White);
        m_uploadButtonText.setString("UPLOAD");
        sf::FloatRect textRect = m_uploadButtonText.getLocalBounds();
        m_uploadButtonText.setOrigin(sf::Vector2f(textRect.position.x + textRect.size.x / 2.0f, textRect.position.y + textRect.position.y / 2.0f));
        m_uploadButtonText.setPosition(sf::Vector2f(m_uploadButton.getPosition().x + m_uploadButton.getSize().x / 2.0f,
                                       m_uploadButton.getPosition().y + m_uploadButton.getSize().y / 2.0f));
    }

    // Algoritmos
    float currentX = m_uploadButton.getPosition().x + m_uploadButton.getSize().x + paddingX;
    float buttonWidth = 100.0f;

    // GAS
    m_gasButton.setSize(sf::Vector2f(buttonWidth, 30));
    m_gasButton.setPosition(sf::Vector2f(currentX, m_filePathInputRect.getPosition().y));
    m_gasButton.setFillColor(sf::Color(180, 30, 0));
    m_gasButton.setOutlineColor(sf::Color::Black);
    m_gasButton.setOutlineThickness(1);
    currentX += buttonWidth + paddingX;
    if (font_loaded) {
        m_gasButtonText.setFont(m_font);
        m_gasButtonText.setCharacterSize(14);
        m_gasButtonText.setFillColor(sf::Color::White);
        m_gasButtonText.setString("Run GAS");
        sf::FloatRect textRect = m_gasButtonText.getLocalBounds();
        m_gasButtonText.setOrigin(sf::Vector2f(textRect.position.x + textRect.size.x / 2.0f, textRect.position.y + textRect.position.y / 2.0f));
        m_gasButtonText.setPosition(sf::Vector2f(m_gasButton.getPosition().x + m_gasButton.getSize().x / 2.0f,
                                    m_gasButton.getPosition().y + m_gasButton.getSize().y / 2.0f));
    }

    // GAS-O1
    m_gasO1Button.setSize(sf::Vector2f(buttonWidth, 30));
    m_gasO1Button.setPosition(sf::Vector2f(currentX, m_filePathInputRect.getPosition().y));
    m_gasO1Button.setFillColor(sf::Color(100, 200, 10));
    m_gasO1Button.setOutlineColor(sf::Color::Black);
    m_gasO1Button.setOutlineThickness(1);
    currentX += buttonWidth + paddingX;
    if (font_loaded) {
        m_gasO1ButtonText.setFont(m_font);
        m_gasO1ButtonText.setCharacterSize(14);
        m_gasO1ButtonText.setFillColor(sf::Color::White);
        m_gasO1ButtonText.setString("Run GAS-O1");
        sf::FloatRect textRect = m_gasO1ButtonText.getLocalBounds();
        m_gasO1ButtonText.setOrigin(sf::Vector2f(textRect.position.x + textRect.size.x / 2.0f, textRect.position.y + textRect.position.y / 2.0f));
        m_gasO1ButtonText.setPosition(sf::Vector2f(m_gasO1Button.getPosition().x + m_gasO1Button.getSize().x / 2.0f,
                                      m_gasO1Button.getPosition().y + m_gasO1Button.getSize().y / 2.0f));
    }

    // GAS-O2
    m_gasO2Button.setSize(sf::Vector2f(buttonWidth, 30));
    m_gasO2Button.setPosition(sf::Vector2f(currentX, m_filePathInputRect.getPosition().y));
    m_gasO2Button.setFillColor(sf::Color(130, 60, 220));
    m_gasO2Button.setOutlineColor(sf::Color::Black);
    m_gasO2Button.setOutlineThickness(1);
    if (font_loaded) {
        m_gasO2ButtonText.setFont(m_font);
        m_gasO2ButtonText.setCharacterSize(14);
        m_gasO2ButtonText.setFillColor(sf::Color::White);
        m_gasO2ButtonText.setString("Run GAS-O2");
        sf::FloatRect textRect = m_gasO2ButtonText.getLocalBounds();
        m_gasO2ButtonText.setOrigin(sf::Vector2f(textRect.position.x + textRect.size.x / 2.0f, textRect.position.y + textRect.position.y / 2.0f));
        m_gasO2ButtonText.setPosition(sf::Vector2f(m_gasO2Button.getPosition().x + m_gasO2Button.getSize().x / 2.0f,
                                      m_gasO2Button.getPosition().y + m_gasO2Button.getSize().y / 2.0f));
    }

    // Mensajitos
    m_messageBoxRect.setSize(sf::Vector2f(300, 50));
    m_messageBoxRect.setFillColor(sf::Color(255, 255, 100, 200));
    m_messageBoxRect.setOutlineColor(sf::Color(150, 150, 0));
    m_messageBoxRect.setOutlineThickness(1);
    m_messageBoxRect.setOrigin(sf::Vector2f(m_messageBoxRect.getSize().x / 2.0f, m_messageBoxRect.getSize().y / 2.0f));
    m_messageBoxRect.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.0f, MAP_HEIGHT / 2.0f));

    if (font_loaded) {
        m_messageBoxText.setFont(m_font);
        m_messageBoxText.setCharacterSize(16);
        m_messageBoxText.setFillColor(sf::Color::Black);
        m_messageBoxText.setString("");
        m_messageBoxText.setOrigin(sf::Vector2f(m_messageBoxText.getLocalBounds().position.x + m_messageBoxText.getLocalBounds().size.x / 2.0f,
                                   m_messageBoxText.getLocalBounds().position.y + m_messageBoxText.getLocalBounds().size.y / 2.0f));
        m_messageBoxText.setPosition(m_messageBoxRect.getPosition());
    }

    loadData(jsonPath);
}
Visualizer::~Visualizer() {}

void Visualizer::run() {
    while (m_window.isOpen()) {
        handleEvents();
        update();
        render();
    }
}

void Visualizer::handleEvents() {
    // SFML 3.0 event polling
    while (const std::optional<sf::Event> event = m_window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            m_window.close();
        } else if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
                sf::Vector2f mousePos = m_window.mapPixelToCoords(mouseButtonPressed->position);

                // Check UPLOAD button
                if (m_uploadButton.getGlobalBounds().contains(mousePos)) {
                    loadData(m_filePathInputString);
                }
                // Check GAS button
                else if (m_gasButton.getGlobalBounds().contains(mousePos)) {
                    executeAlgorithm(GAS);
                }
                // Check GAS-O1 button
                else if (m_gasO1Button.getGlobalBounds().contains(mousePos)) {
                    executeAlgorithm(GAS_O1);
                }
                // Check GAS-O2 button
                else if (m_gasO2Button.getGlobalBounds().contains(mousePos)) {
                    executeAlgorithm(GAS_O2);
                }
                // Check if input field is clicked
                else if (m_filePathInputRect.getGlobalBounds().contains(mousePos)) {
                    m_isInputActive = true;
                    m_filePathInputRect.setOutlineColor(sf::Color::Blue); // Highlight active input
                } else {
                    m_isInputActive = false;
                    m_filePathInputRect.setOutlineColor(sf::Color::Black);
                }
            }
        } else if (const auto* textEntered = event->getIf<sf::Event::TextEntered>()) {
            if (m_isInputActive) {
                if (textEntered->unicode < 128) { // Only handle ASCII characters
                    if (textEntered->unicode == '\b') { // Backspace
                        if (!m_filePathInputString.empty()) {
                            m_filePathInputString.pop_back();
                        }
                    } else if (textEntered->unicode == '\r' || textEntered->unicode == '\n') { // Enter key
                        loadData(m_filePathInputString);
                        m_isInputActive = false;
                        m_filePathInputRect.setOutlineColor(sf::Color::Black);
                    } else {
                        m_filePathInputString += static_cast<char>(textEntered->unicode);
                    }
                    m_filePathInputText.setString(m_filePathInputString);
                }
            }
        }
    }
}

void Visualizer::update() {
    if (m_showMessageBox && m_messageBoxClock.getElapsedTime().asSeconds() > m_messageBoxDisplayTime) {
        m_showMessageBox = false;
    }
}

void Visualizer::render() {
    m_window.clear(sf::Color::Black);

    sf::RectangleShape mapBackground(sf::Vector2f(WINDOW_WIDTH, MAP_HEIGHT));
    mapBackground.setFillColor(sf::Color(30, 30, 30));
    mapBackground.setPosition(sf::Vector2f(0, 0));
    m_window.draw(mapBackground);

    drawRequests();
    drawVehicles();
    drawRoutes();

    drawMenuBar();
    drawFilePathInput();
    drawButtons();
    drawMessageBox();

    m_window.display();
}


void Visualizer::drawRequests() {
    for (const auto& req : m_current_requests) {
        // Origen
        sf::CircleShape originShape(4);
        originShape.setFillColor(sf::Color::Green);
        originShape.setPosition(simToScreen(req.origin.first, req.origin.second));
        originShape.setOrigin(sf::Vector2f(originShape.getRadius(), originShape.getRadius()));
        m_window.draw(originShape);

        // Destino
        sf::CircleShape destShape(4);
        destShape.setFillColor(sf::Color::Red);
        destShape.setPosition(simToScreen(req.destination.first, req.destination.second));
        destShape.setOrigin(sf::Vector2f(destShape.getRadius(), destShape.getRadius()));
        m_window.draw(destShape);

        // Linea
        sf::VertexArray requestLine(sf::PrimitiveType::Lines, 2);
        requestLine[0].position = simToScreen(req.origin.first, req.origin.second);
        requestLine[0].color = sf::Color(100, 100, 100);
        requestLine[1].position = simToScreen(req.destination.first, req.destination.second);
        requestLine[1].color = sf::Color(100, 100, 100);
        m_window.draw(requestLine);
    
        if (font_loaded) {
            sf::Text originCoordText(m_font);
            originCoordText.setCharacterSize(10);
            originCoordText.setFillColor(sf::Color::White);
            originCoordText.setString("(" + std::to_string(static_cast<int>(req.origin.first)) + "," + std::to_string(static_cast<int>(req.origin.second)) + ")");
            sf::Vector2f textPosOrigin = simToScreen(req.origin.first, req.origin.second);
            originCoordText.setPosition(sf::Vector2f(textPosOrigin.x + originShape.getRadius() + 2, textPosOrigin.y - originShape.getRadius() - 8));
            m_window.draw(originCoordText);

            sf::Text destCoordText(m_font);
            destCoordText.setCharacterSize(10);
            destCoordText.setFillColor(sf::Color::White);
            destCoordText.setString("(" + std::to_string(static_cast<int>(req.destination.first)) + "," + std::to_string(static_cast<int>(req.destination.second)) + ")");

            sf::Vector2f textPosDest = simToScreen(req.destination.first, req.destination.second);
            destCoordText.setPosition(sf::Vector2f(textPosDest.x + destShape.getRadius() + 2, textPosDest.y - destShape.getRadius() - 8));
            m_window.draw(destCoordText);
        }
    }
}

void Visualizer::drawVehicles() {
    for (const auto& veh : m_current_vehicles) {
        sf::CircleShape vehicleShape(8);
        vehicleShape.setFillColor(sf::Color::Blue);
        vehicleShape.setPosition(simToScreen(veh.location.first, veh.location.second));
        vehicleShape.setOrigin(sf::Vector2f(vehicleShape.getRadius(), vehicleShape.getRadius()));
        m_window.draw(vehicleShape);

        if (font_loaded) {
            sf::Text vehicleIdText(m_font, std::to_string(veh.id), 12u);
            vehicleIdText.setFillColor(sf::Color::White);
            sf::Vector2f textPos = simToScreen(veh.location.first, veh.location.second);
            vehicleIdText.setPosition(sf::Vector2f(textPos.x + vehicleShape.getRadius(), textPos.y - vehicleShape.getRadius() - 5));
            m_window.draw(vehicleIdText);

            sf::Text vehicleCoordText(m_font);
            vehicleCoordText.setCharacterSize(10);
            vehicleCoordText.setFillColor(sf::Color::Blue);
            vehicleCoordText.setString("(" + std::to_string(static_cast<int>(veh.location.first)) + "," + std::to_string(static_cast<int>(veh.location.second)) + ")");

            vehicleCoordText.setPosition(sf::Vector2f(textPos.x + vehicleShape.getRadius() + 2, textPos.y + vehicleShape.getRadius() + 2));
            m_window.draw(vehicleCoordText);
        }
    }
}

void Visualizer::drawRoutes() {
    sf::Color routeColor = sf::Color::Cyan;

    for (const auto& veh : m_current_vehicles) {
        if (!veh.currentRoutePath.empty()) {
            sf::VertexArray routeLine(sf::PrimitiveType::LineStrip);

            for (const auto& point : veh.currentRoutePath) {
                sf::Vertex v;
                v.position = simToScreen(point.first, point.second);
                v.color = routeColor;
                routeLine.append(v);
            }
            m_window.draw(routeLine);
        }
    }
}

sf::Vector2f Visualizer::simToScreen(double x_sim, double y_sim) {
    float usableMapWidth = static_cast<float>(WINDOW_WIDTH) - 2 * MAP_PADDING;
    float usableMapHeight = static_cast<float>(MAP_HEIGHT) - 2 * MAP_PADDING;

    float scale_x = usableMapWidth / MAX_SIM_COORD;
    float scale_y = usableMapHeight / MAX_SIM_COORD;

    float screen_x = static_cast<float>(x_sim) * scale_x;
    float screen_y = static_cast<float>(y_sim) * scale_y;

    screen_x += MAP_PADDING;
    screen_y += MAP_PADDING;

    return sf::Vector2f(screen_x, screen_y);
}

void Visualizer::drawMenuBar() {
    m_window.draw(m_menuBar);
}

void Visualizer::drawButtons() {
    m_window.draw(m_uploadButton);
    if (font_loaded) m_window.draw(m_uploadButtonText);

    m_window.draw(m_gasButton);
    if (font_loaded) m_window.draw(m_gasButtonText);

    m_window.draw(m_gasO1Button);
    if (font_loaded) m_window.draw(m_gasO1ButtonText);

    m_window.draw(m_gasO2Button);
    if (font_loaded) m_window.draw(m_gasO2ButtonText);
}

void Visualizer::drawFilePathInput() {
    m_window.draw(m_filePathInputRect);
    if (font_loaded) m_window.draw(m_filePathInputText);
}

void Visualizer::drawMessageBox() {
    if (m_showMessageBox) {
        m_window.draw(m_messageBoxRect);
        if (font_loaded) {
            m_window.draw(m_messageBoxText);
        }
    }
}

void Visualizer::showMessage(const std::string& message, float duration) {
    m_messageBoxText.setString(message);

    sf::FloatRect textRect = m_messageBoxText.getLocalBounds();
    m_messageBoxText.setOrigin(sf::Vector2f(textRect.position.x + textRect.size.x / 2.0f, textRect.position.y + textRect.position.y / 2.0f));
    m_messageBoxText.setPosition(m_messageBoxRect.getPosition());

    m_messageBoxDisplayTime = duration;
    m_showMessageBox = true;
    m_messageBoxClock.restart();
}

void Visualizer::loadData(const std::string& filePath) {
    showMessage("Loading data from " + filePath + "...", 2.0f);
    auto data = readRequestsAndVehiclesFromJson(filePath);
    if (!data.first.empty() || !data.second.empty()) {
        m_initial_requests = data.first;
        m_initial_vehicles = data.second;
        m_current_requests = m_initial_requests;
        m_current_vehicles = m_initial_vehicles;
        showMessage("Data loaded successfully!", 2.0f);
    } else {
        showMessage("Failed to load data from " + filePath, 3.0f);
        m_initial_requests.clear();
        m_initial_vehicles.clear();
        m_current_requests.clear();
        m_current_vehicles.clear();
    }
}

void Visualizer::executeAlgorithm(GASVariant variant) {
    if (m_initial_requests.empty() || m_initial_vehicles.empty()) {
        showMessage("Please load data first using the UPLOAD button.", 3.0f);
        return;
    }

    std::vector<Request> requestsToPlan = m_initial_requests;
    std::vector<Vehicle> vehiclesToPlan = m_initial_vehicles;

    std::string algoName;
    switch (variant) {
        case GAS:
            algoName = "GAS";
            std::cout << "Running GAS planning algorithm...\n";
            planRoutesGAS(requestsToPlan, vehiclesToPlan);
            break;
        case GAS_O1:
            algoName = "GAS-O1";
            std::cout << "Running GAS-O1 planning algorithm...\n";
            planRoutesGASO1(requestsToPlan, vehiclesToPlan);
            break;
        case GAS_O2:
            algoName = "GAS-O2";
            std::cout << "Running GAS-O2 planning algorithm...\n";
            planRoutesGASO2(requestsToPlan, vehiclesToPlan);
            break;
    }
    std::cout << algoName << " planning complete.\n";

    m_current_requests = requestsToPlan;
    m_current_vehicles = vehiclesToPlan;

    showMessage(algoName + " algorithm executed!", 2.0f);
}