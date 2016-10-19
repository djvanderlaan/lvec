
#### Initialise vector

```{r}
a <- lvec(4, "integer")
as.rvec(a)
```

Returns uninitialised values.

### Unify classes

 We currently have a `cppr::logical` and a `cppr::boolean`; a `cppr::rstring`
 and `std::string`; a `cppr::integer` and a `int`; a `cppr::numeric` and
 a `double`. Try to unify this: one of each type. This might also reduce the
 code in `as_rvec` for example. 




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
