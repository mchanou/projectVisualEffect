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




#ifndef GM_PARAMETRICS_CURVES_MySpline_H
#define GM_PARAMETRICS_CURVES_MySpline_H

#include "../gmlib/modules/parametrics/gmpcurve.h"



template <typename T>
class MySpline : public GMlib::PCurve<T,3> {
      GM_SCENEOBJECT(MySpline)

  public:
    MySpline(const GMlib::DVector<GMlib::Vector<T,3>>& C);
    //Constructror using Least square
    MySpline(const GMlib::DVector<GMlib::Vector<T,3>>& P, int n);
    //MySpline( const MySpline<T>& copy );
    virtual ~MySpline();
    // Check if MySpline is closed
    bool            isClosed() const override;


  protected:
    // Virtual functions from PCurve, which have to be implemented locally
    //Compute the Parametric equation of the spline
    void            eval(T t, int d,bool /*l*/) const; //override
    //Define the domain of the Spline
    T               getStartP() const override;
    T               getEndP()   const override;
    //Find the interval of the following value of t
    int             findIndex(T t) const;//T is the type selected and is the value for which we will have to find the index and return a i
    //Scale the knot vetor between 0 and 1
    T               makeW(int d, int i, T t) const;
    //Generate the knot vector with the number of control points
    void            makeKnotVector(int m) ;
    //Compute the products of the differents coefficient of the parametric equation
    GMlib::Vector<T,4>               makeB(int i, T t) const;
    std::vector<T>  _t;
    int             degree;
    int             order;
    int             m;
    int             numKnots;
    GMlib::DVector<GMlib::Vector<T,3>> _C;
    };


    /*template <typename T>
    inline
        MySpline<T>::MySpline(  T radius1, T radius2 ) : PCurve<T,3>(20, 0, 7) {

    }

    template <typename T>
    inline
        MySpline<T>::MySpline( const MySpline<T>& copy ) : PCurve<T,3>(copy) {}

    */
    //Constructor
    template <typename T>
    inline
        MySpline<T>::MySpline(const GMlib::DVector<GMlib::Vector<T,3>>& C) {
        this->degree = 3;
        this->order = degree+1;
        this->m=C.getDim();
        this->numKnots= m+order;
        _C=C;
        this->makeKnotVector(m);

    }
    template <typename T>
    inline
        MySpline<T>::MySpline(const GMlib::DVector<GMlib::Vector<T,3>>& P, int n) {
        this->degree = 3;
        this->order = degree+1;
        //Get the number of Vector inside the DVector
        this->m=P.getDim();
        this->makeKnotVector(n);
        std::cout<<"t= "<<_t<<std::endl;
        GMlib::DMatrix<T> A(m, n, T(0));
        for(int j=0; j<m; j++){
            //I'm not sure about this one
            T x=_t[degree]+j*(_t[n]-_t[degree])/(m-1);
            int i = findIndex(x);

            GMlib::Vector<T,4> b = makeB(i,x);
            //Here too I may need to ask the relation between the formula on page 102
            A[j][i-3]=b[0];
            A[j][i-2]=b[1];
            A[j][i-1]=b[2];
            A[j][i]=b[3];
        }
        GMlib::DMatrix<T> AT;
        AT=A;
        AT.transpose();
        std::cout<<A<<std::endl;
        GMlib::DVector<GMlib::Vector<T,3>> y = AT*P;
        GMlib::DMatrix<T> B;
        B=AT*A;
        //GMlib::DMatrix<T> BI = B.invert();
        //According to the expression of the least square saying that Bc=y we can deduce c=BI*y (Referencre p103)
        B.invert();
        _C = B*y;





    }
    //Destructor of MySpline
    template <typename T>
    MySpline<T>::~MySpline() {}


    //**************************************
    //        Public local functons       **
    //**************************************

    template <typename T>
    inline
        int MySpline<T>::findIndex(T t) const {
        for(int i=3; i<_t.size()-order; i++){
                if(t>=_t[i] && t<_t[i+1]){
                        return i;
                    }
        }
        return _t.size()-order-1 ;
    }

    template <typename T>
    inline
        void  MySpline<T>::makeKnotVector(int n){
        //This function generate the knot vector according to the index
        this->numKnots =this->order+n;
            this->_t.resize(numKnots);
            for(int i=0; i<numKnots;i++){
                if(i<order){
                    _t[i]=0;
                }else if(i>=numKnots-order){
                    _t[i]=n-degree;
                }else{
                    _t[i]=(i-degree);
                }
        }
    }


    template <typename T>
    inline
        T MySpline<T>::makeW(int d, int i,T t) const {
        T W=(t-_t[i])/(_t[i+d]-_t[i]);
            return W;
            }


    template <typename T>
    inline
        GMlib::Vector<T,4> MySpline<T>::makeB( int i,T t) const {
        T v0=(1-makeW(1,i,t))*(1-makeW(2,i-1,t));
        T v1=(1-makeW(1,i,t))*(makeW(2,i-1,t))+(makeW(1,i,t))*(1-makeW(2,i,t));
        T v2=(makeW(1,i,t))*(makeW(2,i,t));

        GMlib::Vector<T,4> b;
        b[0]=v0*(1-makeW(3,i-2,t));
        b[1]=v0*(makeW(3,i-2,t))+v1*(1-makeW(3,i-1,t));
        b[2]=v1*(makeW(3,i-1,t))+v2*(1-makeW(3,i,t));
        b[3]=v2*(makeW(3,i,t));
        std::cout<<"i="<<i<<" t=" <<t<<std::endl;
        return b;
    }
    //***************************************************
    // Overrided (public) virtual functons from PCurve **
    //***************************************************

    template <typename T>
    inline
        bool MySpline<T>::isClosed() const{
            return false;
    }



    //******************************************************
    // Overrided (protected) virtual functons from PCurve **
    //******************************************************



    template <typename T>//implementation of the eval function that should return the scale
    void MySpline<T>::eval( T t, int d, bool /*l*/ ) const {

        this->_p.setDim( d + 1 );
        int i=findIndex(t);
        GMlib::Vector<T,4> b = makeB(i,t);
        std::cout <<"B= "<< b <<std::endl;
        //Formula of the B-Spline factor matrix
        this->_p[0]=b[0]*_C[i-degree] + b[1]*_C[i-degree+1]+ b[2]*_C[i-degree+2]+b[3]*_C[i];

    }


    template <typename T>
    T MySpline<T>::getStartP() const {
        return _t[degree];
    }

    template <typename T>
    T MySpline<T>::getEndP()const {
        int end =_t.size()-order;
        return _t[end];
    }


#endif // GM_PARAMETRICS_CURVES_MySpline_H
