#include <iostream>
#include <stdlib.h>    
#include <time.h> 

using namespace std;

class Visitor{	//visitor class will represent the people who take part in the simulation
	
	private:
		int o_pr; //the office that the visitor wants to go to
		int f_pr; //the floor that the visitor wants to go to
		int prio; //his priority
		int served; //variable that determines wether the visitor was served or not
		
	public:
		Visitor(int fo_pr, int ff_pr):o_pr(fo_pr),f_pr(ff_pr){ //Constructor function
			
			this->served=0;
		}
		
		int setserved(){ //this function will be used later when a visitor has reaced his office in order to get a served value
			return this->served=rand()%2;
		}
		
		int getserved(){ //getter that returns whether the visitor is served or not
			return this->served;
		}
		
		int getfloor(){ //getter that returns visitor's floor
			return this->f_pr;
		}
		
		int getoffice(){ //getter that returns visitor's office
			return this->o_pr;
		}
		
		void setprio(int i){ //this function is used when a visitor reaches the groundfloor. this is where his priority is set
			this->prio=i;
		}
		
		int getprio(){ //getter that returns visitor's priority
			return this->prio;
		}
		
		void printprio(){  //function that prints visitor's priority
			cout << " priority is :" << this->prio << endl;
		}
		
		~Visitor(){} //Destructor function
		
}; 

class Space{ //base class which every other class inherits from (note: groundfloor inherits from floor class)
	protected:
		int N; //capacity is used by every child
		
	public:
		Space(int fN):N(fN){} //Constructor function
		
		virtual void enter(Visitor*){} 		 //every function is inherited by the mother class 
		virtual void enter(Visitor*, int){} //because later on instead of using the children directly i will pass their addresses at pointers to the mother class
		virtual Visitor* exit(){}			//Note: Elevator object is an exception to the above because it has to use other classes as arguments in its methods
		virtual Visitor* exit(int, int){}
		virtual Visitor* exit(int){}
		virtual void exit(Visitor*){}
		virtual void make_served(){}
		virtual int get_ofcount(){}
		virtual int getoff(int){}
		virtual void print_prio(int, int, int){}
		virtual int get_encount(){}
		virtual int getno(){}
		virtual int get_flcount(){}
		virtual void operate(){}
		
		~Space(){} //Destructor function
		
		
};


class Office: public Space{ //Office class that the visitor has to go to
		
	private:
	
		int oc;	//oc member is the number of the office
		int ofcount; //how many people are in an office
		Visitor** offvisit; //array that stores all the visitors in an office
		
	public:
		Office(int fNo,int foc):Space(fNo),oc(foc){ //Constructor function
			cout << "Office number "<< this->oc <<" has been created."  <<endl; 
			
			offvisit= new Visitor*[fNo];
			for(int j=0;j<fNo;j++){
				offvisit[j]=NULL;
			}
			
			ofcount=0;
			
		}
		
		
		void enter(Visitor* fvarr){ //Office enter takes a visitor and places him in the first empty cell it finds
			
			int i=0;
			for(i;i<N;i++){
				if(offvisit[i]==NULL){
					break;
				}
			}
			
			offvisit[i]=fvarr;
			ofcount++;
		}
		
		Visitor* exit(){ //Office exit removes the first visitor (who was served) from the array  
		
			Visitor* temp=NULL;
			int i=0;
			for(i;i<N;i++){
				if(offvisit[i]!=NULL && offvisit[i]->getserved()==1 ){
					temp=offvisit[i];
					offvisit[i]=NULL;
					ofcount--;
					break;
				}
			}
			
			return temp;	

		}
						
		void make_served(){ //this function sets a visitor's served value to either 0 or 1
			
			for(int i=0;i<N;i++){
				if(offvisit[i]!=NULL){
					if (offvisit[i]->getserved()!=1){ //if the visitor is already served don't reset his value
						offvisit[i]->setserved();
					}
				}
			}
	
		}	
		
		
		int get_ofcount(){ //getting how many people are in an office
			return ofcount;
		}
		
		~Office(){
			cout << "End of the work!" <<endl; //Destructor function
			
			for(int j=0;j<N;j++){
				//offvisit[j]=NULL;
				//delete offvisit[j];
			}
			delete[] offvisit;
		}
};



