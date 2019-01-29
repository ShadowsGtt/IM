#ifndef _INETADDRESS_H
#define _INETADDRESS_H


#include <netinet/in.h>
#include <string>

using namespace std;


namespace net
{

namespace sockets
{
 const struct sockaddr* sockaddr_cast(const struct sockaddr_in* addr);
}


/// This is an POD interface class.
class InetAddress
{
public:
  InetAddress(string ip, uint16_t port);

  /// Constructs an endpoint with given struct @c sockaddr_in
  /// Mostly used when accepting new connections
  explicit InetAddress(const struct sockaddr_in& addr) : addr_(addr)
  { }
  sa_family_t family() const { return addr_.sin_family; }
  string toIp() const;
  string toIpPort() const;
  uint16_t toPort() const;

  // default copy/assignment are Okay

  uint32_t ipNetEndian() const;
  uint16_t portNetEndian() const { return addr_.sin_port; }
  const struct sockaddr* getSockAddr() const { return sockets::sockaddr_cast(&addr_); }


 private:
    struct sockaddr_in addr_;

};

}

#endif  // _INETADDRESS_H

