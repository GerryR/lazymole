/**
* @file CartesianGrid.cpp
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

#include <sstream>
#include "CartesianGrid.h"

namespace mla
{

    CartesianGrid::CartesianGrid(const size_t nx, const size_t ny, const size_t nz,
                                 const double dx, const double dy, const double dz,
                                 const size_t resx, const size_t resy, const size_t resz,
                                 const Point3D p0)
            : _nx(resx*nx), _ny(resy*ny), _nz(resz*nz),
              _dx(dx/resx), _dy(dy/resy), _dz(dz/resz), _p0(p0), _is2d(false),
              _resx(resx), _resy(resy), _resz(resz)
    { }

    CartesianGrid::CartesianGrid(const size_t nx, const size_t ny,
                                 const double dx, const double dy,
                                 const size_t resx, const size_t resy,
                                 const Point2D p0)
            : _nx(resx*nx), _ny(resy*ny), _nz(1),
              _dx(dx/resx), _dy(dy/resy), _dz(1.0), _p0(p0), _is2d(true),
              _resx(resx), _resy(resy), _resz(1)
    { }

    size_t CartesianGrid::numberOfCells() const
    {
        return _nx*_ny*_nz;
    }

    size_t CartesianGrid::numberOfFaces() const
    {
        return _nx*_ny*_nz + _nx*_ny + _ny*_nz + _nx*_nz;
    }

    std::vector<size_t> CartesianGrid::neighbors(const size_t id) const
    {
        std::vector<size_t> cells;
        auto ids = this->splitId(id);
        for (int s0 = -1; s0 <= 1; s0++)
            for (int s1 = -1; s1 <= 1; s1++)
                for (int s2 = -1; s2 <= 1; s2++) {
                    if (ids[0]+s0 >= 0 && ids[0]+s0 < _nx &&
                        ids[1]+s1 >= 0 && ids[1]+s1 < _ny &&
                        ids[2]+s2 >= 0 && ids[2]+s2 < _nz) {
                        if (!(s0==0 && s1==0 && s2==0)) {
                            cells.push_back(this->mergeIds(ids[0]+s0, ids[1]+s1, ids[2]+s2));
                        }
                    }
                }
        return cells;
    }

    size_t CartesianGrid::nx() const
    {
        return _nx;
    }

    size_t CartesianGrid::ny() const
    {
        return _ny;
    }

    size_t CartesianGrid::nz() const
    {
        return _nz;
    }

    bool CartesianGrid::is2d() const
    {
        return _is2d;
    }

    double CartesianGrid::dx() const
    {
        return _dx;
    }

    double CartesianGrid::dy() const
    {
        return _dy;
    }

    double CartesianGrid::dz() const
    {
        return _dz;
    }

    size_t CartesianGrid::resx() const
    {
        return _resx;
    }

    size_t CartesianGrid::resy() const
    {
        return _resy;
    }

    size_t CartesianGrid::resz() const
    {
        return _resz;
    }

    size_t CartesianGrid::idCell(const Point3D p) const
    {
        int idx = static_cast<int>((p.get(0) - _p0.get(0)) / _dx);
        int idy = static_cast<int>((p.get(1) - _p0.get(1)) / _dy);
        int idz = static_cast<int>((p.get(2) - _p0.get(2)) / _dz);
        // Handle a point exactly on a boundary
        if (p.get(0)-_p0.get(0) == _dx*_nx)
            idx--;
        if (p.get(1)-_p0.get(1) == _dy*_ny)
            idy--;
        if (p.get(2)-_p0.get(2) == _dz*_nz)
            idz--;

        if (idx >= 0 && idx < _nx && idy >= 0 && idy < _ny && idz >= 0 && idz < _nz)
            return idz * _ny * _nx + idy * _nx + idx;
        else
        {
            std::stringstream s;
            s << "Point " << p << " is outside the domain." << std::endl;
            throw std::invalid_argument(s.str());
        }
    }


    bool CartesianGrid::isInside(const Point3D p) const
    {
        try
        {
            idCell(p);
        }
        catch (std::invalid_argument e)
        {
            return false;
        }
        return true;
    }

    double CartesianGrid::volumeCell() const
    {
        return _dx*_dy*_dz;
    }


    double CartesianGrid::volumeGrid() const
    {
        return volumeCell() * numberOfCells();
    }

    Point3D CartesianGrid::centerOfCell(const size_t id) const
    {
        auto ids = splitId(id);
        return centerOfCell(ids[0], ids[1], ids[2]);
    }

    Point3D CartesianGrid::centerOfCell(const size_t idx, const size_t idy, const size_t idz) const
    {
        assert(idx < _nx && idy < _ny && idz < _nz);
        return _p0 + Point3D(idx*_dx, idy*_dy, idz*_dz) + Point3D(.5*_dx, .5*_dy, .5*_dz);
    }

    size_t CartesianGrid::idNeighbor(const size_t id, const Direction dir) const
    {
        auto ids = splitId(id);
        switch (dir)
        {
            case XP:
                return (ids[0] != _nx-1) ? mergeIds(ids[0]+1, ids[1], ids[2]) : numberOfCells();
            case XM:
                return (ids[0] != 0    ) ? mergeIds(ids[0]-1, ids[1], ids[2]) : numberOfCells();
            case YP:
                return (ids[1] != _ny-1) ? mergeIds(ids[0], ids[1]+1, ids[2]) : numberOfCells();
            case YM:
                return (ids[1] != 0    ) ? mergeIds(ids[0], ids[1]-1, ids[2]) : numberOfCells();
            case ZP:
                return (ids[2] != _nz-1) ? mergeIds(ids[0], ids[1], ids[2]+1) : numberOfCells();
            case ZM:
                return (ids[2] != 0    ) ? mergeIds(ids[0], ids[1], ids[2]-1) : numberOfCells();
        }
    }

    std::array<size_t, 3> CartesianGrid::splitId(const size_t id) const
    {
        assert(id < numberOfCells());
        size_t cid = id;
        size_t idz = cid / (_ny*_nx);
        cid = cid % (_ny*_nx);
        size_t idy = cid / _nx;
        cid = cid % _nx;
        size_t idx = cid;
        std::array<size_t, 3> out = {{idx, idy, idz}};
        return out;
    }

    size_t CartesianGrid::mergeIds(const size_t idx, const size_t idy, const size_t idz) const
    {
        assert(idx < _nx && idy < _ny && idz < _nz);
        return idz*_ny*_nx + idy*_nx + idx;
    }

}
