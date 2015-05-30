<a href='Hidden comment: The summary is a short, single-sentence description of this page.'></a>

<a href='Hidden comment: 
The wiki syntax is pretty straightforward for anyone familiar with wiki markup.
Take a look at http://code.google.com/p/support/wiki/WikiSyntax for more detail.
'></a>

Due to the extreme difference in our engine design compared to traditional monolithic design, we have had to adopt some specific naming conventions to keep things organised and understandable.

# Module #
Our engine is divided up into separate modules which are loading dynamically at run-time through the OS. Through the use of a common interface [ModuleInterface](http://nls.rwcproductions.com/class_module_interface.html) we can interact with each module using a generic set of functions.

# Component #
Each module maintains a list of components that are used in the module to do various things (e.g. render a cube, play a sound, etc).  Components are linked through, most commonly, an entity.

# Entity #
Entities are anything that exists in the game engine. They are the most basic type of game object, and therefore contain the most common data any object can have: location, rotation, scale, and a name to represent it within each module.

# Factory #
A factory is simple any class, method, or function that returns a new instance, with some initialization, of the mentioned object.

# Manager #
A manager can have broad meaning, and usually not a great design decision because of that. So what we have is a set of rules that define what it is and what it can't be.

**Can**
  * Load and unload
  * Start and stop
  * Maintain a list of loaded objects
  * Loops over a given set of objects to call a member method

**Can't**
  * Create
  * Process anything about the objects