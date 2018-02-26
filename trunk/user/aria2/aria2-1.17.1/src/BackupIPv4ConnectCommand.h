/* <!-- copyright */
/*
 * aria2 - The high speed download utility
 *
 * Copyright (C) 2013 Tatsuhiro Tsujikawa
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 * In addition, as a special exception, the copyright holders give
 * permission to link the code of portions of this program with the
 * OpenSSL library under certain conditions as described in each
 * individual source file, and distribute linked combinations
 * including the two.
 * You must obey the GNU General Public License in all respects
 * for all of the code used other than OpenSSL.  If you modify
 * file(s) with this exception, you may extend this exception to your
 * version of the file(s), but you are not obligated to do so.  If you
 * do not wish to do so, delete this exception statement from your
 * version.  If you delete this exception statement from all source
 * files in the program, then also delete it here.
 */
/* copyright --> */
#ifndef BACKUP_IPV4_CONNECT_COMMAND_H
#define BACKUP_IPV4_CONNECT_COMMAND_H

#include "Command.h"

#include <string>

#include "SharedHandle.h"
#include "TimerA2.h"

namespace aria2 {

class RequestGroup;
class DownloadEngine;
class SocketCore;

// Used to communicate mainCommand and backup IPv4 connection command.
// When backup connection succeeds, ipaddr is filled with connected
// IPv4 address and socket is a socket connected to the ipaddr.  If
// mainCommand wants to cancel backup connection command, cancel
// member becomes true.
struct BackupConnectInfo {
  std::string ipaddr;
  SharedHandle<SocketCore> socket;
  bool cancel;
  BackupConnectInfo();
};

// Make backup connection to IPv4 address. This is a simplest RFC 6555
// "Happy Eyeballs" implementation.
class BackupIPv4ConnectCommand : public Command {
public:
  BackupIPv4ConnectCommand(cuid_t cuid,
                           const std::string& ipaddr, uint16_t port,
                           const SharedHandle<BackupConnectInfo>& info,
                           Command* mainCommand,
                           RequestGroup* requestGroup, DownloadEngine* e);
  ~BackupIPv4ConnectCommand();
  virtual bool execute();
private:
  std::string ipaddr_;
  uint16_t port_;
  SharedHandle<SocketCore> socket_;
  SharedHandle<BackupConnectInfo> info_;
  Command* mainCommand_;
  RequestGroup* requestGroup_;
  DownloadEngine* e_;
  Timer startTime_;
  Timer timeoutCheck_;
  time_t timeout_;
};

} // namespace aria2

#endif // BACKUP_IPV4_CONNECT_COMMAND_H
