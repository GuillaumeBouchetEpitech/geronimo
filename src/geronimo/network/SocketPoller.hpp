
#pragma once

#include <functional>
#include <unordered_map>
#include <sys/epoll.h> // for epoll_create1(), epoll_ctl(), struct epoll_event
#include <memory>

namespace gero {
namespace network {

template<typename SocketType>
class SocketPoller {
private:
  int32_t _epollFd = -1;

  // TODO: socketManager -> weak_ref_pool<...>

  struct SocketData {
    SocketType* socketPtr;
    bool isRead;
    bool isWrite;
  };

  std::unordered_map<int32_t, SocketData> _socketsHashMap;

  uint32_t _totalEvents = 0;
  std::unique_ptr<epoll_event[]> _eventsBuffer{nullptr};

public:
  SocketPoller() = default;
  SocketPoller(const SocketPoller& other) = delete; // TODO
  SocketPoller(SocketPoller&& other) = delete; // TODO
  SocketPoller& operator=(const SocketPoller& other) = delete; // TODO
  SocketPoller& operator=(SocketPoller&& other) = delete; // TODO
  ~SocketPoller();

public:
  bool create(uint32_t inTotalEvents = 10U);
  bool close();

public:
  bool add(SocketType& inSocket, bool inRead = true, bool inWrite = true);
  bool remove(SocketType& inSocket, bool inRead = true, bool inWrite = true);
  bool wait(std::function<bool(SocketType&, bool, bool)> inCallback, std::function<void(SocketType&)> inFailCallback, float timeout = 0.0f);

  bool isValid() const;
  bool clear();
};

} // namespace network
} // namespace gero

#include "SocketPoller.inl"
