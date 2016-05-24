# dynarray

Slightly adjusted custom implementation for std::experimental::dynarray container.
This version of this container in contrast is not designed to be used with stack allocation schemes
and so requires heap memory (like std::vector) to store its elements.
This change of behaviour implies minor adjustments to the API.

Requires a C++11 conformant compiler in order to work.

Tries to be as much as possible conformant to the standard library, STL containers,
algorithms and its iterator facilities.

The advantages of this container compared to std::vector is that it object size is smaller
since it does not require a current-capacity pointer.
This means that it could be used as a building block for std::vector.
Using a large number (array) of these instances will require less memory compared to std::vector
which can be important in memory critical applications.

The major disadvantage is that it cannot shrink or grow in size after initial construction
of an instance of this container.
