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




#ifndef GM_PARAMETRICS_CURVES_MyBlendingSpline_H
#define GM_PARAMETRICS_CURVES_MyBlendingSpline_H

#include "../gmlib/modules/parametrics/gmpcurve.h"
#include "../gmlib/modules/parametrics/curves/gmpsubcurve.h"



template <typename T>
class MyBlendingSpline : public GMlib::PCurve<T,3> {
    GM_SCENEOBJECT(MyBlendingSpline)

  public:
    MyBlendingSpline(GMlib::PCurve<T,3>* mCu, int n);
    //MyBlendingSpline( const MyBlendingSpline<T>& copy );
    virtual ~MyBlendingSpline();
    // Check if MyBlendingSpline is closed
    bool            isClosed() const override;


  protected:
    // Virtual functions from PCurve, which have to be implemented locally
    void            eval(T t, int d,bool /*l*/) const; //override;
    T               getStartP() const override;
    T               getEndP()   const override;
    int             findIndex(T t) const;//T is the type selected and is the value for which we will have to find the index and return a i
    T               makeW(int d, int i, T t) const;
    void            makeKnotVector(int n) ;
    void            closedMakeKnotVector(int n);
    GMlib::Vector<T,2>            makeBlendingSpline(int i, T t) const;
    T               makeB(T t) const;
    void                        localSimulate(double dt) override;
    GMlib::PCurve<T,3>* _mCu;
    GMlib::DVector<GMlib::PCurve<T,3>*>       localCurve;
    std::vector<T>  _t;
    int             degree;
    int             order;
    int             _n;
  };



    //Constructor
    template <typename T>
    inline
        MyBlendingSpline<T>::MyBlendingSpline(GMlib::PCurve<T, 3> *mCu, int n ) {
        this->_mCu=mCu;
        this->_n=n;
        this->degree= 2;
        this->order = degree+1;
        if (_mCu->isClosed()==true){
                this->closedMakeKnotVector(_n);
            }else{
                this->makeKnotVector(_n);
        }
            //std::cout<<_t << std ::endl;
        for(int i=0; i<n; i++){
            GMlib::PCurve<T,3>* p = new GMlib::PSubCurve<T>( _mCu, _t[i], _t[i+2], _t[i+1]);
            p->toggleDefaultVisualizer();
            p->sample(20,0);
            //p->setColor(GMlib::GMcolor::green());
            this->insert(p);
            p->setVisible(true);
            p->setCollapsed(true);
            this->localCurve.push_back(p);
        }
        this->localCurve.push_back(this->localCurve[0]);


    }


    //Destructor of MyBlendingSpline
    template <typename T>
    MyBlendingSpline<T>::~MyBlendingSpline() {}


    //**************************************
    //        Public local functons       **
    //**************************************

    template <typename T>
    inline
        int MyBlendingSpline<T>::findIndex(T t) const {
        for(int i=1; i<localCurve.getDim()-1; i++){
                if(t>=_t[i] && t<_t[i+1]){
                        return i;
                    }
        }
        return localCurve.getDim()-1 ;
    }

    template <typename T>
    inline
        void  MyBlendingSpline<T>::makeKnotVector(int n){
        //This function generate the knot vector according to the index
            this->_t.resize(order+n);
            for(int i=0; i<order+n;i++){
                if(i<order){
                    _t[i]=0;
                }else if(i>=n){
                    _t[i]=n-degree;
                }else{
                    _t[i]=(i-degree);
                }
        }
    }

    template <typename T>
    inline
    void MyBlendingSpline<T>::closedMakeKnotVector(int n) {
        // This function generates the knot vector according to the index
        this->_t.resize(order + n);
        std::cout<< order <<std ::endl;
        T s = 0;
        T e = 2 * M_PI;
        T delta = (e - s) / n;

        for (int i = 0; i < order + n ; i++) {
            _t[i] = s + (i - 1) * delta;
        }
    }

    template <typename T>
    inline
        T MyBlendingSpline<T>::makeW(int d, int i,T t) const {
        T W=(t-_t[i])/(_t[i+d]-_t[i]);
            return W;
            }


    template <typename T>
    inline
        T MyBlendingSpline<T>::makeB(T t) const {
        //Our B function is a polynomial function of first order
        T B=3*t*t-2*t*t*t;
        return B;
    }
    /*Page150 and page 115*/
    template <typename T>
    inline
        GMlib::Vector<T,2> MyBlendingSpline<T>::makeBlendingSpline(int i, T t) const {
        GMlib::Vector<T,2> b;

        b[1]=makeB(makeW(1,i,t));
        b[0]=1-b[1];

        return b;

    }
    //***************************************************
    // Overrided (public) virtual functons from PCurve **
    //***************************************************

    template <typename T>
    inline
        bool MyBlendingSpline<T>::isClosed() const{
            return true;
    }



    //******************************************************
    // Overrided (protected) virtual functons from PCurve **
    //******************************************************



    template <typename T>//implementation of the eval function that should return the scale
    void MyBlendingSpline<T>::eval( T t, int d, bool /*l*/ ) const {

        this->_p.setDim( d + 1 );
        int i=findIndex(t);
        //std::cout << t << " " << i << std::endl;
        GMlib::Vector<T,4> b = makeBlendingSpline(i,t);
        GMlib::DVector<GMlib::Vector<T,3> >m1=this->localCurve[i-1]->evaluateParent(t,d);
        GMlib::DVector<GMlib::Vector<T,3> >m2=this->localCurve[i]->evaluateParent(t,d);

        this->_p[0]=b[0]*m1[0] + b[1]*m2[0];
    }


    template <typename T>
    void MyBlendingSpline<T>::localSimulate(double dt) {

        this->sample(50,0);
        static double t=0;
        t += dt;
        GMlib::Vector<float,3> vec(sin(t),cos(t),0);
        vec *= 0.025;
        for (int i=0; i<localCurve.getDim()-1; i++){
            localCurve[i]->translate(vec);
            vec=-1*vec;
         }


        //this->move(vec);
        for (int i=0; i<localCurve.getDim()-1; i++){
            localCurve[i]->rotate( GMlib::Angle(2*dt), GMlib::Vector<float,3>( 1.0f, 0.0f, 0.0f) );
            //localCurve[i]->translate(dt*2.0f);
        }

        this->setEditDone(true);
    }

    template <typename T>
    T MyBlendingSpline<T>::getStartP() const {
        return 0;
    }

    template <typename T>
    T MyBlendingSpline<T>::getEndP()const {
        return 2*M_PI;
    }


#endif // GM_PARAMETRICS_CURVES_MyBlendingSpline_H
