# Computational-Fluid-Dynamics (CFD)
CFD Solver for Laval Nozzle and Shock Tube (one-dimensional Riemann problem)

This project is based on the mathematical fundamentals of the CFD course conducted by the institute of Thermal Turbomachinery and Machine Dynamics at Graz University of Technology. 
[Course Link](https://online.tugraz.at/tug_online/wbLv.wbShowLVDetail?pStpSpNr=207174)

## Implemented 1D CFD Solvers C++

* Central
* LaxWendroff
* MacCormack
* ROE 


## UserInterface in C#/.NET WPF
The C++ CFD solver is wrapped and provides data via a C++/CLI interface to the UI. 
--> C++/CLI extension of Visual Studio must be installed to compile this project.

![Screen Cast](https://github.com/JTiefnig/Computational-Fluid-Dynamics/blob/master/Media/ScreenCastCFD.gif)


### Exercise 1 [Laval Nozzle](https://en.wikipedia.org/wiki/De_Laval_nozzle)
![Laval Nozzle Screen Shot](https://github.com/JTiefnig/Computational-Fluid-Dynamics/blob/master/Media/ScreenShotLaval.jpg?raw=true)

### Exercise 2 [Shock Tube](https://en.wikipedia.org/wiki/Sod_shock_tube)
![Shock Tube Screen Shot](https://github.com/JTiefnig/Computational-Fluid-Dynamics/blob/master/Media/ScreenShotShockTube.jpg?raw=true)

## License
[MIT](https://choosealicense.com/licenses/mit/)
