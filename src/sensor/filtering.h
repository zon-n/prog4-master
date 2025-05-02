#include <Arduino.h>

int filtering(int data1, int data2, int trust1, int trust2);
int kalmanFilter(int data1, int data2, int trust1, int trust2);
int complementaryFIlter(int data1, int data2, int trust1, int trust2);
