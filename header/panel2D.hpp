class panel2D{
public:
	long double integral;
	long double xcenter;
	long double ycenter;
	long double xradius;
	long double yradius;
	long double* xNodes;
	long double* yNodes;
	bool isLeaf;
	panel2D(long double xcenter, long double ycenter, long double xradius, long double yradius){
		this->xcenter	=	xcenter;
		this->ycenter	=	ycenter;
		this->xradius	=	xradius;
		this->yradius	=	yradius;
		this->isLeaf	=	false;
		this->integral	=	0.0;
	}
	~panel2D() {
		delete[] xNodes;
		delete[] yNodes;
	}
};