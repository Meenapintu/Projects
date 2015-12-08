/*
* Copyright (c) 2006-2009 Erin Catto http://www.box2d.org
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

/* 
 * Base code for CS 296 Software Systems Lab 
 * Department of Computer Science and Engineering, IIT Bombay
 * Instructor: Parag Chaudhuri
 */

#ifndef _DOMINOS_HPP_
#define _DOMINOS_HPP_

#ifndef DEGTORAD
#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#endif

//structure to store current surface velocity of a fixture

namespace cs296
{
  
  class dominos_t : public base_sim_t
  {
  public:
    dominos_t();
      
    void keyboard(unsigned char key);
	static base_sim_t* create()
	{
		return new dominos_t;
	}
	private:
	///This is the base on which the the rectangles that form the hind section of missile rests.
	b2Body* m_bodyA;
	///This body is used to drag the cone from base to lift
	/**
	 * There exist a prismatic joint between m_bodyA and m_bodyB.
	 */ 
	b2Body* m_bodyB;
	///This is a static body on which m_bodyA1 slides using prismatic joint
	b2Body* m_bodyA1;
	///This body is used to drag the hind section of missile to lift
	///There exist a prismatic joint between m_bodyA1 and m_bodyB1
	b2Body* m_bodyB1;
	///This body is used to drag missile from one lift to another
	b2Body* m_bodyB2;
	///This body is used to drag missile from lift to missile launcher
	b2Body* m_bodyB3;
	///This is the conical part of missile		
	b2Body* m_cone;
	///This is the base on which the conical part of missile rests initially
	b2Body* m_conebase;
	///This is the lift which carries the missile from base to middle
	b2Body* m_lift;
	///This is a static body along which M_lift slides using prismatic joint
	b2Body* m_wall;
	///This is just a static wall
	b2Body* m_wall2;
	///This is the lift that carries missile from middle to top (to the launcher)
	b2Body* m_lift3;
	///This is a static body along which M_lift3 slides using prismatic joint
	b2Body* m_wall3;
	///This is an array of rectangles that form the hind section of missiles
	b2Body* domin[4];
	///This is the body that makes the base of missile launcher
	b2Body* gunbase;
	///This is an invisible body about which missile launcher rotates
	b2Body* gunanch;
	///This forms the top of missile launcher
	b2Body* guntop;
	///This is the prismatic joint between m_bodyA and m_bodyB
	b2PrismaticJoint* m_joint;
	///This is the prismatic joint between m_bodyA1 and m_bodyB1
	b2PrismaticJoint* m_joint1;
	///This is the prismatic joint between m_lift and m_bodyB2
	b2PrismaticJoint* m_joint2;
	///This is the prismatic joint between m_lift3 and m_bodyB3
	b2PrismaticJoint* m_joint3;
	///This is the prismatic joint between m_lift3 and m_wall3
	b2PrismaticJoint* m_joint_lift3;		
	///This is the prismatic joint between m_lift and m_wall
	b2PrismaticJoint* m_joint_lift;
	///This is the revolute joint between gunbase and gunanch
	b2RevoluteJoint* m_base;
	///This is the revolute joint between guntop and gunanch
	b2RevoluteJoint* m_top;
	 

  };
}
  
#endif
