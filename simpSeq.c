#include<stdio.h>
#include<math.h>

double funcao(double x){
    return ( x*x + 3.0 );
}

/*Function definition to perform integration by Simpson's 1/3rd Rule */
double simpsons(double f(double x),double a,double b,int n){
  double h,integral,x,sum=0;
  int i;
  h=fabs(b-a)/n;
  for(i=1;i<n;i++){
    x=a+i*h;
    if(i%2==0){
      sum=sum+2*f(x);
    }
    else{
      sum=sum+4*f(x);
    }
  }
  integral=(h/3)*(f(a)+f(b)+sum);
  return integral;
}
 
// Inicio do programa
main(){
  int n,i=2;
  double a,b,h,x,sum=0,integral,eps,integral_new;
   
  /*Entrada de dados */
  printf("\nDiga o limite inicial: ");
  scanf("%lf",&a);
  printf("\nDiga o limite final: ");  
  scanf("%lf",&b);
  printf("\nEntre o numero de sub-intervalos/ precisao ");
  scanf("%lf",&eps);
  
  integral_new = simpsons(funcao,a,b,i);
 
  /* Perform integration by simpson's 1/3rd for different number of sub-intervals until they converge to the given accuracy:*/
  do{
    integral = integral_new;
    i=i+2;
    integral_new = simpsons(funcao,a,b,i);
  }while(fabs(integral_new - integral)>=eps);
   
  /*Print the answer */
  printf("\nA integral eh: %lf para %d sub-intervalos.\n",integral_new,i);
}