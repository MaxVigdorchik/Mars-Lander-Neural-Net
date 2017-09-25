// Mars lander simulator
// Version 1.9
// Mechanical simulation functions
// Gabor Csanyi and Andrew Gee, August 2016

// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation, to make use of it
// for non-commercial purposes, provided that (a) its original authorship
// is acknowledged and (b) no modified versions of the source code are
// published. Restriction (b) is designed to protect the integrity of the
// exercise for future generations of students. The authors would be happy
// to receive any suggested modifications by private correspondence to
// ahg@eng.cam.ac.uk and gc121@eng.cam.ac.uk.

#include "lander.h"

void autopilot (void)
// Autopilot to adjust the engine throttle, parachute and attitude control
{
    double K_h = 0.0190; //0.0413 maximum for scenario 1, [0.17, 0.0190] range for scenario 5
    double K_p = 0.055; //0.55 Works best for 0.0190 K_h
    double h = position.abs() - MARS_RADIUS;
    double descent_rate = velocity * position.norm();
    double error = -(0.5 + K_h*h + descent_rate);
    double P_out = K_p * error; //Proportional control.

    double mass = UNLOADED_LANDER_MASS + FUEL_DENSITY * fuel * FUEL_CAPACITY;
    double weight = GRAVITY * MARS_MASS * mass / position.abs2();
    double atm_density = atmospheric_density(position);
    double lander_drag = (0.5) * atm_density * DRAG_COEF_LANDER * LANDER_AREA *
	velocity.abs2() * (velocity.norm() * position.norm()); //Last term for direction
    //Should check if delta > 1 (essentially out of control, gravity overpowering thrust)
    double delta = (weight + lander_drag) / MAX_THRUST;
    if(delta > 1)
	delta = 1; //not much else to do here
    else if(delta < 0) //drag overpowering, eventually speed will slow enough that this isnt the case
	delta = 0;

    if(P_out <= -delta)
	throttle = 0;
    else if(-delta < P_out && P_out < (1 - delta))
	throttle = delta + P_out;
    else
	throttle = 1;

    ofstream fout;
    fout.open("results.txt", std::ios_base::app); //Works in append Mode
    fout << h << ' ' << descent_rate << endl;
}

void numerical_dynamics (void)
// This is the function that performs the numerical integration to update the
// lander's pose. The time step is delta_t (global variable).
{
    static vector3d previous_position = position; //init for first time
    
    double lander_mass = UNLOADED_LANDER_MASS + FUEL_DENSITY * fuel * FUEL_CAPACITY;
    double atm_density = atmospheric_density(position);
    double proj_area = 1; //This may be useful in the future, for now assume like a sphere
    
    vector3d gravity = - GRAVITY * MARS_MASS * lander_mass * position.norm() / (position.abs2());
    vector3d thrust = thrust_wrt_world();
    vector3d lander_drag = (-0.5) * atm_density * DRAG_COEF_LANDER * LANDER_AREA *
	proj_area * velocity.abs2() * velocity.norm();
    vector3d parachute_drag;
    if(parachute_status == DEPLOYED)
	parachute_drag = -1/2 * atm_density * DRAG_COEF_LANDER * PARACHUTE_AREA *
	     velocity.abs2() * velocity.norm();
    else
	parachute_drag = vector3d(0,0,0);

    vector3d acceleration = (gravity + thrust + lander_drag + parachute_drag) / lander_mass;
    if(simulation_time == 0.0)
    {
	previous_position = position;
	position = position + delta_t * velocity;
	velocity = velocity + delta_t * acceleration;
	return; //Just gets the proper initialization in this timestep, then leaves function.
    }
    vector3d current_position = position; //Lets previous position get reset
    
    if(EULER) //No need to use previous positions for this
    {
	position = position + delta_t * velocity;
	velocity = velocity + delta_t * acceleration;
	previous_position = current_position;
    }
    else
    {
	position = 2 * position - previous_position + delta_t * delta_t * acceleration;
	velocity = (0.5 / delta_t) * (position - previous_position);
	previous_position = current_position; //Updated with the position at this function's start
    }
#ifdef LANDER_MAIN
    // Here we can apply an autopilot to adjust the thrust, parachute and attitude
    if (autopilot_enabled && !neural) autopilot();
    else if(autopilot_enabled && neural) neural_autopilot();
    
    // Here we can apply 3-axis stabilization to ensure the base is always pointing downwards
    if (stabilized_attitude) attitude_stabilization();
#endif   
}

