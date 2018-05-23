//
// Created by Christiaan Adrianus Miermans on 23.05.18.
// C.Miermans@lmu.de
// Class to index objects based on a key, namely a Eigen vector of integers. This class is specialized to two or three dimensions.
// Constructor: Lattice hashMap;
// requires the header-only class Eigen, see http://eigen.tuxfamily.org/
//

#ifndef GLOBULE_HASHMAP_H
#define GLOBULE_HASHMAP_H

#define CHECKS true


using namespace Eigen;

namespace std {
    template <> // Hash function to order the keys
    struct hash<Vector3i> {
        std::size_t operator()(const Vector3i &key) const {
            return std::hash<int>()(key[0]) ^ std::hash<int>()(key[1]) ^ std::hash<int>()(key[2]);
        }
    };
}
namespace std {
    template <> // Hash function to order the keys
    struct hash<Vector2i> {
        std::size_t operator()(const Vector2i &key) const {
            return std::hash<int>()(key[0]) ^ std::hash<int>()(key[1]);
        }
    };
}

/** Class to store a 'lattice' or grid that contains objects at a specified number of (three-dimensional) coordinates
 * @example HashMap<int> hashMap;
 * */
class Lattice {
    std::unordered_map<Vector3i, int> lattice;

    /** Release a compute at an *occupied* validate*/
    void release(const Vector3i& key) { // tested
        if( CHECKS )
            assert(isOccupied(key));
        lattice.erase(key);
        if( CHECKS )
            assert(not isOccupied(key));
    }

public:
    /** Returns true if there is not yet a value at the key "vector"*/
    bool isOccupied(const Vector3i& coordinate) const { // tested
        return not(lattice.find(coordinate) == lattice.end());
    }


    /** Returns the vaue of the lattice at @param key  */
    int &operator[](const Vector3i & key) { // tested
        return lattice[key];
    }


    /** Displace the value at @param oldKey to the key @param newKey */
    void move(const Vector3i & oldKey, const Vector3i & newKey) { // tested
        Lattice::move({oldKey}, {newKey}, {lattice[oldKey]});
    }

    /** Displaces multiple keys @param oldKeys to @param newKeys at the same time. Optionally, the user can also assign new values @param values to these keys. */
    void move(const std::vector<Vector3i> & oldKeys, const std::vector<Vector3i> & newKeys, std::vector<int> values = {}) { // tested
        if( CHECKS ) {
            for (auto &el : oldKeys)
                assert(isOccupied(el));
            assert( oldKeys.size() == newKeys.size() );
        }

        if( not values.size() ) { // tested
            values = std::vector<int>(oldKeys.size());
            for( int i = 0; i < oldKeys.size(); i ++ )
                values[i] = lattice[oldKeys[i]];
        }
        else if( CHECKS )
            assert( values.size() == oldKeys.size() );

        for( auto & el : oldKeys )
            release(el);
        for( int i = 0; i < oldKeys.size(); i ++ )
            lattice[newKeys[i]] = values[i];

        if( CHECKS )
            for (auto &el : newKeys)
                assert(isOccupied(el));
    }
};


#endif //GLOBULE_HASHMAP_H
