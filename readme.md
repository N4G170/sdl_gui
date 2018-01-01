# sdl_gui project

I uploaded the code to github for convenience, but if anyone finds it useful, even as a bad example, good.
NOTE:github is marking the project as C, but it is C++ only.

When creating my projects I always had problems with my GUIs, when I started a new project, I could never, easily, move the old gui to it so I ended up making new ones (before you ask I did not want to use an external gui, I wanted to keep dependencies to the minimum, in this case SDL2 only).
As I like to learn how things work, I started making my own little gui lib inside my [generic project](https://github.com/N4G170/generic).

Through time, the gui suffered 2 complete redesigns, the first moved from the original mishmash of classes, files, rules, and more, into a cleaner hierarchical structure,
but after a while I noticed the errors of my approach, so ... redesign. In the second and current redesign, the hierarchical structure was simplified and managers were added.
The current version is easy to use and somewhat clean, it has its problems, like the amount of work needed to add a new element to the gui, too much.

When creating a demo showcase for the elements in gui, inside the generic project, I decided to remove the gui from it and create a new separated project for it, a so was born 'sdl_gui'.

After porting the gui to this project, I started making a new demo showcase.

When making the demo, I noticed that some actions could be simplified or eliminated altogether, then I started thinking about solutions for the new situation.
And so my ECS like system was born, in the [generic project](https://github.com/N4G170/generic) (I jump between my project like crazy).
The new ECS system is similar(on a smaller scale) to how unity3d objects and components are created and managed.

I really enjoy working on this project, it made me look differently to GUIs, they are more complicated than I thought. My favourite part was working with utf8 and creating the font sheet texture, for easier use of text (as sdl ttf rendering in not the quickest).

###### Existing elements.
Currently sdl_gui has the following elements (present in demo):
- BaseButton: Basic button that can use colour or images as transition (active/inactive/over/pressed)
- BasicFrameAnimation: basic animation that uses images (one per frame, or one divided in areas)
- Colliders: mainly used with the buttons to check is the mouse is over it or not
- Button: Specialization of the BaseButton but can have a label(this one show the over-complication of adding a new element to a new one)
- Camera: gui elements outside the camera area are not rendered
- Checkbox: Can be simple or radio, and can be grouped (radio functionality only works in groups)
- sliders: both horizontal and vertical sliders
- Image: basic image to be rendered, used by the other elements, but not in the cleanest way
- Label: The one I enjoyed making the most (mainly because of the issues with the font), it supports some formatting like:
  - Bold: \<b> \</b>
  - Italic: \<i> \</i>
  - New line: \\n
  - Tab: \\t
- Layout: basic structure used to arrange(grid, line, column,...) all elements that it is the parent of.
- ProgressBar: like a loading bar, can be horizontal or vertical
- ScrollBox: this one is a bit incomplete, it basically is a list of labels with a scrollbar
- SelectableButton: similar to a checkbox but is a basic button
- Textbox: Basic input box, very limited (I stop working on it when I started working on the ECS, as to not waste time)
- Tooltip: Mouse over and hold over the tooltip detect area to show tooltip
- ScrollBar: Vertical only

All the previous elements function on top of:
- Resource manager: manages images and fonts
- Gui manager: holds the elements and calls theirs main functions, like Render, Input, Logic
- MouseInteraction: mainly on the buttons, checks if the mouse is over the element or not and if there were clicks, then calls the appropriate callback
- Transform: each element has one, it holds the position, size and parent-children relations of the element. (the new transform from the ECS is better than this one)  

Some of this elements have problems that will be solved when I move to my ECS system.

## Running the program
If you are using one of the release builds, double click the .exe on windows or run in a terminal on linux.
The provided build represents a demo showcasing the elements present on the project. Use the button on the top to change panels and try the elements.

## Building the program

If you are building this project I recommend using a linux distro, as it is so easier.

### If on Linux

First install all the dependencies:
- SDL2
- SDL2_image
- SDL2_ttf
- cmake

After that, open terminal window at the base of the project (the folder with this file) and run the commands:

```
- mkdir build
- cd build
- cmake ..
- make -j
```
If no errors appear, the build is complete and you can run the program with the command ./demo_sdl_gui
I did not configure any install instructions, so if you want to move the build, copy the folder 'data' and the files 'demo_sdl_gui' and libsdl_gui.so from the build folder.
As you can see, the linux build creates a separated .so lib with only the gui files, to improve portability.
Sadly on windows I was unable to do so, as my code is not ready for it, when implementing the ECS I'll take extra care to make the code ready for it.

NOTE: As cmake creates the executable as a shared object (I have yet to find why), you have to run the program through the terminal, rather than double click

### On windows

As my SDL2 cmake modules are not working properly on windows, the 'vs' folder contains a Visual Studio solution with the project and all the needed dependencies linked, so it should be a matter of opening the solution on a VS 15+ and build it.

NOTE: the project only builds when targeting x64.

If the solution does not work, or you use another build system, you have to add the files by hand to you project and take care of the dependencies.
For ease of building, you can find the needed dependencies inside the vs folder.
NOTE: I do not own any of the code from the dependencies. Their license allows them to be freely used and shared.

To run the program, you need the data folder next to the .exe as well as all of [SDL2](https://www.libsdl.org/), [SDL2_image](https://www.libsdl.org/projects/SDL_image/), [SDL2_ttf](https://www.libsdl.org/projects/SDL_ttf/) dlls (you can find them in my release or download them from the linked SDL website).

NOTE: the windows builds does not make sdl_gui as a separated lib. As I never had to build dlls, I was not aware of the need to export and import functions with "declspec", and as I will change the entire structure of this project with the ECS system and to add the export/import dll would be too messy, I decided to prepare add it during the convertion to the ECS.

## TODO

As I said before, I'll move the project to the new ECS system and make it ready for correct .lib/.dll creation on windows.
After the new system in implemented, I'll correct and finish the existing elements.

If you find any bug of error, let me know.