void neural_autopilot(void)
{
    Network *net = autopilot_net;
    vector<NNode*>::iterator out_iter;
    
    double in[12];
    in[0] = 1.0; //Constant bias input
    in[1] = position.x / 100000;
    in[2] = position.y / 100000;
    in[3] = position.z / 100000;
    in[4] = velocity.x / 10;
    in[5] = velocity.y / 10;
    in[6] = velocity.z / 10;
    in[7] = fuel;
    in[8] = orientation.x; //orientation probably not needed for simple cases
    in[9] = orientation.y;
    in[10] = orientation.z;
    in[11] = parachute_status;

    net->load_sensors(in);
    if(!(net->activate())) return; 
    out_iter = net->outputs.begin(); 
    throttle = (*out_iter)->activation;
    ++out_iter;
    parachute_status = static_cast<parachute_status_t>((*out_iter)->activation > 0.9);
}

void initialize_simulation (void)
// Lander pose initialization - selects one of 10 possible scenarios
{
    // The parameters to set are:
    // position - in Cartesian planetary coordinate system (m)
    // velocity - in Cartesian planetary coordinate system (m/s)
    // orientation - in lander coordinate system (xyz Euler angles, degrees)
    // delta_t - the simulation time step
    // boolean state variables - parachute_status, stabilized_attitude, autopilot_enabled
    // scenario_description - a descriptive string for the help screen
    remove("results.txt");
    
    scenario_description[0] = "circular orbit";
    scenario_description[1] = "descent from 10km";
    scenario_description[2] = "elliptical orbit, thrust changes orbital plane";
    scenario_description[3] = "polar launch at escape velocity (but drag prevents escape)";
    scenario_description[4] = "elliptical orbit that clips the atmosphere and decays";
    scenario_description[5] = "descent from 200km";
    scenario_description[6] = "";
    scenario_description[7] = "";
    scenario_description[8] = "";
    scenario_description[9] = "";

    switch (scenario) {

    case 0:
	// a circular equatorial orbit
	position = vector3d(1.2*MARS_RADIUS, 0.0, 0.0);
	velocity = vector3d(0.0, -3247.087385863725, 0.0);
	orientation = vector3d(0.0, 90.0, 0.0);
	delta_t = 0.1;
	parachute_status = NOT_DEPLOYED;
	stabilized_attitude = false;
	autopilot_enabled = false;
	break;

    case 1:
	// a descent from rest at 10km altitude
	position = vector3d(0.0, -(MARS_RADIUS + 10000.0), 0.0);
	velocity = vector3d(0.0, 0.0, 0.0);
	orientation = vector3d(0.0, 0.0, 90.0);
	delta_t = 0.1;
	parachute_status = NOT_DEPLOYED;
	stabilized_attitude = false;
	autopilot_enabled = true;
	break;

    case 2:
	// an elliptical polar orbit
	position = vector3d(0.0, 0.0, 1.2*MARS_RADIUS);
	velocity = vector3d(3500.0, 0.0, 0.0);
	orientation = vector3d(0.0, 0.0, 90.0);
	delta_t = 0.1;
	parachute_status = NOT_DEPLOYED;
	stabilized_attitude = false;
	autopilot_enabled = true;
	break;

    case 3:
	// polar surface launch at escape velocity (but drag prevents escape)
	position = vector3d(0.0, 0.0, MARS_RADIUS + LANDER_SIZE/2.0);
	velocity = vector3d(0.0, 0.0, 5027.0);
	orientation = vector3d(0.0, 0.0, 0.0);
	delta_t = 0.1;
	parachute_status = NOT_DEPLOYED;
	stabilized_attitude = false;
	autopilot_enabled = true;
	break;

    case 4:
	// an elliptical orbit that clips the atmosphere each time round, losing energy
	position = vector3d(0.0, 0.0, MARS_RADIUS + 100000.0);
	velocity = vector3d(4000.0, 0.0, 0.0);
	orientation = vector3d(0.0, 90.0, 0.0);
	delta_t = 0.1;
	parachute_status = NOT_DEPLOYED;
	stabilized_attitude = false;
	autopilot_enabled = true;
	break;

    case 5:
	// a descent from rest at the edge of the exosphere
	position = vector3d(0.0, -(MARS_RADIUS + EXOSPHERE), 0.0);
	velocity = vector3d(0.0, 0.0, 0.0);
	orientation = vector3d(0.0, 0.0, 90.0);
	delta_t = 0.1;
	parachute_status = NOT_DEPLOYED;
	stabilized_attitude = true;
	autopilot_enabled = true;
	break;

    case 6:
	break;

    case 7:
	break;

    case 8:
	break;

    case 9:
	break;

    }
}
