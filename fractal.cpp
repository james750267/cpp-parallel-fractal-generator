#include <iostream>
#include <fstream>
#include <complex>
# include <omp.h>
# include <ctime>

void timestamp ( void ); // utilizado para retornar o tempo de execução
int **matrix2d ( int m, int n ); // retorna uma matriz bidimencional
int min ( int a, int b ); // retorna o menor dos valores entre a e b
using namespace std;

int main (int argc, char *argv[])  {
int i, j;
	int width =atoi(argv[1]);
	int height = atoi(argv[2]);
	int max_iteracoes = atoi(argv[3]);
	int threads = atoi(argv[4]);
	unsigned int nb_iter = 0;
	ofstream img ("mandelbrot.ppm");
	cout<<"Gerando um fractal com dimensão "<< width <<"x"<< height <<" com "<< threads <<" threads na pool e com número máximo de iterações igual a"<< max_iteracoes <<endl;
	int **values = matrix2d ( height , width);

	double wtime = omp_get_wtime();
	if (!img.is_open())cout << "Não foi possível abrir o arquivo";
	// roda em paralelo o cáculo de cada ponto
	# pragma omp parallel num_threads (threads)  \
		shared (values) \
		private (i, j, nb_iter)
 	{
		# pragma omp for
		for (i = 0; i < width; i++) {
			for (j = 0; j < height; j++)  {
			int val;
			// instancia os números complexos
			complex<float> point((float)i/width-1.5, (float)j/height-0.5);
			complex<float> z(0, 0);
				for (nb_iter=0; nb_iter <= max_iteracoes;nb_iter++) {
					z = z * z + point;
					// se z maior que dois então o ponto não pertence ao conjunto mandelbrot
					if(abs (z) > 2) break;
		    	}
			    val = (nb_iter < max_iteracoes)? (255*nb_iter)/33: 255;
	          values[i][j] = val;
	       	}
        }
      }
    // imprime tempo de execução
	cout<<"Tempo de execução: "<< (omp_get_wtime() - wtime)<<" segundos" <<endl;

	// imprime na imagem o cabeçalho
	img << "P3\n";
	img << width << " " << height << "\n";
	img << 255 << "\n";
	// imprime os matrix2d na imagem
	for ( i = 0; i < height; i++ ){
	    for ( int jlo = 0; jlo < width; jlo = jlo + 4 ){
		    int jhi = min ( jlo + 4, width );
		    for ( j = jlo; j < jhi; j++ ){
		    	// cria um efeito de cor legal
		        img << "  " << values[i][j]
		            << "  " << (abs(255-values[i][j]))
		            << "  " << (abs(255-(values[i][j])/2 )) << "\n";
		    }
		    img << "\n";
	    }
	}
	img.close ( );
	 return 0;
}
void timestamp(){
# define TIME_SIZE 40

  static char time_buffer[TIME_SIZE];
  const struct std::tm *tm_ptr;
  size_t len;
  std::time_t now;

  now = std::time ( NULL );
  tm_ptr = std::localtime ( &now );

  len = std::strftime ( time_buffer, TIME_SIZE, "%d %B %Y %I:%M:%S %p", tm_ptr );

  return;
# undef TIME_SIZE
}

int **matrix2d ( int m, int n ){
	int **a;
	int i;
	a = new int *[m];

	if ( a == NULL ){
		cerr << "\n";
		cerr << "I4PP_NEW - Fatal error!\n";
		cerr << "  Unable to allocate row pointer array.\n";
		exit( 1 );
  	}

  	for ( i = 0; i < m; i++ ){
		a[i] = new int[n];
		if ( a[i] == NULL ){
			cerr << "\n";
			cerr << "I4PP_NEW - Fatal error!\n";
			cerr << "  Unable to allocate row array.\n";
			exit ( 1 );
	    }
  	}

  return a;
}

int min ( int a, int b ){
  int value;
  if(a<b) value = a;
  else value = b;
  return value;
}
