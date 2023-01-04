#include <webots/Robot.hpp>
#include <webots/Motor.hpp>
#include <webots/DistanceSensor.hpp>


#define TIME_STEP 64
#define MAX_SPEED 6.28
using namespace webots;

int main(int argc, char **argv) {
  Robot *robot = new Robot();
  
  //Obtenemos los motores izquierdo y derecho respectivamente
  //Los nombres vienen definidos en la documentacion del e-puck
  Motor *right_motor = robot->getMotor("right wheel motor");
  Motor *left_motor = robot->getMotor("left wheel motor");
  
  //Ponemos los motores en infinito
  right_motor->setPosition(INFINITY);
  left_motor->setPosition(INFINITY);
  
  //Ponemos la velocidad de los motores en 0 no se mueve
  right_motor->setVelocity(0.0);
  left_motor->setVelocity(0.0);
  
  //Obtenemos los sensores de proximidad que tiene el e-puck
  //Solo utilizaremos los tres primeros sensores 
  DistanceSensor *prox_sensors[3];
  for(int i=0;i<3;i++){
    std::string ps_name = "ps" + std::to_string(i);
    prox_sensors[i] = robot->getDistanceSensor(ps_name);
    prox_sensors[i]->enable(TIME_STEP);
    //Obtenemos los sensores de proximidad y los activamos
  }
  
  while (robot->step(TIME_STEP) != -1) {
    //Variables de la velocidad de los motores
    double right_s = MAX_SPEED;
    double left_s = MAX_SPEED;
        
    //Si el sensor detecta algo al frente, gira a la izquierda
    //para seguir la pared derecha 
    if(prox_sensors[0]->getValue() > 100){
      left_s = -MAX_SPEED;
      right_s = MAX_SPEED;
    }else{
      //si el sensor detecta la pared derecha, sigue adelante
      if(prox_sensors[2]->getValue() > 100)left_s = right_s = MAX_SPEED;
      else{
        //Si no detecta ninguna pared gira a la izquierda
        //para encontrar la pared derecha 
        left_s = MAX_SPEED;
        right_s = MAX_SPEED/2;
      }
      //Este sensor se usa para cuando este en una arista
      //este lo que hace es girar a la derecha para encontrar
      //la pared derecha 
      if(prox_sensors[1]->getValue() >100){
        left_s = MAX_SPEED/2;
        right_s = MAX_SPEED;
      }
    }
    
    //Ponemos la velocidad a los motores
    right_motor->setVelocity(right_s);
    left_motor->setVelocity(left_s);
    
  };

  delete robot;
  return 0;
}
