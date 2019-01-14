//
// Created by Christiaan Adrianus Miermans on 14.01.169.
// C.Miermans@lmu.de
// Class to index objects based on a key, namely a vector. This class is specialized to two or three dimensions.
// Constructor: Lattice_multi hashMap;
//

#ifndef HASH_MAP_MULTI
#define HASH_MAP_MULTI

#include <Eigen/Dense>
#include <vector>
#include <unordered_map>

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
class Lattice_multi {
    friend class Geometry;
//protected:
    std::unordered_map<Vector3i, std::vector<int>> lattice;

public:
    /** Returns true if there is not yet a value at the key "vector"*/

    /** Release a compute at an *occupied* validate*/
    void release(const int value, const Vector3i& key) {
        if( global::CHECKS ) {
            assert(isOccupied(key));
            assert( std::find(lattice[key].begin(), lattice[key].end(), value) != lattice[key].end() );
        }
        auto y = lattice[key];
        lattice[key].erase(std::find(lattice[key].begin(), lattice[key].end(), value));
        y = lattice[key];
        std::remove(lattice[key].begin(), lattice[key].end(), value);
        if( not numberOfElements(key) )
            lattice.erase(key);
    }

    void bind(const int value, const Vector3i & key) {

        if( not isOccupied(key) )
            lattice[key] = {value};
        else {
            if( global::CHECKS ) assert( std::find(lattice[key].begin(), lattice[key].end(), value) == lattice[key].end());
            lattice[key].push_back(value);
        }
    }

    bool isOccupied(const Vector3i& coordinate) const {
        return not(lattice.find(coordinate) == lattice.end());
    }

    int numberOfElements(const Vector3i& coordinate) {
//        if( global::CHECKS )
//            assert( isOccupied(coordinate) );
        auto x = lattice[coordinate].size();
        if( isOccupied(coordinate) )
            return int(lattice[coordinate].size());
        else
            return 0;
    }

    /** Returns the vaue of the lattice at @param key  */
    std::vector<int> &operator[](const Vector3i & key) {
        return lattice[key];
    }


    /** Displace the value at @param oldKey to the key @param newKey */
    void move(const int value, const Vector3i & oldKey, const Vector3i & newKey) {
        Lattice::move({value}, {oldKey}, {newKey}, {value});
    }

    /** Displaces multiple keys @param oldKeys to @param newKeys at the same time. Optionally, the user can also assign new values @param newValues to these keys. */
    void move(const std::vector<int> oldValues, const std::vector<Vector3i> & oldKeys, const std::vector<Vector3i> & newKeys, std::vector<int> newValues = {}) { // tested
        if( global::CHECKS ) {
            for (auto &el : oldKeys)
                assert(isOccupied(el));
            assert( oldKeys.size() == newKeys.size() );
        }

        if( not newValues.size() )
            newValues = oldValues;

        if( global::CHECKS )
            assert( newValues.size() == oldKeys.size() );

        for( int i = 0; i < oldValues.size(); ++i )
            release(oldValues[i], oldKeys[i]);
        for( int i = 0; i < oldKeys.size(); i ++ ) {
            if( not isOccupied(newKeys[i]) )
                lattice[newKeys[i]] = {newValues[i]};
            else
                lattice[newKeys[i]].push_back(newValues[i]);
        }

        if( global::CHECKS )
            for (auto &el : newKeys)
                assert(isOccupied(el));
    }
};


#endif //HASH_MAP_MULTI
