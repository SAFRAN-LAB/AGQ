class panel1D{
public:
	double integral;
	double xcenter;
	double xradius;
	double* xNodes;
	bool isLeaf;
	panel1D(double xcenter, double xradius){
		this->xcenter	=	xcenter;
		this->xradius	=	xradius;
		this->isLeaf	=	false;
		this->integral	=	0.0;
	}
};