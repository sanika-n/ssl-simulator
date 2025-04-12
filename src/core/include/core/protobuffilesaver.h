/***************************************************************************
 *   Copyright 2020 Andreas Wendler                                        *
 *   Robotics Erlangen e.V.                                                *
 *   http://www.robotics-erlangen.de/                                      *
 *   info@robotics-erlangen.de                                             *
 *                                                                         *
 *   This program is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   any later version.                                                    *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

 #ifndef PROTOBUFFILESAVER_H
 #define PROTOBUFFILESAVER_H
 
 #include <google/protobuf/message.h>
 #include <QDataStream>
 #include <QFile>
 #include <QMutex>
 #include <QObject>
 #include <QString>
 
 /*!
  * \class ProtobufFileSaver
  * \brief Saves serialized protobuf messages to a file in a thread-safe manner.
  *
  * The file is only created when \ref saveMessage is called for the first time.
  * Messages are written sequentially in a binary format using Qt's QDataStream.
  */
 class ProtobufFileSaver : QObject
 {
     Q_OBJECT
 
 public:
     /*!
      * \brief Constructs a ProtobufFileSaver.
      * \param filename Base filename to save to.
      * \param filePrefix Prefix to add to the start of saved messages (if any).
      * \param parent Optional QObject parent.
      */
     ProtobufFileSaver(QString filename, QString filePrefix, QObject *parent = nullptr);
 
     /*!
      * \brief Saves a protobuf message to the file.
      *
      * This function is thread-safe and can be called from any thread.
      * If the file is not yet opened, it will open it first.
      *
      * \param message The protobuf message to be serialized and written.
      */
     void saveMessage(const google::protobuf::Message &message);
 
     /*!
      * \brief Closes the output file stream.
      */
     void close() { m_file.close(); }
 
 private:
     //! Opens the file stream if not already opened.
     void open();
 
 private:
     QString m_filename;     //!< Output filename (or base path)
     QString m_filePrefix;   //!< Optional prefix for message entries
     QFile m_file;           //!< Output file handle
     QDataStream m_stream;   //!< Stream for binary writing
     QMutex m_mutex;         //!< Mutex for thread safety
     /* 
     * A mutex ensures that only one thread at a time can lock
     * it and enter a critical section of code (like writing
     * to a file or modifying shared data).
     */
 };
 
 #endif // PROTOBUFFILESAVER_H
 