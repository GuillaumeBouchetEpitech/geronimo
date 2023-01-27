
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


TEST(network_wrapper, test_tcp_socket_with_poller_basic) {

  using MyTcpSocket = gero::network::TcpSocket;
  using MyTcpPoller = gero::network::SocketPoller<MyTcpSocket>;

  constexpr uint16_t k_port = 6667;
  const std::string k_payloadCtoS = "Hello from the client";
  const std::string k_payloadStoC = "Hello from the server";

  TestStep currentStep = TestStep::initializing;
  bool is_running = true;

  std::thread serverSideThread([&currentStep, &is_running, &k_payloadCtoS, &k_payloadStoC]()
  {

    MyTcpSocket listenSocket;
    MyTcpPoller serverPoller;

    ASSERT_EQ(listenSocket.create(false), true);
    ASSERT_EQ(listenSocket.isValid(), true);
    ASSERT_EQ(listenSocket.isInitialised(), true);
    ASSERT_EQ(listenSocket.isBlocking(), false);
    ASSERT_EQ(listenSocket.setAsReusingAddress(), true);
    ASSERT_EQ(listenSocket.listen("127.0.0.1", k_port), true);
    ASSERT_EQ(serverPoller.create(1), true);
    ASSERT_EQ(serverPoller.add(listenSocket, true, false), true);

    ASSERT_EQ(currentStep, TestStep::initializing);
    currentStep = TestStep::serverReady;

    MyTcpSocket remoteSocket;

    while (is_running)
    {

      if (currentStep == TestStep::clientReceived)
      {
        currentStep = TestStep::serverConfirm;
      }

      serverPoller.wait([&](MyTcpSocket& currSocket, bool isRead, bool isWrite) -> bool
      {
        static_cast<void>(isWrite); // unused

        if (!isRead)
          return true;

        if (currSocket == listenSocket)
        {
          assert(listenSocket.isValid() == true);
          assert(listenSocket.isInitialised() == true);

          assert(remoteSocket.isValid() == false);
          assert(remoteSocket.isInitialised() == false);
          assert(currSocket.accept(remoteSocket, false) == gero::network::socket::status::done);
          assert(remoteSocket.isValid() == true);
          assert(remoteSocket.isInitialised() == true);
          assert(remoteSocket.isBlocking() == false);
          assert(serverPoller.add(remoteSocket, true, false) == true);
        }
        else if (currSocket == remoteSocket) {

          assert(remoteSocket.isValid() == true);
          assert(remoteSocket.isInitialised() == true);

          if (currentStep == TestStep::clientReady)
          {
            {
              constexpr uint32_t bufferSize = 128;
              char tmpBuffer[bufferSize + 1];

              std::size_t totalReceived = 0;
              assert(currSocket.receive(tmpBuffer, bufferSize, totalReceived) == gero::network::socket::status::done);
              assert(totalReceived == k_payloadCtoS.size());

              tmpBuffer[totalReceived] = '\0';

              assert(k_payloadCtoS == tmpBuffer);
            }

            currentStep = TestStep::serverReceived;

            {
              std::size_t sent = 0;
              assert(remoteSocket.send(k_payloadStoC.data(), k_payloadStoC.size(), sent) == gero::network::socket::status::done);
              assert(sent == k_payloadStoC.size());
            }
          }
          else if (
            currentStep == TestStep::serverConfirm ||
            currentStep == TestStep::clientDone
          ) {

            {
              constexpr uint32_t bufferSize = 128;
              char tmpBuffer[bufferSize + 1];

              std::size_t totalReceived = 0;
              assert(remoteSocket.receive(tmpBuffer, bufferSize, totalReceived) == gero::network::socket::status::error);
              assert(totalReceived == 0);
            }

            currentStep = TestStep::serverDone;
            return false; // remove
          }
        }

        return true;
      },
      [](MyTcpSocket& currSocket)
      {
        static_cast<void>(currSocket); // unused
      },
      0.125f);
    }
  });

  ASSERT_EQ(wait_step(1.0f, currentStep, TestStep::serverReady), true);


  {
    MyTcpPoller clientPoller;

    MyTcpSocket clientSocket;
    ASSERT_EQ(clientSocket.create(false), true);
    ASSERT_EQ(clientSocket.isValid(), true);
    ASSERT_EQ(clientSocket.isInitialised(), true);
    ASSERT_EQ(clientSocket.isBlocking(), false);
    ASSERT_EQ(clientSocket.connect("127.0.0.1", k_port, 0.5f), true);
    ASSERT_EQ(clientPoller.create(1), true);
    ASSERT_EQ(clientPoller.add(clientSocket, true, false), true);

    ASSERT_EQ(currentStep, TestStep::serverReady);
    currentStep = TestStep::clientReady;

    std::size_t sent = 0;
    ASSERT_EQ(clientSocket.send(k_payloadCtoS.data(), k_payloadCtoS.size(), sent), gero::network::socket::status::done);
    ASSERT_EQ(sent, k_payloadCtoS.size());

    clientPoller.wait([&](MyTcpSocket& currSocket, bool isRead, bool isWrite) -> bool
    {
      static_cast<void>(isWrite); // unused

      if (!isRead)
        return true;

      {
        constexpr uint32_t bufferSize = 128;
        char tmpBuffer[bufferSize + 1];

        std::size_t totalReceived = 0;
        assert(currSocket.receive(tmpBuffer, bufferSize, totalReceived) == gero::network::socket::status::done);
        assert(totalReceived == k_payloadStoC.size());

        tmpBuffer[totalReceived] = '\0';
        assert(k_payloadStoC == tmpBuffer);
      }

      assert(currentStep == TestStep::serverReceived);
      currentStep = TestStep::clientReceived;

      return true;
    },
    [](MyTcpSocket& currSocket)
    {
      static_cast<void>(currSocket); // unused
    },
    1.0f);

    ASSERT_EQ(wait_step(1.0f, currentStep, TestStep::serverConfirm), true);
    ASSERT_EQ(currentStep, TestStep::serverConfirm);

    clientSocket.disconnect();

    ASSERT_EQ(currentStep, TestStep::serverConfirm);
    currentStep = TestStep::clientDone;
  }

  ASSERT_EQ(wait_step(1.0f, currentStep, TestStep::serverDone), true);
  ASSERT_EQ(currentStep, TestStep::serverDone);

  is_running = false;

  if (serverSideThread.joinable())
    serverSideThread.join();
}



