/****************************************************************************
 *   Copyright 2021 Andreas Wendler                                        *
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

 #ifndef CONFIGURATION_H
 #define CONFIGURATION_H
 
 #include <QString>
 #include <google/protobuf/message.h>
 #include <google/protobuf/text_format.h>
 #include <QFile>
 #include <iostream>
 
 #include "config/config.h"
 
 /*!
  * \brief Loads a protobuf configuration file from disk.
  *
  * This function reads a .txt configuration file located in the configured directory,
  * parses it using Google Protobuf TextFormat parser, and fills the provided message.
  *
  * \param configFile Name of the configuration file (without extension).
  * \param message Pointer to the protobuf message object to populate.
  * \param allowPartial Whether to allow partial message parsing.
  * \return True if the file was successfully parsed and loaded, false otherwise.
  */
 inline bool loadConfiguration(const QString &configFile, google::protobuf::Message *message, bool allowPartial)
 {
     QString fullFilename = QString(ERFORCE_CONFDIR) + configFile + ".txt";
     QFile file(fullFilename);
     if (!file.open(QFile::ReadOnly)) {
         std::cout <<"Could not open configuration file "<<fullFilename.toStdString()<<std::endl;
         return false;
     }
     QString str = file.readAll();
     file.close();
     std::string s = qPrintable(str);
 
     google::protobuf::TextFormat::Parser parser;
     parser.AllowPartialMessage(allowPartial);
     parser.ParseFromString(s, message);
     return true;
 }
 
 /*!
  * \brief Saves a protobuf message to a configuration file on disk.
  *
  * Serializes the provided message into a .txt file using Google Protobuf TextFormat.
  *
  * \param configFile Name of the file to save (without extension).
  * \param message Pointer to the protobuf message object to write.
  */
 inline void saveConfiguration(const QString &configFile, google::protobuf::Message *message)
 {
     QString fullFilename = QString(ERFORCE_CONFDIR) + configFile + ".txt";
     QFile file(fullFilename);
     if (!file.open(QFile::WriteOnly)) {
         std::cout <<"Could not open configuration file "<<fullFilename.toStdString()<<std::endl;
         return;
     }
 
     std::string output;
     google::protobuf::TextFormat::Printer printer;
     printer.PrintToString(*message, &output);
 
     file.write(output.data(), output.length());
     file.close();
 }
 
 #endif // CONFIGURATION_H
 