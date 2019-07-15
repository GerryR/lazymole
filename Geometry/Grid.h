/**
* @file Grid.h
* @brief Base class for grids
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

#ifndef LMA_GRID_H
#define LMA_GRID_H

#include <memory>
#include <cstddef>
#include <vector>
#include "Point.h"

namespace mla {

    class Grid {

    protected:

        // Constructor
        Grid() {};

    public:

        // Destructor
        virtual ~Grid() {};

        // Interface
        virtual size_t numberOfCells() const = 0;

        virtual size_t numberOfFaces() const = 0;

        virtual std::vector<size_t> neighbors(const size_t id) const = 0;

        virtual Point3D centerOfCell(const size_t id) const = 0;

        virtual size_t resx() const = 0;

        virtual size_t resy() const = 0;

        virtual size_t resz() const = 0;

    };

}


#endif //LMA_GRID_H