class Entrance: public Space{ 
	
	private:

		Visitor** entered; //array that stores all the visitors in an entrance
		int ecount; //how many people are in an entrance
		
	public:
		Entrance(int fNe):Space(fNe){ //Constructor function
			cout<<"The Entrance has been created!" <<endl; 
			
			entered= new Visitor*[fNe];
			int j=0;
			for(j;j<fNe;j++){
				entered[j]=NULL;
			}
			
			ecount=0;
			
		}
	
		
 
		void enter(Visitor* fvarr){  //Entrance enter takes a visitor and places him in the first empty cell it finds
			
			int i=0;
			
			
			for(i;i<N;i++){
				if(entered[i]==NULL){
					break;
				}
			}
			
			entered[i]=fvarr;
			ecount++;
		}
		
		
		Visitor* exit(int f,int o){  //Entrance exit removes the first visitor (who was served) from the array 
			 						 //Arguments f and o are used because the function behaves differently if we are exiting from a floor's entrance
			
			if(f==0){ //exiting from groundfloor
				Visitor* temp;
			
				int i=0;
				for(i;i<N;i++){
					if(entered[i]!=NULL){
						break;
					}
				}
				temp=entered[i];
				entered[i]=NULL;
			
			
				ecount--;

				return temp;
			
			}
			
			if(f==1){  //exiting from floor
				Visitor* temp;
				
				int k=0;
				for(k;k<N;k++){
					if(entered[k]!=NULL && entered[k]->getoffice()==o){ //making sure if the visitor wants to go to the desired office
						break;
					}
				}
				temp=entered[k];
				entered[k]=NULL;
				ecount--;
				return temp;
			
			}
		}
		
		int getoff(int i){ //this function is used to ensure the visitor wants to go to the office we are currently on
			int bingo=0;
			
			int k=0;
			for(k;k<N;k++){
				if(entered[k]!=NULL && entered[k]->getoffice()==i){
					bingo=1;
					break;
				}
			}
		
			return bingo;
		}
		
		void print_prio(int n, int f, int i){ //function to print visitor's priority
			if (f==0){						  //again f and i arguments are used because it behaves differently if we are on the floor
				if (entered[n]!=NULL){
					cout<<"You are not allowed to enter! ";
					entered[n]->printprio();
				}
			}
			if (f==1){
				if (entered[n]!=NULL){
					cout<<"Please wait for outside in office: "<< i ;
					entered[n]->printprio();
				}
			}
		}
		
		int get_encount(){ //getting how many people are in an entrance
			return ecount;
		}
		
		
		~Entrance(){ //Destructor function
			cout << "End of waiting people!" << endl;
			for(int j=0;j<N;j++){
				//entered[j]=NULL;
				//delete entered[j];
			}
			delete[] entered;

		}
};	

class Floor: public Space{
		
	private:
		
		int FNo; //office's capacity
		int fc; //fc member is floor's number
		int fcount; //how many people are in a floor
		Entrance* Ef; //floor's entrance 
		Space* Sef; //pointer to the mother
		Office* Of[10]; //every floor has 10 offices
		Space* Sof[10]; //pointer array to the mothers


	public:
		Floor(int fNf, int fFNo, int ffc ):Space(fNf),FNo(fFNo), fc(ffc){ //Constructor function
		
			if(this->fc!=0){
				cout<<"A Floor has been created with number:"<< this->fc << endl; 
			}
			
			
			if(this->FNo!=0){ //don't create offices and entrance if office capacity is zero (only in groundfloor)
				Ef= new Entrance(fNf);
				Sef=Ef;	
				
				for(int i=0;i<10;i++){
					Of[i]= new Office(fFNo,i+1);
				}
				for(int i=0;i<10;i++){
					Sof[i]= Of[i];
				}
			}
			
			fcount=0;
			
		}
		
