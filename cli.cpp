#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include "epos_setup.h"
#include "main.h"

int AMOUNT_OF_MOTORS = 0;
int RPI = 0;
std::string PATH_LIB_WIN = "";
std::vector<std::tuple<void*, void*, int, int>> motors;

void motor_selector();
void set_motors(int amount_of_motors);
void set_environment(int env_type);
void set_path(const std::string& path);
void enable();
void disable();
void go_home();
void set_home();
void get_current_position_motors();
void move();
void intro();
void main_menu();

void motor_selector() {
    int amount_of_motors;
    std::cin >> amount_of_motors;
    set_motors(amount_of_motors);
    if (amount_of_motors == 1) {
        std::cout << "You have selected 1 motor\n";
    } else if (amount_of_motors == 2) {
        std::cout << "You have selected 2 motors\n";
    } else {
        std::cout << "Invalid input, please enter a valid number\n";
        motor_selector();
    }
}

void set_motors(int amount_of_motors) {
    AMOUNT_OF_MOTORS = amount_of_motors;
}

void set_environment(int env_type) {
    RPI = env_type;
}

void set_path(const std::string& path) {
    PATH_LIB_WIN = path;
}

void enable() {
    for (auto& motor : motors) {
        enable_epos(std::get<0>(motor), std::get<1>(motor), std::get<2>(motor), std::get<3>(motor));
    }
}

void disable() {
    for (auto& motor : motors) {
        disable_epos(std::get<0>(motor), std::get<1>(motor), std::get<2>(motor),
                     reinterpret_cast<unsigned int *>(std::get<3>(motor)));
    }
}

void go_home() {
    for (auto& motor : motors) {
        go_home_motor(std::get<0>(motor), std::get<1>(motor), std::get<2>(motor),
                      reinterpret_cast<unsigned int *>(std::get<3>(motor)));
    }
}

void set_home() {
    for (auto& motor : motors) {
        set_home_position(std::get<0>(motor), std::get<1>(motor), std::get<2>(motor),
                          reinterpret_cast<unsigned int *>(std::get<3>(motor)));
    }
    main_menu();
}

void get_current_position_motors() {
    for (auto& motor : motors) {
        std::cout << get_current_position(std::get<0>(motor), std::get<1>(motor), std::get<2>(motor),
                                          reinterpret_cast<unsigned int *>(std::get<3>(motor))) << std::endl;
    }
}

void move() {
    int max_position, min_position, num_of_movements;
    std::cout << "Please enter the maximum and minimum position you would like to move the motors to.\n";
    std::cout << "Max position: ";
    std::cin >> max_position;
    std::cout << "Min position: ";
    std::cin >> min_position;
    std::cout << "Please enter the amount of movements you would like to make: ";
    std::cin >> num_of_movements;
    while (num_of_movements > 0) {
        for (auto& motor : motors) {
            move_to_position(std::get<0>(motor), std::get<1>(motor), std::get<2>(motor),
                             reinterpret_cast<unsigned int *>(std::get<3>(motor)), max_position);
            move_to_position(std::get<0>(motor), std::get<1>(motor), std::get<2>(motor),
                             reinterpret_cast<unsigned int *>(std::get<3>(motor)), max_position);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            move_to_position(std::get<0>(motor), std::get<1>(motor), std::get<2>(motor),
                             reinterpret_cast<unsigned int *>(std::get<3>(motor)), min_position);
            move_to_position(std::get<0>(motor), std::get<1>(motor), std::get<2>(motor),
                             reinterpret_cast<unsigned int *>(std::get<3>(motor)), min_position);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        num_of_movements--;
    }
}

void intro() {
    std::string message_home = R"(
        ___    ____  ________  ____________   _____   ________
       /   |  / __ \/ ____/ / / /  _/ ____/  /  _/ | / / ____/
      / /| | / /_/ / /   / /_/ // // __/     / //  |/ / /
     / ___ |/ _, _/ /___/ __  // // /___   _/ // /|  / /____
    /_/  |_/_/ |_|\____/_/ /_/___/_____/  /___/_/ |_/\____(_)
    )";

    std::string message_amount_select = "Welcome to the Maxon Motor interface, to start, please select your environment (RPI = 1, PC = 0).\n";
    std::cout << message_home << message_amount_select;
    int env_type;
    std::cin >> env_type;
    set_environment(env_type);
    if (RPI == 0) {
        std::string path;
        std::cout << "Please enter the path to the EposCmd64.dll file (C:/PATH-TO-LIB/EposCmd64.dll): ";
        std::cin >> path;
        set_path(path);
    }
    std::cout << "Please select the amount of motors you would like to control.\n";
    motor_selector();
    std::cout << "Setting up motors...\n";
    for (int i = 0; i < AMOUNT_OF_MOTORS; ++i) {
        motors.push_back(epos_setup(RPI, i, ("USB" + std::to_string(i)).c_str(), PATH_LIB_WIN.c_str()));
        std::cout << "Motor " << i + 1 << " setup complete\n";
    }
    std::cout << "If you don't see any errors, the motors are ready to be controlled.\n";
    main_menu();
}

void main_menu() {
    while (true) {
        int choice = 0;
        std::cout << "You can now choose the following options, remember to enable the motors before moving them.\n";
        std::cout << "• Enable Motors (1)\n";
        std::cout << "• Disable Motors (2)\n";
        std::cout << "• Go Home (3)\n";
        std::cout << "• Set Home (4)\n";
        std::cout << "• Move Motors (5)\n";
        std::cout << "• Get position (6)\n";
        std::cin >> choice;
        switch (choice) {
            case 1:
                enable();
                break;
            case 2:
                disable();
                break;
            case 3:
                go_home();
                break;
            case 4:
                set_home();
                break;
            case 5:
                move();
                break;
            case 6:
                get_current_position_motors();
                break;
            default:
                std::cout << "Invalid input, please enter a valid number\n";
                main_menu();
                break;
        }
    }
}

int main() {
    intro();
    return 0;
}