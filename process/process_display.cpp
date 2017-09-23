#include "../memory/operator.cpp"
#include <stdlib.h>
#include <iostream>
using namespace std;
#define MAX_LENTH_OF_LINE 1024
#define MAX_CONSTRUCTION_AMOUNT 5 
#define DEFAULT_LINES 2
#define CONSTRUCTION_PER_PROCESS 2
#define CPU 1
#define IO 2

class myProcess {
	public:
		_int pid;
		_int statu;
		_int time_to_run;
		_int current_time;  // current executing clock
		_int construction_lines;
		_int current_line;  // the line we are executing  from 0th line
		int_ARRAY_ARRAY construction;  // construction[i][0] property  [1] time it takes  [2]==1 finished this line ==0 not 
		 					//	����ǧ���Խ�硣������ 
		static int current_construct_location;
		static int process_amount;
		/*myProcess() {
			process_amount++;
			cout << "Input the instruction amount for this process\n";
			cin >> this->construction_lines;
			cout << "Amount get!\n";
			this->current_line = 0;
			this->current_time = 0;
			this->time_to_run = 0;
		//	this->construction_lines = DEFAULT_LINES;
			for (int i = 0; i < this->construction_lines; i++)
				for (int j = 0; j < 2; j++)
					this->construction[i][j] = 0;
		//	setConstructions();
		//	cout << "finished set constructions" << endl;
		}*/
		myProcess()
		{
			process_amount++;
		}
		myProcess(int pid)  // ��ַ 
		{
			int segmentPage = CreateProcess(pid,PAGE_SIZE);
			this->pid = _int(pid);
			this->pid = pid;
			this->statu = _int(pid);
			this->time_to_run = _int(pid);
			this->current_time = _int(pid);  // current executing clock
			this->construction_lines = _int(pid);
			this->construction_lines = 0;
			
			this->current_line = _int(pid);			
			this->current_line = 0;
//			this->pid = pid;
			this->construction = int_ARRAY_ARRAY(pid,MAX_CONSTRUCTION_AMOUNT,3);
			int statu = 0;//��ʼ��״̬ 
			//this->construction_lines = construction_lines;
			this->current_time = 0;
			this->time_to_run = 0;
			for (int i = 0; i < MAX_CONSTRUCTION_AMOUNT; i++)
				for (int j = 0; j < 3; j++)   // ������3 ����2������ 
					this->construction.assignment(i,j,0);
		//	setConstructions();
		}
		
		void setConstructions();

		
		void setTimeToRun(int time_to_run) {
			this->time_to_run = time_to_run;
		}
		
		void setStatu(int statu) {
			this->statu = statu;	
			/*
				statu = -1  waiting for memory
				statu = 0  get the memory
				statu = 1 ready in the ready queue
				statu = 2 running
				statu = 3 waiting in the waiting queue
				statu = 4 finished state
			
			*/
		}
		
		int getMem()
		{
			_int i = _int(pid);
			_int share = _int(0);  // shared memory    
			while(_read(share.addr, sizeof(int), 1) == NULL){    // ���������������� signal ! 
				this->statu = -1; // waiting for memory state		
				return -1;
			}
			this->statu = 0; // get the resource
			return 0;
		}
		

		
		int getCurrentProperty() {
			return this->construction[+current_line][0];
		}
		
		int getTimeToRun() {
			return +this->time_to_run;
		}
		
	/*	_Process(unsigned char processName,  int sizeInByte) {
			this->processName = processName;
			this->sizeInByte = sizeInByte;
		}
	*/	
	
		int runOneStep();
			
		~myProcess() {}
		
		
	/*	int getSize() {
			return this->sizeInByte;
		} */
};

	int myProcess::current_construct_location = 0;  // ��ʼ����̬���ݳ�Ա 
	int myProcess::process_amount = 0;


int SetPositionByLine(FILE *fp, int nLine)  
{  
    int i = 0;  
    char buffer[MAX_LENTH_OF_LINE + 1];
    fpos_t pos;  
  
    rewind(fp);  
    for (; i < nLine; i++)  
        fgets(buffer, MAX_LENTH_OF_LINE, fp);  
    fgetpos(fp, &pos);  
    return 0;  
}  






