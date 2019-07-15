/**
* @file Input.cpp
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

#include "Input.h"

namespace lma
{
    Input::Input(const std::string& fileName)
    {
        config = YAML::LoadFile(fileName);
    }

    // GRID PARAMETERS
    size_t Input::nx() const
    {
        return config["grid"]["dimensions"]["nx"].as<size_t>();
    }
    size_t Input::ny() const
    {
        return config["grid"]["dimensions"]["ny"].as<size_t>();
    }
    size_t Input::nz() const
    {
        return config["grid"]["dimensions"]["nz"].as<size_t>();
    }


    double Input::dx() const
    {
        return config["grid"]["cell size"]["dx"].as<double>();
    }
    double Input::dy() const
    {
        return config["grid"]["cell size"]["dy"].as<double>();
    }
    double Input::dz() const
    {
        return config["grid"]["cell size"]["dz"].as<double>();
    }


    size_t Input::refx() const
    {
        return config["grid"]["refinement"]["refx"].as<size_t>();
    }
    size_t Input::refy() const
    {
        return config["grid"]["refinement"]["refy"].as<size_t>();
    }
    size_t Input::refz() const
    {
        return config["grid"]["refinement"]["refz"].as<size_t>();
    }

    // INPUT PARAMETERS
    std::string Input::field() const
    {
        return config["input"]["field"]["file"].as<std::string>();
    }
    size_t Input::fieldSkip() const
    {
        return config["input"]["field"]["skip"].as<size_t>();
    }
    bool Input::fieldLog() const
    {
        return config["input"]["field"]["log"].as<bool>();
    }

    std::string Input::source() const
    {
        return config["input"]["source"]["file"].as<std::string>();
    }
    std::string Input::target() const
    {
        return config["input"]["target"]["file"].as<std::string>();
    }

    // OUTPUT PARAMETERS
    std::string Input::outputRes() const
    {
        return config["output"]["resistance"]["file"].as<std::string>();
    }
    std::string Input::outputPath() const
    {
        return config["output"]["path"]["file"].as<std::string>();
    }


}
