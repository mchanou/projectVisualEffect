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




#ifndef GM_PARAMETRICS_CURVES_MySubCurve_H
#define GM_PARAMETRICS_CURVES_MySubCurve_H

#include "../gmlib/modules/parametrics/gmpcurve.h"
#include "../gmlib/modules/parametrics/curves/gmpsubcurve.h"



template <typename T>
class MySubCurve : public GMlib::PCurve<T,3> {
    GM_SCENEOBJECT(MySubCurve)

  public:
    MySubCurve(GMlib::DVector<GMlib::Vector<T,3>>& C);
    virtual ~MySubCurve();
    // Check if MySubCurve is closed
    bool            isClosed() const override;
    void            sample(int n, int d);

  protected:
    GMlib::DVector<GMlib::Vector<T,3>> _C;

    int             degree;
    int             order;
    int             k;
    // Virtual functions from PCurve, which have to be implemented locally
    void            eval(T t, int d,bool /*l*/) const; //override;
    T               getStartP() const override;
    T               getEndP()   const override;
    int             doublePart(std::vector<GMlib::DVector<GMlib::Vector<T,3>>>& V, int n);
    void            smoothPartClosed(std::vector<GMlib::DVector<GMlib::Vector<T,3>>>& V, int n);
    void    laneRiesenfeldClosed();

  };

    //Constructor
    template <typename T>
    inline
        MySubCurve<T>::MySubCurve(GMlib::DVector<GMlib::Vector<T,3>>& C ) {
        this->_C=C;

        //Level of refinement
        this->k=3 ;
    }


    //Destructor of MySubCurve
    template <typename T>
    MySubCurve<T>::~MySubCurve() {}


    template <typename T>
    inline
        int MySubCurve<T>::doublePart(std::vector<GMlib::DVector<GMlib::Vector<T,3>>>& V, int n){
        for(int i=n-1;i>0; i--){
            V[2*i][0]=V[i][0];
            V[2*i-1][0]=(V[i][0]+V[i-1][0])/2;
        }
        return 2*n-1;
    }
    template <typename T>
    inline
        void MySubCurve<T>::smoothPartClosed(std::vector<GMlib::DVector<GMlib::Vector<T,3>>>& V,int n){
        for(int j=1;j<degree; j++){
            for(int i=0; i<n-1; i++){
                V[i][0]=(V[i][0]+V[i+1][0])/2;
            }
            V[n-1][0]=V[0][0];
        }
    }


    template <typename T>
    inline
        void MySubCurve<T>::laneRiesenfeldClosed(){
        int n = _C.getDim();
        int m = std::pow(2, k)*n+1;
        std::vector<GMlib::DVector<GMlib::Vector<T,3>>>& V=_visu[0].sample_val;
        V.resize(m);
        //for
        for(int i=0; i<m; i++){
            V[i].setDim(1);        }
        for(int i=0; i<n; i++){
            V[i][0]=_C[i];
        }
        V[n][0]=_C[0];
        n++;
        for(int i=0; i<k; i++){
            n=doublePart(V, n);
            smoothPartClosed(V, n);
        }
        GMlib::Sphere<T, 3>& s=_visu[0].sur_sphere;
        s.reset();
        computeSurroundingSphere(V,s);
    }

    template <typename T>
    inline
        void MySubCurve<T>::sample(int n, int d){
        this->degree=d;
        this->order = degree+1;
        _visu.resize(1);
        laneRiesenfeldClosed();
        this->setEditDone(true);

    }


    //**************************************
    //        Public local functons       **
    //**************************************

    //***************************************************
    // Overrided (public) virtual functons from PCurve **
    //***************************************************

    template <typename T>
    inline
        bool MySubCurve<T>::isClosed() const{
            return false;
    }



    //******************************************************
    // Overrided (protected) virtual functons from PCurve **
    //******************************************************



    template <typename T>//implementation of the eval function that should return the scale
    void MySubCurve<T>::eval( T t, int d, bool /*l*/ ) const {}


    template <typename T>
    T MySubCurve<T>::getStartP() const {
        return 0;
    }

    template <typename T>
    T MySubCurve<T>::getEndP()const {
        return 0;
    }


#endif // GM_PARAMETRICS_CURVES_MySubCurve_H
