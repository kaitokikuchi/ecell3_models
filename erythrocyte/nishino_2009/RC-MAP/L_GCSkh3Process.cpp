#include "libecs.hpp"
#include "Process.hpp"
#include "Stepper.hpp"
#include "ContinuousProcess.hpp"
USE_LIBECS;
 
LIBECS_DM_CLASS( L_GCSkh3Process, ContinuousProcess )
{
  
 public:
  
  LIBECS_DM_OBJECT( L_GCSkh3Process, Process ){
    
    INHERIT_PROPERTIES( Process );
    
    PROPERTYSLOT_SET_GET( Real, KmA );
    PROPERTYSLOT_SET_GET( Real, KmB );
    PROPERTYSLOT_SET_GET( Real, KmC );
    PROPERTYSLOT_SET_GET( Real, Ki );
    PROPERTYSLOT_SET_GET( Real, Vmf );
    PROPERTYSLOT_SET_GET( Real, alpha );
    
  }
  
  L_GCSkh3Process()
    :
    KmA( 0.0 ),
    KmB( 0.0 ),
    KmC( 0.0 ),
    Ki( 0.0 ),
    Vmf( 0.0 ),
    alpha( 0.0 )
    {
      ; // do nothing
    }
  
  SIMPLE_SET_GET_METHOD( Real, KmA );
  SIMPLE_SET_GET_METHOD( Real, KmB );
  SIMPLE_SET_GET_METHOD( Real, KmC );
  SIMPLE_SET_GET_METHOD( Real, Ki );
  SIMPLE_SET_GET_METHOD( Real, Vmf );
  SIMPLE_SET_GET_METHOD( Real, alpha );
  
  virtual void initialize(){
    
    Process::initialize();
    
    S0 = getVariableReference( "S0" );
    S1 = getVariableReference( "S1" );
    S2 = getVariableReference( "S2" );
    P0 = getVariableReference( "P0" );
    P1 = getVariableReference( "P1" );
    P2 = getVariableReference( "P2" );
    C0 = getVariableReference( "C0" );
    E0 = getVariableReference( "E0" );
    E1 = getVariableReference( "E1" );

    ConstAct = E0.getCoefficient();
    
  }
  
  virtual void fire(){
  
    Real velocity = 1;
  
    Real ActCo = E0.getVariable()->getValue();
    
    if( ActCo == 0 ){
      
      velocity = ConstAct;
      
    }
    
    else{

      Real A = 1000 * S0.getVariable()->getMolarConc(); // glutamate
      Real B = 1000 * S1.getVariable()->getMolarConc(); // cysteine
      Real C = 1000 * S2.getVariable()->getMolarConc(); // ATP
      Real GSH = 1000 * E1.getVariable()->getMolarConc(); // GSH

      Real KmAi = KmA * ( 1 + GSH / Ki );
      
      Real M = (A * B * C) / (alpha * KmAi * KmB * KmC);
      Real N = 1 + (A / KmA) + (A * B / (KmA * KmB)) + (A * C / (KmA 
	* KmC)) + (A * B * C / (alpha * KmA * KmB * KmC));
      
      velocity *= Vmf * N_A * S0.getVariable()->getSuperSystem()->getSize();
      velocity *= M / N;
      velocity *= ActCo / 1000 / 1000 / 3600;

    }

    setFlux( velocity );

}

 protected:
   
  VariableReference S0;
  VariableReference S1;
  VariableReference S2;
  VariableReference P0;
  VariableReference P1;
  VariableReference P2;
  VariableReference C0;
  VariableReference E0;
  VariableReference E1;
   
  Real ConstAct;
   
  Real KmA;
  Real KmB;
  Real KmC;
  Real Ki;
  Real Vmf;
  Real alpha;

};

LIBECS_DM_INIT( L_GCSkh3Process, Process );
