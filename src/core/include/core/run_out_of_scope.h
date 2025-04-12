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

 #ifndef RUN_WHEN_OUT_OF_SCOPE
 #define RUN_WHEN_OUT_OF_SCOPE
 
 namespace core {
 namespace internal {
 
 /*!
  * \class Runner
  * \brief Executes a lambda function when the object goes out of scope (RAII).
  *
  * This utility class stores a lambda and runs it in the destructor,
  * allowing deferred or cleanup actions to be defined inline.
  *
  * \tparam T A callable type (e.g., lambda)
  */
 template<class T>
 class Runner {
 public:
     /*!
      * \brief Constructs the Runner with a lambda.
      * \param lambda The function to run when this object is destroyed.
      */
     Runner(T lambda) : m_lambda(lambda) {}
 
     /*!
      * \brief Destructor: automatically calls the stored lambda.
      */
     ~Runner() {
         m_lambda();
     }
 
 private:
     T m_lambda; //!< Stored lambda function
 };
 
 /*!
  * \brief Utility function to create a Runner object.
  * \tparam T Type of the lambda/callable
  * \param t The lambda function
  * \return A Runner that will execute the lambda when destroyed
  */
 template <class T>
 Runner<T> make_runner(T t) {
     return Runner<T>{t};
 }
 
 } // namespace internal
 } // namespace core
 
 /*!
  * \def RUN_WHEN_OUT_OF_SCOPE(X)
  * \brief Executes the given block X when the current scope ends.
  *
  */
 #define RUN_WHEN_OUT_OF_SCOPE(X) auto r{core::internal::make_runner([&]() X )}
 
 #endif // RUN_WHEN_OUT_OF_SCOPE
 