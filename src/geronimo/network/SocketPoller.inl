
// #include "SocketPoller.hpp"

#include "geronimo/system/TraceLogger.hpp"
#include "geronimo/system/ErrorHandler.hpp"

#include <unistd.h> // ::close()
#include <cerrno>
#include <cstring>

// #define D_DEBUG_SOCKET_POLLER

#if defined D_DEBUG_SOCKET_POLLER
  #define D_MYDEBUG D_MYERR
#else
  #define D_MYDEBUG(stream)
#endif

namespace gero {
namespace network {

template<typename SocketType>
SocketPoller<SocketType>::~SocketPoller() {
  if (isValid())
    close();
}

template<typename SocketType>
bool SocketPoller<SocketType>::create(uint32_t inTotalEvents /*= 10U*/) {

  if (isValid()) {
    D_MYERR("attempting to re-create an already initialised epoll");
    return false;
  }

  _epollFd = ::epoll_create1(EPOLL_CLOEXEC);
  if (_epollFd < 0) {
    D_MYERR("failed to create epoll, message: " << std::strerror(errno));
    return false;
  }

  if (inTotalEvents != _totalEvents)
  {
    _totalEvents = inTotalEvents;
    _eventsBuffer = std::make_unique<epoll_event[]>(_totalEvents);
  }
  return true;
}

template<typename SocketType>
bool SocketPoller<SocketType>::close() {
  if (!isValid())
  {
    D_MYERR("attempting to close an invalid epoll");
    return false;
  }

  _socketsHashMap.clear();

  const int32_t result = ::close(_epollFd);
  if (result == -1)
  {
    D_MYERR("failed to close epoll, message: " << std::strerror(errno));
    return false;
  }

  _epollFd = -1;
  return true;
}

template<typename SocketType>
bool SocketPoller<SocketType>::add(SocketType& inSocket, bool inRead /*= true*/, bool inWrite /*= true*/) {
  if (!isValid()) {
    D_MYERR("trying to add a socket of an invalid epoll");
    return false;
  }
  if (!inSocket.validate()) {
    D_MYERR("trying to add an invalid socket");
    return false;
  }
  if (inSocket.isBlocking()) {
    D_MYERR("trying to add a blocking socket");
    return false;
  }
  if (!inRead && !inWrite) {
    D_MYERR("was not set as read and/or write");
    return false;
  }

  epoll_event tmpEvent;
  tmpEvent.events = 0;
  if (inRead)
    tmpEvent.events |= EPOLL_EVENTS::EPOLLIN;
  if (inWrite)
    tmpEvent.events |= EPOLL_EVENTS::EPOLLOUT;
  tmpEvent.events |= EPOLL_EVENTS::EPOLLET;
  // tmpEvent.events |= EPOLL_EVENTS::EPOLLRDHUP;
  tmpEvent.data.fd = inSocket._handle;

  if (_socketsHashMap.count(inSocket._handle) > 0)
  {
      D_MYERR("epoll add -> SOCKET EXIST (" << inSocket._handle << ")");
      return false;
  }

  const int result = ::epoll_ctl(_epollFd, EPOLL_CTL_ADD, inSocket._handle, &tmpEvent);
  if (result < 0) {
    D_MYERR("failure to add a socket to epoll, message: " << std::strerror(errno));
    return false;
  }

  _socketsHashMap[inSocket._handle] = { &inSocket, inRead, inWrite };

  // D_MYDEBUG(" --------------- added");
  return true;
}

template<typename SocketType>
bool SocketPoller<SocketType>::remove(SocketType& inSocket, bool inRead /*= true*/, bool inWrite /*= true*/)
{
  if (!isValid()) {
    D_MYERR("trying to remove a socket of an invalid epoll");
    return false;
  }
  if (!inSocket.isValid()) {
    D_MYERR("trying to remove an invalid socket");
    return false;
  }
  if (!inRead && !inWrite) {
    D_MYERR("was not set as read and/or write");
    return false;
  }

  D_MYDEBUG(" removing, handle: " << inSocket._handle);

  auto it = _socketsHashMap.find(inSocket._handle);
  if (it == _socketsHashMap.end())
  {
    D_MYERR("epoll remove -> SOCKET NOT FOUND (" << inSocket._handle << ")");
    return false;
  }

  SocketData& socketData = it->second;

  epoll_event tmpEvent;
  tmpEvent.events = 0;
  if (socketData.isRead && inRead)
  {
    tmpEvent.events |= EPOLL_EVENTS::EPOLLIN;
    socketData.isRead = false;
  }
  if (socketData.isWrite && inWrite)
  {
    tmpEvent.events |= EPOLL_EVENTS::EPOLLOUT;
    socketData.isWrite = false;
  }

  const bool dataToDelete = (!socketData.isRead && !socketData.isWrite);
  if (dataToDelete)
  {
    tmpEvent.events |= EPOLL_EVENTS::EPOLLET;
    // tmpEvent.events |= EPOLL_EVENTS::EPOLLRDHUP;
  }

  tmpEvent.data.fd = inSocket._handle;

  if (::epoll_ctl(_epollFd, EPOLL_CTL_DEL, inSocket._handle, &tmpEvent)) {
    D_MYERR("Failed to del file descriptor to epoll, handle: " << inSocket._handle << ", message: " << std::strerror(errno));
    return false;
  }

  if (dataToDelete)
    _socketsHashMap.erase(it);

  D_MYDEBUG(" removed, handle: " << inSocket._handle);
  return true;
}

template<typename SocketType>
bool SocketPoller<SocketType>::wait(
  std::function<bool(SocketType&, bool, bool)> inCallback,
  std::function<void(SocketType&)> inFailCallback,
  float timeout /*= 0.0f*/
) {

  if (!isValid()) {
    D_MYERR("trying to wait for sockets of an invalid epoll");
    return false;
  }
  if (!inCallback) {
    D_MYERR("callback needed");
    return false;
  }

  const int32_t realTimeout = timeout > 0.0f ? int32_t(timeout * 1000.0f) : -1;

  // D_MYDEBUG(" --------------- waiting");

  const int32_t eventCount = ::epoll_wait(_epollFd, _eventsBuffer.get(), int32_t(_totalEvents), realTimeout);

  if (eventCount <= 0)
    return true;

  D_MYDEBUG(" ---------- eventCount=" << eventCount);

  // printf("%d ready events\n", event_count);
  for (std::size_t index = 0; index < std::size_t(eventCount); ++index) {
    const epoll_event& currEvent = _eventsBuffer[index];
    const int32_t currFd = currEvent.data.fd;
    const uint32_t currEvents = currEvent.events;

    // if (!(currEvents & EPOLL_EVENTS::EPOLLIN))
    //     continue;

    {

#ifdef D_DEBUG_SOCKET_POLLER

      std::stringstream sstr;

      sstr << "[";
      if (currEvents & EPOLL_EVENTS::EPOLLIN)
        sstr << " EPOLLIN";
      if (currEvents & EPOLL_EVENTS::EPOLLPRI)
        sstr << " EPOLLPRI";
      if (currEvents & EPOLL_EVENTS::EPOLLOUT)
        sstr << " EPOLLOUT";
      if (currEvents & EPOLL_EVENTS::EPOLLRDNORM)
        sstr << " EPOLLRDNORM";
      if (currEvents & EPOLL_EVENTS::EPOLLRDBAND)
        sstr << " EPOLLRDBAND";
      if (currEvents & EPOLL_EVENTS::EPOLLWRNORM)
        sstr << " EPOLLWRNORM";
      if (currEvents & EPOLL_EVENTS::EPOLLWRBAND)
        sstr << " EPOLLWRBAND";
      if (currEvents & EPOLL_EVENTS::EPOLLMSG)
        sstr << " EPOLLMSG";
      if (currEvents & EPOLL_EVENTS::EPOLLERR)
        sstr << " EPOLLERR";
      if (currEvents & EPOLL_EVENTS::EPOLLHUP)
        sstr << " EPOLLHUP";
      if (currEvents & EPOLL_EVENTS::EPOLLRDHUP)
        sstr << " EPOLLRDHUP";
      if (currEvents & EPOLL_EVENTS::EPOLLEXCLUSIVE)
        sstr << " EPOLLEXCLUSIVE";
      if (currEvents & EPOLL_EVENTS::EPOLLWAKEUP)
        sstr << " EPOLLWAKEUP";
      if (currEvents & EPOLL_EVENTS::EPOLLONESHOT)
        sstr << " EPOLLONESHOT";
      if (currEvents & EPOLL_EVENTS::EPOLLET)
        sstr << " EPOLLET";
      sstr << " ]";

      std::string str = sstr.str();

      D_MYDEBUG(" ---------------------- [" << currFd << "], event[" << index << "] => " << str);

#endif

    }

    const bool isRead = (currEvents & EPOLL_EVENTS::EPOLLIN);
    const bool isWrite = (currEvents & EPOLL_EVENTS::EPOLLOUT);

    auto it = _socketsHashMap.find(currFd);
    if (it == _socketsHashMap.end())
    {
      D_MYERR("socket ready but not found, currFd => " << currFd);

      // NOTE: the socket is already auto-removed from epoll

      continue;
    }

    /**
    if (currEvents & EPOLL_EVENTS::EPOLLHUP)
    {
        // D_MYLOG(" --------------- close (EPOLLHUP)");
        D_MYLOG("errno: " << errno);
        D_MYLOG("  ===> " << std::strerror(errno));

        it->second->close();
        remove(it->second);
        inFailCallback(it->second);
        continue;
    }

    if(currEvents & EPOLL_EVENTS::EPOLLERR)
    {
        // D_MYLOG(" --------------- close (EPOLLERR)");

        D_MYLOG("epoll err -> EPOLLERR");

        it->second->close();
        remove(it->second);
        inFailCallback(it->second);
        continue;
    }
    //*/

    // if (!it->second->isValid())
    //     continue;

    // const bool isRead = (currEvents & EPOLL_EVENTS::EPOLLIN);
    // const bool isWrite = (currEvents & EPOLL_EVENTS::EPOLLOUT);

    // if (isRead || isWrite)
    {
      SocketData& currData = it->second;
      SocketType& currSocket = *currData.socketPtr;

      const bool success = inCallback(currSocket, isRead, isWrite);
      if (!success) {
        if (currSocket.isValid())
          currSocket.close();

        // NOTE: the socket is already auto-removed from epoll

        if (inFailCallback)
          inFailCallback(currSocket);
      }
    }
  }

  return true;
}

template<typename SocketType>
bool SocketPoller<SocketType>::isValid() const
{
  return _epollFd != -1;
}

template<typename SocketType>
bool SocketPoller<SocketType>::clear() {
  if (!isValid()) {
    D_MYERR("trying to clear sockets of an invalid epoll");
    return false;
  }

  if (!close() || !create(_totalEvents))
  {
    D_MYERR("failed to clear epoll");
    return false;
  }
  return true;
}

} // namespace network
} // namespace gero