		int getno(){ //getting floor's number
			return this->fc;
		}
		
	
		void enter(Visitor* fvarr, int a){ //floor enter places a visitor in floor's entrance then proceeds to the entering stage of the office
			
			Sef->enter(fvarr);
			fcount++;
		
			for(int i=0;i<10;i++){ //for every office
				if(Sof[i]->get_ofcount() < this->FNo){ //if the visitor fits
					if(Sef->getoff(i+1)){ //and if this is the office he wants to go to
						
				
						Visitor* temp = Sef->exit(1,i+1); //remove him from the entrance
				
						Sof[i]->enter(temp); //and place him in the office
						cout<<"Entering office number: "<< i+1 ; 
						temp->printprio();
				
						break; //only one can enter at a time so if one person went in no need to continue the loop
					}
				}
				else{ //print the desired messages if he couldn't fit
					cout<<"Please wait for outside in office: "<< i+1<<" ";
					fvarr->printprio(); 
					break;
				}
			}
			
		}
		
		Visitor* exit(){ //floor exit function removes the first visitor that was served and also proceeds to enter those that wait in the entrance 
			
			Visitor* oftemp=NULL;
			Visitor* retemp=NULL;
			for(int i=0;i<10;i++){ //for every floor
				if(Sof[i]->get_ofcount()!=0){ //if there is at least one person inside
					
					if((retemp=Sof[i]->exit())!=NULL){ //if we managed to find someone
					
						oftemp=retemp; //place him temporarily in a variable
						fcount--;
						for(int j=0;j<Sef->get_encount();j++){ //now check for every person in the entrance if he can go in the office
							if(Sof[i]->get_ofcount()<FNo){ // if office count is less than its capacity 
						
								if(Sef->getoff(i+1)){ //if we find someone that wants this office
									Visitor* temp; 
									temp=Sef->exit(1,i+1); //remove him from the entrance
									Sof[i]->enter(temp); //and place him in that office
									cout<<"Entering office number: "<< i+1 ;
									temp->printprio();
								}
							}
							else{ //for those that couldn't enter yet again..
								Sef->print_prio(j,1,i+1); 
								
							}
						}
					break; //we only return one visitor at a time, so if we found somebody break the loop
					}
					
				}	
			}
			
			return oftemp; //return the visitor 
			
		}
			
		int get_flcount(){ //getting how many people are in a floor
			return this->fcount;
		}
	
		
		
		void make_served(){ //this function will go at every office that has at least one person inside and give him a served value(either 0 or 1)
		
			for (int i=0; i<10; i++){
				if(Sof[i]->get_ofcount()!=0){
					Sof[i]->make_served();			
				}
			}
			
		}
		
		virtual void print_prio(int){} //groundfloor has to inherit those functions
		virtual int get_gcount(){}
		virtual void enter(Visitor*, int, int, int ){}
		virtual Visitor* exit(int){}
		virtual void wait(Visitor*, int){}
		virtual void dec_counter(){}
		
		~Floor(){ //Destructor function
			delete Ef;
			for(int i=0;i<10;i++){
				delete Of[i]; 
			}
			cout<<"End of service!" <<endl;
		}
		
};



class GroundFloor: public Floor{ 
	
	private:
		
		Entrance* Eg; //groundfloor's entrance
		Space* Seg; //pointer to the mother
		Visitor** groundvisit; //array that stores all the visitors in a ground floor 
		Visitor** finished; //array that stores all the visitors who finished their work from their offices
		int gcount; //how many people are in a groundfloor
		
	public:
		GroundFloor(int fNg):Floor(fNg,0,0){  //Constructor function
			
			cout << "A Ground Floor has been created " << endl;
			
			Eg=new Entrance(fNg);
			
			groundvisit= new Visitor*[fNg];
			for(int j=0;j<fNg;j++){
				groundvisit[j]=NULL;
			}
			
			finished= new Visitor*[fNg];
			for(int k=0;k<fNg;k++){
				finished[k]=NULL;
			}
			
			Seg=Eg;
			
			gcount=0;
			
		}
		
