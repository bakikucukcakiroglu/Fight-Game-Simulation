#include "Character.h"
#include <fstream>
#include <iostream>


using namespace std;

void sortByName(Character** community[]);
Character** properAlive(Character** member, Character**community[], int memberIndex);
bool warChecker(Character** community[], int deadCounter);

int main(int argc, char* argv[]) 
{

	ifstream read(argv[1]); 	//provides us toread input from input file
	ofstream write(argv[2]);	//provides us to write output to given output file

	
	Character** two_communities = new Character*[10]; //creates an array which has a length of 10 and keeps member of both communities

	int nMaxRounds;		//maximum round number the war can be continue

	read>>nMaxRounds;	

	for(int i = 0; i < 10; i++) 	// this loop creates charachters and fill them into an array
	{

		string name;			//keeps the name of the character 
		string type;			//keeps the type of the character
		int attack;				//keeps the attack point of the character
		int defense;			//keeps the defense point of the character
		int remainingHealth;	//keeps the remaning helath of the character

		read>>name>>type>>attack>>defense>>remainingHealth;

		two_communities[i]= new Character(name, type, attack, defense, remainingHealth, nMaxRounds); //creates the chracter with given fields and put it into an array
		
		
	}


	Character** community_1[5]={&two_communities[0],&two_communities[1],&two_communities[2],&two_communities[3],&two_communities[4]}; 	//array of community1
	Character** community_2[5]={&two_communities[5],&two_communities[6],&two_communities[7],&two_communities[8],&two_communities[9]};	//array of community2

	sortByName(community_1);	//sorts community1 array by members' name(alphabetical order)
	sortByName(community_2);	//sorts community1 array by members' name(alphabetical order)



	bool warContinue=true; 				//checks if war conditions proper to continuty of war
	string attackerName;				//name of the attacker of the round
	string defenseName;					//name of the defender of the round
	string usingSpecial;				//indicates if attacker using superpower or not
	int roundCounter=0;					//counts number of rounds
	Character** lastKilled_1=nullptr;	//keeps the last killed member of community1
	Character** lastKilled_2=nullptr;	//keeps the last killed member of community2
	int deadCounter_1=0;				//counts deads of community1
	int deadCounter_2=0;				//counts deads of community2
	
	while(roundCounter<nMaxRounds && warContinue)
	{	
		roundCounter++;	//every round increments round number by 1

		
		for(int i=0; i<10; i++){	//every round adds the healt point of previous round to recent health history index to provide
									// health history from being noncurrent for characters whose helath doesn't change this round

			two_communities[i]->healthHistory[roundCounter]=two_communities[i]->healthHistory[roundCounter-1];
		}

		read>>attackerName>>defenseName>>usingSpecial;

		Character** attacker;	//keeps the attacker character's pointer 
		Character** defender;	//keeps the defender character's pointer
		int attackerIndex;		//keeps the attacker character's index in its community array
		int defenderIndex;		//keeps the defender character's index in its community array


		for(int i=0; i<5; i++)	//finds the attacker and the defender from given names 
		{

			if(roundCounter%2==1) 	//if round number is odd it means that community1 is attacking
			{

				if((**community_1[i]).name==attackerName)	// seeks for the character with the given name in community1 array
				{
					attacker=community_1[i];				//equalize attacker pointer to character with the given attacker name
					attackerIndex=i;
				}
				if((**community_2[i]).name==defenseName) 	// seeks for the character with the given name in community2 array
				{
					defender=community_2[i];				//equalize defender pointer to character with the given defender name
					defenderIndex=i;
				}
			}else{					//if round number is even it means that community2 is attacking

				if((**community_2[i]).name==attackerName)	// seeks for the character with the given name in community2 array
				{
					attacker=community_2[i];				//equalize attacker pointer to character with the given attacker name
					attackerIndex=i;

				}
				if((**community_1[i]).name==defenseName)	// seeks for the character with the given name in community1 array
				{
					defender=community_1[i];				//equalize defender pointer to character with the given defender name
					defenderIndex=i;
				}

			}
		}

		if(roundCounter%2==1) 	//if round number is odd it means that community1 is attacking
		{
			attacker=properAlive(attacker,community_1, attackerIndex);	//finds and returns proper alive member of attacker community and equalice it to attacker
			defender=properAlive(defender,community_2, defenderIndex);	//finds and returns proper alive member of defender community and equalice it to defender

		}else{					//if round number is even it means that community2 is attacking

			attacker=properAlive(attacker,community_2, attackerIndex);	//finds and returns proper alive member of attacker community and equalice it to attacker
			defender=properAlive(defender,community_1, defenderIndex);	//finds and returns proper alive member of defender community and equalice it to defender

		}

		int damage= (**attacker).attack-(**defender).defense; //calculates the damage of the attacker by substracting defender's defense point from attacker's attack point

		if(usingSpecial=="SPECIAL")	//checks if the attacker using special power or not
		{

			if((**attacker).type=="Dwarfs" && (**attacker).nRoundsSinceSpecial>=20 ) 	//if the attacker is a dwarf and it uses its speacial, damage given to defender doubles
			{
				damage*=2;									//doubles the damage given the defender 
				(**attacker).nRoundsSinceSpecial=-1;		//adjust the attacker's round since special as -1
			}
			if((**attacker).type=="Wizards" && (**attacker).nRoundsSinceSpecial>=50)	//if the attacker is a wizard and it uses its special, reveals the last killed member of its community
			{

				if(roundCounter%2==1)	//if round number is odd it means that community1 is attacking
				{	
					if(lastKilled_1) 			//checks if last killed pointer nullptr or not
					{

						if((**lastKilled_1).isAlive==false) 	//checks if the last killed member of the community is alive rigth now
						{
							(**lastKilled_1).isAlive=true; 													//reveals last killed member of the community
							(**lastKilled_1).remainingHealth=(**lastKilled_1).healthHistory[0];				//makes last killed's health its initial health
							(**lastKilled_1).healthHistory[roundCounter]=(**lastKilled_1).remainingHealth;	//adds its new health to health histort
							(**lastKilled_1).nRoundsSinceSpecial=-1;										//adjust the last killed's round since special as -1
							deadCounter_1--;																//reduces dead counter by 1
							(**attacker).nRoundsSinceSpecial=-1;											//adjust the attacker's round since special as -1

						}else{									//checks if the last killed member of the community is alive rigth now

							(**lastKilled_1).remainingHealth=(**lastKilled_1).healthHistory[0];				//makes last killed's health its initial health
							(**lastKilled_1).healthHistory[roundCounter]=(**lastKilled_1).remainingHealth;	//adds its new health to health histort
							(**attacker).nRoundsSinceSpecial=-1;											//adjust the attacker's round since special as -1
						}
					}

				}else{					//if round number is even it means that community2 is attacking

					
					if(lastKilled_2)			//checks if last killed pointer nullptr or not
					{
						if((**lastKilled_2).isAlive==false)		//checks if the last killed member of the community is alive rigth now
						{	
							(**lastKilled_2).isAlive=true;													//reveals last killed member of the community
							(**lastKilled_2).remainingHealth=(**lastKilled_2).healthHistory[0];				//makes last killed's health its initial health
							(**lastKilled_2).healthHistory[roundCounter]=(**lastKilled_2).remainingHealth;	//adds its new health to health histort
							(**lastKilled_2).nRoundsSinceSpecial=-1;										//adjust the last killed's round since special as -1
							deadCounter_2--;																//reduces dead counter by 1
							(**attacker).nRoundsSinceSpecial=-1;											//adjust the attacker's round since special as -1

						}else{									//checks if the last killed member of the community is alive rigth now

							(**lastKilled_2).remainingHealth=(**lastKilled_2).healthHistory[0];				//makes last killed's health its initial health
							(**lastKilled_2).healthHistory[roundCounter]=(**lastKilled_2).remainingHealth;	//adds its new health to health histort
							(**attacker).nRoundsSinceSpecial=-1;											//adjust the attacker's round since special as -1
						}
					}
				}

			}
			if((**attacker).type=="Elves" && (**attacker).nRoundsSinceSpecial>=10)		//if the attacker is a elf and it uses its special, it can give his half health to hobbit of the community
			{
				if(roundCounter%2==1)		//if round number is odd it means that community1 is attacking
				{
					for(int i=0; i<5; i++)		//seeks for the hobbit of the community
					{
						if((**community_1[i]).type=="Hobbit")	//checks if the type of the community member is hobbit or not
						{
							(**community_1[i]).remainingHealth+= (**attacker).remainingHealth/2;				//adds the half health of the elf to hobbit's health point
							(**community_1[i]).healthHistory[roundCounter]=(**community_1[i]).remainingHealth;	//adds hobbit's new health to its health histort

							break;
						}
					}
					(**attacker).remainingHealth-=(**attacker).remainingHealth/2;  			//reduces the health of the elf by the health given to hobbit
					(**attacker).nRoundsSinceSpecial=-1;									//adjust the attacker's round since special as -1
					(**attacker).healthHistory[roundCounter]=(**attacker).remainingHealth;	//adds elf's new health to its health histort


				}else{						//if round number is even it means that community2 is attacking
					
					for(int i=0; i<5; i++)		//seeks for the hobbit of the community
					{
						if((**community_2[i]).type=="Hobbit")	//checks if the type of the community member is hobbit or not
						{

							(**community_2[i]).remainingHealth+= (**attacker).remainingHealth/2;				//adds the half health of the elf to hobbit's health point
							(**community_2[i]).healthHistory[roundCounter]=(**community_2[i]).remainingHealth;	//adds hobbit's new health to its health histort

							break;
						}
					}
					(**attacker).remainingHealth-=(**attacker).remainingHealth/2;			//reduces the health of the elf by the health given to hobbit
					(**attacker).nRoundsSinceSpecial=-1;									//adjust the attacker's round since special as -1
					(**attacker).healthHistory[roundCounter]=(**attacker).remainingHealth;	//adds elf's new health to its health histort

				}
			}
		}

		if(damage>0)		//checks if damage bigger that zero 
		{ 
			if(damage>=(**defender).remainingHealth)		//checks if damage is bigger than or equal to defender's remaining health
			{

				(**defender).remainingHealth = 0;				//adjusts defender's remaining health to 0
				(**defender).isAlive=false;						//changes defender's status as not alive
				(**defender).healthHistory[roundCounter]=0;		//adds defender's new health(0) to its helath history
				
				if(roundCounter%2==1)			//if round number is odd it means that community1 is attacking
				{
					lastKilled_2=defender;	 	//adjust last killes member as defender
					deadCounter_2++;			//increment dead counter by 1

				}else{							//if round number is even it means that community2 is attacking		

					lastKilled_1=defender;		//adjust last killed member as defender
					deadCounter_1++;			//increment dead counter by 1
				}
				

			}else{											//checks if damage is bigger than or equal to defender's remaining health

				(**defender).remainingHealth= (**defender).remainingHealth-damage;			//substracts damage point of attacker from remaining health of the defender
				(**defender).healthHistory[roundCounter]=(**defender).remainingHealth;		//adds defender's new helath to its health history
			}
		}

		if(roundCounter%2==1){					//if round number is odd it means that community1 is attacking

			warContinue = warChecker(community_2, deadCounter_2);	//checks if the war is proper to continue

		}else{									//if round number is even it means that community2 is attacking	

			warContinue = warChecker(community_1, deadCounter_1);	//checks if the war is proper to continue
		}

		for(int i=0; i<10; i++){		//increments n rounds since special of every member at the end of every round

			two_communities[i]->nRoundsSinceSpecial++;	//increments n round since special by 1
		}

	}


	string winner;		//keeps the winner community

	if(!warContinue){	//checks if the war status is proper to continue

		if(roundCounter%2==1){		//if round number is odd and war doesn't continue, it means that community1 is last attacked community and they won

			winner="Community-1";	//adjusts winner as community1
		
		}else{						//if round number is odd and war doesn't continue, it means that community1 is last attacked community and they won

			winner="Community-2";	//adjusts winner as community2
		}

	}else{				//if war status is proper to continue, it means that maximum round is reached so it is a draw

		winner="Draw";	//declares it is a draw
	}

	int totalCasualties= deadCounter_1+deadCounter_2; //sums the casualities of both communities

	write<<winner<<endl;          	//writes the winner community name to output file
	write<<roundCounter<<endl;		//writes the the number of round passed to output file
	write<<totalCasualties<<endl;	//writes total casualities number to output file

	for(int i=0; i<10; i++){		//writes the helath history of every community member to output file

		write<<two_communities[i]->name<<" "; 	//writes the name of the current member to output file
		
		for(int j=0; j<roundCounter+1; j++){	//writes the health history of the community current community member

			write<<two_communities[i]->healthHistory[j]<<" "; //writes the health history of the community current community member one by one

		}

		write<<endl; //blank line
	}
	for(int i = 0 ; i<10 ; i++)
    {
        delete two_communities[i];
    }

	delete[] two_communities;

	return 1;
}

