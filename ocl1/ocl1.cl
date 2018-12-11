__kernel void matrixMul(__global float* A, __global float* B,
		int widthA, int widthB, __global float* C) {
	int rowC = get_global_id(0);
	int columnC = get_global_id(1);
	float value = 0;
	for (int k = 0; k < widthA; ++k) {
		float elementA = A[rowC * widthA + k];
		float elementB = B[k * widthB + columnC];
		value += elementA * elementB;
	}
	C[rowC * widthB + columnC] = value;
}