		void wait(Visitor* fvarr ,int a){ //wait function gives a visitor his priority and places him inside groundfloor's entrance
			 
			 fvarr->setprio(a+1);
			 Seg->enter(fvarr);
			 
			 cout<< "Waiting for the lift." << endl;
			 
			 
		}
		
		
		void enter(Visitor* fvarr, int n, int b, int r ){ //enter function to place the visitors inside the entrance and forbid from entering those who can't fit
		
			if(r==0){   //if the visitor goes from the building to the groundfloor
				if(n<N){ //if he can fit put him in a array
				
					groundvisit[n]=fvarr;
					gcount++;
					
				}
				if(n>=N){ //if he can't fit print the desired message
					cout<<"Sorry, can't fit in the ground floor."<<endl;
				}
				if(n==b-1){ //if the counter is equal to the building counter then we did all the necessary work for every visitor (who went in the building) 
					for(int i=0;i<gcount;i++){ //call the wait function for those that fit in the groundfloor
						wait(groundvisit[i],i);
					}
				}
			}
			
			if(r==1){ //if the visitor is returning from the elevator to the groundfloor place him in a different array
					  
				int i=0; 
				for(i;i<N;i++){
					if(finished[i]==NULL){
						break;
					}
				}
				finished[i]=fvarr;
				
			}
			
		}
		
			
		Visitor* exit(int r){ //exit function removes a visitor 
			 
			if (r==0){ //if the visitor goes from the groundfloor to the elevator remove him from the groundfloor's entrance
		    	Seg->exit(0,0);
				
			}

			if(r==1){  //if the visitor goes from the groundfloor to the building (when he has finished, that is) remove him from the finished array
				Visitor* temp;
				
				int i=0;
				for(i;i<N;i++){
					if(finished[i]!=NULL){
						break;
					}
				}
				temp=finished[i];
				finished[i]=NULL;
			
			
				return temp;
			}		
		
		}
		
		void print_prio(int n){ //function to print visitor's priority 
			Seg->print_prio(n,0,0); //(this is used in the elevator's entering stage so the visitor won't have to leave the entrance only to enter it again)
		}
		
		int get_gcount(){ //getting how many people are in a groundfloor
			return this->gcount;
		}		
			
		void dec_counter(){ //function to decrement groundfloor's counter
			this->gcount--;
		}
		

		~GroundFloor(){ //Destructor function
			delete Eg;
			cout << "End of service!" << endl; 

			for(int j=0;j<N;j++){
				//groundvisit[j]=NULL;
				delete groundvisit[j];
			}
			delete[] groundvisit;
			
			for(int k=0;k<N;k++){
				//finished[k]=NULL;
				//delete finished[k];
			}
			delete[] finished;
		}
	
};


class Elevator: public Space{ 
	
	private:
		
		int lpos,fpos; //the use of these variables will be explained in the function they are used
		int elcount; //how many people are in an elevator
		int fc; //how many people finished
		Visitor** elevisit; //array that stores all the visitors in an elevator
		
	
		
	public:
		Elevator(int fNl):Space(fNl){ //Constructor function
			cout <<"A lift has been created!"<< endl;
			lpos=0;
			fpos=0;
			elcount=0;
			fc=0;
			
			elevisit= new Visitor*[fNl];
			for(int j=0;j<fNl;j++){
				elevisit[j]=NULL;	
			}
			
		}
		
		
		Visitor* exit(int n){ //exit function removes the visitor from the n cell 
			Visitor* temp;
			temp=elevisit[n];
			elevisit[n]=NULL;
			
			return temp;
		
		
		}
		
		
		int empty_all(Floor* FGfbe){ //empty all function takes the visitors who finished and places them in the groundfloor
			
			fc=0;
			
			for(int i=0;i<N;i++){
				if(elevisit[i]==NULL){ //if we stumbled upon a non-existent visitor run the loop again
					continue;
				}
				if(elevisit[i]->getserved()==1 ){
					
					Visitor* temp = exit(i);
					elcount--;
					FGfbe->enter(temp,i,0,1);
					fc++;
				}
			}
			
			return fc; //empty all has to return how many finished
			
			
		}
		
		void StopDown(Space* SFbe[]){  //stop down function that goes through every floor and takes those that finished
			
			for(int f=0;f<4;f++){ //give served values to those that are in the offices
				SFbe[f]->make_served();
			}
			
			for(int f=3;f>0;f--){ //since we are stopping down, the loop begins from the last floor
				for(int i=0;i<N;i++){ //for Nl visitors (elevator can't take more than that) (N is used because it is inherited from space class)
				
					if (SFbe[f]->getno()== f+1 ){ //just making sure we are on the right floor
				
						if(elcount<N){ //if there is room for more
							int j=0; 
							for(j;j<N;j++){ //find an empty position
								if(elevisit[j]==NULL){
									break;
								}
							}
					
							Visitor* temp=SFbe[f]->exit(); //find a served visitor
				
							if(temp!=NULL){ //if he exists place him in the empty position
								enter(temp,j);
								elcount++;
								continue;
							}
						}	
					
					}
				}
			}
		    
		}
		
