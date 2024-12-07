#pragma once
#include <bits/stdc++.h>
#include <zmq.hpp>
const int MAIN_PORT = 6040;

void send_message(zmq::socket_t &socket, const std::string &msg) {
    zmq::message_t message(msg.size());
    memcpy(message.data(), msg.c_str(), msg.size());
    socket.send(message);
}

std::string receive_message(zmq::socket_t &socket) {
    zmq::message_t message;
    int chars_read;
    try {
        chars_read = (int)socket.recv(&message);
    } catch (...) {
        chars_read = 0;
    }
    if (chars_read == 0) {
        throw -1;
    }
    std::string received_msg(static_cast<char *>(message.data()),
                             message.size());
    return received_msg;
}

void connect(zmq::socket_t &socket, int port) {
    std::string address = "tcp://127.0.0.1:" + std::to_string(port);
    socket.connect(address);
}

void disconnect(zmq::socket_t &socket, int port) {
    std::string address = "tcp://127.0.0.1:" + std::to_string(port);
    socket.disconnect(address);
}

int bind(zmq::socket_t &socket, int id) {
    const int max_retries = 1000; // Maximum number of ports to try
    int port = MAIN_PORT + id;
    std::string address;
    for (int i = 0; i < max_retries; ++i) {
        try {
            address = "tcp://127.0.0.1:" + std::to_string(port);
            socket.bind(address);
            return port; // Successfully bound
        } catch (const zmq::error_t &e) {
            // Handle ZeroMQ-specific errors if needed
            port++; // Try the next port
        } catch (...) {
            // Catch any other exceptions
            throw std::runtime_error("Unknown error occurred during bind");
        }
    }

    // If we exhaust retries, throw an exception
    throw std::runtime_error("Error: Unable to bind to a port after " +
                             std::to_string(max_retries) + " attempts");
}

void unbind(zmq::socket_t &socket, int port) {
    std::string address = "tcp://127.0.0.1:" + std::to_string(port);
    socket.unbind(address);
}
