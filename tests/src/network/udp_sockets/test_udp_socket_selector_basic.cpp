
#include "headers.hpp"

#include <chrono>  // std::chrono::milliseconds
#include <cstring> // memset
#include <iostream>
#include <list>
#include <mutex>
#include <thread>
#include <cassert>

namespace {

  enum class TestStep
  {
    initializing = 0,
    serverReady,
    clientReady,
    serverReceived,
    clientReceived,
    serverConfirm,
    clientDone,
  };

  bool wait_until(float timeoutSec, const std::function<bool()>& callback) {
    constexpr int64_t k_msToWait = 100;
    int32_t iterationsLeft = std::max(1, int32_t(timeoutSec * 1000.0f / float(k_msToWait)));
    while (callback() && iterationsLeft-- > 0)
      std::this_thread::sleep_for(std::chrono::milliseconds(k_msToWait));
    return iterationsLeft > 0;
  }

  bool wait_step(float timeoutSec, TestStep& currentStep, TestStep inTargetStep)
  {
    return wait_until(timeoutSec, [&currentStep, inTargetStep](){ return currentStep != inTargetStep; });
  }

}

TEST(network_wrapper, test_udp_socket_with_selector_basic) {

  using MyUdpSocket = gero::network::UdpSocket;
  using MyUdpSelector = gero::network::SocketSelector<MyUdpSocket>;

  constexpr uint16_t k_port = 7777;
  const std::string k_payloadCtoS = "Hello from the client";
  const std::string k_payloadStoC = "Hello from the server";

  TestStep currentStep = TestStep::initializing;
  bool is_running = true;

  std::thread serverSideThread([&currentStep, &is_running, &k_payloadCtoS, &k_payloadStoC]()
  {

    MyUdpSocket serverSocket;
    MyUdpSelector serverSelector;

    ASSERT_EQ(serverSocket.create(true), true);
    ASSERT_EQ(serverSocket.isValid(), true);
    ASSERT_EQ(serverSocket.isInitialised(), true);
    ASSERT_EQ(serverSocket.isBlocking(), true);
    ASSERT_EQ(serverSocket.setAsReusingAddress(), true);
    ASSERT_EQ(serverSocket.bind("127.0.0.1", k_port), true);
    ASSERT_EQ(serverSelector.add(serverSocket), true);

    ASSERT_EQ(currentStep, TestStep::initializing);
    currentStep = TestStep::serverReady;

    while (is_running)
    {
      if (!serverSelector.waitRead(0.125f))
        continue;

      constexpr uint32_t bufferSize = 128;
      char tmpBuffer[bufferSize + 1];

      gero::network::IpAddress remoteAddress;
      uint16_t remotePort;

      {
        std::size_t totalReceived = 0;
        ASSERT_EQ(serverSocket.receive(tmpBuffer, bufferSize, totalReceived, remoteAddress, remotePort), gero::network::socket::status::done);
        ASSERT_EQ(totalReceived, k_payloadCtoS.size());

        tmpBuffer[totalReceived] = '\0';
        ASSERT_EQ(k_payloadCtoS, tmpBuffer);
      }

      ASSERT_EQ(currentStep, TestStep::clientReady);
      currentStep = TestStep::serverReceived;

      {
        std::size_t sent = 0;
        ASSERT_EQ(serverSocket.send(k_payloadStoC.data(), k_payloadStoC.size(), remoteAddress, remotePort, sent), gero::network::socket::status::done);
        ASSERT_EQ(sent, k_payloadStoC.size());
      }

      ASSERT_EQ(wait_step(1.0f, currentStep, TestStep::clientReceived), true);

      ASSERT_EQ(currentStep, TestStep::clientReceived);
      currentStep = TestStep::serverConfirm;
    }

    serverSocket.close();
  });

  ASSERT_EQ(wait_step(1.0f, currentStep, TestStep::serverReady), true);

  {
    MyUdpSelector clientSelector;

    MyUdpSocket clientSocket;
    ASSERT_EQ(clientSocket.create(true), true);
    ASSERT_EQ(clientSocket.isValid(), true);
    ASSERT_EQ(clientSocket.isInitialised(), true);
    ASSERT_EQ(clientSocket.isBlocking(), true);
    ASSERT_EQ(clientSelector.add(clientSocket), true);

    ASSERT_EQ(currentStep, TestStep::serverReady);
    currentStep = TestStep::clientReady;

    {
      std::size_t sent = 0;
      ASSERT_EQ(clientSocket.send(k_payloadCtoS.data(), k_payloadCtoS.size(), "127.0.0.1", k_port, sent), gero::network::socket::status::done);
      ASSERT_EQ(sent, k_payloadCtoS.size());
    }

    const int32_t totalToRead = clientSelector.waitRead(1.0f);
    ASSERT_EQ(totalToRead, 1);

    constexpr uint32_t bufferSize = 128;
    char tmpBuffer[bufferSize + 1];

    {
      gero::network::IpAddress remoteAddress;
      uint16_t remotePort;
      std::size_t totalReceived = 0;
      ASSERT_EQ(clientSocket.receive(tmpBuffer, bufferSize, totalReceived, remoteAddress, remotePort), gero::network::socket::status::done);
      ASSERT_EQ(totalReceived, k_payloadStoC.size());

      tmpBuffer[totalReceived] = '\0';
      ASSERT_EQ(k_payloadStoC, tmpBuffer);
    }

    ASSERT_EQ(currentStep, TestStep::serverReceived);
    currentStep = TestStep::clientReceived;

    ASSERT_EQ(wait_step(1.0f, currentStep, TestStep::serverConfirm), true);

    clientSocket.close();

    currentStep = TestStep::clientDone;

  }

  is_running = false;

  if (serverSideThread.joinable())
      serverSideThread.join();

  ASSERT_EQ(currentStep, TestStep::clientDone);

}






