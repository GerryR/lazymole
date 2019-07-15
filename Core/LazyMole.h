/**
* @file LazyMole.h
* @brief Class implementing the algorithm to compute minimum hydraulic
*        resistance and least resistance path
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

#ifndef LMA_LAZYMOLE_H
#define LMA_LAZYMOLE_H

#include <cstddef>
#include <iostream>
#include <CellField.h>
#include <boost/heap/fibonacci_heap.hpp>
#include <limits>
#include <cmath>

namespace mla {

    class LazyMole {

    private:

        enum Label {
            UNVISITED = 0,
            VISITED,
            SCANNED
        };

        struct CellElement {

            double res;
            size_t cell;

            CellElement(double r, size_t c)
                    : res(r), cell(c) {}

            inline bool operator<(CellElement const & rhs) const { return res < rhs.res; }
        };

        typedef typename boost::heap::fibonacci_heap<CellElement> Heap;
        typedef typename boost::heap::fibonacci_heap<CellElement>::handle_type HandleType;

        Heap heap;

        CellField<Label> status;

        CellField<HandleType> cellElementHandles;

        CellField<size_t> previous;

        CellField<double> smallestRes;

        Grid* gridPtr;

        CellField<double> field;

        bool isReady;

        const double INF = std::numeric_limits<double>::max();

        const size_t EMPTY = std::numeric_limits<size_t>::max();

    public:

        LazyMole(Grid* gridPtr, CellField<double>& field, const std::vector<size_t> cellIds) :
                gridPtr(gridPtr), field(field),
                status(gridPtr, UNVISITED), previous(gridPtr, std::numeric_limits<size_t>::max()),
                smallestRes(gridPtr, std::numeric_limits<double>::max()),
                cellElementHandles(gridPtr, HandleType()) {
            for(auto i = 0; i < cellIds.size(); i++) {
                cellElementHandles[cellIds[i]] = heap.push(CellElement(0., cellIds[i]));
                status[cellIds[i]] = VISITED;
            }
            isReady = false;
        }

        Grid* grid() {
            return gridPtr;
        };

        CellField<double>* const run() {
            while (!heap.empty()) {
                const CellElement e = heap.top();
                heap.pop();

                size_t cCell = e.cell;
                const double cRes = -e.res;

                status[cCell] = SCANNED;
                smallestRes[cCell] = cRes;

                // Loop on neighbors
                auto neighbors = gridPtr->neighbors(cCell);
                for (auto nCell : neighbors) {
                    if (status[nCell] != SCANNED) {
                        const double cnRes = computeResistance(cCell, nCell);
                        const double nRes = cRes + cnRes;
                        if (status[nCell] == UNVISITED) {
                            previous[nCell] = cCell;
                            status[nCell] = VISITED;
                            cellElementHandles[nCell] = heap.push(CellElement(-nRes, nCell));
                        } else /* status[nCell->id()] == VISITED */ {
                            if (nRes <  -(*cellElementHandles[nCell]).res) {
                                previous[nCell] = cCell;
                                heap.increase(cellElementHandles[nCell], CellElement(-nRes, nCell));
                            }
                        }
                    }
                }
            }

            isReady = true;
            return &smallestRes;
        }

        CellField<size_t> path(const size_t cell) {
            CellField<size_t> pathField(grid(), 0);

            if(!isReady)
                return pathField;

            size_t cId = cell;
            bool isTheStart = false;
            while(!isTheStart) {
                pathField.set(cId, 1);
                size_t pCell = previous[cId];
                if(pCell != EMPTY) {
                    cId = pCell;
                } else {
                    isTheStart = true;
                }
            }
            return pathField;
        };

        void exportPath(const size_t cell, const std::string& fileName) const {
            if(!isReady)
                return;

            std::ofstream outStream;
            outStream.open(fileName);
            if (outStream.is_open()) {
                size_t cId = cell;
                bool isTheStart = false;
                while(!isTheStart) {
                    Point3D center = gridPtr->centerOfCell(cId);
                    outStream << center.get(0) << ","
                              << center.get(1) << ","
                              << center.get(2) << std::endl;
                    size_t pCell = previous[cId];
                    if(pCell != EMPTY) {
                        cId = pCell;
                    } else {
                        isTheStart = true;
                    }
                }
            }
            outStream.close();


        }

    private:

        double computeResistance(const size_t cCell, const size_t nCell) const {
            // NOTE: it works only for Cartesian grids, it could be generalized for generic grids
            // using the distance between center of cells and a midpoint (either a corner or center of face)
            double dist = gridPtr->centerOfCell(cCell).distanceFrom(gridPtr->centerOfCell(nCell));

            auto k1 = field.getFromCell(cCell);
            auto r1 = dist/2.0/k1;

            auto k2 = field.getFromCell(nCell);
            auto r2 = dist/2.0/k2;

            return r1 + r2;
        }

    };
}


#endif //LMA_LAZYMOLE_H
