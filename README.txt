MESH IMPORTS =============================================================

Each mesh can only contain a signle texture, the texture must be inside the models folder
What models to import are defined in "renderer.cpp , initGeometry()"

MODELS MUST USE .OBJ WITH THE FOLLOWING SETTINGS (BLENDER)

PATH MODE = ABSOLUTE

EXPORT SETTINGS =
EXPORT UVS,
EXPORT MATERIALS,
EXPORT TRIANGULATED MESH


FILE DESCRIPTIONS ================================================================

WinEntry.cpp

winEntry controls the start and gameloop of the program


Game.cpp
controls all in application logic, such as camera movement.
This also contains the gameloop where renderPass() is called at the end
this also calls all init methods

windowManager.cpp
this controls all window related tasks and handeling. you dont need to think much about this, it just creates a window which is drawn to

renderer.cpp
contains all model rendering related tasks, this includes loading .obj files and textures.

