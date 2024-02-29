
#include <iostream>

#include "scenario.h"
#include "testtorus.h"



// hidmanager
#include "hidmanager/defaulthidmanager.h"


// gmlib
#include <scene/light/gmpointlight.h>
#include <scene/sceneobjects/gmpathtrack.h>
#include <scene/sceneobjects/gmpathtrackarrows.h>
#include "mySpline.h"
#include "myStar.h"
#include "myBlendingSpline.h"
#include "mySubCurve.h"
#include "mySurface.h"
#include <parametrics/curves/gmpcircle.h>
#include <parametrics/curves/gmpline.h>
#include <parametrics/surfaces/gmpbeziersurf.h>
#include <parametrics/surfaces/gmpplane.h>
#include <parametrics/surfaces/gmpcylinder.h>
#include <parametrics/visualizers/gmpsurfnormalsvisualizer.h>

// qt
#include <QQuickItem>


template <typename T>
inline
std::ostream& operator<<(std::ostream& out, const std::vector<T>& v) {
  out << v.size() << std::endl;
  for(uint i=0; i<v.size(); i++) out << " " << v[i];
  out << std::endl;
  return out;
}




void Scenario::initializeScenario() {

  // Insert a light
  GMlib::Point<GLfloat,3> init_light_pos( 2.0, 4.0, 10 );
  GMlib::PointLight *light = new GMlib::PointLight(  GMlib::GMcolor::white(), GMlib::GMcolor::white(),
                                                     GMlib::GMcolor::white(), init_light_pos );
  light->setAttenuation(0.8f, 0.002f, 0.0008f);
  this->scene()->insertLight( light, false );

  // Insert Sun
  this->scene()->insertSun();

  // Default camera parameters
  int init_viewport_size = 600;
  GMlib::Point<float,3>  init_cam_pos( 0.0f, 0.0f, 0.0f );
  GMlib::Vector<float,3> init_cam_dir( 0.0f, 1.0f, 0.0f );
  GMlib::Vector<float,3> init_cam_up(  1.0f, 0.0f, 0.0f );

  // Projection cam
  auto proj_rcpair = createRCPair("Projection");
  proj_rcpair.camera->set(init_cam_pos,init_cam_dir,init_cam_up);
  proj_rcpair.camera->setCuttingPlanes( 1.0f, 8000.0f );
  proj_rcpair.camera->rotateGlobal( GMlib::Angle(-45), GMlib::Vector<float,3>( 1.0f, 0.0f, 0.0f ) );
  proj_rcpair.camera->translateGlobal( GMlib::Vector<float,3>( 0.0f, -20.0f, 20.0f ) );
  scene()->insertCamera( proj_rcpair.camera.get() );
  proj_rcpair.renderer->reshape( GMlib::Vector<int,2>(init_viewport_size, init_viewport_size) );


  /***************************************************************************
   *                                                                         *
   * Standar example, including path track and path track arrows             *
   *                                                                         *
   ***************************************************************************/

  GMlib::Material mm(GMlib::GMmaterial::polishedBronze());
  mm.set(45.0);

  // auto ptom = new TestTorus(1.0f, 0.4f, 0.6f);
  // ptom->toggleDefaultVisualizer();
  // ptom->sample(60,60,1,1);
  // this->scene()->insert(ptom);
  // auto ptrack = new GMlib::PathTrack();
  // ptrack->setLineWidth(2);
  // ptom->insert(ptrack);
  // auto ptrack2 = new GMlib::PathTrackArrows();
  // ptrack2->setArrowLength(2);
  // ptom->insert(ptrack2);
  GMlib::DVector<GMlib::Vector<float,3>> C(7);
  C[0]={5.0f, 2.0f, 8.0f};
  C[1]={2.0f, 9.0f, 2.0f};
  C[2]={2.0f, 3.0f, 4.0f};
  C[3]={3.0f, 4.0f, 5.0f};
  C[4]={3.0f, 6.0f, 0.0f};
  C[5]={1.0f, 4.0f, 0.0f};
  C[6]={5.0f, 8.0f, 3.0f};

  // auto firstSpline= new MySpline<float>(C);
  // firstSpline->toggleDefaultVisualizer();
  // firstSpline->sample(60,0);
  // this->scene()->insert(firstSpline);


  // auto ACircle= new GMlib::PCircle<float>(5.0);
  // ACircle->toggleDefaultVisualizer();
  // ACircle->sample(60,0);
  // this->scene()->insert(ACircle);

  // GMlib::DVector<GMlib::Vector<float,3>> PV(20);
  // for (int i=0; i<20; i++){
  //     float t = ACircle->getParStart()+i*ACircle->getParDelta()/19;
  //     PV[i]=ACircle->getPosition(t);
  // }

  // auto secondSpline= new MySpline<float>(PV,8);
  // secondSpline->toggleDefaultVisualizer();
  // secondSpline->sample(60,0);
  // secondSpline->setColor(GMlib::GMcolor::blue());
  // this->scene()->insert(secondSpline);

  // auto mCu= new MyStar<float>(C);
  // mCu->toggleDefaultVisualizer();
  // mCu->sample(60,0);
  // mCu->setColor(GMlib::GMcolor::green());
  // this->scene()->insert(mCu);

  // auto blendingSpline= new MyBlendingSpline<float>(mCu, 10);
  // blendingSpline->toggleDefaultVisualizer();
  // blendingSpline->sample(50,0);
  // blendingSpline->setColor(GMlib::GMcolor::red());
  // this->scene()->insert(blendingSpline);


  GMlib::DMatrix<GMlib::Vector<float,3>> vec(2,2, float(0) );
  vec[0][0]={1.0f, 4.0f, 3.0f};
  vec[0][1]={1.0f, 1.0f, 2.0f};
  vec[1][0]={2.0f, 3.0f, 4.0f};
  vec[1][1]={3.0f, 4.0f, 5.0f};

  // Bezier Surface as a model Surface
  // auto mSu = new GMlib::PBezierSurf<float>(vec);
  // mSu->toggleDefaultVisualizer();
  // mSu->sample(50, 50, 1, 1);
  // mSu->setMaterial(GMlib::GMmaterial::ruby());
  // this->scene()->insert(mSu);



  // Plane as a Model Surface
  // GMlib::Point<float,3>p;
  // p={3.0f, 4.0f, 5.0f};
  // auto mSu = new GMlib::PPlane<float>(p, vec[0][0], vec[0][1]);
  // mSu->toggleDefaultVisualizer();
  // mSu->sample(50, 50, 1, 1);
  // mSu->setMaterial(GMlib::GMmaterial::ruby());
  // this->scene()->insert(mSu);

  //Cylinder
  // GMlib::Point<float,3>p;
  // p={3.0f, 4.0f, 5.0f};
  // auto mCy = new GMlib::PCylinder<float>(2.0f,3.5f, 5.0f);
  // mCy->toggleDefaultVisualizer();
  // mCy->sample(50, 50, 1, 1);
  // mCy->setMaterial(GMlib::GMmaterial::ruby());
  // this->scene()->insert(mCy);

  auto mTo = new GMlib::PTorus<float>(1.0f, 0.4f, 0.6f);
  mTo->toggleDefaultVisualizer();
  mTo->sample(50, 50, 1, 1);
  mTo->setMaterial(GMlib::GMmaterial::ruby());
  this->scene()->insert(mTo);

  //auto * n_viz= new GMlib::PSurfNormalsVisualizer<float,3>;
  auto blendingSu = new MySurface<float>(mTo, 3, 3);
  blendingSu->toggleDefaultVisualizer();
  //blendingSu->insertVisualizer(n_viz);
  blendingSu->sample(30, 30, 1, 1);
  //blendingSu->setMaterial(GMlib::GMmaterial::ruby());
  this->scene()->insert(blendingSu);

  // auto line = new GMlib::PLine<float>(GMlib::Point<float,3>(C[0]),GMlib::Point<float,3>(C[1]));
  // line->toggleDefaultVisualizer();
  // line->sample(2,0);
  // line->setColor(GMlib::GMcolor::yellow());
  // this->scene()->insert(line);

  // line = new GMlib::PLine<float>(GMlib::Point<float,3>(C[1]),GMlib::Point<float,3>(C[2]));
  // line->toggleDefaultVisualizer();
  // line->sample(2,0);
  // line->setColor(GMlib::GMcolor::yellow());
  // this->scene()->insert(line);

  // line = new GMlib::PLine<float>(GMlib::Point<float,3>(C[2]),GMlib::Point<float,3>(C[3]));
  // line->toggleDefaultVisualizer();
  // line->sample(2,0);
  // line->setColor(GMlib::GMcolor::yellow());
  // this->scene()->insert(line);

  // line = new GMlib::PLine<float>(GMlib::Point<float,3>(C[3]),GMlib::Point<float,3>(C[4]));
  // line->toggleDefaultVisualizer();
  // line->sample(2,0);
  // line->setColor(GMlib::GMcolor::yellow());
  // this->scene()->insert(line);

  // line = new GMlib::PLine<float>(GMlib::Point<float,3>(C[4]),GMlib::Point<float,3>(C[5]));
  // line->toggleDefaultVisualizer();
  // line->sample(2,0);
  // line->setColor(GMlib::GMcolor::yellow());
  // this->scene()->insert(line);

  // line = new GMlib::PLine<float>(GMlib::Point<float,3>(C[5]),GMlib::Point<float,3>(C[6]));
  // line->toggleDefaultVisualizer();
  // line->sample(2,0);
  // line->setColor(GMlib::GMcolor::yellow());
  // this->scene()->insert(line);

  // auto subDivCurve= new MySubCurve<float>(C);
  // subDivCurve->toggleDefaultVisualizer();
  // subDivCurve->sample(5,2);
  // subDivCurve->setColor(GMlib::GMcolor::red());
  // this->scene()->insert(subDivCurve);

}




void Scenario::cleanupScenario() {

}




void Scenario::callDefferedGL() {

  GMlib::Array< const GMlib::SceneObject*> e_obj;
  this->scene()->getEditedObjects(e_obj);

  for(int i=0; i < e_obj.getSize(); i++)
    if(e_obj(i)->isVisible()) e_obj[i]->replot();
}

