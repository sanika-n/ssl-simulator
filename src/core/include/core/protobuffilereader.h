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

 #ifndef PROTOBUFFILEREADER_H
 #define PROTOBUFFILEREADER_H
 
 #include <google/protobuf/message.h>
 #include <QDataStream>
 #include <QFile>
 
 /*!
  * \class ProtobufFileReader
  * \brief Reads protobuf-encoded messages from a file.
  *
  * This class reads serialized protobuf messages sequentially from a binary file
  * written using QDataStream. It is primarily used for log replay or debugging.
  */
 class ProtobufFileReader : public QObject
 {
 public:
     /*!
      * \brief Constructs an empty ProtobufFileReader.
      */
     ProtobufFileReader();
 
     /*!
      * \brief Opens the file for reading.
      * \param filename Path to the file.
      * \param filePrefix Optional prefix to interpret within the stream (currently unused).
      * \return True if the file was opened successfully.
      */
     bool open(QString filename, QString filePrefix);
 
     /*!
      * \brief Reads the next protobuf message from the stream.
      * \param message A protobuf message object to populate.
      * \return True if a message was successfully read and parsed.
      */
     bool readNext(google::protobuf::Message &message);
 
 private:
     QFile m_file;          //!< Input file stream
     QDataStream m_stream;  //!< Qt data stream for reading binary data
 };
 
 #endif // PROTOBUFFILEREADER_H
 