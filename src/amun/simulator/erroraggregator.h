/****************************************************************************
 *   Copyright 2021 Tobias Heineken                                        *
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

#ifndef SIM_AGGREGATOR_H
#define SIM_AGGREGATOR_H

/**
 * @file erroraggregator.h
 * @brief Creates a class that maps the source of errors to the list containing the errors
 * 
 */

/// Includes (optional)
// #include <QObject>
// #include <QMap>    
// #include "protobuf/sslsim.h"  

/**
 * @class ErrorAggregator
 * @brief Aggregates the error based on the source they are from.
 * 
 * The goal of this class (.h file and .cpp file both together) is the following:
 * We define a class named ErrorAggregator which holds objects of the type map, where the key is the source from which the errors come and the value for a certain key is the list of all those errors.
 * Note: The only types of errors it does this for is of the type SSLSimError.
 * This class also enables us to return and clear those errors whenever needed.
 */


#include <QObject>
#include <QMap>    
#include "protobuf/sslsim.h"    

namespace camun {
    namespace simulator {

        enum class ErrorSource;
        class ErrorAggregator : public QObject {
            Q_OBJECT
        public:
            /**
            * @brief The constructor function.
            */
            ErrorAggregator(QObject* parent) : QObject(parent) {}
        
        // for slotting and signalling.
        public slots:
         /**
         * @brief aggregates the errors
         * @param e stores the source of the error in this variable
         * @param eror stores the list of errors in this variable
         */
            void aggregate(SSLSimError eror, ErrorSource e);

        public:
            /**
             * @brief Returns the errors of a particular source
             * @param e stores the source of the error in this variable
             * @return returns a list of all the errors from the source e
             */
            QList<SSLSimError> getAggregates(ErrorSource e);
        private:
            /// @brief  a map to store the key as the error source and the list of errors as the value.
            QMap<ErrorSource, QList<SSLSimError>> m_data;
        };
    }
}
#endif

