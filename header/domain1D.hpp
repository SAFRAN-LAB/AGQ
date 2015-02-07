#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include "panel1D.hpp"
#include "Gauss_Legendre_Nodes_and_Weights.hpp"

class domain1D {
public:
	double xcenter;
	double Lx;
	double integral;
	double epsilon;
	int nMaxLevels;
	std::vector< std::vector<panel1D*> > tree;
	std::vector<panel1D*> leafNodes;
	int nNodes;
	long double* nodes;
	long double* weights;
	virtual double integrand(double x) {
		return 0.0;
	};
	domain1D() {};
	~domain1D() {};
	void panel1DNodes(int j, int k) {
		tree[j][k]->xNodes	=	new double[nNodes];
		for (int l=0; l<nNodes; ++l) {
			tree[j][k]->xNodes[l]	=	tree[j][k]->xcenter	+	tree[j][k]->xradius*nodes[l];
		}
	}
	void panel1DIntegral(int j, int k) {
		for (int m=0;m<nNodes;++m) {
				tree[j][k]->integral+=weights[m]*integrand(tree[j][k]->xNodes[m]);
		}
		tree[j][k]->integral*=tree[j][k]->xradius;
	}
	void create_Root() {
		panel1D* root		=	new panel1D(xcenter, Lx);
		std::vector<panel1D*> level;
		level.push_back(root);
		tree.push_back(level);
		panel1DNodes(0,0);
		panel1DIntegral(0,0);
		nMaxLevels	=	0;
	}
	void create_Tree(int j, int k) {
		double xradius	=	0.5*tree[j][k]->xradius;
        //      Create children of node. The children are numbered as shown below.
        /************************/
        //						//
        //         0    1		//
		//		_____|_____		//
        //                      //
        /************************/
		if ((int)tree.size()==j+1) {
			std::vector<panel1D*>	level;
			tree.push_back(level);
			nMaxLevels=j+1;
		}
		int n	=	tree[j+1].size();

		panel1D* child0	=	new panel1D(tree[j][k]->xcenter-xradius,xradius);
		tree[j+1].push_back(child0);
		panel1DNodes(j+1,n);
		panel1DIntegral(j+1,n);
		panel1D* child1	=	new panel1D(tree[j][k]->xcenter+xradius,xradius);
		tree[j+1].push_back(child1);
		panel1DNodes(j+1,n+1);
		panel1DIntegral(j+1,n+1);
		double error	=	tree[j][k]->integral;
		for (int l=0;l<2;++l) {
			error-=tree[j+1][n+l]->integral;
		}
		if (fabs(error)<epsilon) {
			tree[j][k]->isLeaf	=	true;
			leafNodes.push_back(tree[j][k]);
		}
		else {
			for (int l=0; l<2; ++l) {
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
		double push	=	0.125;
		myfile << "\\draw[red](" << xcenter-Lx << ",0)--(" << xcenter+Lx << ",0);";
		for (int k=0; k<(int)leafNodes.size(); ++k) {
			myfile << "\\draw [ultra thin] (" << leafNodes[k]->xcenter-leafNodes[k]->xradius << "," << -push << ")";
			myfile << "-- (" << leafNodes[k]->xcenter-leafNodes[k]->xradius << "," << push << ");";
		}
		myfile << "\\draw(" << xcenter+Lx << "," << -push << ")--(" << xcenter+Lx << "," << push <<");";
		myfile<< "\\node at (" << xcenter-Lx << "," << -2*push << ") {\\tiny$" << xcenter-Lx << "$};" << std::endl;
		myfile<< "\\node at (" << xcenter+Lx << "," << -2*push << ") {\\tiny$" << xcenter+Lx << "$};" << std::endl;
		myfile << "\\end{tikzpicture}" << std::endl;
		myfile << "\\end{document}" << std::endl;
		myfile.close();
	}
};