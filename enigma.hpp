#pragma once
#include <string>
#include <vector>
#include <utility>

class IEncryptionElement
{
	public:
	virtual char encrypt(char ch) = 0;
	virtual void push() = 0;
};

class Plugboard : public IEncryptionElement
{
	IEncryptionElement* child;
	std::vector<std::pair<char,char> > pairs;
	char swich(char ch)
	{
		for(auto i : pairs)
		{
			if(i.first == ch) return i.second;
			if(i.second == ch) return i.first;
		}
		return ch;
	}
	public:
	Plugboard(std::string config,IEncryptionElement* c=nullptr)
	{
		child = c;
		if(config.size()%2==1)
			throw new int; //fixeme 
		for(int i  = 0;i < config.size() ;i+=2)
		{
			pairs.push_back(std::make_pair(config[i],config[i+1]));
		}
	}
	virtual char encrypt(char ch)
	{
		//std::cout<<"DEBUG PLUGBOARD : "<<ch<<std::endl;
		return this->swich( (child==nullptr)?(ch):(child->encrypt(this->swich(ch))) );
	}
	virtual void push()
	{
		if(child==nullptr)
			return;
		else
			child->push();
	}
};

class Rotor : public IEncryptionElement
{
	IEncryptionElement* child;
	int offset;
	int ring;
	std::vector<int> wires;
	int notch;//fixme or update
	public:
	Rotor(IEncryptionElement* c,std::string config,int n)
	{
		child = c;
		offset = 1;
		notch=n;
		ring=1;
		for(int i = 0;i<config.size();i++)
		{
			wires.push_back((config[i]-('A'+i-26))%26);
		}
	}
	virtual char encrypt(char ch)
	{		
		char ret  = (((ch-'A')+wires[ (ch-'A' + (offset-ring)) %wires.size() ] )% 26)+'A' ;
		ret  = child->encrypt(ret);
		//std::cout<<"DEBUG ROTOR : "<<ret<<std::endl;
		int i=0;	
		for(;i<wires.size();i++)
		{
			if (( ( ( i )+wires[ ( i  + (offset-ring) ) %wires.size() ] ) % 26)+'A' == ret ) 	break;
		}
		//std::cout<<"DEBUG ROTOR : "<<i+'A'<<std::endl;
		return i+'A';	
	}
	virtual void push()
	{	
		if(offset == notch)
			child->push();	
		offset++;
		offset = offset % 26;
	}
	void set_offset(char offset)
	{
		this->offset = ((int)offset-'A' )%26;
	}
	void set_ring(char ring)
	{
		this->ring = (int)ring-'A';
	}
};

struct wireing
{
	std::string wires;
	int notch;
};

const wireing ROTOR_I 	=		wireing{"EKMFLGDQVZNTOWYHXUSPAIBRCJ",16};
const wireing ROTOR_II	=		wireing{"AJDKSIRUXBLHWTMCQGZNPYFVOE", 4};
const wireing ROTOR_III = 	wireing{"BDFHJLCPRTXVZNYEIWGAKMUSQO",21};
const wireing ROTOR_IV  = 	wireing{"ESOVPZJAYQUIRHXLNFTGKDCMWB", 9};
const wireing ROTOR_V   = 	wireing{"VZBRGITYUPSDNHLXAWMJQOFECK",25};


const std::string REFLECTOR_B("AYBRCUDHEQFSGLIPJXKNMOTZVW");
const std::string REFLECTOR_C("AFBVCPDJEIGOHYKRLZMXNWQTSU");

std::string clear(std::string input)
{
	for(int i = 0; i < input.size();i++)
	{
		if(input[i]<'A' or input[i]>'Z')
		{	
			input.erase(i,1);i--;
		}		
	}	
	return input;
}

class EnigmaMachine
{
	Plugboard * reflector;
	Rotor * r1;
	Rotor * r2;
	Rotor * r3;
	Plugboard * plugboard;

	public:
	EnigmaMachine(std::string reflector,const wireing rotor1,const wireing rotor2,const wireing rotor3,std::string configplugboard)
	{
		this->reflector = new Plugboard(reflector);
		r1 = new Rotor(this->reflector,rotor1.wires,rotor1.notch);
		r2 = new Rotor(r1,rotor2.wires,rotor2.notch);
		r3 = new Rotor(r2,rotor3.wires,rotor3.notch);
		plugboard = new Plugboard(configplugboard,r3);
	}
	
	std::string encrypt(std::string input )
	{
		std::string ret;
		for(auto i : input)
		{
			plugboard->push();
			ret+=plugboard->encrypt(i);
		}
		return ret;
	}
	void set_position(int a,int b,int c)
	{
		r1->set_offset(a);
		r2->set_offset(b);
		r3->set_offset(c);
	}
	void set_ring(int a,int b,int c)
	{
		r1->set_ring(a);
		r2->set_ring(b);
		r3->set_ring(c);
	}
	virtual ~EnigmaMachine()
	{
		delete r1;
		delete r2;
		delete r3;
		delete plugboard;
	}
};