/*
	We have different kinds of instruction
	some just use CPU -----------dispatch CPU and use, change dispatch when the clock is up
	some IO like input, just change its state to waiting and return waiting, despite the running time and change dispatch.
*/

// ��������ʱ�䣬 ��������ָ���ʱ�䣬 ���Լ���ִ�е�������һ��ָ�� 
int myProcess::runOneStep() {  // line is the line of the construction table 
	this->current_time++;
	int time_sum = 0, flag = 0;
	for (int t = 0; t < +this->construction_lines; t++) {
		time_sum += this->construction[t][1];
		if (time_sum > +this->current_time) {  // ��current_time �״δ��� current_time��ʱ��õ� 
			this->current_line = t; // Executing the (t+1)th construction 
			flag = 1;
			break;
		} else if (time_sum == +this->current_time) {
			this->current_line = t;
			flag = 1;
			this->construction.assignment(t,2,1); // JUST FINSHED THIS CONSTRUCTION
		/*	cout << "\n\n pid " << this->pid << " �����˵� " << t << "��ָ�� \n\n"; 
			this->current_line++; // ����һ�� 
			cout << "תȥִ�е� " << current_line << "+1 ��ָ�" << this->construction[this->current_line][0] << endl;  
			*/
			break;
		}	
	}
	if (flag == 1) {
		switch (this->construction[+this->current_line][0]) {
			case CPU:
//				cout << "In runOneStep, current process pid:" << +this->pid << " is using the CPU\n";
				if (this->construction[+this->current_line][2] == 1) {
//					cout << "And we just finish the construction\n";
				//	if (this->current_line + 1 <= this->construction_lines)   ���ǰ�������һ�ŵ�����ȥ   Ҫ�ǵ� ���������� 
				//		this->current_line++;
					return 3;  // ������ CPU ָ�� 
				} else 
					return CPU;  // ��ǰ������ CPU ָ�� 
			case IO:
//				cout << "In runOneStep, current process pid:" << +this->pid << " is doing IO\n";
				if (this->construction[+this->current_line][2] == 1) {
//					cout << "And we just finish the IO\n";
				//	if (this->current_line + 1 <= this->construction_lines)
				//		this->current_line++;
					return 4;  // ������ IO ָ�� 
				}
				return IO; //����ǰ���ڽ���IO  
		}
		
	}

	else 
		return -1;
}

void myProcess::setConstructions() {
	FILE *fptr;
	char name;
	int construct_time;
	fptr = fopen("construction.txt", "r");
	
	int input = 0;
	cout<<"Please input construction lines:";
	cin >> input;
	this->construction_lines = input;
//	cout << "\n\npid " << +this->pid << "Construction lines are " << +this->construction_lines << "\n\n";
 	cout << "Amount get!\n";
 	for (int i = 0; i < +this->construction_lines; i++) {
		SetPositionByLine(fptr, current_construct_location + i); // Read the i-1 line  
		fscanf(fptr, "%c %d", &name, &construct_time);
//		cout << "name:" << name << " construction_time" << construct_time<< endl;
		switch (name) {
			case 'C': // CPU 1
//				cout << "This is a calculation construct, it takes " << construct_time << endl;
				this->construction.assignment(i,0,1);
				this->construction.assignment(i,1,construct_time);
				this->construction.assignment(i,2,0); // unfinished
				this->time_to_run += construct_time; 
				break;
			case 'K':  // K 2
//				cout << "Keyboard construction, it takes " << construct_time << endl;
				this->construction.assignment(i,0,2);
				this->construction.assignment(i,1,construct_time);
				this->time_to_run += construct_time;
				this->construction.assignment(i,2,0);
				break;
			default:
//				cout << "Invalid construction\n";
				break; 
		} 
		
	}
	
//	for(int j = 0; j < +this->construction_lines; j++)
//		cout << +this->construction[j][0] << "\t" << +this->construction[j][1] << "\t FINISH STATE IS " << +this->construction[j][2] << endl;
	
	current_construct_location += +this->construction_lines; // װ����һ�����̵�ָ��󣬸��ĵ�ǰָ��λ�� 
	
//	cout << "After setting current constructions, now construction location is" << current_construct_location << endl; 
	
//	cout << "Total time " << +this->time_to_run << endl << endl << endl;
	fclose(fptr);
	
}



