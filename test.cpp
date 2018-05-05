//Testing bench for VirtuaProc 1.0
//Code from this file will be moved to various modules for refactoring in the implementation phase
//Author: Shreyak Chakraborty
//License: GNU GPL v3

#include <iostream>
#include <cstdlib>
#include <random>
#include <ctime>
#include <string>
#include <algorithm>
#include <vector>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>
#include <fstream>

std::shared_mutex semaphore;   //manage concurrency for WRITE locks


#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#define RUN_TIME 10   //Total running time in simulated CPU cycles
#define NUM_P 20  //Number of processes
#define NUM_R 6   //Number of resources


int trace_p[NUM_P*RUN_TIME][3];  //the Tracing Table for Processes
int trace_r[NUM_P*RUN_TIME][NUM_R];  //the Tracing Table for Resources
int row=0;
int r=1;

class Resource  //defines a system resource
{
	private:
		int data;
		int rcount; //reader count
		int wcount; //writer count

	public:
	    sf::RectangleShape shape;

		Resource(int val)
		{
			name="resource";
			data=0;
			shape.setSize(sf::Vector2f(70,70));
			shape.setFillColor(sf::Color(0,200,200));
			shape.setPosition(80*(r),100);
			++r;
		}
		std::string name;
		int read();  //read data
		void write(int); //write data

};

int Resource::read()
{
	return data;
}

void Resource::write(int val)
{
	data=val;
}




class Process    //defines a system process (simulated using a std::thread)
{
public:
    int state;
    int wt;   //waiting time of process
     sf::CircleShape shape;

	Process(int id)
	{
		pid=id;
		wt=0;
		state=0; //start as inactive
        shape.setRadius(20);
        shape.setPosition(50*(pid-1),20);


	}

	int pid;     //PID of the process
	void job();  //the task executed by the process
	void kill(); //terminate a process
};


std::vector<Process> P;   //process vector
std::vector<Resource> R;   //resource vector

void init_resc()
{
	//Initialize Resource Vector with n values
	for(int i=0;i<NUM_R;i++){

		R.push_back(Resource(i+1));
	}
}

void update_trace(int p,int s,int r)  //update the Resource Tracing Table
{
	trace_p[row][0]=p;  //process ID
	trace_p[row][1]=s;  //process state
	trace_p[row][2]=r;  //resource ID
    for(int u=0;u<NUM_R;u++)
        trace_r[row][u]=R[u].read();  //store all resource values
	++row;
}

void view_trace()
{
    std::cout<<"P S R\n";
    for(int i=0;i<NUM_P*(RUN_TIME-1);i++)
        std::cout<<trace_p[i][0]<<" "<<trace_p[i][1]<<" "<<trace_p[i][2]<<std::endl;
    std::cout<<row;
}

void show_resc()
{
	 std::cout<<"\nResource Values:\n";
	 for(int i=0;i<NUM_P*(RUN_TIME-1);i++)
     {
         for(int u=0;u<NUM_R;u++)
            std::cout<<trace_r[i][u]<<" ";
         std::cout<<"\n";
     }

	 std::cout<<"\n";
}


/*
The job() function implements the work that a system process does.
The current version makes processes access system resources from the global resource vector in either READ(R) or WRITE(W) modes
concurrently.
*/
void job(int pid,int mode,int rnum)
{

	int dat;

	//randomly request for access to a system resource


	if(mode==1)   //try to get READ Lock
	{
		std::shared_lock<std::shared_mutex> r_lock(semaphore);
		dat=R[rnum].read();
		P[pid].state=1;
	}
	if(mode==2) //try to get WRITE Lock
	{
		std::lock_guard<std::shared_mutex> w_lock(semaphore);
		R[rnum].write(pid);
	}

	P[pid].state=mode;
	update_trace(pid,P[pid].state,rnum);

}

