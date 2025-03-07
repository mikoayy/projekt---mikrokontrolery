#ifndef __SERVO__
#define __SERVO__



void servo_init(int PIN);

void set_angle(int angle);

int map(float value, float fromLow, float fromHigh, float toLow, float toHigh);


#endif // !1


