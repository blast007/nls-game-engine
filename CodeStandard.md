<a href='Hidden comment: The summary is a short, single-sentence description of this page.'></a>

<a href='Hidden comment: 
The wiki syntax is pretty straightforward for anyone familiar with wiki markup.
Take a look at http://code.google.com/p/support/wiki/WikiSyntax for more detail.
'></a>

Many projects will start without something that documents how the programmers involved will structure their code.  These projects will often dissolve into a mess of inconsistent and buggy code, like the result of a bunch of monkeys trying to build a banana tree.  Projects that start with a consistent set of structural guidelines, aka a “coding standard”, will often be more productive as each member will be able to easily follow where multiple people have coded before.

(Gratuitously stolen from the [Second Life Coding Standard](http://wiki.secondlife.com/wiki/Coding_Standard) and then modified.)

# Unicode #
Use UTF-8 for all source files, string serialization, and communication between processes. Do not use UTF-16 except when interfacing with Win32.

# Line Endings #
All text files **must** use UNIX (linefeed only) line endings when committed.

Exceptions are allowed only if the file in question:
  * Is used, or relevant, only on Windows
  * Requires DOS style (carriage-return linefeed) line endings for correctness.

Files that have DOS line endings must either:
  * have a suffix that is one of:
  * .bat
  * .sln
  * .vcxproj
  * **or** be located somewhere under a directory named _windows_ or _vstool_

# Comments #
When commenting, address the "why" and not the "what" unless the what is difficult to see. If what the code does requires commenting, also include why it has to be done that way so maintainers can follow your reasoning. Use complete sentences and correct spelling.
Prefer the use of [doxygen](http://www.doxygen.org/) compatible comment markup. (See [Using Doxygen](http://wiki.secondlife.com/wiki/Using_Doxygen).)

Use C++ style comments (the double slash) for single line comments and member variable descriptions. Use classic C style comments `/* */` for temporarily commenting out large sections of code.  Be sure that such commented out code is removed before commit - see the **[Dead code](#Dead_code.md)** section.

## Special Comment Tokens ##
Sometimes you know something is broken or a bad idea but need to do it anyway - please follow the special comment token guidelines to not confuse the next engineer. Append your name or initials and the date of the comment when using these special comment tokens.

| `*FIX:` | The associated code is actually broken and has known failure cases. All instances of these should be fixed before shipping. Do not use this tag for hacks or suggested development. Do not simply tag the code with the token alone or simply provide instructions since then it is unclear if that is how it is broken. For example: "`// *FIX: invalidate stored textures when number of faces change`" — is really unclear if it is an instruction or a buggy side effect of the code segment. |
|:--------|:----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `*HACK:` | The associated code relies on external information or processes outside the normal control flow for the function to work.                                                                                                                                                                                                                                                                                                                                                                           |
| `*TODO:` | The associated code should be optimized, clarified, or made more reliable. Provide instructions along with why it is probably a good idea to execute on the comment.                                                                                                                                                                                                                                                                                                                                |
| `*NOTE:` | This denotes that there is something tricky or special about the associated code that needs an explanation above and beyond answering the typical 'what' and 'why.'                                                                                                                                                                                                                                                                                                                                 |

### Examples ###
```c++

// *FIX: This will fail if the packets arrive out of order.
```
```c++

// *HACK: Filter out some extra information from this
// packet and put it into the status bar since it is more
// up to date than the last balance update.
```
```c++

// *TODO: This call does not always need to be made
// and is computationally expensive. Write a test to see
// if it must be made during this iteration.
```
```c++

// *NOTE: The tile is actually 257 pixels wide, so
// we have to fudge the texture coordinates a bit for this
// to look right.
```

# Dead code #
Do not leave commented-out code in any commits that you make. Keep the streets clean and delete that code instead.

No zombies in the code: block-commented out code is especially problematic because it is not distinguishable from live code in a grep. Our version control system is good enough that any code that was checked in is able to resurrected even after deletion.

# File names #
File names should be significant and of reasonable length, with no spaces, and the file should be written in UpperCamelCase with a lower case file extension.

File names must end with the appropriate suffix:
  * **.cpp** for ANSI C++ code
  * **.h** for included declarations
  * **.inl** for large chunks of inlined C++ code. Avoid this.
  * **.asm** for assembly files. _Do not do this without a note from some deity forgiving your transgressions._

# Source Files #
All source code files should begin at minimum with the header shown below:

```c++

/**
* \file file base name
* \author Firstname Lastname
* \date Optional iso8601 creation date - eg. 2012-04-01
* \brief A brief description of the purpose of the file
*
* A full description of the purpose of the file.
*
*/
```
For more detailed information see the [Documentation Standards](DocumentationStandards.md).

See the [Doxygen Special Commands](http://www.stack.nl/~dimitri/doxygen/commands.html) list for more information.

All source files should end in a blank line.

# Naming Conventions #
The following name conventions apply to non-iterators:
  * Names should include one or more English words that are relevant to the entity being named. Try to use non-programming words, like EmployeeList rather than LinkedListOfStrings
  * Variables are in MKS (meters, kilograms, seconds) unless otherwise specified, e.g. frame\_time is seconds, frame\_time\_ms is milliseconds, object\_length is meters.
  * File-scoped const global variables or preprocessor defines need only consist of all uppercase letters separated with underscores, e.g. SECONDS\_BETWEEN\_CLEANUP
  * Global variables start with a lowercase 'g', followed by the name in UpperCamelCase, e.g. gGlobalTimeCounter
  * Local variables and function parameters are in lowercase, e.g. frame\_count
  * Local variables are not camelCase: thisIsNotALocalVariable
  * You may optionally append a suffix character signifying the type, for example, sTimeInSecondsf is a float
  * Functions (NOT methods, see below) are in lowercase, e.g. main(), update\_world\_time()
  * Classes are in UpperCamelCase: MyClass
  * Static class member variables are to ALWAYS be referenced by the class name, e.g. MyClass::staticMember
  * Non-static class member variables shall always be prefixed with the “this” pointer, e.g. this->cameraPosition
  * Class methods (not functions, see above) are UpperCamelCase, e.g. DestroyWorld()
  * Enums are to be contained inside namespaces. The namespace should have the descriptive name in UPPER\_CASE while the enum contained therein should be named TYPE to signify the fact that it is the type that is being referenced, not a value.  Eg. INDEX\_VALUES::TYPE where INDEX\_VALUES is the namespace and TYPE is the single contained enumeration.
  * Enum values should be in UPPER\_CASE and accessed using the container namespace, eg: INDEX\_VALUES::ALL\_INDICES where INDEX\_VALUES is the namespace and ALL\_INDICES is a value in the INDEX\_VALUES::TYPE enumeration.

Note that some consider the scope prefixes to certain classes of variable names to be too close to “Hungarian Notation” for comfort: however, Hungarian Notation is about data types - and the data type should be easy to intuitively understand from the variable or function name itself.  These prefixes are just for scope.

# Indentation #
Use tabs for line indentation and ensure that your editor is not converting tabs to spaces. When possible, use indentation to clarify variable lists and Boolean operations.

Use spaces to place comments or other code superstructure that is not basic indentation on related columns.

# Braces #
Source code should be written using braces on the same line, similar to the C/C++ K&R standard (not the more spaced-out GNU standard), i.e.
```c++

while (foo < 10) {
...
}
```

And
```c++

if (foo < 10) {
...
}
else if (foo > 100) {
...
}
else {
...
}
```

Use braces for clarity, even when not strictly required, e.g.
```c++

for (int j = 0; j < NUM_TEXTURES; ++j) {
print(j);
}
```

# Classy Classes #
Class declarations should be written for programmers who have no time.  Imagine you are wanting to find the name of a method you need, and you have to wade through several pages of code before you ever some to even the right area of the code to begin your search.  Could be quite frustrating when you are in crunch-time!  To this end, let’s abide by these simple rules:
  * _public_ items should be declared before _private_ items, as our theoretical time-limited programmer is more likely to be looking for a public method than a private one.
  * Properties should be grouped by purpose and sorted by name
  * Methods should be grouped by purpose and sorted by name
  * Properties should be listed after methods

## Obvious things we all know ##
  * No magic numbers - put them in consts or in #defines and give them readable useful names.
  * Use `TRUE` and `FALSE` when working with `BOOL` (but prefer `bool` in general)
  * Use `nullptr` for pointer compares to 0
  * Use `'\0'` for character compares to 0
  * Use `const` and `inline` rather than ` #define` whenever possible. Prefer optimizing after you have proven that the code you want to inline actually has a significant benefit from in-lining.
  * Remember that C++ does not have garbage collection
  * Comment non-intuitive member variables
  * Fix all compiler warnings. Except when the warning relates to a non-standard vendor specific item such as Mircosoft's "secure" version of certain functions.
  * Take advantage of the fact that gcc generates different, sometimes better, warnings than Visual Studio.
  * Don't use assignments in while loops, e.g. `while (foo = nextData())` as this causes gcc to emit warnings. Use `while ((foo = nextData()) != NULL)` or restructure the code.

# References #
  * [Second Life Coding Standard](http://wiki.secondlife.com/wiki/Coding_Standard)
  * [Mozilla C++ Portability Guide](http://developer.mozilla.org/en/C___Portability_Guide)