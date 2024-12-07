#include "zmq_functions.hpp"
#include <gtest/gtest.h>
#include <zmq.hpp>

class ZeroMQTest : public ::testing::Test {
  protected:
    zmq::context_t context;
    zmq::socket_t socket;

    ZeroMQTest() : context(1), socket(context, zmq::socket_type::req) {}

    void SetUp() override {
        // Prepare socket for tests
    }

    void TearDown() override {
        // Clean up resources after each test
        socket.close();
    }
};

// Test send_message and receive_message
TEST_F(ZeroMQTest, SendAndReceiveMessage) {
    zmq::socket_t server_socket(context, zmq::socket_type::rep);
    int port = bind(server_socket, 0);

    connect(socket, port);

    // Send a message
    send_message(socket, "Hello, ZeroMQ!");

    // Receive a message on the server
    std::string received_msg = receive_message(server_socket);

    EXPECT_EQ(received_msg, "Hello, ZeroMQ!");

    // Clean up
    disconnect(socket, port);
    unbind(server_socket, port);
}

// Test connect and disconnect
TEST_F(ZeroMQTest, ConnectSocket) {
    zmq::socket_t server_socket(context, zmq::socket_type::rep);
    int port = bind(server_socket, 0);

    EXPECT_NO_THROW(connect(socket, port));

    // Clean up
    disconnect(socket, port);
    unbind(server_socket, port);
}

TEST_F(ZeroMQTest, DisconnectSocket) {
    zmq::socket_t server_socket(context, zmq::socket_type::rep);
    int port = bind(server_socket, 0);

    connect(socket, port);
    EXPECT_NO_THROW(disconnect(socket, port));

    // Clean up
    unbind(server_socket, port);
}

// Test bind and unbind
TEST_F(ZeroMQTest, BindSocket) {
    zmq::socket_t server_socket(context, zmq::socket_type::rep);

    int port = bind(server_socket, 0);
    EXPECT_GE(port, MAIN_PORT);

    // Clean up
    unbind(server_socket, port);
}

TEST_F(ZeroMQTest, UnbindSocket) {
    zmq::socket_t server_socket(context, zmq::socket_type::rep);

    int port = bind(server_socket, 0);
    EXPECT_NO_THROW(unbind(server_socket, port));
}

// Test edge cases for bind
TEST_F(ZeroMQTest, BindCollision) {
    zmq::socket_t server_socket1(context, zmq::socket_type::rep);
    zmq::socket_t server_socket2(context, zmq::socket_type::rep);

    int port1 = bind(server_socket1, 0);
    int port2 = bind(server_socket2, 0);

    EXPECT_NE(port1, port2);

    // Clean up
    unbind(server_socket1, port1);
    unbind(server_socket2, port2);
}

int main(int argc, char **argv) {

    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}