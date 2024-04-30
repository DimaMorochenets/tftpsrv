#ifndef TFTPSERVER_H
#define TFTPSERVER_H

#include <QObject>
#include <QScopedPointer>
#include <QUdpSocket>

namespace tftp {

Q_NAMESPACE

enum ServerState
{
  Active,
  Inactive,
  Busy,
  Undefined
};

Q_ENUM_NS(ServerState)

class TFTPServer : public QObject
{
  Q_OBJECT

  Q_PROPERTY(ServerState state READ getState WRITE setState NOTIFY stateChanged FINAL)

public:
  explicit TFTPServer(QObject *parent = nullptr);

signals:
  void stateChanged();

private:
  ServerState getState() const;
  void setState(const ServerState &state);

private:
  QScopedPointer<QUdpSocket> m_socket;
  ServerState m_state;
};

} // namespace tftp

#endif // TFTPSERVER_H
