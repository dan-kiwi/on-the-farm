float* objParser(const char* filename)
{
	char* file = read_file(filename);
	//retreive size of file
	int curr_i = 0;
	float* v = (float*)malloc(sizeof(float) * 3 * 264);
	float* vt = (float*)malloc(sizeof(float) * 2 * 613);
	float* vn = (float*)malloc(sizeof(float) * 3 * 240);

	char* curr_ptr = file;
	while (!(*curr_ptr == '\n' && *(curr_ptr + 1) == 'v'))
		curr_ptr++;
	curr_ptr++;

	char* end_ptr;
	int curr_v = 0;
	while (!(*curr_ptr == 'v' && *(curr_ptr + 1) == 't')) 
	{
		curr_ptr++;
		for (int i = 0; i < 3; i++) {
			v[curr_v++] = strtof(curr_ptr, &end_ptr);
			curr_ptr = end_ptr;
		}
		curr_ptr++;
	}

	curr_v = 0;
	while (*curr_ptr == 'v' && *(curr_ptr + 1) == 't') 
	{
		curr_ptr += 2;
		for (int i = 0; i < 2; i++) {
			vt[curr_v++] = strtof(curr_ptr, &end_ptr);
			curr_ptr = end_ptr;
		}
		curr_ptr++;
	}

	curr_v = 0;
	while (*curr_ptr == 'v' && *(curr_ptr + 1) == 'n') 
	{
		curr_ptr += 2;
		for (int i = 0; i < 3; i++) {
			vn[curr_v++] = strtof(curr_ptr, &end_ptr);
			curr_ptr = end_ptr;
		}
		curr_ptr++;
	}

	while (!(*curr_ptr == 'f' && *(curr_ptr + 1) == ' '))
		curr_ptr++;

	curr_v = 0;
	int v1, v2, v3, v4;
	int vt1, vt2, vt3, vt4;
	int vn1, vn2, vn3, vn4;
	float* openGl = (float*)malloc(sizeof(float) * 24 * 484);
	while (*curr_ptr != '\0') 
	{
		while (*curr_ptr != 'f')
			curr_ptr++;
		curr_ptr++;
		v1 = strtof(curr_ptr, &end_ptr);
		curr_ptr = end_ptr;
		curr_ptr += 1;
		vt1 = strtof(curr_ptr, &end_ptr);
		curr_ptr = end_ptr;
		curr_ptr += 1;
		vn1 = strtof(curr_ptr, &end_ptr);
		curr_ptr = end_ptr;
		curr_ptr += 1;
		v2 = strtof(curr_ptr, &end_ptr);
		curr_ptr = end_ptr;
		curr_ptr += 1;
		vt2 = strtof(curr_ptr, &end_ptr);
		curr_ptr = end_ptr;
		curr_ptr += 1;
		vn2 = strtof(curr_ptr, &end_ptr);
		curr_ptr = end_ptr;
		curr_ptr += 1;
		v3 = strtof(curr_ptr, &end_ptr);
		curr_ptr = end_ptr;
		curr_ptr += 1;
		vt3 = strtof(curr_ptr, &end_ptr);
		curr_ptr = end_ptr;
		curr_ptr += 1;
		vn3 = strtof(curr_ptr, &end_ptr);
		curr_ptr = end_ptr;
		curr_ptr += 1;
		if (*curr_ptr != 'f') {
			v4 = strtof(curr_ptr, &end_ptr);
			curr_ptr = end_ptr;
			curr_ptr += 1;
			vt4 = strtof(curr_ptr, &end_ptr);
			curr_ptr = end_ptr;
			curr_ptr += 1;
			vn4 = strtof(curr_ptr, &end_ptr);
			curr_ptr = end_ptr;
			curr_ptr += 1;
		} else {
			v4 = NULL;
			vt4 = NULL;
			vn4 = NULL;
		}

		openGl[curr_v++] = v[v1 * 3 - 3];
		openGl[curr_v++] = v[v1 * 3 - 2];
		openGl[curr_v++] = v[v1 * 3 - 1];
		openGl[curr_v++] = vt[vt1 * 2 - 2];
		openGl[curr_v++] = vt[vt1 * 2 - 1];
		openGl[curr_v++] = vn[vn1 * 3 - 3];
		openGl[curr_v++] = vn[vn1 * 3 - 2];
		openGl[curr_v++] = vn[vn1 * 3 - 1];
		openGl[curr_v++] = v[(v2 * 3) - 3];
		openGl[curr_v++] = v[(v2 * 3) - 2];
		openGl[curr_v++] = v[(v2 * 3) - 1];
		openGl[curr_v++] = vt[vt2 * 2 - 2];
		openGl[curr_v++] = vt[vt2 * 2 - 1];
		openGl[curr_v++] = vn[vn2 * 3 - 3];
		openGl[curr_v++] = vn[vn2 * 3 - 2];
		openGl[curr_v++] = vn[vn2 * 3 - 1];
		openGl[curr_v++] = v[v3 * 3 - 3];
		openGl[curr_v++] = v[v3 * 3 - 2];
		openGl[curr_v++] = v[v3 * 3 - 1];
		openGl[curr_v++] = vt[vt3 * 2 - 2];
		openGl[curr_v++] = vt[vt3 * 2 - 1];
		openGl[curr_v++] = vn[vn3 * 3 - 3];
		openGl[curr_v++] = vn[vn3 * 3 - 2];
		openGl[curr_v++] = vn[vn3 * 3 - 1];
		if (v4 == NULL)
			continue;
		openGl[curr_v++] = v[v3 * 3 - 3];
		openGl[curr_v++] = v[v3 * 3 - 2];
		openGl[curr_v++] = v[v3 * 3 - 1];
		openGl[curr_v++] = vt[vt3 * 2 - 2];
		openGl[curr_v++] = vt[vt3 * 2 - 1];
		openGl[curr_v++] = vn[vn3 * 3 - 3];
		openGl[curr_v++] = vn[vn3 * 3 - 2];
		openGl[curr_v++] = vn[vn3 * 3 - 1];
		openGl[curr_v++] = v[v4 * 3 - 3];
		openGl[curr_v++] = v[v4 * 3 - 2];
		openGl[curr_v++] = v[v4 * 3 - 1];
		openGl[curr_v++] = vt[vt4 * 2 - 2];
		openGl[curr_v++] = vt[vt4 * 2 - 1];
		openGl[curr_v++] = vn[vn4 * 3 - 3];
		openGl[curr_v++] = vn[vn4 * 3 - 2];
		openGl[curr_v++] = vn[vn4 * 3 - 1];
		openGl[curr_v++] = v[v1 * 3 - 3];
		openGl[curr_v++] = v[v1 * 3 - 2];
		openGl[curr_v++] = v[v1 * 3 - 1];
		openGl[curr_v++] = vt[vt1 * 2 - 2];
		openGl[curr_v++] = vt[vt1 * 2 - 1];
		openGl[curr_v++] = vn[vn1 * 3 - 3];
		openGl[curr_v++] = vn[vn1 * 3 - 2];
		openGl[curr_v++] = vn[vn1 * 3 - 1];
	}

	free(file);
	free(v);
	free(vt);
	free(vn);
	
	return openGl;
}
