/*
 * The simulator is divided into various components.
 * If you are versed with mythology you may be able to
 * guess each components purpose.
 *
 * Vyasa: tells you all that is happening.
 * the intermediary between the backend and
 * frontend.
 */

#ifndef VYASA_H
#define VYASA_H

#include <QObject>
#include <QUdpSocket>
#include <QString>
#include <QNetworkDatagram>

/**
 * @class Vyasa
 * @brief The Vyasa class is the intermediary between the backend and frontend of the simulator.
 * It handles the communication with the SSL-Vision and processes the data received as QByteArray.
 * It also uses QUdpSocket to listen for incoming datagrams and emits a signal when data is received.
 */

class Vyasa: public QObject {
    Q_OBJECT
    public:
        /**
        * @brief Constructor for Vyasa
        * Initializes the parent class of QObject to a null pointer and creates a QUdpSocket.
        */
        explicit Vyasa(QObject* parent = 0);
        ~Vyasa();
        // todo : add the receiver part to vyasa

        /**
        * @brief Sets the port and address for the QUdpSocket
        * @param port The port number to listen on
        * @param address The address to listen on
        */
        void setPortAndAddress(int port, const QString& address);
        // void sendCommand(float velX, int id);

    signals:
        /**
        * @brief Signal emitted when data is received from the socket
        * @param Buffer a pointer to the QByteArray containing the data
        * This signal is emitted when data is received from the socket.
        */
        void recievedState(QByteArray *buffer);

    public slots:
        /**
        * @brief Handles incoming datagrams from the socket
        * This slot is called when data is received from the socket.
        * It processes the data and emits the recievedState signal.
        */
        void handleDatagrams();
        /**
        * @brief Logs an error if the socket fails to connect.
        * @param socketError The error code from the socket
        */
        void onSocketError(QAbstractSocket::SocketError socketError);

    private:
        QHostAddress _addr; //address to listen to for ssl-vision data
        quint16 _port; //port to listen to for ssl-vision data
        QUdpSocket* socket;
        QNetworkDatagram datagram;
        QByteArray buffer;
};
#endif // VYASA_H
