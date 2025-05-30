/***************************************************************************
 *   Copyright 2015 Michael Eischer, Philipp Nordhus                       *
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

#ifndef CONFIG_H
#define CONFIG_H

#ifdef RELATIVE_DATA_DIRS
static const char* const ERFORCE_DATADIR = "./data/";
static const char* const ERFORCE_CONFDIR = "./config/";
static const char* const ERFORCE_LIBDIR = "./libs/";
static const char* const ERFORCE_STRATEGYDIR = "./strategy/";
static const char* const ERFORCE_AUTOREFDIR = "./autoref";
static const char* const GAMECONTROLLER_EXECUTABLE_LOCATION = "ssl-game-controller_v2.16.1_linux_amd64";
#elif defined(_WIN32)
static const char* const ERFORCE_DATADIR = "./data/";
static const char* const ERFORCE_CONFDIR = "./config/";
static const char* const ERFORCE_LIBDIR = "./libs/";
// maybe
static const char* const ERFORCE_STRATEGYDIR = "../software/strategy/";
static const char* const ERFORCE_AUTOREFDIR = "../software/autoref";
static const char* const GAMECONTROLLER_EXECUTABLE_LOCATION = "ssl-game-controller_v2.16.1_linux_amd64";
#else // _WIN32
static const char* const ERFORCE_DATADIR = "/home/bhanu/abhiyaan/ssl-simulator-new/ssl-simulator/data/";
static const char* const ERFORCE_CONFDIR = "/home/bhanu/abhiyaan/ssl-simulator-new/ssl-simulator/config/";
static const char* const ERFORCE_LIBDIR = "/home/bhanu/abhiyaan/ssl-simulator-new/ssl-simulator/libs/";
static const char* const ERFORCE_STRATEGYDIR = "/home/bhanu/abhiyaan/ssl-simulator-new/ssl-simulator/strategy/";
static const char* const ERFORCE_AUTOREFDIR = "/home/bhanu/abhiyaan/ssl-simulator-new/ssl-simulator/autoref/";
static const char* const GAMECONTROLLER_EXECUTABLE_LOCATION = "/home/bhanu/abhiyaan/ssl-simulator-new/ssl-simulator/dependencies/ssl-game-controller_v2.16.1_linux_amd64";
#endif // _WIN32



// colors
static const char* const UI_YELLOW_COLOR_LIGHT = "yellow";
static const char* const UI_YELLOW_COLOR_DARK = "#303010";
static const char* const UI_BLUE_COLOR_LIGHT = "dodgerblue";
static const char* const UI_BLUE_COLOR_DARK = "#05223d";
static const char* const UI_AUTOREF_COLOR_LIGHT = "lightgray";
static const char* const UI_AUTOREF_COLOR_DARK = "#161616";

// pathfinding
static const char* const SAVE_PATHFINDING_INPUT_ALL = "Save pathfinding input: all inputs";
static const char* const SAVE_PATHFINDING_INPUT_STANDARDSAMPLER = "Save pathfinding input: standard sampler inputs";
static const char* const SAVE_PATHFINDING_INPUT_ENDINOBSTACLE = "Save pathfinding input: end in obstacle inputs";
static const char* const SAVE_PATHFINDING_INPUT_ESCAPEOBSTACLE = "Save pathfinding input: escape obstacle inputs";

#endif // CONFIG_H
