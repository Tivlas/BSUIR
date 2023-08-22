void array_C(double **c, int k, double& mainDiagSum_C, double& sideDiagSum_C) {

	for (int i = 0; i < k; i++) {
		for (int j = 0; j < k; j++) {
			c[i][j] = static_cast<double>(i * i + j * j) / static_cast<double>(i + j + 1); // формула из условия
			if (i == j) {
				mainDiagSum_C += c[i][j];
			}
			if (j == k - 1 - i) {
				sideDiagSum_C += c[i][j];
			}
		}
	}
}

void array_D(double **d, int k, double& mainDiagSum_D, double& sideDiagSum_D) {
	for (int i = 0; i < k; i++) {
		for (int j = 0; j < k; j++) {

			if (j <= 8) {
				d[i][j] = (i * i + j * j); 
			}
			else {
				d[i][j] = (i * i + j * j) / 2.0;
			}
			// считаем сумму
			if (i == j) {
				mainDiagSum_D += d[i][j];
			}
			if (j == k - 1 - i) {
				sideDiagSum_D += d[i][j];
			}
		}
	}
}