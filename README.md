# SpanningTreeProtocol
Implementation of Rapid Spanning Tree Protocol (RSTP) according to standard IEEE 802.1D-2004.

## Implementation
Project has been written in C++14 and it is totally platform/OS independent. The STP is running
as a separated thread run via STP's API.
The STP described in specification is base on state machines. Adopted *state* design pattern
in order to implement these state machines in *object* approach. There are also other adopted
design patterns like *singleton* (to make sure have only one instance of class), *decorator* (adds
some extra information during the logging message on every level of wrapping sub-class) or
*null object* (to make sure that if *user* didn't set parameter then code will call just *empty*
function without checking every time if instance has been created).

## Tests
At the moment (September, 2018), the project have few unit tests written based on framework
*Google Test*.

## Author

* **Pawel Maslanka** - *Main developer* - [pawmas] (https://github.com/pawmas)

Contact: pawmas@hotmail.com

## Todo
* Extend STP's API to set other parameters of Spanning Tree Protocol
* Continue work with unit tests for other state machines
* Add comment into source code (or just add reference point to the STP's specification) to be more
  understood for other developers
* Run RSTP conformance tests (IXIA/Spirent)
* Evaluate project into MSTP (according to specification IEEE 802.1s)
