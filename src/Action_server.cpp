#include<ros/ros.h>
#include<actionlib/server/simple_action_server.h>
#include<nhk_2020/taskAction.h>
#include<geometry_msgs/Twist.h>
#include<std_msgs/Bool.h>
#include<bits/stdc++.h>

class taskAction{
	private:
		ros::NodeHandle nh;

		actionlib::SimpleActionServer<nhk_2020::taskAction> Task;
		std::string order_action;

		nhk_2020::taskFeedback feedback_;
		nhk_2020::taskResult result_;
		void posCb(const geometry_msgs::Twist::ConstPtr&mg);
 		ros::Subscriber pSb=nh.subscribe<geometry_msgs::Twist>("cmd_pos",10,&taskAction::posCb,this);


	public:
		taskAction(std::string name):
				Task(nh,name,boost::bind(&taskAction::exeCb,this,_1),false),
				order_action(name){
		 			Task.start();
		 		}
		~taskAction(void){}
		void exeCb(const nhk_2020::taskGoalConstPtr &goal);
		double nowx=0,nowy=0,nowz=0;
		double x=0,y=0,z=0;
		double Dx=0,Dy=0,Dz=0;
		int limit=50;

};
void taskAction::posCb(const geometry_msgs::Twist::ConstPtr&msg){
 		nowx=msg->linear.x;                  
 		nowy=msg->linear.y;
 		nowz=msg->linear.z;                  
		std::cout<<nowx<<std::endl;
 }

void taskAction::exeCb(const nhk_2020::taskGoalConstPtr &goal){
		ros::Rate loop_rate(1);
		bool success=true;

		x=goal->Goal.linear.x;
		y=goal->Goal.linear.y;
		z=goal->Goal.angular.z;	


        Dx=x;
		Dy=y;
		Dz=z;
	    std::cout<<Dx<<" "<<Dy<<" "<<Dz<<std::endl;
	while(Dx>limit || Dy>limit){

    	if(Task.isPreemptRequested()||!ros::ok()){
		   	Task.setPreempted();
	   	   	success=false;
		   	break;
		}
		feedback_.passing=false;
	
    	Dx=x-sqrt(nowx*nowx);
		Dy=y-sqrt(nowy*nowy);
		Dz=z-sqrt(nowz*nowz);
		
		loop_rate.sleep();
		std::cout<<"prompt"<<std::endl;
	}

	if(success){
		
			result_.reached=1;
			nowx=0;
			nowy=0;
			std::cout<<"true"<<std::endl;
			loop_rate.sleep();
			Task.setSucceeded(result_);
	}
}	

int main(int argc,char** argv){
		ros::init(argc,argv,"Action_server");

		taskAction task(ros::this_node::getName());
		ros::spin();

		return 0;
}


