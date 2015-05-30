# Script Interface #

## Config Section ##
Most items registered before the config script is called are in the Engine:: namespace and are designed to be removed after configuration has completed. Other items can be registered before the config script, and then carried forward for use during gameplay scripts.

Some examples of what could be registered for the config scripts include :
  * Listing and loading modules
  * Setting the per-user data path
  * Setting the log file name
  * Loading per-user config {scripts|files - pick one}
  * Setting asset paths - including where the game scripts are located
  * Setting initial game script name and the function to call
  * Detecting operating system and GUI

### Global Functions ###
  * void Engine::LOG(Engine::LOG\_PRIORITY, string message); // Send a message to the log file
  * void Engine::SetLogFile(string name, string file\_extension); // Sets the file name (including file extension) of the log.

### Enums ###
  * **Engine::SYSTEM\_DIRS**
    * USER  - The user’s home folder
    * DOCUMENTS - The user’s documents folder
    * PICTURES  - The user’s pictures folder
    * MUSIC  - The user’s music folder
    * VIDEO  - The user’s video folder
    * DESKTOP  - The user’s desktop folder
  * **Engine::MODULE\_STATUS**
    * NOT\_FOUND  - Library does not exist.
    * LOAD\_ERROR  - Library loading error.
    * START\_ERROR  - Module starting error.
    * EXISTS  - Library exists and is available for loading.
    * LOADED  - Library loaded and module started.
  * **Engine::LOG\_PRIORITY**
    * INFO  - General information.
    * FLOW  - Program flow for debugging purposes.
    * WARN  - General warnings, usually about data that is not correctly formed, but wasn't a showstopper.
    * CONFIG  - Errors with the configuration - usually showstoppers, but not always; the code might be able to continue using a default value.
    * ERR  - Errors in operation - usually showstoppers, but not always.
    * MISSRESS  - A requested resource doesn't exist: missing files, etc.  A default may be provided.
    * DEPRECATE  - Deprecation warning.

### Global Objects ###
  * **ModuleLoader**
    * Methods
      * Engine::MODULE\_STATUS LoadModule(string);
      * Engine::MODULE\_STATUS GetModuleStatus(string);
  * **OS**
    * Methods
      * string GetPath(Engine::SYSTEM\_DIRS);
      * void ShowInfo(string, string);
      * void ShowWarning(string, string);
      * void ShowError(string, string);
  * **Engine**
    * Methods
      * void Shutdown(); - Tells the engine to begin shutdown.
      * void SetUserDataFolder(); - Sets the current data folder.
      * void SetGameScript(); - Sets the main gameplay script.

## Gameplay Section ##
### Functions ###

### Enums ###