#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include "panel2D.hpp"
#include "Gauss_Legendre_Nodes_and_Weights.hpp"

class domain2D {
public:
	double xcenter;
	double ycenter;
	double Lx;
	double Ly;
	double integral;
	double epsilon;
	int nMaxLevels;
	std::vector< std::vector<panel2D*> > tree;
	std::vector<panel2D*> leafNodes;
	int nNodes;
	long double* nodes;
	long double* weights;
	virtual double integrand(double x, double y) {
		return 0.0;
	};
	domain2D() {};
	~domain2D() {};
	void panel2DNodes(int j, int k) {
		tree[j][k]->xNodes	=	new double[nNodes];
		tree[j][k]->yNodes	=	new double[nNodes];
		for (int l=0; l<nNodes; ++l) {
			tree[j][k]->xNodes[l]	=	tree[j][k]->xcenter	+	tree[j][k]->xradius*nodes[l];
			tree[j][k]->yNodes[l]	=	tree[j][k]->ycenter	+	tree[j][k]->yradius*nodes[l];
		}
	}
	void panel2DIntegral(int j, int k) {
		for (int m=0;m<nNodes;++m) {
			for (int n=0; n<nNodes; ++n) {
				tree[j][k]->integral+=weights[m]*weights[n]*integrand(tree[j][k]->xNodes[m],tree[j][k]->yNodes[n]);
			}
		}
		tree[j][k]->integral*=(tree[j][k]->xradius*tree[j][k]->yradius);
	}
	void create_Root() {
		panel2D* root		=	new panel2D(xcenter, ycenter, Lx, Ly);
		std::vector<panel2D*> level;
		level.push_back(root);
		tree.push_back(level);
		panel2DNodes(0,0);
		panel2DIntegral(0,0);
		nMaxLevels	=	0;
	}
	void create_Tree(int j, int k) {
		double xradius	=	0.5*tree[j][k]->xradius;
		double yradius	=	0.5*tree[j][k]->yradius;
        //      Create children of node. The children are numbered as shown below.
        /************************/
        //       ___________    //
        //      |     |     |   //
        //      |  3  |  2  |   //
        //      |_____|_____|   //
        //      |     |     |   //
        //      |  0  |  1  |   //
        //      |_____|_____|   //
        //                      //
        /************************/
		if ((int)tree.size()==j+1) {
			std::vector<panel2D*> level;
			tree.push_back(level);
			nMaxLevels=j+1;
		}
		int n	=	tree[j+1].size();

		panel2D* child0	=	new panel2D(tree[j][k]->xcenter-xradius,tree[j][k]->ycenter-yradius,xradius,yradius);
		tree[j+1].push_back(child0);
		panel2DNodes(j+1,n);
		panel2DIntegral(j+1,n);
		panel2D* child1	=	new panel2D(tree[j][k]->xcenter+xradius,tree[j][k]->ycenter-yradius,xradius,yradius);
		tree[j+1].push_back(child1);
		panel2DNodes(j+1,n+1);
		panel2DIntegral(j+1,n+1);
		panel2D* child2	=	new panel2D(tree[j][k]->xcenter+xradius,tree[j][k]->ycenter+yradius,xradius,yradius);
		tree[j+1].push_back(child2);
		panel2DNodes(j+1,n+2);
		panel2DIntegral(j+1,n+2);
		panel2D* child3	=	new panel2D(tree[j][k]->xcenter-xradius,tree[j][k]->ycenter+yradius,xradius,yradius);
		tree[j+1].push_back(child3);
		panel2DNodes(j+1,n+3);
		panel2DIntegral(j+1,n+3);
		double error	=	tree[j][k]->integral;
		for (int l=0;l<4;++l) {
			error-=tree[j+1][n+l]->integral;
		}
		if (fabs(error)<epsilon) {
			tree[j][k]->isLeaf	=	true;
			leafNodes.push_back(tree[j][k]);
		}
		else {
			for (int l=0; l<4; ++l) {
				create_Tree(j+1,n+l);
			}
		}
	}
	void obtain_Quadrature(double epsilon) {
		Gauss_Legendre_Nodes_and_Weights(nNodes, nodes, weights);
		this->epsilon	=	epsilon;
		create_Root();
		create_Tree(0,0);
		integral	=	0.0;
		for (int k=0; k<(int)leafNodes.size(); ++k) {
			integral+=leafNodes[k]->integral;
		}
	};
	void outputAdaptiveGrid(std::string filename) {
		std::ofstream myfile;
		myfile.open(filename.c_str());
		myfile << "\\documentclass{standalone}" << std::endl;
		myfile << "\\usepackage{tikz}" << std::endl;
		myfile << "\\begin{document}" << std::endl;
		myfile << "\\begin{tikzpicture}" << std::endl;
		for (int k=0; k<(int)leafNodes.size(); ++k) {
			myfile << "\\draw (" << leafNodes[k]->xcenter-leafNodes[k]->xradius << ",";
			myfile << leafNodes[k]->ycenter-leafNodes[k]->yradius << ") rectangle (";
			myfile << leafNodes[k]->xcenter+leafNodes[k]->xradius << ",";
			myfile << leafNodes[k]->ycenter+leafNodes[k]->yradius << ");" << std::endl;
		}
		double push	=	0.125;
		myfile<< "\\node at (" << xcenter-Lx-push << "," << ycenter-Ly-push << ") {\\tiny$(" << xcenter-Lx << "," << ycenter-Ly << ")$};" << std::endl;
		myfile<< "\\node at (" << xcenter-Lx-push << "," << ycenter+Ly+push << ") {\\tiny$(" << xcenter-Lx << "," << ycenter+Ly << ")$};" << std::endl;
		myfile<< "\\node at (" << xcenter+Lx+push << "," << ycenter-Ly-push << ") {\\tiny$(" << xcenter+Lx << "," << ycenter-Ly << ")$};" << std::endl;
		myfile<< "\\node at (" << xcenter+Lx+push << "," << ycenter+Ly+push << ") {\\tiny$(" << xcenter+Lx << "," << ycenter+Ly << ")$};" << std::endl;
		myfile << "\\end{tikzpicture}" << std::endl;
		myfile << "\\end{document}" << std::endl;
		myfile.close();
	}
};