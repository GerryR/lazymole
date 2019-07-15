# Lazy Mole

Compute the minimum hydraulic resistance (MHR) and least resistance path (LRP)
in heterogeneous porous media.

## How to run Lazy Mole
1. Download the `lazyMole.exe` executable from [here](https://github.com/GerryR/lazymole/releases)
1. Open the Windows Command Prompt
2. Change directory to the folder containing the `lazyMole.exe` file
3. Run the executable: `lazyMole.exe path/to/root`

You can also add the folder containing the `lazyMole.exe` file to the
`PATH` variable:
1. Go to System (Control Panel)
2. Advanced System Settings
3. Environment Variables
Now you can call `lazyMole.exe` from all the folders.

## Input
Create a folder with the input files.
The `Examples` folder contains a few examples. A possible folder structure is:

```
root/
  |  config.yaml
  |  field.dat
  |  source.dat
  |  target.dat
```

The `config.yaml` file is the configuration file (YAML format).
Details about this file are included in the example folders.

The `field.dat` contains the values of the conductivity field
on a Cartesian grid (2D or 3D). The values are sorted according
to their x, y, and z coordinates, in this order.

The `source.dat` file contains a list of unique indexes for the source cells.

the `target.dat` file contains a list of unique indexes for the target cells.

For example, using a grid with `Nx*Ny*Nz` cells, the unique index `id` of a cell
with directional indexes (`idx`, `idy`, `idz`) can be found as:
```
id = idz*Nx*Ny + idy*Nx + idx
```

## Output
The executable generates the map of the minimum hydraulic resistance
between the cells specified in `source.dat` and all the cells of the domain.
Moreover, there will be a file containing the least resistance path
from the cells specified in `source.dat` and the cells specified in `target.dat`.

## Citations
Rizzo, Calogero B., and Felipe PJ de Barros. [Minimum hydraulic resistance and least resistance path in heterogeneous porous media.](https://doi.org/10.1002/2017WR020418) Water Resources Research 53.10 (2017): 8596-8613.

Rizzo, Calogero B., and Felipe PJ de Barros. [Minimum Hydraulic Resistance Uncertainty and the Development of a Connectivity‐Based Iterative Sampling Strategy.](https://doi.org/10.1029/2019WR025269) Water Resources Research.
