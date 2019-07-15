//
// Created by gerry on 07/10/15.
//

#ifndef LMA_CELLFIELD_H
#define LMA_CELLFIELD_H

#include <cstddef>
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <assert.h>
#include <math.h>
#include <boost/math/special_functions/erf.hpp>
#include <CartesianGrid.h>
#include "Field.h"

namespace mla {

    template<typename C>
    class CellField : public Field<C> {

    public:

        CellField(Grid *grid, C value = 0) : Field<C>(grid, grid->numberOfCells(), value) {};

        virtual C getFromCell(const size_t cell) const {
            assert(cell < this->gridPtr->numberOfCells());
            return this->values[cell];
        };

        void exportToFile(const std::string fileName) const {
            std::ofstream outStream;
            outStream.open(fileName);

            if (!outStream) {
                throw std::runtime_error("ERROR: ERROR: cannot open the file" + fileName);
            }

            for (auto v : this->values) {
                outStream << v << std::endl;
            }
            outStream.close();
        };

    };


    class ConductivityField : public CellField<double> {

    public:

        ConductivityField(CartesianGrid* grid, double value = 0)
                : CellField<double>(grid, value) {};

        void import(std::istream& inStream, const size_t nSkip = 0, double sigma2 = 1.0, bool isLog = true,
                    const int connected = 0) { // connected=1 -> connected | connected=-1 disconnected
            char line[256];
            for (size_t i = 0; i < nSkip; i++) {
                inStream.getline(line, 256);
            }

            if (!isLog && (connected == 1 || connected == -1)) {
                throw std::runtime_error("ERROR: cannot use log with connected fields");
            }

            CartesianGrid* cGrid = (CartesianGrid*) gridPtr;

            for (size_t k = 0; k < cGrid->nz()/cGrid->resz(); k++)
                for (size_t j = 0; j < cGrid->ny()/cGrid->resy(); j++)
                    for (size_t i = 0; i < cGrid->nx()/cGrid->resx(); i++) {
                        double val;
                        inStream >> val;

                        if (inStream.eof()) {
                            std::cerr << "WARNING: not enough values in iStream for the conductivity" << std::endl;
                            break;
                        }

                        if (connected == 1 || connected == -1) {
                            //std::cout << "Note: using Zinn transformation, assuming the field is initially normal"
                            //<< std::endl;
                            try {
                                // Needed because erf_inv(-1) does not converge
                                double t = std::abs(val);
                                t = t < 0.000001 ? 0.000001 : t;
                                val = -connected * std::sqrt(2.0) *
                                      boost::math::erf_inv(2.0 * std::erf(t / std::sqrt(2.0)) - 1.0);
                            } catch (...) {
                                std::cerr << "ERROR in erf_inv function!" << std::endl;
                            }
                        }

                        val *= std::sqrt(sigma2);

                        for (size_t x = cGrid->resx()*i; x < cGrid->resx()*(i+1); x++)
                            for (size_t y = cGrid->resy()*j; y < cGrid->resy()*(j+1); y++)
                                for (size_t z = cGrid->resz()*k; z < cGrid->resz()*(k+1); z++) {
                                    size_t id = cGrid->mergeIds(x,y,z);
                                    this->values[id] = isLog ? std::exp(val) : val;
                                }
                    }

        }

    };

}


#endif //LMA_CELLFIELD_H
