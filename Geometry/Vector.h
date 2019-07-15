/**
* @file Vector.h
* @brief Class for vectors
*
* @author Calogero B. Rizzo
*
* @copyright This file is part of the lazymole software.
*            Copyright (C) 2019 Calogero B. Rizzo
*
* @license This program is free software: you can redistribute it and/or modify
*          it under the terms of the GNU General Public License as published by
*          the Free Software Foundation, either version 3 of the License, or
*          (at your option) any later version.
*
*          This program is distributed in the hope that it will be useful,
*          but WITHOUT ANY WARRANTY; without even the implied warranty of
*          MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*          GNU General Public License for more details.
*
*          You should have received a copy of the GNU General Public License
*          along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef LMA_VECTOR_H
#define LMA_VECTOR_H


#include <cstddef>
#include <Point.h>
#include <assert.h>

namespace mla {

    template<typename T, size_t N>
    class Vector {
    public:

        // Constructors
        Vector() {
            std::array<T, N> t {};
            t.fill(0.);
            t[0] = 1.;
            p2 = Point<T, N>(t);
        };

        Vector(const Point<T, N> &startPoint, const Point<T, N> &endPoint) : p1(startPoint), p2(endPoint) {};

        Vector(const Point<T, N> &endPoint) : p1(Point<T, N>()), p2(endPoint) {};

        Vector(const Vector<T, N - 1> &v) : p1(v.getStartPoint()), p2(v.getEndPoint()) {};

        // Destructor
        ~Vector() {};

        // Generic Functions
        T get(size_t i) const {
            return p2.get(i) - p1.get(i);
        };

        Point<T, N> getStartPoint() const {
            return p1;
        }

        Point<T, N> getMiddlePoint() const {
            return (p1 + p2) / 2;
        };

        Point<T, N> getEndPoint() const {
            return p2;
        };

        T norm() const {
            return p1.distanceFrom(p2);
        };

        Vector<T, N> versor() const {
            const Point<T, N> p3 = p2 - p1;
            const Point<T, N> pNull = Point<T, N>();
            Vector<T, N> v(pNull, p3);
            return v / v.norm();
        };

        T dot(const Vector<T, N> v) const {
            T sum = 0.;
            for (size_t i = 0; i < N; i++) {
                sum += this->get(i) * v.get(i);
            }
            return sum;
        };

        T angle(const Vector<T, N> v) const {
            return std::acos(this->dot(v) / this->norm() / v.norm());
        };

        static Vector<T, N> cross(const std::array<Vector<T, N>, N - 1> vectors) {
            if (N == 2) {
                // Rotation of 90 degrees
                Point<T, N> p3 = vectors[0].p2 - vectors[0].p1;
                std::array<T, N> a {{-p3.get(1), p3.get(0)}};
                Point<T, N> p3rotated(a);
                return Vector<T, N>(vectors[0].p1, p3rotated + vectors[0].p1);
            }
            if (N == 3) {
                // Cross product
                Point<T, N> p0 = vectors[0].p2 - vectors[0].p1;
                Point<T, N> p1 = vectors[1].p2 - vectors[1].p1;
                std::array<T, N> a;
                a[0] = p0.get(1) * p1.get(2) - p0.get(2) * p1.get(1);
                a[1] = p0.get(2) * p1.get(0) - p0.get(0) * p1.get(2);
                a[2] = p0.get(0) * p1.get(1) - p0.get(1) * p1.get(0);
                Point<T, N> p3(a);
                return Vector<T, N>(vectors[0].p1, p3 + vectors[0].p1);
            }
            assert(false);
        };

        // Operators
        Vector<T, N> &operator=(Vector<T, N> const &v2) {
            p1 = v2.p1;
            p2 = v2.p2;
            return (Vector<T, N> &)*this;
        };

        Vector<T, N> operator+(Vector<T, N> const &v2) const {
            return Vector<T, N>(this->p1, v2.p2 - v2.p1 + this->p2);
        };

        Vector<T, N> operator-(Vector<T, N> const &v2) const {
            return Vector<T, N>(this->p1, v2.p1 - v2.p2 + this->p2);
        };

        Vector<T, N> operator+() const {
            return Vector<T, N>(p1, p2);
        };

        Vector<T, N> operator-() const {
            return Vector<T, N>(- p1, - p2);
        }

        Vector<T, N> operator*(T const &val) const {
            return Vector<T, N>(p1, (p2 - p1) * val + p1);
        };

        Vector<T, N> operator/(T const &val) const {
            return Vector<T, N>(p1, (p2 - p1) / val + p1);
        };

        // Output
        friend std::ostream& operator <<(std::ostream& os, const Vector<T, N> &v1) {
            os << std::string("[") << v1.getStartPoint() << std::string(" -> ") << v1.getEndPoint() << std::string("]");
            return os;
        }


    private:

        // Vector start in p1 and end in p2
        Point<T, N> p1, p2;

    };

    class Vector3D : public mla::Vector<double, 3> {

    public:
        Vector3D() : mla::Vector<double, 3>() {};

        Vector3D(const mla::Vector<double, 2> &v) :  mla::Vector<double, 3>(v) {}

        Vector3D(const mla::Vector<double, 3> &v) :  mla::Vector<double, 3>(v.getStartPoint(), v.getEndPoint()) {};

        Vector3D(const Point3D &startPoint, const Point3D &endPoint) : mla::Vector<double, 3>(startPoint, endPoint) {};

        Vector3D(const Point3D &endPoint) : mla::Vector<double, 3>(endPoint) {};

    };

    class Vector2D : public mla::Vector<double, 2> {

    public:
        Vector2D() : mla::Vector<double, 2>() {};

        Vector2D(const mla::Vector<double, 2> &v) : mla::Vector<double, 2>(v.getStartPoint(), v.getEndPoint()) {};

        Vector2D(const Point2D &startPoint, const Point2D &endPoint) : mla::Vector<double, 2>(startPoint, endPoint) {};

        Vector2D(const Point2D &endPoint) : mla::Vector<double, 2>(endPoint) {};

        Vector2D cross() const {
            std::array<mla::Vector<double, 2>, 1> a = {*this};
            return Vector2D(mla::Vector<double, 2>::cross(a));
        }

        Vector3D cross(const Vector2D &v1) const {
            std::array<mla::Vector<double, 3>, 2> a = {Vector3D(*this), Vector3D(v1)};
            return Vector3D(mla::Vector<double, 3>::cross(a));
        }

    };

}

#endif //LMA_VECTOR_H
