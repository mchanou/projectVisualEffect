/**********************************************************************************
**
** Copyright (C) 1994 Narvik University College
** Contact: GMlib Online Portal at http://episteme.hin.no
**
** This file is part of the Geometric Modeling Library, GMlib.
**
** GMlib is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** GMlib is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with GMlib.  If not, see <http://www.gnu.org/licenses/>.
**
**********************************************************************************/




#ifndef GM_PARAMETRICS_CURVES_MyStar_H
#define GM_PARAMETRICS_CURVES_MyStar_H

#include "../gmlib/modules/parametrics/gmpcurve.h"



template <typename T>
class MyStar : public GMlib::PCurve<T,3> {
    GM_SCENEOBJECT(MyStar)

public:
    MyStar(const GMlib::DVector<GMlib::Vector<T,3>>& C);
    //Constructror using Least square
    MyStar(const GMlib::DVector<GMlib::Vector<T,3>>& P, int n);
    //MyStar( const MyStar<T>& copy );
    virtual ~MyStar();
    // Check if MyStar is closed
    bool            isClosed() const override;


protected:
    // Virtual functions from PCurve, which have to be implemented locally
    void            eval(T t, int d,bool /*l*/) const; //override;
    T               getStartP() const override;
    T               getEndP()   const override;
};


template <typename T>
inline
    MyStar<T>::MyStar(const GMlib::DVector<GMlib::Vector<T,3>>& C) {
}

//Destructor of MyStar
template <typename T>
MyStar<T>::~MyStar() {}


template <typename T>
inline
    bool MyStar<T>::isClosed() const{
    return true;
}



//******************************************************
// Overrided (protected) virtual functons from PCurve **
//******************************************************



template <typename T>
void MyStar<T>::eval(T t, int d, bool /*l*/) const {

    this->_p.setDim( d + 1 );
    // Your B-spline parametric equations go here
    T x = std::cos(t) + 0.5 * std::cos(3* t);
    T y = std::sin(t) + 0.5 * std::sin(3*t-1);
    T z = 0.3*std::sin(5 * t) + std::sin(t);

    this->_p[0][0] = x;
    this->_p[0][1] = y;
    this->_p[0][2] = z;
}


template <typename T>
T MyStar<T>::getStartP() const {
    return 0;
}

template <typename T>
T MyStar<T>::getEndP()const {
    return 2*M_PI;
}


#endif // GM_PARAMETRICS_CURVES_MyStar_H
