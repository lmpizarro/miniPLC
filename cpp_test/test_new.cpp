/**
 * @file /example1/main.cpp
 * @author Philippe Lucidarme
 * @date December 2019
 * @brief File containing example of serial port communication
 *
 * This example send a string through the serial device
 * The program expect receiving a string from the device
 * The received string is expected to terminated by a carriage return
 *
 * The following code has been tested with this Arduino Uno sketch
 * that return the uppercase character received on the serial device
 *
 * // Setup, initialize
 * void setup() {
 *      Serial.begin(115200);
 * }
 *
 * // Loop forever
 * void loop() {
 * // If serial data is pending, read and write the character uppercased
 * if (Serial.available())
 *      Serial.write( toupper (Serial.read()) );
 * }
 *
 * @see https://lucidar.me
 */

#include <vector>
#include <string>
#include <sstream>
#include <iostream>

// Serial library
#include "../serialib/lib/serialib.h"
#include <unistd.h>
#include <stdio.h>

#if defined(_WIN32) || defined(_WIN64)
// for serial ports above "COM9", we must use this extended syntax of "\\.\COMx".
// also works for COM0 to COM9.
// https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilea?redirectedfrom=MSDN#communications-resources
#define SERIAL_PORT "\\\\.\\COM1"
#endif
#if defined(__linux__) || defined(__APPLE__)
#define SERIAL_PORT "/dev/ttyACM0"
#endif

int parse_int(char *buffer)
{
    std::vector<std::string> parsed_response;
    std::stringstream sstr_buffer;
    sstr_buffer << buffer;
    while (sstr_buffer.good())
    {
        std::string substr;
        getline(sstr_buffer, substr, ':');
        parsed_response.push_back(substr);
    }

    if (parsed_response.size() == 2)
    {
        return std::stoi(parsed_response[1]);
    }

    return -1;
}

char send_receive(char *buffer)
{
    serialib serial;
    std::vector<std::string> parsed_response;

    // Connection to serial port
    char errorOpening = serial.openDevice(SERIAL_PORT, 9600);

    // If connection fails, return the error code otherwise, display a success message
    if (errorOpening != 1)
        return errorOpening;

    // Write the string on the serial device
    serial.writeString(buffer);
    std::cout << "String sent: " << buffer << std::endl;

    // Read the string
    serial.readString(buffer, '\n', 14, 2000);
    std::cout << "String read: " << buffer << std::endl;

    serial.closeDevice();
    return 1;
}


int main(/*int argc, char *argv[]*/)
{
    // Serial object
    serialib serial;

    char message_1[32] = ":1a1;\n";

    send_receive(message_1);
    int ar = parse_int(message_1);

    strcpy(message_1, ":1d1;\n");
    send_receive(message_1);
    int dr = parse_int(message_1);

    std::cout << "a1:" << ar << ":d1:" << dr << std::endl;
}