		void StopUp(Space* SFbe[],int Nfi){  //stop up function that goes through every floor and places the visitors who want to enter it 
			
			
			for(int f=0;f<4;f++){ //for every floor
	
				for(int i=0; i<N; i++){ //for Nl people (N is used because it is inherited from space class)
			
					if (elcount == 0){ //if there is no one on the elevator this process is skipped 
						break;
					}
					if (elevisit[i]==NULL){  //if we stumbled upon a non-existent visitor run the loop again
						continue;
					}
					if ( elevisit[i]->getfloor()== f+1 ){ //if this is the floor the visitor wants to go to
				
						if(SFbe[f]->get_flcount()<Nfi){ //if he can fit in the floor
							Visitor* temp = exit(i);   //remove him from the elevator
							elcount--;	
				
							cout<<"Entering floor number: " << f+1 ;
							temp->printprio();
							SFbe[f]->enter(temp, i); //and place him on the floor
							
							continue; 
						}
						else{ //if he can't fit print the desired messages
							cout<<"Sorry, floor number " <<f+1<<" is full. ";
							elevisit[i]->printprio();
						}
					
					}
				}
			}
			
		
		}
		
		void enter(Visitor* fvarr,int i){ //enter function places a visitor in the i cell

				
				cout<<"Visitor in the lift. " ;
				elevisit[i]=fvarr;
				elevisit[i]->printprio();
				
		
			
		}
		
		int operate(Floor* FGfbe,Space* SFbe[],int Ngi,int Nfi){
			
				int i=0;
				lpos=fpos; //lpos is the counter that goes through everyone in the groudfloor, fpos's use is explained below  
				
				//Entering stage
				for(lpos;lpos<Ngi;lpos++){
					if(FGfbe->get_gcount()==0){ //if we took everyone from the groundfloor we may skip the entering stage
						break;
					}
					if(elcount<N){ //if the visitor can fit in the elevator
						
						int j=0;
						for(j;j<N;j++){
							if(elevisit[j]==NULL){ //find an empty position
								break;
							}
						}
						
						Visitor* temp = FGfbe->exit(0); //remove him from the groundfloor
						
						if(temp!=NULL){ //if he exists
							FGfbe->dec_counter(); //decrement the counter of those who are on the groundfloor
							enter(temp,j); //and place him in the elevator
						
							elcount++;
							i++;
							continue; //continue statement is used in order to execute the following if-block in the next loop 
						}
					}
					if(elcount==N){ //if the counter is equal to the capacity this means we can't take any more visitors
						if(i==N){ //if this is the first visitor who couldn't fit 
							fpos=lpos; //then place his postion in fpos so the loop will begin from this one in the next operation
						}
						 
						FGfbe->print_prio(lpos); //and print the desired messages
						i++;

					}
				
						
				}
			
			//entering stage is over, we may call the remaining functions
			StopUp(SFbe,Nfi); 
			
			StopDown(SFbe);
			
			empty_all(FGfbe);
			
			
		
			
		}
		
		
		~Elevator(){ //Destructor function
			cout << "No more ups and downs!" << endl;
			for(int j=0;j<N;j++){
				//elevisit[j]=NULL;	
				//delete elevisit[j];
			}
			delete[] elevisit;
		}
};



class Building: public Space{   
	
	private:
		int Nb,Nf,Ng,No,Nl,K,L; //capacities 
		GroundFloor* Gfb; //Building's groundfloor
		Floor* Fg;   //Pointer to groundfloor's mother
		Elevator* Eb; //Building's elevator
		Space* Seb;   //pointer to the mother
		Floor* Fb[4];  //A building has 4 Floors
		Space* Sfb[4]; //pointer array to the mothers
		Visitor** buildvisit; //array that stores all the visitors in a building
	