void Process::kill()
{
    state=0;
}




   void report(sf::RenderWindow* w)   //show report of process and resource activity
   {
       char ch;
       w->setActive();
       bool start=true;

       int g,i=0,j,cycles=0;
       sf::Font font;
       sf::Text label1; //for labeling PIDs
       sf::Text label2; //for labeling resource allocations
       sf::Text label3; //for labeling resource values

       font.loadFromFile("arial.ttf");
       label1.setFont(font);
       label2.setFont(font);
       label3.setFont(font);

       while(w->isOpen())
       {
           if(cycles==RUN_TIME-1)
           {
              std::cout<<"Enter a key to exit";
              std::cin>>ch;
              return;
           }

         w->clear(sf::Color(255,255,255));

          for(j=i;j<i+(NUM_P-1);j++)
          {
            if(trace_p[j][1]==1)
                P[trace_p[j][0]-1].shape.setFillColor(sf::Color(200,200,0));
            else if(trace_p[j][1]==2)
                P[trace_p[j][0]-1].shape.setFillColor(sf::Color(0,0,200));


            label1.setString(std::to_string(trace_p[j][0]));
            label1.setCharacterSize(16);
            label1.setPosition(50*(P[trace_p[j][0]-1].pid-1),22);
            label1.setStyle(sf::Text::Bold);
            label1.setColor(sf::Color::Black);

            label2.setString(std::to_string(trace_p[j][2]));
            label2.setCharacterSize(16);
            label2.setPosition(51*(P[trace_p[j][0]-1].pid-1),60);
            label2.setStyle(sf::Text::Bold);
            label2.setColor(sf::Color::Red);

            w->draw(P[trace_p[j][0]-1].shape);
            w->draw(label1);
            w->draw(label2);

            for(g=0;g<NUM_R;g++)
            {

                label3.setString(std::to_string(trace_r[j][g]));
                label3.setCharacterSize(16);
                label3.setPosition(R[g].shape.getPosition());

                label3.setColor(sf::Color::Blue);

                w->draw(R[g].shape);
                w->draw(label3);
            }

          }

          i=j;

          w->display();

          sf::sleep(sf::seconds(0.5));

          ++cycles;
       }


   }








std::string getFileContents (std::ifstream& File)
{
    std::string Lines = "";        //All lines

    if (File)                      //Check if everything is good
    {
	while (File.good ())
	{
	    std::string TempLine;                  //Temp line
	    std::getline (File , TempLine);        //Get temp line
	    TempLine += "\n";                      //Add newline character

	    Lines += TempLine;                     //Add newline
	}
	return Lines;
    }
    else                           //Return error
    {
	return "ERROR File does not exist.";
    }
}

int main()
{
	srand((unsigned int)time(0));

	init_resc();

	//The Process and Thread vectors are directly related
    std::vector<std::thread> T;  //thread vector

    sf::RenderWindow main_window(sf::VideoMode(1024,768), "VirtuaProc Visualizer");
    int a,b,i,j,num=20;


//-------------------------------------------Command Parsing--------------------------------------------
    std::string input;
    bool once=false;  //if simulation has been run once

	std::ifstream Reader ("title.txt");
    std::string Art = getFileContents(Reader);
    std::cout << Art << std::endl;
    Reader.close ();
    std::cout<<"VirtuaProc Concurrent Process Simulator version 1.0\n";
    std::cout<<"COMMAND CONSOLE STARED...."<<std::endl;
CMD:std::cout<<">";
	std::cin>>input;

	if(input == "exit"){
		goto EXIT;
	}
	else if(input == "clear"){
		system("clear");
		goto CMD;
	}
	else if(input == "start")
            goto START;
    else if(input == "viewproc" || input == "vp")
    {
        if(once)
            view_trace();
        else
            std::cout<<"No data available!\n";
        goto CMD;
    }
    else if(input == "viewresc" || input == "vr")
    {
        if(once)
            show_resc();
        else
            std::cout<<"No data available!\n";
        goto CMD;
    }
    else if(input == "show" || input == "")
    {
        if(once)
            report(&main_window);  //display report
        else
            std::cout<<"No data available!\n";
        goto CMD;
    }
	else
	{
        std::cout<<"Invalid input. Please check help file by typing 'help'\n";
        goto CMD;
	}

	//----------------------------------------------------------------------------------------------

START:
            std::cout<<"\nSTARTED.."<<std::endl;
            for(j=0;j<RUN_TIME;j++)
            {

                if(j>0)
                {
                    std::random_shuffle(P.begin(),P.end());
                }

                 for(i=0;i<NUM_P;i++)
                 {
                     a=(rand()%2)+1;
                     b=(rand()%(R.size()));
                    if(j==0)
                        P.push_back(Process(i+1));  //Initialize Process Vector
                    //The thread vector assigns the job() function to a particular process P[i]
                    T.push_back(std::thread(job,P[i].pid,a,b)); //start the corresponding thread for P[i]
                 }
            }

           for(i=0;i<T.size();i++)
                   T[i].join();

           std::cout<<"\nFINISHED.."<<std::endl;
           once=true;
           goto CMD;  //return to command console

//

EXIT:	  return 0;
}
