#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <array>

#include "enigma.hpp"

wireing mapStringToWires(std::string input)
{
	if(input == "1") return ROTOR_I;
	else if(input =="2") return ROTOR_II;
	else if(input =="3") return ROTOR_III;
	else if(input =="4") return ROTOR_IV;
	else return ROTOR_V;
}

struct config
{
	std::string reflector;
	std::array<wireing,3> rot;
	std::string plugboard;
	std::array<int,3> p;
	std::array<int,3> r;
};

#define ERROR {std::cerr<<"not suported or bad config"<<std::endl;exit(-1);}

config generate_config(std::string input)
{
	config ret;
	std::stringstream ss(input);
	std::string temp;
	getline(ss,temp,';');
	if(temp=="B") ret.reflector = REFLECTOR_B;
	else if(temp=="C") ret.reflector = REFLECTOR_C;
	else ERROR
	getline(ss,temp,';');
	std::stringstream t(temp);
	for(int i=0;i<3;i++)
	{
		std::string a;
		getline(t,a,',');
		if(a=="I") ret.rot[i]=ROTOR_I;
		else if(a=="II") ret.rot[i]=ROTOR_II;
		else if(a=="III") ret.rot[i]=ROTOR_III;
		else if(a=="IV") ret.rot[i]=ROTOR_IV;
		else if(a=="V") ret.rot[i]=ROTOR_V;
		else ERROR
	}
	getline(ss,temp,';');
	std::stringstream tt(temp);
	for(int i=0;i<3;i++)
	{
		std::string a;
		getline(tt,a,',');
		ret.p[i]=std::stoi(a);
	}
	getline(ss,temp,';');
	std::stringstream ttt(temp);
	for(int i=0;i<3;i++)
	{
		std::string a;
		getline(ttt,a,',');
		ret.r[i]=std::stoi(a);
	}
	getline(ss,temp);
	ret.plugboard = temp;
	return ret;
}

int main(int argc,char ** args)
{
	if(argc <= 2) {  return -1;	}
	
	auto c =generate_config(args[1]);
	std::string input=args[2];
	EnigmaMachine e(c.reflector,c.rot[0],c.rot[1],c.rot[2],c.plugboard);
	std::cout<<input<<std::endl;
	e.set_position(c.p[0],c.p[1],c.p[2]);
	e.set_ring(c.r[0],c.r[1],c.r[2]);
	std::string aaa = e.encrypt(input);
	std::cout<<aaa<<std::endl;
	e.set_position(c.p[0],c.p[1],c.p[2]);
	std::cout<<e.encrypt(aaa)<<std::endl;

	return 0;
}