		int bcount; //how many people are in a building
		int fc; //fc is how many people finished their work in their offices. when we reach the exiting stage, we will run a loop for fc people.
		
	public:
		Building(int fN, int fNf, int fNg, int fNo, int fNl, int fK, int fL ):Space(fN),Nf(fNf),Ng(fNg),No(fNo),Nl(fNl),K(fK),L(fL){ //Constructor function
			cout << "A new building is ready for serving citizens!" << endl;
			
			Gfb= new GroundFloor(fNg);
			Eb= new Elevator(fNl);
			for(int i=0; i<4;i++){
				Fb[i]= new Floor(fNf,fNo,i+1);
			}
			for(int i=0; i<4;i++){
				Sfb[i]= Fb[i];
			}

			buildvisit=new Visitor*[fN];
			for(int j=0;j<fN;j++){
				buildvisit[j]=NULL;
			}
			
			bcount=0;
			fc=0;
			Seb=Eb;
			Fg=Gfb;
			
			
		}
	
		void exit(Visitor* fvarr){ //exit function prints the desired messages 
			
				cout<<"I cannot believe I finished! " ;
				fvarr->printprio();
			
		}
		
		void operate(){ //operate is used to call elevator's operate and remove those that finished
			
			
			fc=Eb->operate(Fg,Sfb,Ng,Nf); //elevator operate will return(through empty_all function) how many finished 
			
			
			for(int i=0; i<fc;i++){ //removing those that finished
				
				Visitor* finale;
				Visitor* temp=Fg->exit(1);
				
				exit(temp);
			}
			
		}
		
		void enter(Visitor* fvarr, int n){ //enter function to place the visitors inside the building and forbid from entering those who can't fit
		
				
				if(n<N){ //if he can fit put him in a array
			
					buildvisit[n]=fvarr;
					bcount++;
				
				}
				if(n>N){ //if he can't fit print the desired message
			 
					cout << "Please, come tomorrow." << endl;
				}
			
				if(n==K-1){ //if the counter is equal to the visitor number then we did all the necessary work for every visitor
				
					for(int i=0;i<bcount;i++){ //now for those that managed to fit, place them in the groundfloor 
					
						Fg->enter(buildvisit[i],i,bcount,0);
						
	
					}
				}
				
			}
			
			
		
		~Building(){ //Destructor function
			
			
			for(int i=0;i<4;i++){
				delete Fb[i];	
			}
			delete Eb;
			delete Gfb;
			cout << "Service not available any longer. Go elsewhere!" << endl;
			for(int j=0;j<N;j++){
				//buildvisit[j]=NULL;
				//delete buildvisit[j];
			}
			delete[] buildvisit;
		}
	
};



int main(int argc, char* argv[]){
	
	srand(time(NULL));
	
	
	int mN=15,mNf=5,mNg=7,mNo=3,mNl=4,K=10,L=3; //the command line arguments 
	
	/*if(argc < 1){  				
        cout<<"Error of argc!"<<endl;
    }
    else{
        mN = atoi(argv[1]);		
        mNf = atoi(argv[2]);		
        mNg = atoi(argv[3]);		
        mNo = atoi(argv[4]);	
		mNl = atoi(argv[5]);		
		K = atoi(argv[6]);		
		L = atoi(argv[7]);			
    }*/
	
	Visitor** varr= new Visitor*[K]; //varr array will store all the visitors
	
	for(int i=0; i<K;i++){ //give every visitor a floor and office preference
		int f_pr=rand()%4+1; 
		int o_pr=rand()%10+1;
		varr[i]=new Visitor(o_pr,f_pr);
		
		
	}
	
	Building Dimosio4Ever(mN,mNf,mNg,mNo,mNl,K,L); //constructing the building
	Space* Sb=&Dimosio4Ever; //pointer to the mother 
	
	for(int a=0; a<K; a++){ //placing the visitors in the building

		Sb->enter(varr[a],a);
	}
	
	for(int lc=0;lc<L;lc++){ //calling operate function for L times
		
		Sb->operate();
	}	
	
	for(int i=0;i<K;i++){ //deleting visitor objects
		delete varr[i];
	}
	delete[] varr;

}
