#A tool to convert a single file VRML models to standalone OpenGL executables#


Convert a VRML file to data file containing vertices:

`./3dpacker input/combo.wrl output/data.h`

Compile the executable:

`cd output`

`make`

Run the executable:

`./glmodel`



The OpenGL part is based on https://learnopengl.com/
