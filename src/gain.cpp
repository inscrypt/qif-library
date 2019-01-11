#include "../include/gain.hpp"

Gain::Gain(){
	num_act = 0;
	prior = NULL;
	matrix.resize(0, std::vector<long double>(0));
}

Gain::Gain(Distribution &prior, std::string file){
	int k;
	FILE *F;

	F = fopen(file.c_str(), "r");

	if(F == NULL){
		fprintf(stderr, "Error reading file!\n");
		exit(EXIT_FAILURE);
	}

	if(!fscanf(F, "%d %d", &num_act, &k)){
		exit(EXIT_FAILURE);
	}

	if(k != prior.num_el || num_act <= 0){
		fprintf(stderr, "Invalid matrix size!\n");
		exit(EXIT_FAILURE);
	}

	this->prior = &prior;
	matrix.resize(num_act, std::vector<long double>(this->prior->num_el));

	for(int i = 0; i < num_act; i++){
		for(int j = 0; j < this->prior->num_el; j++){
			if(!fscanf(F, "%Lf", &matrix[i][j])){
				exit(EXIT_FAILURE);
			}
		}
	}

	fclose(F);
}

Gain::Gain(Distribution &prior, int num_act, int MIN, int MAX){
	this->prior = &prior;
	this->num_act = num_act;

	matrix.resize(this->num_act, std::vector<long double>(this->prior->num_el));

	for(int i = 0; i < this->num_act; i++){
		for(int j = 0; j < this->prior->num_el; j++){
			matrix[i][j] = rand()%(MAX-MIN+1) + MIN;
		}
	}
}

std::string Gain::toString(int precision){
	std::ostringstream out;
	out << std::fixed << std::setprecision(precision);
	
	for(int i = 0; i < num_act; i++){
		for(int j = 0; j < prior->num_el-1; j++){
			out << matrix[i][j] << " ";
		}
		out << matrix[i][prior->num_el-1] << "\n";
	}

	return out.str();
}

void Gain::printToFile(std::string file, int precision){
	std::ofstream F(file);

	if(F.is_open() == false){
		fprintf(stderr, "Error opening the file ""%s""!\n", file.c_str());
		exit(EXIT_FAILURE);
	}

	F << num_act << " " << prior->num_el << "\n";
	F << std::fixed << std::setprecision(precision);
	for(int i = 0; i < num_act; i++){
		for(int j = 0; j < prior->num_el-1; j++){
			F << matrix[i][j] << " ";
		}
		F << matrix[i][prior->num_el-1] << "\n";
	}

	F.close();
}