bool warChecker(Character** community[], int deadCounter) //checks if the ending conditions of the war are met
{

	if(deadCounter==4)	//if there are 4 casualities in a community, war ends
	{

		return false; //indicates war ends by returning false
	}


	for(int i=0; i<5; i++)	//checks if the hobbit of the community is alive
	{

		if((**community[i]).type=="Hobbit") 	//seeks for the hobbit of the community
		{
			return (**community[i]).isAlive;	//if the hobbit is alive returns true otherwise returns false

		}
	}

	return false; 	//indicates war ends

}


Character** properAlive(Character** member, Character**community[], int memberIndex) //finds the proper alive member of a community
{
	if((**member).isAlive){		//checks if the given member is alive. if it is alive, returns this member

		return member; //returns given member
	
	}else{						//if the given member is not alive, returns other proper member of the community

		for(int i=memberIndex+1; i<5;i++)	//checks if there is a member after given member according to alphabetical order
		{

			if((**community[i]).isAlive)	//if there is an alive member after given member, returns this member
			{

				member=community[i]; //equalizes this member to member

				return member; //returns member
			}
		}


		for(int i=memberIndex-1; i>=0; i--) //checks if there is a member before given member according to alphabetical order
		{

			if((**community[i]).isAlive)	//if there is an alive member before given member, returns this member
			{

				member=community[i]; //equalizes this member to member

				return member;	//returns member
			}
		}
	}

	return nullptr; 

		
}	

void sortByName(Character** community[]) //sorts member of communities by their names (alpphabetical order) using bubble sort algorithm
{

	bool isChanged=true; //keeps if there is a change or not

	while(isChanged) //continue while there is a change
	{

		isChanged=false; //adjust change status at the beginning of every loop

		for(int k=0; k<4; k++)	//checks names of charachters one by one
		{

			if(**community[k+1]<**community[k]) //checks if the name of character is lower than next charachter's name
			{

				Character** ptr= community[k]; //changes the places of the characters

				community[k]=community[k+1];
				community[k+1]=ptr;

				isChanged=true;	

			}
		}
	}
}
