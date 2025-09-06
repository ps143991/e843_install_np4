There are several fitting macros in this directory.
The main one I’ve been using is responsefitlandau.C, along with responsefitsimple2.h. In this macro, you define the angle range for the fit. It also extracts the response functions from 2D spectra, meaning the angle cut is applied to the response functions as well. This is important because the resolution of the response functions varies with angle.

As you can see in the macro, the outputs are saved in four different folders: rootout, pngout, pdf, and csv.
The CSV files contain numerical outputs, such as integral values.

In the header file, the only variables fitted are the amplitude and the mean shift, since the peak shape is predefined by the simulation. I normalize the amplitude by dividing it by the number of events in the simulation.
For example, if the simulation was run with 10,000 beam particles, I divide the amplitude by 10,000 to normalize it.

You can find the final figures in the corresponding output file.

Whenever you see a filename containing c, it refers to carbon.
For instance, response_fitcarbon.C and the corresponding .h file are intended to use carbon background data as a background function. However, the data requires significant smoothing and doesn't fit well yet — though the carbon background should still be included in the final analysis.

The file Ex_nobackground.odp contains the final figures that I sent to the theoretician.
You can also find the angular distribution graphs in angulardistribution.ods inside the csv folder