class panel1D{
public:
	long double integral;
	long double xcenter;
	long double xradius;
	long double* xNodes;
	bool isLeaf;
	panel1D(long double xcenter, long double xradius){
		this->xcenter	=	xcenter;
		this->xradius	=	xradius;
		this->isLeaf	=	false;
		this->integral	=	0.0;
	}
	~panel1D() {
		delete[] xNodes;
	}
};