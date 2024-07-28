## lncpp

## WIP

This is a `c++` port of vector based 3d renderer [ln](https://github.com/fogleman/ln).

There are some differences with original implementation.
Two implementations produce incompatible images.
I couldn't manage to make a %100 port. There are some missing shapes.

- [ ] Triangle shape
- [ ] Function shape
- [ ] Mesh shape
- [ ] Cylinder shape
- [ ] Cone shape
- [ ] Plane intersection methods
- [ ] STL File support
- [ ] Change tree implementation from pointer based representation to an array based (implicit representation)

### Usage

The sample programs are available under `[examples](examples)`.

```sh
mkdir build && cd build
cmake ..
make -j$(nproc)
```

[<img src="art/beads.png">]()
