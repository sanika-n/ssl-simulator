#include "vyasa.h"
#include "core/sslprotocols.h"
// #include "protobuf/ssl_wrapper.pb.h"
// #include "protobuf/sslsim.h"
#include <QString>
#include <QUdpSocket>
#include <QNetworkDatagram>
#define LOG qDebug() << "[vyasa] : "

Vyasa::Vyasa(QObject* parent) : QObject(parent), socket(new QUdpSocket(this))
{
    // create a QUDP socket
    this->_addr.setAddress(SSL_VISION_ADDRESS_LOCALHOST);
    this->_port = quint16(SSL_SIMULATED_VISION_PORT);

    // if socket fails to connect
    connect(socket, &QAbstractSocket::errorOccurred,this, &Vyasa::onSocketError);

    socket->bind(_addr, _port, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    if(socket->state() != QAbstractSocket::BoundState){
        LOG << "socket not bound";
    }

    // new syntax, do not use SIGNAL() and SLOT()
    auto success = connect(socket, &QUdpSocket::readyRead, this, &Vyasa::handleDatagrams);
    if(!success){
        LOG << socket->errorString();
    }
}

Vyasa::~Vyasa()
{
    delete socket;
}

void Vyasa::onSocketError(QAbstractSocket::SocketError socketError)
{
    LOG<<"socket error occured and the error is : "<<socketError;
}

void Vyasa::setPortAndAddress(int port, const QString& address)
{
    this->_port = quint16(port);
    this->_addr.setAddress(address);
}

void Vyasa::handleDatagrams()
{
// when data comes in
    while(socket->hasPendingDatagrams()){
        datagram = socket->receiveDatagram();
        buffer = datagram.data();
        emit recievedState(&buffer);
    }
}
