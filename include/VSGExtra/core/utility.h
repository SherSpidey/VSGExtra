//
// Created by ParadoxFang on 2025/6/4.
//

#ifndef UTILITY_H
#define UTILITY_H

#include <iostream>
#include <vsg/maths/mat4.h>

namespace VSGExtra
{
    template <typename T>
    struct Holder
    {
        bool dirty = true;
        T value;

        Holder() = default;

        explicit Holder(const T& val) : dirty(false), value(val)
        {
        }

        // mark value dirty
        void Dirty()
        {
            dirty = true;
        }

        // set 
        void Set(const T& val)
        {
            value = val;
            dirty = false;
        }

        const T& Get() const
        {
            if (dirty)
            {
                std::cerr << "[WARNING] Accessing dirty value: " << value << '\n';
            }

            return value;
        }
    };

    template <typename T>
    struct Schrodinger
    {
        bool observed = false;
        T value;

        Schrodinger() = default;

        explicit Schrodinger(const T& val) : observed(true), value(val)
        {
        }

        void Superpose()
        {
            observed = false;
        }

        // set 
        void Collapse(const T& val)
        {
            value = val;
            observed = true;
        }

        // get
        const T& Reveal() const
        {
            if (!observed)
            {
                std::cerr << "[WARNING] Accessing an unobserved value: " << value << "\n";
            }

            return value;
        }
    };

    using Matrix4DHolder = Holder<vsg::dmat4>;
    
    using QuantumVec3D = Schrodinger<vsg::dvec3>;
}

EVSG_type_name(VSGExtra::Matrix4DHolder);

EVSG_type_name(VSGExtra::QuantumVec3D);

#endif //UTILITY_H
