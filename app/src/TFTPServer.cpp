#include "TFTPServer.h"

namespace tftp {

TFTPServer::TFTPServer(QObject *parent) : QObject{parent} {}

ServerState TFTPServer::getState() const { return m_state; }

void TFTPServer::setState(const ServerState &state) {
  if (m_state != state) {
    m_state = state;

    emit stateChanged();
  }
}

} // namespace tftp
