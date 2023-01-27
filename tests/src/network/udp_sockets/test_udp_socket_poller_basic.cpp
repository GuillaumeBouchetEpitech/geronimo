
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


TEST(network_wrapper, test_udp_socket_with_poller_basic) {

  using MyUdpSocket = gero::network::UdpSocket;
  using MyUdpPoller = gero::network::SocketPoller<MyUdpSocket>;

  constexpr uint16_t k_port = 6667;
  const std::string k_payloadCtoS = "Hello from the client";
  const std::string k_payloadStoC = "Hello from the server";

  TestStep currentStep = TestStep::initializing;
  // D_MYERR("TestStep::initializing");
  bool is_running = true;

  std::thread serverSideThread([&currentStep, &is_running, &k_payloadCtoS, &k_payloadStoC]()
  {

    MyUdpSocket serverSocket;
    MyUdpPoller serverPoller;

    ASSERT_EQ(serverSocket.create(false), true);
    ASSERT_EQ(serverSocket.isValid(), true);
    ASSERT_EQ(serverSocket.isInitialised(), true);
    ASSERT_EQ(serverSocket.isBlocking(), false);
    ASSERT_EQ(serverSocket.setAsReusingAddress(), true);
    ASSERT_EQ(serverSocket.bind("127.0.0.1", k_port), true);
    ASSERT_EQ(serverPoller.create(1), true);
    ASSERT_EQ(serverPoller.add(serverSocket, true, false), true);

    ASSERT_EQ(currentStep, TestStep::initializing);
    currentStep = TestStep::serverReady;

    serverPoller.wait([&](MyUdpSocket& currSocket, bool isRead, bool isWrite) -> bool
    {
      static_cast<void>(isWrite); // unused

      if (!isRead)
        return true;

      gero::network::IpAddress remoteAddress;
      uint16_t remotePort;

      {
        constexpr uint32_t bufferSize = 128;
        char tmpBuffer[bufferSize + 1];

        std::size_t totalReceived = 0;
        assert(currSocket.receive(tmpBuffer, bufferSize, totalReceived, remoteAddress, remotePort) == gero::network::socket::status::done);
        assert(totalReceived == k_payloadCtoS.size());

        tmpBuffer[totalReceived] = '\0';
        assert(k_payloadCtoS == tmpBuffer);
      }

      assert(currentStep == TestStep::clientReady);
      currentStep = TestStep::serverReceived;

      {
        std::size_t sent = 0;
        assert(serverSocket.send(k_payloadStoC.data(), k_payloadStoC.size(), remoteAddress, remotePort, sent) == gero::network::socket::status::done);
        assert(sent == k_payloadStoC.size());
      }

      assert(wait_step(1.0f, currentStep, TestStep::clientReceived) == true);

      currentStep = TestStep::serverConfirm;

      return true;
    },
    [](MyUdpSocket& currSocket)
    {
      static_cast<void>(currSocket); // unused
    },
    1.0f);
  });

  ASSERT_EQ(wait_step(1.0f, currentStep, TestStep::serverReady), true);

  {
    MyUdpPoller clientPoller;

    MyUdpSocket clientSocket;
    ASSERT_EQ(clientSocket.create(false), true);
    ASSERT_EQ(clientSocket.isValid(), true);
    ASSERT_EQ(clientSocket.isInitialised(), true);
    ASSERT_EQ(clientSocket.isBlocking(), false);
    ASSERT_EQ(clientPoller.create(1), true);
    ASSERT_EQ(clientPoller.add(clientSocket, true, false), true);

    ASSERT_EQ(currentStep, TestStep::serverReady);
    currentStep = TestStep::clientReady;

    {
      std::size_t sent = 0;
      ASSERT_EQ(clientSocket.send(k_payloadCtoS.data(), k_payloadCtoS.size(), "127.0.0.1", k_port, sent), gero::network::socket::status::done);
      ASSERT_EQ(sent, k_payloadCtoS.size());
    }

    clientPoller.wait([&](MyUdpSocket& currSocket, bool isRead, bool isWrite) -> bool
    {
      static_cast<void>(isWrite); // unused

      if (!isRead)
        return true;

      {
        constexpr uint32_t bufferSize = 128;
        char tmpBuffer[bufferSize + 1];

        std::size_t totalReceived = 0;
        gero::network::IpAddress remoteAddress;
        uint16_t remotePort;
        assert(currSocket.receive(tmpBuffer, bufferSize, totalReceived, remoteAddress, remotePort) == gero::network::socket::status::done);
        assert(totalReceived == k_payloadStoC.size());

        tmpBuffer[totalReceived] = '\0';
        assert(k_payloadStoC == tmpBuffer);
      }

      assert(currentStep == TestStep::serverReceived);
      currentStep = TestStep::clientReceived;

      return true;
    },
    [](MyUdpSocket& currSocket)
    {
      static_cast<void>(currSocket); // unused
    },
    1.0f);

    ASSERT_EQ(wait_step(1.0f, currentStep, TestStep::serverConfirm), true);

    clientSocket.close();

    currentStep = TestStep::clientDone;
  }

  is_running = false;

  if (serverSideThread.joinable())
      serverSideThread.join();

  ASSERT_EQ(currentStep, TestStep::clientDone);

}



