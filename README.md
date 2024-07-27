## lncpp

## WIP

This is a `c++` port of vector based 3d renderer [ln](https://github.com/fogleman/ln).
I couldn't manage to make a %100 port. There are some missing shapes.
Furthermore there are some differences with original implementation.
Because of this those two implementation produce incompatible images.
And `lncpp` is faster.

- [ ] Triangle shape
- [ ] Function shape
- [ ] Mesh shape
- [ ] Cylinder shape
- [ ] Cone shape
- [ ] Plane intersection methods
- [ ] STL File support
- [ ] Change tree implementation from pointer based to array based (implicit representation)

### Usage

The sample programs are available under `[examples](examples)`.

```sh
mkdir build && cd build
cmake ..
make -j$(nproc)
```
