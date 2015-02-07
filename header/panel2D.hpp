class panel2D{
public:
	double integral;
	double xcenter;
	double ycenter;
	double xradius;
	double yradius;
	double* xNodes;
	double* yNodes;
	bool isLeaf;
	panel2D(double xcenter, double ycenter, double xradius, double yradius){
		this->xcenter	=	xcenter;
		this->ycenter	=	ycenter;
		this->xradius	=	xradius;
		this->yradius	=	yradius;
		this->isLeaf	=	false;
		this->integral	=	0.0;
	}
};