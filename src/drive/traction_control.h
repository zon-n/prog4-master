class TractionControl {
    int ax;
    int ay;
    int az;
    int rx;
    int ry;
    int rz;

    void adjustStraightLine(int ax, int ay, int az, int rx, int ry, int rz);

    void turnAssist(int ax, int ay, int az, int rx, int ry, int rz);

    void throttleControl(int ax, int ay, int az, int rx, int ry, int rz);

    void controleDerapage(int ax, int ay, int az, int rx, int ry, int rz);

};

