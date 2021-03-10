#include "Character.h" 

Character::Character(string _name, string _type, int _attack, int _defense, int _remainingHealth, int _nMaxRounds) 
{

	this->name= _name;
	this->type= _type;
	this->attack= _attack;
	this->defense= _defense;
	this->remainingHealth= _remainingHealth;
	this->nMaxRounds= _nMaxRounds;

	this->isAlive= true;
	this->nRoundsSinceSpecial=0;
	
	this->healthHistory= new int[nMaxRounds+1];
	this->healthHistory[0]=remainingHealth;

}

Character::Character(const Character& character) 
{

	this->name= character.name;
	this->type=character.type;
	this->attack=character.attack;
	this->defense= character.defense;
	this->remainingHealth=character.remainingHealth;
	this->nMaxRounds=character.nMaxRounds;

	this->isAlive= character.isAlive;
	this->nRoundsSinceSpecial=character.nRoundsSinceSpecial;
	
	this->healthHistory= new int[this->nMaxRounds+1];

	for(int i=0; i<nMaxRounds+1; i++){

		this->healthHistory[i]=character.healthHistory[i];
	}

}

Character& Character::operator=(const Character& character) 
{

	if(this==&character)
	{
		return *this;
	}
	
	this->name= character.name;
	this->type=character.type;
	this->attack=character.attack;
	this->defense= character.defense;
	this->remainingHealth=character.remainingHealth;
	this->nMaxRounds=character.nMaxRounds;

	this->isAlive= character.isAlive;
	this->nRoundsSinceSpecial=character.nRoundsSinceSpecial;

	for(int i=0; i<nMaxRounds+1; i++){

		this->healthHistory[i]=character.healthHistory[i];
	}

	return *this;
}

bool Character::operator<(const Character& other) 
{   

	for(int i=0; i<this->name.length(); i++) 
	{											  
		
		if(this->name[i]<other.name[i])
		{
			return true;
		}
		if(this->name[i]>other.name[i])
		{
			return false;
		}
		
	}

	if(this->name.length()!=other.name.length())
	{
		return true;
	}else{

		return false;
	}
	
}



Character::~Character() {

	delete [] healthHistory;

}
