/**
* @file main.cpp
* @brief Entry point for lazymole
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

#include <iostream>
#include <stdexcept>
#include <Point.h>
#include <Vector.h>
#include <CellField.h>
#include <LazyMole.h>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <Input.h>

class Timer
{
public:
    Timer() : beg_(clock_::now()) {}
    void reset() { beg_ = clock_::now(); }
    double elapsed() const {
        return std::chrono::duration_cast<second_>
                (clock_::now() - beg_).count(); }

private:
    typedef std::chrono::high_resolution_clock clock_;
    typedef std::chrono::duration<double, std::ratio<1> > second_;
    std::chrono::time_point<clock_> beg_;
};

std::vector<size_t> loadIds(std::string fileName)
{
    std::ifstream inStream;
    inStream.open(fileName, std::ifstream::in);
    if (!inStream)
    {
        throw std::runtime_error("ERROR: cannot find the file " + fileName);
    }

    std::vector<size_t> ids;
    size_t id;
    while (inStream >> id)
    {
        ids.push_back(id);
    }
    inStream.close();

    return ids;
}

void run(int argc, char** argv)
{
    Timer timer;
    const double tStart = timer.elapsed();

    std::cout << "*********************************************************" << std::endl;
    std::cout << "*-------------------------------------------------------*" << std::endl;
    std::cout << "*------------------- THE LAZY MOLE 3D ------------------*" << std::endl;
    std::cout << "*-------------------------------------------------------*" << std::endl;
    std::cout << "*-------- Computing minimum hydraulic resistance -------*" << std::endl;
    std::cout << "*--------------- of heterogeneous field ----------------*" << std::endl;
    std::cout << "*-------------------------------------------------------*" << std::endl;
    std::cout << "*------ Calogero B. Rizzo, Felipe P.J. de Barros -------*" << std::endl;
    std::cout << "*-------------------------------------------------------*" << std::endl;
    std::cout << "*------------- If you find a bug report to -------------*" << std::endl;
    std::cout << "*----- Calogero B. Rizzo (gerry.rizzo89@gmail.com) -----*" << std::endl;
    std::cout << "*-------------------------------------------------------*" << std::endl;
    std::cout << "*********************************************************" << std::endl;
    std::cout << std::endl;

    std::string configPath;
    if (argc == 1)
    {
        configPath = "";
    }
    else if (argc == 2)
    {
        configPath = std::string(argv[1]);

        if (configPath.back() != '/' && configPath.back() != '\\')
        {
            configPath.push_back('/');
        }
    }
    else
    {
        throw std::runtime_error("ERROR: too many arguments (use 'lazyMole /path/to/config/')");
    }

    std::string configName = configPath + "config.yaml";

    std::cout << "Looking for configuration file '" << configName << "'... " << std::flush;
    lma::Input config(configName);
    std::cout << "OK!" << std::endl;

    // Load parameters
    size_t nx = config.nx();
    size_t ny = config.ny();
    size_t nz = config.nz();

    double dx = config.dx();
    double dy = config.dy();
    double dz = config.dz();

    size_t refx = config.refx();
    size_t refy = config.refy();
    size_t refz = config.refz();

    // Define grid
    std::cout << "Preparing grid... " << std::flush;
    auto grid = new mla::CartesianGrid(nx, ny, nz, dx, dy, dz, refx, refy, refz);
    std::cout << "OK!" << std::endl;

    // Load source ids
    std::cout << "Loading source ids from '" << configPath + config.source() << "'... " << std::flush;
    auto ids = loadIds(configPath + config.source());
    std::cout << "OK!" << std::endl;

    // Load target ids
    std::cout << "Loading target ids from '" << configPath + config.target() << "'... " << std::flush;
    auto idsTarget = loadIds(configPath + config.target());
    std::cout << "OK!" << std::endl;

    // Define conductivity field
    std::cout << "Preparing field... " << std::flush;
    mla::ConductivityField conductivity(grid);
    std::cout << "OK!" << std::endl;

    // Open conductivity file
    std::cout << "Loading field from '" << configPath + config.field() << "'... " << std::flush;
    std::ifstream inStream;
    inStream.open(configPath + config.field(), std::ifstream::in);
    if (!inStream)
    {
        throw std::runtime_error("ERROR: cannot find the field file " + config.field());
    }
    size_t skip = config.fieldSkip();
    bool log = config.fieldLog();

    // Load conductivity
    conductivity.import(inStream, skip, 1.0, log);
    inStream.close();
    std::cout << "OK!" << std::endl;

    // Define Lazy Mole object
    std::cout << "Running algorithm... " << std::flush;
    mla::LazyMole lazyMole(grid, conductivity, ids);

    // Run Lazy Mole
    const double t1 = timer.elapsed();
    auto smallestRes = lazyMole.run();
    const double t2 = timer.elapsed();
    std::cout << "OK!" << std::endl;

    // Output
    std::cout << "Exporting resistance map to '" << configPath + config.outputRes() << "'... " << std::flush;
    smallestRes->exportToFile(configPath + config.outputRes());
    std::cout << "OK!" << std::endl;

    double minRes = 1e20;
    size_t minId = grid->numberOfCells();
    for (size_t i = 0; i < idsTarget.size(); i++)
    {
        if (smallestRes->get(idsTarget[i]) < minRes)
        {
            minId  = idsTarget[i];
            minRes = smallestRes->get(idsTarget[i]);
        }
    }
    std::cout << "Minimum Hydraulic Resistance = " << minRes << std::endl;
    std::cout << "Target ID = " << minId << std::endl;

    std::cout << "Exporting least resistance path to '" << configPath + config.outputPath() << "'... " << std::flush;
    lazyMole.exportPath(minId, configPath + config.outputPath());
    std::cout << "OK!" << std::endl;

    // Free space
    delete grid;

    const double tEnd = timer.elapsed();
    std::cout << std::endl;
    std::cout << "Time elapsed = " << tEnd - tStart << "s (LM time = " << t2 - t1 << "s)" << std::endl;
    std::cout << std::endl;
    //std::cout << "Press Enter to continue... ";
    //std::cin.ignore();
}

int main(int argc, char** argv)
{
    try
    {
        run(argc, argv);
        return EXIT_SUCCESS;
    }
    catch (const std::exception& e)
    {
        std::cerr << std::endl << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}
