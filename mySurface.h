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




#ifndef GM_PARAMETRICS_CURVES_MySurface_H
#define GM_PARAMETRICS_CURVES_MySurface_H

#include "../gmlib/modules/parametrics/gmpcurve.h"
#include "../gmlib/modules/parametrics/simplesubsurf.h"



template <typename T>
class MySurface : public GMlib::PSurf<T,3> {
      GM_SCENEOBJECT(MySurface)
  public:
        // Constructors and destructor
        MySurface( GMlib::PSurf<T,3>* mSu, int nu, int nv);
        virtual ~MySurface();
  protected:
      T su;
      T sv;
      T eu;
      T ev;
      int nu;
      int nv;
      int             degree;
      int             order;
      std::vector<T>  _u;
      std::vector<T>  _v;
      GMlib::PSurf<T,3>* _mSu;
      GMlib::DMatrix<GMlib::PSurf<T,3>*>       localSurf;
      // Virtual functions from PSurf, which have to be implemented locally
      void          eval(T u, T v, int d1, int d2, bool lu = true, bool lv = true ) const override;
      T             getStartPU() const override;
      T             getEndPU()   const override;
      T             getStartPV() const override;
      T             getEndPV()   const override;
      int           findIndex(T t, std::vector<T> knotVector, bool closed) const;//T is the type selected and is the value for which we will have to find the index and return a i
      T             makeW(int d, int i, T t, std::vector<T> knotVector) const;
      T             makeWD(int d, int i, T t, std::vector<T> knotVector) const;
      T             makeB(T t) const;
      T             makeBD(T t) const;
      std::vector<T>          makeKnotVector(int n, T s, T e);
      std::vector<T>          closedMakeKnotVector(int n, T s, T e);
      GMlib::Vector<T,2> makeBlendSurf(int i, T t, std::vector<T> knotVector) const;
      GMlib::Vector<T,2> makeBlendSurfD(int i, T t, std::vector<T> knotVector) const;
      void                        localSimulate(double dt) override;

};

    //Constructor
    template <typename T>
    inline
    MySurface<T>::MySurface(GMlib::PSurf<T,3>* mSu, int nu, int nv) {
    //Not sure about this one and how the knot vectore are made
    this->_mSu = mSu;
    this->degree=1;
    this->order=degree+1;
    this->nu=nu;
    this->nv=nv;
    this->su= _mSu->getParStartU();
    this->sv= _mSu->getParStartV();
    this->eu= _mSu->getParEndU();
    this->ev= _mSu->getParEndV();

    if (_mSu->isClosedU()==true && _mSu->isClosedV()==true){
        _u=closedMakeKnotVector(nu, su, eu);
        _v=closedMakeKnotVector(nv, sv, ev);
    }else if(_mSu->isClosedU()==true && _mSu->isClosedV()==false){
        _u=closedMakeKnotVector(nu, su, eu);
        _v=makeKnotVector(nv, sv, ev);
    }else if (_mSu->isClosedU()==false && _mSu->isClosedV()==true){
        _u=makeKnotVector(nu, su, eu);
        _v=closedMakeKnotVector(nv, sv, ev);
    }else{
        _u=makeKnotVector(nu, su, eu);
        _v=makeKnotVector(nv, sv, ev);
    }
    std::cout <<_u <<_v << std::endl;

    localSurf.setDim(nu, nv);

    for(int i=0; i<nu; i++){
        for(int j=0; j<nv;j++){
            localSurf[i][j] = new PSimpleSubSurf<T>(_mSu, _u[i], _u[i+2],_u[i+1], _v[j],_v[j+2], _v[j+1]);
            localSurf[i][j]->toggleDefaultVisualizer();
            localSurf[i][j]->sample(10,10,1,1);
            this->insert(localSurf[i][j]);
            localSurf[i][j]->setVisible(true);
            localSurf[i][j]->setCollapsed(true);

        }


    }

    }
    //Destructor of MySurface
    template <typename T>
    MySurface<T>::~MySurface() {}



    //******************************************************
    // Overrided (protected) virtual functons from PCurve **
    //******************************************************
    //Redo this function
    template <typename T>
    inline
        int MySurface<T>::findIndex(T t, std::vector<T> knotVector, bool closed) const {
        // if (closed == true){
        //     for(int i=1; i<knotVector.size()-3; i++){
        //         if(t>= knotVector[i] && t<knotVector[i+1]){
        //             return i;
        //         }
        //     }
        //     return knotVector.size()-2 ;
        // }else{
        //     for(int i=degree; i<knotVector.size()-order; i++){
        //         if(t>= knotVector[i] && t<knotVector[i+1]){
        //             return i;
        //         }
        //     }
        //     return knotVector.size()-order-1 ;
        // }
        int r = closed? 3:2;
        for(int i = 1; i <knotVector.size()-r; i++)
            if (t >=knotVector[i] && t < knotVector[i+1]){
                return i;
            }
        return knotVector.size()-r-1;
    }

    // template <typename T>
    // inline
    //     void  MySurface<T>::makeKnotVector(int n, std::vector<T>& knotVector, T s, T e){
    //     //This function generate the knot vector according to the index
    //     int numKnots =this->order+n;
    //     knotVector.resize(numKnots);
    //     // for(int i=degree; i<numKnots;i++){
    //     //     if(i<order){
    //     //         knotVector[i]=0;
    //     //     }else if(i>=numKnots-order){
    //     //         knotVector[i]=n-degree;
    //     //     }else{
    //     //         knotVector[i]=(i-degree);
    //     //     }
    //     // }
    //     T delta = (e - s) / (n-1);
    //     for(int i=degree; i<numKnots;i++){
    //         if(i<order){
    //             knotVector[i]=s;
    //         }else if(i>=numKnots-order){
    //             knotVector[i]=e;
    //         }else{
    //             knotVector[i]=s + (i - 1) * delta;
    //         }
    //     }
    // }

    template <typename T>
    inline
        std::vector<T>  MySurface<T>::makeKnotVector(int n, T s, T e){
        //This function generate the knot vector according to the index
        int numKnots =this->order+n;
        std::vector<T> knotVector;
        knotVector.resize(numKnots);
        std :: cout <<"Open" << std::endl;
        T delta = (e - s) / (n-1);
        for(int i=degree; i<numKnots;i++){
            if(i<order){
                knotVector[i]=s;
            }else if(i>=numKnots-order){
                knotVector[i]=e;
            }else{
                knotVector[i]=s + (i - 1) * delta;
            }
        }
        return knotVector;
    }
    template <typename T>
    inline
        std::vector<T> MySurface<T>::closedMakeKnotVector(int n, T s, T e) {
        // This function generates the knot vector according to the index
        std::vector<T> knotVector;
        knotVector.resize(order + n);
        std :: cout <<"Closed" << std::endl;
        T delta = (e - s) / n;
        for (int i = 0; i < order + n ; i++) {
            knotVector[i] = s + (i - 1) * delta;
        }
        return knotVector;
    }


    template <typename T>
    inline
        T MySurface<T>::makeW(int d, int i, T t, std::vector<T> knotVector) const {

            T W=(t-knotVector[i])/(knotVector[i+d]-knotVector[i]);
           return W;
    }

    template <typename T>
    inline
        T MySurface<T>::makeWD(int d, int i, T t, std::vector<T> knotVector) const {
        T WD=1/(knotVector[i+d]-knotVector[i]);
        return WD;
    }

    template <typename T>
    inline
        T MySurface<T>::makeB(T t) const {
        //Our B function is a polynomial function of first order
        T B=3*t*t-2*t*t*t;
        return B;
    }

    template <typename T>
    inline
        T MySurface<T>::makeBD(T t) const {
        //Our B function is a polynomial function of first order
        T BD=6*t-6*t*t;
        return BD;
    }

    template <typename T>
    inline
        GMlib::Vector<T,2> MySurface<T>::makeBlendSurf(int i, T t, std::vector<T> knotVector) const {
        GMlib::Vector<T,2> b;

        b[1]=makeB(makeW(degree,i,t, knotVector));
        b[0]=1-b[1];

        return b;

    }


    template <typename T>
    inline
        GMlib::Vector<T,2> MySurface<T>::makeBlendSurfD(int i, T t, std::vector<T> knotVector) const {
        GMlib::Vector<T,2> bD;

        bD[1]=makeBD(makeW(degree,i,t,knotVector)+ makeWD(degree, i, t,knotVector));
        bD[0]=-bD[1];

        return bD;

    }

    template <typename T>
    void MySurface<T>::localSimulate(double dt) {
        this->sample(50,0);
        this->setEditDone(true);

    }

    template <typename T>//implementation of the eval function that should return the scale
    void MySurface<T>::eval(T u, T v, int d1, int d2, bool lu = true, bool lv = true ) const {

        this->_p.setDim( d1 + 1, d2+1 );
        int i=findIndex(u,_u, _mSu->isClosedU());
        int j=findIndex(v,_v, _mSu->isClosedV());
        GMlib::Vector<T,2> bu = makeBlendSurf(i,u, _u);
        GMlib::Vector<T,2> bv = makeBlendSurf(j,v, _v);

        GMlib::Vector<T,2> dBu = makeBlendSurfD(i,u, _u);
        GMlib::Vector<T,2> dBv = makeBlendSurfD(j,v, _v);

        std::cout <<"u= "<< u << " i= " << i << " v= " << v << " j= " <<j<< std::endl;
        //std::cout <<"nu= "<< nu << " nv= " << nv << std::endl;
        GMlib::DMatrix<GMlib::Vector<T,3>> S1 = this->localSurf(i-1)(j-1)->evaluateParent(u, v,d1, d2);
        GMlib::DMatrix<GMlib::Vector<T,3>> S2 = this->localSurf(i)(j-1)->evaluateParent(u, v,d1, d2);
        GMlib::DMatrix<GMlib::Vector<T,3>> S3 = this->localSurf(i-1)(j)->evaluateParent(u, v,d1, d2);
        GMlib::DMatrix<GMlib::Vector<T,3>> S4 = this->localSurf(i)(j)->evaluateParent(u, v,d1, d2);
        //Surface component on x
        this->_p[0][0] = bv[0]*(bu[0]* S1[0][0] + bu[1]* S2[0][0]) + bv[1] *( bu[0]* S3[0][0] + bu[1]* S4[0][0]);
        //Surface component on y
        this->_p[0][1] = bv[0] * (dBu[0] * S1[0][0] + bu[0] *S1[0][1] + dBu[1] * S2[0][0] +bu[1] *S2[0][1]) +
                         bv[0] * (dBu[0] * S3[0][0] + bu[0] *S3[0][1] + dBu[1] * S4[0][0] +bu[1] *S4[0][1]) ;
        //Surface component on z
        this->_p[1][0] = dBv[0] * (bu[0]* S1[0][0] + bu[1]* S2[0][0]) + bv[0] * (bu[0] * S1[1][0] + bv[1]* S2[1][0]) +
                         dBv[1] * (bu[0]* S3[0][0] + bu[1]* S4[0][0]) + bv[0] * (bu[0] * S3[1][0] + bv[1]* S4[1][0]) ;


    }
    template <typename T>
    T MySurface<T>::getStartPU() const {
        return su;
    }

    template <typename T>
    T MySurface<T>::getEndPU()const {
        return eu;
    }

    template <typename T>
    T MySurface<T>::getStartPV() const {
        return sv;
    }

    template <typename T>
    T MySurface<T>::getEndPV()const {
        return ev;
    }

#endif // GM_PARAMETRICS_CURVES_MySurface_H
