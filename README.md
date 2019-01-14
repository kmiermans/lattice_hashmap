# lattice_hashmap
A class for a 2D or 3D hashmap (unordered map) based lattice using the Eigen library. Useful for performing physical simulations on a lattice. Maps Eigen::Vector2i or Eigen::Vector3i as a key to integers. Can easily be changed for different key-value bindings, of course.

Example usage:
```
using namespace Eigen;
Lattice hashMap;
Vector3i X = {1,2,3};
hashMap[X] = 1;
bool has_key = hashMap.is_occupied(X);  // true
hashMap.is_occupied(2*X); // false

hashMap.move(X, 2*X);
hashMap.is_occupied(2*X); // true
```
