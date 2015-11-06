
class Objeto{
    public:
        Objeto();
        Objeto(double s, int m, double t);
        void setScale(double s);
        void setModel(int m);
        void setTranslate(double t);
        double getScale();
        double getTranslate();
        int getModel();
    private:
        double scale;
        int model;
        double translate;
};

Objeto::Objeto(){
    scale = 1;
    model = 0;
    translate = -1;
}

Objeto::Objeto(double s, int m, double t){
    scale = s;
    model = m;
    translate = t;
}

void Objeto::setScale(double s){
    scale = s;
}

void Objeto::setModel(int m){
    model = m;
}

void Objeto::setTranslate(double t){
    translate = t;
}

double Objeto::getScale(){
    return scale;
}

double Objeto::getTranslate(){
    return translate;
}

int Objeto::getModel(){
    return model;
}
