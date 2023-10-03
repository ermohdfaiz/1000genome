#include <stdio.h>
#include <math.h>
double similarity_measure(int a, int b, int c, int p, int q, int r)
{
	double dist;
	dist = sqrt(pow((a-p), 2) + pow((b-q), 2) + pow((c-r), 2));
	return dist;
}

int find_min(int n_clust, double a[][n_clust], int row)
{	
	int min = 0;
	for(int i = 1; i < n_clust; i++) {
		if (a[row][min] > a[row][i]) {
			min = i;
		}
	}
	return min;
}

void update_centroid(int *minimal, int noc, int n,double centre[][3],double points[][3])
{	
	for(int i = 0; i < noc; i++) {
		double k = 0;
		double  m = 0;
		double p = 0;
		double q = 0;
		
		for(int j = 0; j < n; j++) {
			 
			if(i == minimal[j]) {
				k++;
				m += points[j][0];
				p += points[j][1];
				q += points[j][2];
				
			}
			
		}
		
		m = m / k;
		p = p / k;
		q = q / k;
		centre[i][0] = m;
		centre[i][1] = p;
		centre[i][2] = q;
	
}				
}

void kmeans(int n, int dim, int n_clust, double centre[][3], double points[][3])
{	int minimal[n];
	
	FILE *fp = fopen("output.txt","w");
	for(int k = 1; k <= 20; k++) {

	for(int j = 0; j < n; j++) {
	
		double d[n][n_clust];
		
		
		for(int i = 0; i < n_clust; i ++) {
			
			d[j][i] = similarity_measure(centre[i][0],centre[i][1],centre[i][2],points[j][0],points[j][1],points[j][2]);
			
		}
		
		minimal[j] = find_min(n_clust, d,j);
		
	}
	
	update_centroid(minimal, n_clust, n, centre, points);
	fprintf(fp,"%f %f %f\n\n",centre[0][0], centre[0][1], centre[0][2]);
	printf("%f %f %f\n\n",centre[0][0], centre[0][1], centre[0][2]);
	
	for(int i = 0; i < n; i++) {
		if(minimal[i] == 0)
		fprintf(fp,"%f %f %f\n",points[i][0], points[i][1],points[i][2]);
	}
	fprintf(fp,"\n");
	fprintf(fp,"%f %f %f\n\n",centre[1][0], centre[1][1], centre[1][2]);
	
	for(int i = 0; i < n; i++) {
		if(minimal[i] == 1)
		fprintf(fp,"%f %f %f\n",points[i][0], points[i][1],points[i][2]);
	}
	fprintf(fp,"\n");
	}
	
	fclose(fp);
}	

	
		
