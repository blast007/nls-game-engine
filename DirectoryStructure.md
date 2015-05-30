<a href='Hidden comment: The summary is a short, single-sentence description of this page.'></a>

<a href='Hidden comment: 
The wiki syntax is pretty straightforward for anyone familiar with wiki markup.
Take a look at http://code.google.com/p/support/wiki/WikiSyntax for more detail.
'></a>

The structure of the folders shall be as follows inside the repository system:
| `bin` | The destination for all compiled executables, dynamically linked libraries, and the configuration script. |
|:------|:----------------------------------------------------------------------------------------------------------|
| `cmake` | Custom CMake modules are located in here.                                                                 |
| `docs` | License files as well as where the documentation created by Doxygen will be placed.                       |
| `include` | All library header files are located here once downloaded by CMake.                                       |
| `lib` | Statically linked libraries are placed here by the build system.                                          |
| `lib_src` | Source code for the libraries.                                                                            |
| `libraries` | Where the library source is downloaded to.                                                                |
| `src` | Used for source code files (such as .h and .cpp files).                                                   |