
### Unify classes

We currently have a `cppr::logical` and a `cppr::boolean`; a `cppr::rstring`
and `std::string`; a `cppr::integer` and a `int`; a `cppr::numeric` and
a `double`. Try to unify this: one of each type. This might also reduce the
code in `as_rvec` for example. 

### Add Boost headers and source files to project

Currently a globally installed Boost installation is needed. Add the necessary
files to the src directory. I have already tried this with boost 1.62. However,
this resulted in compilation errors. Using the old Boost files from ldat, there
are warnings but no errors. It seems bcp doesn't copy all neccessary files. 



ToDo
=============================

- Missing values
- Write tests
- size/length
- Iterators
- Logical vectors (store as bytes?)
- Other storage types?
- Logical index
- Range index
