/**
* @file CartesianGrid.h
* @brief Class for Cartesian grids
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

#ifndef LMA_CARTESIANGRID_H
#define LMA_CARTESIANGRID_H

#include <cstddef>
#include "Point.h"
#include "Grid.h"

namespace mla
{

    enum Direction
    {
        XP = 0, // XP: cell increasing x
        XM,     // XM: cell decreasing x
        YP,
        YM,
        ZP,
        ZM
    };

    class CartesianGrid : public Grid
    {

    public:

        CartesianGrid(const size_t nx, const size_t ny, const size_t nz,
                      const double dx, const double dy, const double dz,
                      const size_t resx = 1, const size_t resy = 1, const size_t resz = 1,
                      const Point3D p0 = Point3D());

        CartesianGrid(const size_t nx, const size_t ny,
                      const double dx, const double dy,
                      const size_t resx = 1, const size_t resy = 1,
                      const Point2D p0 = Point2D());

        // Interface

        virtual size_t numberOfCells() const;

        virtual size_t numberOfFaces() const;

        virtual std::vector<size_t> neighbors(const size_t id) const;

        // Functions

        size_t nx() const;

        size_t ny() const;

        size_t nz() const;

        bool is2d() const;

        double dx() const;

        double dy() const;

        double dz() const;

        size_t resx() const;

        size_t resy() const;

        size_t resz() const;

        size_t idCell(const Point3D p) const;

        bool isInside(const Point3D p) const;

        double volumeCell() const;

        double volumeGrid() const;

        virtual Point3D centerOfCell(const size_t id) const;

        Point3D centerOfCell(const size_t idx, const size_t idy, const size_t idz) const;

        size_t idNeighbor(const size_t id, const Direction dir) const;

        std::array<size_t, 3> splitId(const size_t id) const;

        size_t mergeIds(const size_t idx, const size_t idy, const size_t idz=0) const;

    private:

        size_t _nx, _ny, _nz;
        double _dx, _dy, _dz;
        size_t _resx, _resy, _resz;
        Point3D _p0;
        bool _is2d;
    };

}


#endif //LMA_CARTESIANGRID_H
