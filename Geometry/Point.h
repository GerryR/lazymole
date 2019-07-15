/**
* @file Point.h
* @brief Class for points
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

#ifndef LMA_POINT_H
#define LMA_POINT_H

#include <cstddef>
#include <array>
#include <cmath>
#include <memory>
#include <cassert>

namespace mla {

    template<typename T, std::size_t N>
    class Point {

    public:

        // Constructors
        Point() {
            p.fill(0.);
        };

        Point(const std::array<T, N> &values) {
            for (size_t i = 0; i < N; i++) {
                p[i] = values[i];
            }
        };

        Point(const Point<T, N - 1> &p2) {
            for (size_t i = 0; i < N - 1; i++) {
                p[i] = p2.get(i);
            }
            p[N - 1] = 0.;
        };

        // Destructor
        virtual ~Point() {};

        // Generic Functions
        T distanceFrom(const Point<T, N> &p2) const {
            T sum = 0.;
            for (size_t i = 0; i < N; i++) {
                sum += (p[i] - p2.p[i]) * (p[i] - p2.p[i]);
            }
            return std::sqrt(sum);
        };

        T get(const size_t i) const {
            return p[i];
        };

        void set(const size_t i, const T val) {
            assert(i < p.size());
            p[i] = val;
        }

        // Operators
        Point<T, N> &operator=(Point<T, N> const &p2) {
            for (size_t i = 0; i < N; i++) {
                p[i] = p2.p[i];
            }
            return (Point<T, N>&)*this;
        };

        Point<T, N> operator+(Point<T, N> const &p2) const {
            std::array<T, N> t{};
            for (size_t i = 0; i < N; i++) {
                t[i] = p[i] + p2.p[i];
            }
            return Point<T, N>(t);
        };

        Point<T, N> operator-(Point<T, N> const &p2) const {
            std::array<T, N> t{};
            for (size_t i = 0; i < N; i++) {
                t[i] = p[i] - p2.p[i];
            }
            return Point<T, N>(t);
        };

        Point<T, N> operator+() const {
            return Point<T, N>(p);
        };

        Point<T, N> operator-() const {
            std::array<T, N> t{};
            for (size_t i = 0; i < N; i++) {
                t[i] = -p[i];
            }
            return Point<T, N>(t);
        };

        Point<T, N> operator*(T const &val) const {
            std::array<T, N> t{};
            for (size_t i = 0; i < N; i++) {
                t[i] = p[i] * val;
            }
            return Point<T, N>(t);
        };

        Point<T, N> operator/(T const &val) const {
            std::array<T, N> t{};
            for (size_t i = 0; i < N; i++) {
                t[i] = p[i] / val;
            }
            return Point<T, N>(t);
        };

        bool operator==(const Point<T, N> &p2) const {
            for (size_t i = 0; i < N; i++) {
                if (p[i] != p2.p[i]) {
                    return false;
                }
            }
            return true;
        };

        bool operator!=(const Point<T, N> &p2) const {
            return !((Point<T, N>&)*this == p2);
        };

        // Output
        friend std::ostream& operator <<(std::ostream& os, const Point<T, N> &p1) {
            os << std::string("(");
            for (size_t i = 0; i < N; i++) {
                os << p1.get(i);
                if (i != N - 1) {
                    os << std::string(", ");
                }
            }
            os << std::string(")");
            return os;
        }

        std::array<T, N> p;

    };


    class Point2D : public mla::Point<double, 2> {

    public:
        Point2D() : mla::Point<double, 2>() {};

        Point2D(const double x1, const double x2) : mla::Point<double, 2>(std::array<double, 2>{{x1, x2}}) {};

        Point2D(const Point<double, 2>& p1) {
            p[0] = p1.p[0];
            p[1] = p1.p[1];
        }

    };


    class Point3D : public mla::Point<double, 3> {

    public:
        Point3D() : mla::Point<double, 3>() {};

        Point3D(const double x1, const double x2, const double x3) :
                mla::Point<double, 3>(std::array<double, 3>{{x1, x2, x3}}) {};

        Point3D(const Point2D &p2) : mla::Point<double, 3>(p2) {};

        Point3D(const Point<double, 3> &p1) {
            p[0] = p1.get(0);
            p[1] = p1.get(1);
            p[2] = p1.get(2);
        }



    };

}

#endif //LMA_POINT_H
