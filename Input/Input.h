/**
* @file Input.h
* @brief Read yaml input file
*
* @author Calogero B. Rizzo
*
* @copyright This file is part of the lazymole software.
*            Copyright (C) 2019 Calogero B. Rizzo
*
* @license This program is free software: you can redistribute it and/or modify
*          it under the terms of the GNU General Public License as published by
*          the Free Software Foundation, either version 3 of the License, or
*          (at your option) any later version.
*
*          This program is distributed in the hope that it will be useful,
*          but WITHOUT ANY WARRANTY; without even the implied warranty of
*          MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*          GNU General Public License for more details.
*
*          You should have received a copy of the GNU General Public License
*          along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef LMA_INPUT_H
#define LMA_INPUT_H

#include <yaml-cpp/yaml.h>
#include <sstream>
#include <string>
#include <iostream>

namespace lma
{
    class Input
    {
    public:

        Input(const std::string& fileName);

        size_t nx() const;
        size_t ny() const;
        size_t nz() const;

        double dx() const;
        double dy() const;
        double dz() const;

        size_t refx() const;
        size_t refy() const;
        size_t refz() const;


        std::string field() const;
        size_t fieldSkip() const;
        bool fieldLog() const;

        std::string source() const;
        std::string target() const;
        std::string outputRes() const;
        std::string outputPath() const;

    private:

        YAML::Node config;

    };
}

#endif //LMA_INPUT_H
