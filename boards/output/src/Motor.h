#ifndef MOTOR_H
#define MOTOR_H

class Motor{
		private:
            int pin1_,pin2_;
		public:
			Motor(int pin1,int pin2);
			void run(int speed);
};

#endif
