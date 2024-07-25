#include <iostream>
#include <windows.h>
#include <conio.h>

// Placeholder for serial port initialization and communication
class SerialPort {
public:
    SerialPort(const std::string& portName) {
        // Initialize your serial port here (e.g., COM3)
        // You might need to use CreateFile, SetCommState, etc., from the WinAPI for actual serial communication
    }

    bool isOpen() const {
        // Check if the serial port is open
        return true; // Placeholder
    }

    void write(const char* data) {
        // Write data to the serial port
        std::cout << "Writing to serial port: " << data << std::endl; // Placeholder
    }
};

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void drawMenu(int selection) {
    system("cls"); // Clear screen
    std::cout << "Choose an option:" << std::endl;

    gotoxy(5, 2);
    if (selection == 1) {
        std::cout << "> Turn On";
    }
    else {
        std::cout << "  Turn On";
    }

    gotoxy(20, 2);
    if (selection == 2) {
        std::cout << "> Turn Off";
    }
    else {
        std::cout << "  Turn Off";
    }
}

int main() {
    SerialPort serial("COM3"); // Example: Initialize serial port to COM3
    int selection = 1; // Start with the first option selected
    drawMenu(selection);

    while (true) {
        if (_kbhit()) { // Check if a key is pressed
            char ch = _getch(); // Get the pressed key
            if (ch == -32) { // Arrow keys return two codes, -32 and the code of the specific arrow
                ch = _getch(); // Get the specific arrow key code
                switch (ch) {
                case 75: // Left arrow key
                    if (selection > 1) {
                        selection--;
                        drawMenu(selection);
                    }
                    break;
                case 77: // Right arrow key
                    if (selection < 2) {
                        selection++;
                        drawMenu(selection);
                    }
                    break;
                }
            }
            else if (ch == '\r') { // Enter key is pressed
                std::cout << "\n\nOption " << selection << " selected.\n";
                if (serial.isOpen()) {
                    if (selection == 1) {
                        serial.write("1"); // Turn on
                    }
                    else if (selection == 2) {
                        serial.write("0"); // Turn off
                    }
                }
                else {
                    std::cout << "Serial Port isn't open!\n";
                }
                break; // Exit the loop
            }
        }
    }

    return 0;
}
