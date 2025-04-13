#ifndef VISHNU_H
#define VISHNU_H

#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>

/*!
 * \class Vishnu
 * \brief Listens for Game Controller messages in RoboCup SSL and triggers system setup.
 *
 * Vishnu sets up a UDP socket (multicast or direct), listens for protobuf referee messages,
 * and calls setup logic (via Shunya) when the game enters the NORMAL_FIRST_HALF stage.
 */
class Vishnu : public QObject {
    Q_OBJECT

public:
    /*!
     * \brief Constructs the Vishnu object with a specific address and port.
     * \param parent Qt parent object
     * \param address IP address to bind
     * \param port Port number to listen on
     */
    Vishnu(QObject *parent, QHostAddress address, qint32 port);

    /*!
     * \brief Constructs the Vishnu object using default multicast address and port.
     *
     * Joins the standard RoboCup SSL Game Controller multicast group to receive referee messages.
     * \param parent Qt parent object
     */
    Vishnu(QObject *parent);

private slots:
    /*!
     * \brief Handles incoming datagrams from the Game Controller.
     *
     * This function parses the datagram as a Referee protobuf message,
     * and if the game is in NORMAL_FIRST_HALF stage, it logs the event and
     * triggers the Shunya setup process.
     */
    void handleDatagrams();

private:
    QHostAddress m_addr;     //!< IP address to bind or join for multicast
    qint32 m_port;           //!< Port number used for communication
    QUdpSocket *socket;      //!< UDP socket to receive referee messages
};

#endif // VISHNU_H
