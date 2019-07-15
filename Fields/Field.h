//
// Created by gerry on 06/10/15.
//

#ifndef LMA_FIELD_H
#define LMA_FIELD_H

#include <cstddef>
#include <vector>
#include <Grid.h>
#include <iostream>


namespace mla {

    template<typename C>
    class Field {

    public:

        // Constructors
        Field(Grid* grid, size_t dof, C value) :
                gridPtr(grid), values(dof, value) {};

        // Destructor
        virtual ~Field() {};

        // Functions
        virtual Grid* grid() const {
            return gridPtr;
        };

        virtual size_t dof() const {
            return values.size();
        };

        virtual C get(const size_t id) const {
            return values[id];
        }

        virtual void set(const size_t id, const C val) {
            values[id] = val;
        }

        // Operators
        C operator [](size_t id) const {
            return values[id];
        }

        C& operator [](size_t id) {
            return values[id];
        }


    protected:

        Grid* gridPtr;
        std::vector<C> values;

    };
}


#endif //LMA_FIELD_H
