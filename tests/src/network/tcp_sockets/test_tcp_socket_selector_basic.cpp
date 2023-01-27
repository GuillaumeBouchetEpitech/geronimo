
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
    serverDone,
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

TEST(network_wrapper, test_tcp_socket_with_selector_basic) {

  using MyTcpSocket = gero::network::TcpSocket;
  using MyTcpSelector = gero::network::SocketSelector<MyTcpSocket>;

  constexpr uint16_t k_port = 6666;
  const std::string k_payloadCtoS = "Hello from the client";
  const std::string k_payloadStoC = "Hello from the server";

  TestStep currentStep = TestStep::initializing;
  bool is_running = true;

  std::thread serverSideThread([&currentStep, &is_running, &k_payloadCtoS, &k_payloadStoC]()
  {

    MyTcpSocket listenSocket;
    MyTcpSocket remoteSocket;
    MyTcpSelector serverSelector;

    ASSERT_EQ(listenSocket.create(true), true);
    ASSERT_EQ(listenSocket.isValid(), true);
    ASSERT_EQ(listenSocket.isInitialised(), true);
    ASSERT_EQ(listenSocket.isBlocking(), true);
    ASSERT_EQ(listenSocket.setAsReusingAddress(), true);
    ASSERT_EQ(listenSocket.listen("127.0.0.1", k_port), true);
    ASSERT_EQ(serverSelector.add(listenSocket), true);

    ASSERT_EQ(currentStep, TestStep::initializing);
    currentStep = TestStep::serverReady;

    while (is_running)
    {
      if (!serverSelector.waitRead(0.125f))
        continue;

      if (serverSelector.isReady(listenSocket))
      {
        ASSERT_EQ(remoteSocket.isValid(), false);
        ASSERT_EQ(remoteSocket.isInitialised(), false);
        ASSERT_EQ(listenSocket.accept(remoteSocket, true), gero::network::socket::status::done);
        ASSERT_EQ(remoteSocket.isValid(), true);
        ASSERT_EQ(remoteSocket.isInitialised(), true);
        ASSERT_EQ(remoteSocket.isBlocking(), true);
        ASSERT_EQ(serverSelector.add(remoteSocket), true);
      }
      else if (serverSelector.isReady(remoteSocket))
      {
        ASSERT_EQ(remoteSocket.isValid(), true);
        ASSERT_EQ(remoteSocket.isInitialised(), true);

        if (currentStep == TestStep::clientReady)
        {
          {
            constexpr uint32_t bufferSize = 128;
            char tmpBuffer[bufferSize + 1];

            std::size_t totalReceived = 0;
            ASSERT_EQ(remoteSocket.receive(tmpBuffer, bufferSize, totalReceived), gero::network::socket::status::done);
            ASSERT_EQ(totalReceived, k_payloadCtoS.size());

            tmpBuffer[totalReceived] = '\0';

            ASSERT_EQ(k_payloadCtoS, tmpBuffer);
          }

          currentStep = TestStep::serverReceived;

          {
            std::size_t sent = 0;
            ASSERT_EQ(remoteSocket.send(k_payloadStoC.data(), k_payloadStoC.size(), sent), gero::network::socket::status::done);
            ASSERT_EQ(sent, k_payloadStoC.size());
          }

          ASSERT_EQ(wait_step(1.0f, currentStep, TestStep::clientReceived), true);

          currentStep = TestStep::serverConfirm;
        }
        else if (
          currentStep == TestStep::serverConfirm ||
          currentStep == TestStep::clientDone
        ) {
          {
            constexpr uint32_t bufferSize = 128;
            char tmpBuffer[bufferSize + 1];

            std::size_t totalReceived = 0;
            ASSERT_EQ(remoteSocket.receive(tmpBuffer, bufferSize, totalReceived), gero::network::socket::status::error);
            ASSERT_EQ(totalReceived, 0);
          }

          ASSERT_EQ(serverSelector.remove(remoteSocket), true);

          currentStep = TestStep::serverDone;
        }
      }

    }
  });

  ASSERT_EQ(wait_step(1.0f, currentStep, TestStep::serverReady), true);

  {
    MyTcpSelector clientSelector;

    MyTcpSocket clientSocket;
    ASSERT_EQ(clientSocket.create(true), true);
    ASSERT_EQ(clientSocket.isValid(), true);
    ASSERT_EQ(clientSocket.isInitialised(), true);
    ASSERT_EQ(clientSocket.isBlocking(), true);
    ASSERT_EQ(clientSocket.connect("127.0.0.1", k_port, 0.5f), true);
    ASSERT_EQ(clientSelector.add(clientSocket), true);

    ASSERT_EQ(currentStep, TestStep::serverReady);
    currentStep = TestStep::clientReady;

    {
      std::size_t sent = 0;
      ASSERT_EQ(clientSocket.send(k_payloadCtoS.data(), k_payloadCtoS.size(), sent), gero::network::socket::status::done);
      ASSERT_EQ(sent, k_payloadCtoS.size());
    }

    ASSERT_EQ(clientSelector.waitRead(1.0f), true);

    {
      constexpr uint32_t bufferSize = 128;
      char tmpBuffer[bufferSize + 1];

      std::size_t totalReceived = 0;
      ASSERT_EQ(clientSocket.receive(tmpBuffer, bufferSize, totalReceived), gero::network::socket::status::done);
      ASSERT_EQ(totalReceived, k_payloadStoC.size());

      tmpBuffer[totalReceived] = '\0';

      ASSERT_EQ(k_payloadStoC, tmpBuffer);
    }

    ASSERT_EQ(currentStep, TestStep::serverReceived);
    currentStep = TestStep::clientReceived;

    ASSERT_EQ(wait_step(1.0f, currentStep, TestStep::serverConfirm), true);

    clientSocket.disconnect();

    ASSERT_EQ(currentStep, TestStep::serverConfirm);
    currentStep = TestStep::clientDone;
  }

  ASSERT_EQ(wait_step(1.0f, currentStep, TestStep::serverDone), true);

  is_running = false;

  if (serverSideThread.joinable())
    serverSideThread.join();
}






